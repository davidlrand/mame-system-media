# ET532 — Hardware Reference

George Scolaro's **ET532** (PROJECT:ET532, **rev 1A, dated 1989-01-14**, ©1988-90
George Scolaro, 941 Chehalis Drive, Sunnyvale CA). An Ethernet/serial superset of
the pc532: the same NS32532 CPU cluster + DP8490 SCSI, plus an on-board DP8390
Ethernet controller and two SCC2698 octal UARTs (16 serial lines). Runs
stand-alone (jumper J4) or plugs into a 532 system over the 532SC bus (CONN1).
**Never physically built.** The MAME driver (`src/mame/homebrew/et532.cpp`) models
the stand-alone board.

Sources for this document:
- ET532 schematic, 8 sheets (Omation SCHEMA), recovered design archive
- PAL equations `KSER/PALS/{DEC32,COPETH,DRAMC,DRAMEN,WAIT}.TDL`
- `src/mame/homebrew/et532.cpp`

Legend: **[SCH]** = read from the rev-1A schematic / PAL source (authoritative).
**[MAME]** = a decision in the driver model (may not be literal rev-1A hardware).

---

## 1. CPU cluster & clocks  (schematic sheet 1: "CLOCK, CPU AND BUFFERS")

| Ref | Part | Role |
|-----|------|------|
| U13 | **NS32532** | CPU (D0-31 data bus, A0-31, BE0-3, status S0-3) |
| —   | **NS32381** | FPU (slave; `/SDONE`,`/FSSR` handshake, pull-ups R1/R2 10K) [MAME: `fpu`] |
| U12 | **40 MHz** crystal oscillator | master clock |
| U23 | 74AS00 | clock/ready gating (`/MRDY`,`/MHLDA`) |
| U14 | 74AS374 | status/BE latch |
| U20 | 74AS645 | peripheral data-bus buffer (`/PERCONF`, `DIR`=DDIN) |
| U21 | 74AS02 | NOR (BCLK/ready) |

- **BCLK = 20 MHz** (40 MHz XTAL ÷ 2). The driver clocks both NS32532 and NS32381
  at `40_MHz_XTAL/2`. **[SCH/MAME]**
- The NS32532's `/INT` and `/NMI` are the only CPU interrupt inputs (see §5).

---

## 2. Memory map  (PAL `DEC32`, PAL16R8B, sheet 2 = U24)

`DEC32` decodes the NS32532's 4 GB space into eight regions. **7 wait states are
inserted on every peripheral I/O access** (per the DEC32 / WAIT PALs).

| Range (decode) | Region | Notes |
|----------------|--------|-------|
| `0000'0000`–`03ff'ffff` | **EPROM** | read-only (`eprom = ieprom & conf & ddin`); single 8-bit 27C512 on BD0-7 |
| `0400'0000`–`07ff'ffff` | **DRAM** | (`dram = idram`; not /conf-conditioned, for speed into the clocked DRAMC PAL) |
| `0800'0000`–`0fff'ffff` | **COPS** | 93C46 serial EEPROM (Ethernet MAC) — see §4 |
| `1000'0000`–`17ff'ffff` | **PERBD** | on-board peripherals, sub-decoded by the 74F138 (§3) |
| `1800'0000`–`1fff'ffff` | **PEROBD** | off-board peripherals (532SC expansion bus, CONN1) |
| `2000'0000`–`27ff'ffff` | **IPERDMA** | SCSI/Ethernet CPU-driven pseudo-DMA window (§6) |

`PER` = `0800'0000`–`1fff'ffff` (all peripherals incl. EEPROM). The IPERDMA window
is decoded *without* `IOINH`/`IODEC` so pseudo-DMA cycles bypass normal I/O decode.

### MAME addresses actually wired (`et532.cpp` `cpu_map`)
| Address | Device |
|---------|--------|
| `0000'0000`–`0000'ffff` | EPROM (64 KB 27C512), region `"eprom"` |
| `0400'0000`–`047f'ffff` | DRAM — **8 MB** (the design default) |
| `0800'0000`            | COPS (93C46) r/w |
| `2000'0000`–`27ff'ffff` | IPERDMA pseudo-DMA |

