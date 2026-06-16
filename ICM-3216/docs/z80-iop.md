# ICM-3216 Z80 I/O processor and the MiniBus SCSI bridge

The ICM-3216 does not attach its NCR5385 SCSI controller to the NS32016
directly. Instead a dedicated **Z80 I/O processor (IOP)**, running its own
firmware ROM (`600045_003`, 16 KB), owns the NCR5385 and shuttles disk and
tape data between the SCSI bus and NS32016 main memory across the
board's **MiniBus**. The host hands the IOP I/O requests through a one-byte
mailbox and gets completion interrupts back.

This note documents that interface as reverse-engineered from the firmware
ROM and validated against a working boot — the emulated bridge reproduces
the hardware's main-memory address for every one of the ~266,700 MiniBus
transfers in a full UNIX boot (see the validation note at the end). It is
the contract the MAME `icm3216` driver implements in its emulated-IOP (LLE)
mode.

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

| reg    | read                                     | write                                              |
|--------|------------------------------------------|----------------------------------------------------|
| `C010` | host command (consuming it clears BUSY)  | MiniBus address low — loads counter U44 (AD01-08)  |
| `C011` | bus status: bit7 ready, bit6 cmd pending | MiniBus address high — latch U59 (AD09-15 + AD00)  |
| `C012` | main-memory data, low byte of the word   | MiniBus address page — latch U76 (A16-23)          |
| `C013` | main-memory data, high byte (advances)   | host-facing status byte (BUSY/IRS)                 |
| `C014` | —                                        | DMA control register (PAL U36)                     |
| `C015` | —                                        | interrupt / DMA control (PAL U38: ZINT, ZWAIT, …)  |
| `C016` | —                                        | main-memory write, low byte (74LS646)              |
| `C017` | —                                        | main-memory write, high byte / strobe (74LS646)    |

The `C011` and `C012` writes do not come from ordinary firmware stores — the
firmware never targets them directly. They are written only by the IOP's NMI
handler, as explained next.

## The MiniBus address protocol (U44 / U59 / U76)

This is the heart of the design. The bridge is **word-addressed**
(byte address = word address << 1), and a 24-bit physical address is held
across three data-bus-loaded latches:

```
word_address = (U76 << 15) | ((U59 >> 1) << 8) | U44
byte_address = word_address << 1
```

- **U44** (AD01-08) is a CTTL-clocked `PAL20X8` **counter**, loaded from `C010`.
  It supplies the low 256 word-addresses (512 bytes) of a segment and counts up
  once per transferred word.
- **U59** (AD09-15 plus the AD00 byte-lane bit) and **U76** (page, A16-23) are
  latches holding the high address and 64 KB page. (`U59 >> 1` drops AD00, which
  selects the byte lane within the 16-bit word.)

The subtle part is how U59/U76 get loaded, because **the firmware never writes
`C011` or `C012`**. It writes only the low byte to `C010`. The high address and
page reach U59/U76 from the data bus through the IOP's **NMI handler** (ROM
`0x0233`):

```
0233: exx                ; switch to the alternate set holding this segment's address
      ld   ($C011),hl    ; HL' -> C011/C012 : low byte = U59, high byte = U76
      ld   ($C014),a     ; DMA control (PAL U36)
      ...
      retn
```

That handler runs on **ADOVF** — the U44 counter's carry-out, latched as ADOVF2
on CTTL and gating the `74HCT74` that drives the Z80 `/NMI`. ADOVF asserts in two
situations:

1. **At transfer setup.** The firmware leads each address load by writing
   `C010 = 0xFF`, i.e. it preloads the counter to terminal count. That asserts
   ADOVF at once, NMIs the IOP, and the handler latches this segment's high
   address and page. The firmware then writes the offset low byte to `C010`.
   (The full firmware sequence is three `C010` writes — `0xFF`, `0x00`, low byte
   — at ROM `0x10C2`–`0x10E1`.)
2. **Across a 512-byte boundary.** As the counter rolls over mid-transfer it
   asserts ADOVF again, and the handler reloads U59/U76 with the next segment —
   so a transfer walks across page boundaries, whether the buffer is flat or
   scatter-gathered through the NS32082 page table.

Because the page comes from the IOP's own running pointer (the `dataptr` it
fetched from the `iocb`), disk data lands on the buffer's physical page even
when that differs from the command table's page — which is exactly what booting
UNIX requires (the kernel's buffer cache lives on different pages from its
`cpt`).

Reads stream a 16-bit word at a time and auto-increment: a `C012` read returns
the low byte, a `C013` read the high byte and advances to the next word. Writes
post the low byte to `C016` and commit on the high byte to `C017`.

## SCSI data movement (74LS646)

The NCR5385 is an 8-bit device on the Z80 bus; main memory is the 16-bit NS32016
bus. A 74LS646 transceiver pair (U60/U61) bridges the two: on each NCR5385 DREQ
it shuttles one byte, packing two SCSI bytes into one main-memory word on input
(or unpacking one word into two SCSI bytes on output). This runs in hardware —
the DREQ cycle clocks the byte through the '646 and advances the U44 word
counter; the Z80 is **not** in the per-byte loop. It only sets up the address
latch and arms the chip.

The Z80's only per-transfer involvement is the **ADOVF NMI** described above:
when U44 reaches terminal count (every 512 bytes, and once at setup via the
`C010 = 0xFF` preload) the handler reloads U59/U76 for the next segment. On
completion the NCR5385 interrupts the IOP, which writes the result into the
`iocb` and raises the host interrupt on the NS32202 ICU (IR13).

## The two MAME emulation paths

The `icm3216` driver can emulate the IOP two ways, selected by the
**"I/O Processor"** machine-configuration setting:

- **Emulated Z80 (LLE)** — the default. The real Z80 firmware runs and drives
  the NCR5385 over MAME's nscsi bus, reproducing everything above. This is the
  faithful model of the hardware.
- **Simulated (HLE)** — the Z80 is suspended and the mailbox is serviced in
  software, running SCSI commands directly against the disk/tape images. Faster,
  but less faithful.

Both boot National Semiconductor UNIX System V to single user.

## Validation

The LLE address path was checked exhaustively rather than spot-checked. A
reference build captured the MiniBus byte address of every transfer across a
full UNIX boot; the data-bus-faithful build (the one described here, with no
inspection of the IOP's CPU registers) was then compared against it transfer
for transfer: **266,701 transfers, identical** — same sequence, same hash.

One implementation note on the LLE path: MAME's nscsi DMA has no equivalent of
the board's ZWAIT line (PAL U38), which on real hardware stalls the '646 byte
mover while the ADOVF NMI handler reloads the address latch mid-burst. MAME
instead advances the full 24-bit word counter directly within a transfer —
equivalent to the hardware for the contiguous in-burst case — and relies on the
handler's latch loads only at transfer boundaries. The byte-exact validation
above confirms the two are indistinguishable over a real workload.
