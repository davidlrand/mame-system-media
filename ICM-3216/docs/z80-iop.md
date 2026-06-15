# ICM-3216 Z80 I/O processor and the MiniBus SCSI bridge

The ICM-3216 does not attach its NCR5385 SCSI controller to the NS32016
directly. Instead a dedicated **Z80 I/O processor (IOP)**, running its own
firmware ROM (`600045_003`, 16 KB), owns the NCR5385 and shuttles disk and
tape data between the SCSI bus and NS32016 main memory across the
board's **MiniBus**. The host hands the IOP I/O requests through a one-byte
mailbox and gets completion interrupts back.

This note documents that interface as reverse-engineered from the firmware
ROM and verified against a working boot. It is the contract the MAME
`icm3216` driver implements in its emulated-IOP (LLE) mode.

Clocks: NS32016 host = 10 MHz, Z80 IOP = 5 MHz.

## Host side — the mailbox at 0xA000A0

A single byte at NS32016 physical address `0xA000A0`:

- **write** = a command byte to the IOP
- **read** = IOP status

Status bits:

| bit  | meaning                                            |
|------|----------------------------------------------------|
| 0x80 | BUSY — host spins on this clearing before each cmd |
| 0x20 | a SCSI bus reset was seen                           |
| 0x10 | interrupt request queued                           |
| 0x07 | low three bits = subchannel that completed         |

Command set (low nibble is the subchannel where applicable):

| command   | function                                                       |
|-----------|----------------------------------------------------------------|
| `0x00`+3  | write command-pointer-table base (3 address bytes, LSB first)  |
| `0x01`    | acknowledge interrupt                                          |
| `0x03`    | SCSI bus reset                                                 |
| `0x05`    | reset the I/O controller                                       |
| `0x10`\|n | start I/O on subchannel n                                     |
| `0x20`\|n | abort I/O on subchannel n                                     |

## Command structures in main memory

The host establishes a **command pointer table** (`cpt`) once, with command
`0x00` followed by the table's 24-bit base address. `cpt[n]` is a pointer to
the I/O control block (`iocb`) for subchannel *n*.

The `iocb` layout (from the host UNIX driver, `io/scsi.c` / `sys/scsi.h`):

```
offset 0   byte0
offset 1   { lun:3, :1, devid:3, :1 }     devid = (byte>>1)&7, lun = (byte>>5)&7
offset 2   cdb[12]                        the SCSI command descriptor block
offset 14  status                         SCSI status written back by the IOP
offset 15  chanstatus                     channel status written back by the IOP
offset 16  dataptr   (32-bit)             data buffer
offset 20  tptptr    (32-bit)             page-table pointer (0 = flat/buffered)
offset 24  limit     (32-bit)
offset 28  linkptr   (32-bit)
```

A start-I/O (`0x10|n`) makes the IOP read `cpt[n]`, fetch the `iocb`, program
the NCR5385 (destination ID = `iocb.devid`), transfer the CDB, move the data,
write `status`/`chanstatus` back into the `iocb`, and interrupt the host.

For raw (`B_PHYS`) I/O `tptptr` is non-zero: `dataptr` is then an offset and
`tptptr` points at an array of NS32082 page-table entries (512-byte pages), so
the transfer scatters/gathers through the page table rather than addressing a
single flat buffer.

## IOP side — registers

The IOP firmware uses memory-mapped registers only (no Z80 I/O-space ports):

- `C010`–`C017` — host mailbox + MiniBus main-memory bridge
- `C020`–`C02F` — the NCR5385 SCSI controller

Bridge registers:

| reg    | read                                   | write                                   |
|--------|----------------------------------------|-----------------------------------------|
| `C010` | host command (consuming it clears BUSY) | low byte of the MiniBus address strobe |
| `C011` | bus status: bit7 ready, bit6 cmd pending | DMA data path (NMI handler)            |
| `C012` | main-memory data, low byte of the word  | —                                       |
| `C013` | main-memory data, high byte (advances)  | host-facing status byte (BUSY/IRS)      |
| `C015` | —                                       | per-word transfer index                 |
| `C016` | —                                       | main-memory write, low byte             |
| `C017` | —                                       | strobe / main-memory write, high byte   |

## The MiniBus address protocol (U44 / U59 / U76)

This is the heart of the design. The bridge is **word-addressed**
(byte address = word address << 1), and a 24-bit physical address is split
between two sources latched at the address strobe:

```
byte_address = (page << 16) | ((offset & 0x7FFF) << 1)
```

- **page** (bits 16–23, the 64 KB page) is the high byte the IOP holds in its
  `DE` register; it is captured into the bridge's **high latch (U76)** at the
  first of the firmware's three address-strobe writes to `C010`.
- **offset** (the low 15 word-bits) is the value the IOP holds in `HL` at the
  final strobe, latched into **U59/U44**. The firmware deliberately masks its
  computed offset to `0x7FFF` (and the NMI data-mover masks `H & 0x7F`) because
  bit 15 and above — the page — belong to the bridge's high latch, not to the
  offset.

The firmware's address-load routine writes `C010` three times — `0xFF`, `0x00`,
then the offset low byte — with `DE` holding the address high half at the `0xFF`
write and `HL` the full word offset at the third.

Reads stream a 16-bit word at a time and auto-increment: a `C012` read returns
the low byte, a `C013` read the high byte and advances to the next word. Writes
post the low byte to `C016` and commit on the high byte to `C017`.

Because the page is taken from the IOP's own `DE` (i.e. from the `dataptr` it
fetched), disk data lands on the buffer's physical page even when that differs
from the command table's page — which is exactly what booting UNIX requires
(the kernel's buffer cache lives on different pages from its `cpt`).

## SCSI data movement (74LS646)

The NCR5385 is an 8-bit device on the Z80 bus; main memory is the 16-bit NS32016
bus. On each NCR5385 DREQ the controller raises the Z80 **NMI** (handler at ROM
`0x0233`); a 74LS646 transceiver pair (U60/U61) shuttles one byte per request,
packing two SCSI bytes into one main-memory word on input (or unpacking one word
into two SCSI bytes on output). The Z80 only programs the address latch and arms
the chip — it is not in the per-byte loop. On completion the NCR5385 interrupts
the IOP, which writes the result into the `iocb` and raises the host interrupt
on the NS32202 ICU (IR13).

## The two MAME emulation paths

The `icm3216` driver can emulate the IOP two ways, selected by the
**"I/O Processor"** machine-configuration setting:

- **Simulated (HLE)** — the default. The Z80 is suspended and the mailbox is
  serviced in software, running SCSI commands directly against the disk/tape
  images. Chosen as the default purely for performance.
- **Emulated (LLE)** — the real Z80 firmware runs and drives the NCR5385 over
  MAME's nscsi bus, reproducing everything above byte for byte. Slower, fully
  supported, and the faithful model of the hardware.

Both boot National Semiconductor UNIX System V to single user.