(No ICU mapping — the ET532 has no NS32202; interrupts go straight to `/INT`, §5.)

---

## 3. On-board peripheral sub-decode  (74F138 = U30, sheet 2)

The 74F138 splits PERBD using **A22–A24** (each step = `0040'0000` = A22):

| A24 A23 A22 | 74F138 out | Base | Device |
|:-----------:|-----------|------|--------|
| 0 0 0 | `/OCT0`  | `1000'0000` | SCC2698 #0 (serial 0–7)  |
| 0 0 1 | `/OCT1`  | `1040'0000` | SCC2698 #1 (serial 8–15) |
| 0 1 0 | `/SCSI`  | `1100'0000` | DP8490 registers |
| 0 1 1 | `/SCSID` | `1140'0000` | DP8490 pseudo-DMA data strobe |
| 1 0 0 | `/ETHER` | `1180'0000` | DP8390 registers |
| 1 0 1 | `/ETHERD`| `11c0'0000` | DP8390 remote-DMA data port |

MAME register windows: OCT0/OCT1 `…+0x3f` (`scc2698b::map`); SCSI `…+0x07`
(`dp8490::map`); ETHER `…+0x0f` (`dp8390::cs_read/cs_write`); ETHERD is a single
byte port wrapping `dp8390::remote_read/remote_write` (the NIC is on the 8-bit
peripheral bus).

---

## 4. COPS — 93C46 serial EEPROM (Ethernet MAC)  (PAL `COPETH`, GAL20V8, sheet 2)

A single byte port at `0800'0000`, bit-banged. From `COPETH.TDL`:

| | write (`!ddin`) | read (`ddin`) |
|---|---|---|
| **bit 0** | `CS`  (`cs = !ddin & bd00`)  | `DO` from EEPROM (`bd00o = din`) |
| **bit 1** | `SK`  (clock, `sk = !ddin & bd01`) | — |
| **bit 2** | `DI`  (data in, `dout = !ddin & bd02`) | — |

(Selects latch and hold while `/cops` is inactive: `cs = … | cs & !cops`, etc., so
CS/SK/DI keep their last-written values between COPS accesses.)  The driver
implements exactly this — `cops_w`: `cs=BIT(0)`, `sk=BIT(1)`, `di=BIT(2)`;
`cops_r`: returns DO in bit 0.

### Programming / reading the 93C46 (Microwire)

The 93C46 is a 1 Kbit Microwire EEPROM (64 × 16; the 6-byte MAC = 3 words at word
addresses 0–2).  All access is bit-banged through the single COPS byte port at
`0800'0000`.  Useful byte values to **write**:

| want | CS | SK | DI | byte |
|---|:--:|:--:|:--:|:----:|
| idle / deselect            | 0 | 0 | 0 | `0x00` |
| select, clock low,  send 0 | 1 | 0 | 0 | `0x01` |
| select, clock low,  send 1 | 1 | 0 | 1 | `0x05` |
| select, clock high, send 0 | 1 | 1 | 0 | `0x03` |
| select, clock high, send 1 | 1 | 1 | 1 | `0x07` |

A command/data bit is shifted **into** the 93C46 on the **rising edge of SK**; DO is
sampled by **reading** the port (bit 0).  All fields are MSB-first.

**READ word N** (opcode `10`):
1. Raise CS (`0x01`).
2. Clock the 9-bit command **`1 10 aaaaaa`** — start bit `1`, opcode `10`, then the
   6-bit address.  Per bit: write the clock-low byte (DI set), then the clock-high
   byte (DI set) to pulse SK.
3. Clock out 16 data bits: per bit write `0x03` (SK↑), read the port and take bit 0
   (DO), then write `0x01` (SK↓).  (The part emits a leading dummy `0` before D15.)
4. Drop CS (`0x00`) before the next command.

Read word addresses 0, 1, 2 to get the three MAC words; the monitor then writes
them into the DP8390's physical-address registers (PAR0–5, page 1).

**WRITE** (rarely needed — the MAC is programmed once) requires the enable prefix:
**EWEN** `100 11xxxx`, then **WRITE** (opcode `01`: `1 01 aaaaaa` + 16 data bits),
then poll DO with CS re-raised — DO is low during the internal write, high when
done — then **EWDS** `100 00xxxx` to re-lock.

`COPETH` also generates the **Ethernet pseudo-DMA handshake** (separate from the
EEPROM): `rack = etherd & iord & prqs`, `wack = etherd & iowr & prqs`, and the NIC
wait `etherw = etherd & !prqs | ether & !acks` (`prqs`/`acks` = synchronized
PRQ/ACK from the DP8390).

---

## 5. Interrupts

### Real hardware (rev 1A) — **there is no NS32202 on the ET532 schematic**
Sheet 1 shows the NS32532's `/INT` and `/NMI` as the only interrupt inputs; sheet 2
("ICU, EPROM, DRAM CONTROL") gathers the sources with discrete logic (U21 74AS02,
U9 4040 counter, the WAIT PAL, U32/U23 74LS14 inverters) rather than a 32202.
Interrupt **sources** found in the schematic: **[SCH]**

| Signal | Source | Sheet |
|--------|--------|-------|
| `/SCSII`  | DP8490 SCSI interrupt | 5 (SCSI INTERFACE) |
| `/INTETH` | DP8390 Ethernet interrupt | 2/6 |
| `INTRA`,`INTRB`,`INTRC`,`INTRD` ×2 | the two SCC2698s (one per channel pair → 8 lines) | 7 (U35/U36), 8 (U33/U34) |
| `/IR1`, `/G1/IR2`, `/IR3` | off-board / 532SC expansion interrupt requests | 5 |
| `/NMI` | non-maskable | 1 |

So in stand-alone rev-1A form the design combines these into the CPU `/INT`; full
NS32202-style vectoring was evidently expected to come from the host 532's ICU over
the 532SC bus (or was not yet drawn at rev 1A — the board was never built).

### MAME model — no ICU; sources OR'd to `/INT`, software polls **[MAME]**
Matching the hardware, `et532.cpp` has **no NS32202**.  All maskable sources are
combined by an `INPUT_MERGER_ANY_HIGH` (`m_irqs`) whose output drives the NS32532
`INPUT_LINE_IRQ0` directly (active-high, no invert).  Merger input assignment:

| in | source | in | source |
|----|--------|----|--------|
| 0  | DP8490 SCSI       | 5 | SCC2698 #0 INTRD |
| 1  | DP8390 Ethernet   | 6 | SCC2698 #1 INTRA |
| 2  | SCC2698 #0 INTRA  | 7 | SCC2698 #1 INTRB |
| 3  | SCC2698 #0 INTRB  | 8 | SCC2698 #1 INTRC |
| 4  | SCC2698 #0 INTRC  | 9 | SCC2698 #1 INTRD |

The input *numbers* are arbitrary — `ANY_HIGH` simply OR's them — so they carry **no
vectoring meaning**; they exist only to give each source its own line.  The 532SC
`/IR1–3` and `/NMI` are not wired yet.

**Software model — run the NS32532 non-vectored (`CFG.I = 0`).**  Any source
asserting raises `/INT`; the CPU traps through the single fixed **NVI** vector, and
the ISR **polls the device status registers** to find the cause(s):

| Poll | Register |
|------|----------|
| DP8490 (SCSI)      | Bus-and-Status / interrupt-status registers |
| DP8390 (Ethernet)  | ISR (Interrupt Status Register), page 0 |
| SCC2698 #0 / #1    | each chip's per-block ISR (+ IPCR/MISR) → which channel pair A–D |

There is **no board-level "which-device" register** to read first — none exists in
the rev-1A schematic — so the poll order is the software's choice.  (If a later
board rev adds such a register, or a 32202 for vectored dispatch, update this
section and the driver together; the NS32000 core supports both modes via `CFG.I`.)

---

## 6. SCSI / Ethernet CPU-driven pseudo-DMA (IPERDMA, `2000'0000`–`27ff'ffff`)

Identical mechanism to the pc532: the NS32532's dynamic bus sizing + cycle
extension move bytes between the CPU and the DP8490/DP8390 without a DMAC. The
driver's `dma_r`/`dma_w` + `drq_w`/`irq_w` state machine (states IDLE / WR1-3 /
RD1-4) packs/unpacks 4 bytes per 32-bit CPU access, asserting `rdy_w` to extend the
cycle until the byte is ready. SCSI `drq_handler → drq_w`; SCSI `irq_handler` →
the interrupt merger (`in_w<0>`) **and** `irq_w` (the latter ends a pseudo-DMA
burst on phase change).

---

## 7. Device summary (MAME)

| Driver tag | Device | Address | IRQ |
|-----------|--------|---------|-----|
| `cpu`     | NS32532 @ 20 MHz | — | `/INT` from `irqs`, `/NMI` (unwired) |
| `fpu`     | NS32381 @ 20 MHz | slave | — |
| `irqs`    | INPUT_MERGER_ANY_HIGH → NS32532 `/INT` **[MAME]** | — | — |
| `dp8490`  | DP8490 SCSI (ncr5380 family) | `1100'0000` | → merge in0 |
| `dp8390`  | DP8390D Ethernet + 8 KB (6264) packet buffer | `1180'0000` / `11c0'0000` | → merge in1 |
| `eeprom`  | 93C46 (16-bit) — MAC, via COPS | `0800'0000` | — |
| `duart0`  | SCC2698B (serial 0–7) @ 3.6864 MHz | `1000'0000` | → merge in2–5 |
| `duart1`  | SCC2698B (serial 8–15) @ 3.6864 MHz | `1040'0000` | → merge in6–9 |
| `serial0`–`serial15` | RS-232 ports | — | — |

**Console = SCC2698 #0 channel A ↔ `serial0`** (the banner path). The remaining 15
channels are not yet cross-wired (driver TODO).

---

## 8. Open items / to confirm

Genuinely open:

- **Remaining 15 serial channels** and the **532SC expansion bus** (PEROBD +
  `/IR1–3`, `/NMI`) — not yet modelled in the driver.
- **True per-hole drill sizes** — not present in the gerbers (every hole flashes a
  single "drill target" aperture; sizes live only in the P-CAD binary drill table).
  Board renders use pad-derived holes.

Confirmed against `et532mon.a32` (v0.2) — no longer open:

- **No ICU / fully polled** — the monitor enables no interrupts; `putc`/`getc` poll
  the SCC2698 SR and the SCSI boot polls the DP8490 STAT1 (matches §5).
- **SCSI = programmed I/O at `1100'0000`** — the monitor drives the DP8490 directly
  (`+0` data/CSDR, `+1` ICR, `+2` MODE, `+3` TCR, `+4` STAT1) with a manual REQ/ACK
  handshake. `/SCSID` (`1140'0000`) and the IPERDMA window (`2000'0000`) are the
  *DMA* path for an OS driver — the monitor does not use them.
- **Console** = SCC2698 #0 ch A at `1000'0000`, 8N1 @ 9600 (`MR1=13`, `MR2=07`,
  `CSR=bb`, `CR=05`).

## 9. Boot monitor (`et532mon.a32`)

A small position-independent NS32k monitor (reset entry at EPROM address 0), built
with the Definicon DSI-32 toolchain — **not** pc532-derived. v0.2 commands:
`D` dump, `S` deposit, `G` go, `L` load a 512-byte block from the console, `B` SCSI
boot. After the banner it auto-tries SCSI block 0.

All paths confirmed working in MAME (2026-06-17): `D`/`G`, `L` console-load, `B`
SCSI block-0 read (programmed-I/O REQ/ACK, target 1) and the reset autoboot — a
payload loaded from either the console or a SCSI disk validates and runs.

**Boot block** (512 bytes, magic `0xe5320bb0`): `+0` magic, `+4` load addr, `+8`
start addr, `+12` length, `+16` checksum (sum of payload bytes), `+20` payload. The
SCSI boot issues READ(6) of LBA 0 (CDB `08 00 00 00 01 00`) from target 1 into a
RAM buffer, validates magic+checksum, copies the payload to the load address and
jumps to the start address.