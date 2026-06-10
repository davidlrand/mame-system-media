# Xerox 820-II / 16-8 — the SASI rigid-disk system

How the rigid-disk (hard drive) configurations of the 820-II and 16/8 work,
established 2026-06 by emulating them in MAME (`x820iis`, `x168s`, `x1685s`)
to the point of **booting CP/M from the rigid disk**. Sources: the recovered
Balcones v5.0 firmware source ([`../source/rom-v50/`](../source/rom-v50/)),
the Shugart SA1403D OEM manual (bitsavers 39022-0, Dec 1980), the
`vintagecomputer.net` 820-II hard-disk install thread (id=400), the Xerox
MS-DOS Release .39 "Getting Started" document (on `DOS_UTIL_ASM.IMD`), and
direct disassembly of the period utilities (`FMT.COM`, `FMT40.COM`).

## The host adapter

The SASI host adapter is a Z80PIO-based card that **replaces the floppy
controller card — same slot, same I/O decode**. A rigid-equipped machine
therefore has *no* FD1797: floppies and rigid alike are served over SASI.

| ports | function |
|---|---|
| 0x10 / 0x11 | PIO A data/ctrl = the SASI **data bus** (PARDY → per-byte ACK) |
| 0x12 / 0x13 | PIO B data/ctrl = the SASI **control lines** |

Port 0x12 bits (in): 0=BSY 1=MSG 2=C/D 3=REQ 4=I/O 6=parity-err;
(out): 5=SEL 7=RST. Select sequence: data 0x01 on port 0x10, raise SEL,
poll BSY, drop SEL, then 6-byte CDBs by REQ/ACK handshake. Attested three
ways: the driver source (`sa1403.mac`: `Sasid equ pioAd`), the monitor's
in-ROM driver, and `FMT40.COM` bit-banging the same sequence.

## The controller

Behaviorally an SA1400-series (Shugart SA1403D-class) bridge controller:
up to four drives as LUNs, flat logical-block addressing,
`lba = (sector-1) + track·spt` with **1-based sectors**. The Xerox driver
source says **"Prom Set AS31"** and cites a **"DTC Reference Manual,
February 4, 1981"**, and uses a `c.init` (0Ch) command absent from the
Shugart Dec-80 manual — the Xerox unit's controller is likely a DTC build
of the design. Floppy formats are selected per-LUN with the Define-Floppy
command (0xC0); the Xerox firmware's codes are **00=SD/SS, 01=SD/DS,
06=DD/SS, 07=DD/DS** (the Dec-80 manual documents 00-03; bit 2 is a
sector-size bit in the later firmware).

LUN map (`sa1403.mac` `p2l`): floppies on LUN 0-2, rigid on LUN 3. CP/M
sees **A: = LUN0 SD · B: = LUN1 SD · C: = LUN0 DD (the same physical drive
as A:) · D: = LUN2 DD · E:-H: = the four rigid partitions**.

## The rigid disk and its layout

The 8" unit's drive is an SA1004-class: **256 cylinders × 4 heads ×
32 × 256-byte sectors = 8 MB formatted** (`FMT` initializes "Track 1023" =
1024 tracks). The 16/8's advertised "10 MB" 5.25" drive presents the same
logical space (8 MB formatted; the exact drive model is undetermined — the
Quantum Q2040 seen in `FMT40` is an 8", 40 MB, 1984 part, an add-on Xerox
was testing, *not* the 16/8 drive).

Partitioning (`rigdpb.mac`, confirmed by the period install thread):
64 KB "pseudo-tracks" of 512 CP/M records; track 0 (2 cylinders) reserved
for boot; partitions **E=64 / F=32 / G=16 / H=16 pseudo-tracks =
4 / 2 / 1 / 1 MB**, each reserving its first pseudo-track as a system
track. Partition E's system track = LBA 256-511.

## Boot and install flow

- `L` at the monitor menu = `LA` (boot floppy A:). **`L<letter>` boots a
  rigid partition** (`LE`, `LG`, ...). After a rigid boot the letters swap:
  the booted partition becomes A:.
- At signon, `lcp` reads boot/partition configuration from the rigid system
  track, **sector 32** (an E5 = factory-fresh disk is handled gracefully).
- Install flow (period doc + thread): boot a floppy → **`FMT`** (initialize
  the rigid; fills data fields with 6C) → **`PART`** (set the partition
  table / per-partition O/S ids — *PART.COM is not in any dump in hand*;
  without it the deblocker's SELDSK rejects partitions with "Invalid
  Media") → `SYSGEN` onto a partition → `PIP` the files → `L<letter>`.
  The MS-DOS side mirrors this with `RIGIDSYS.EXE` + `COPY2RIG.BAT`.
- The console of a loaded system is dispatched per the **IOBYTE** on the
  16/8 ("RX") monitor (the 820-iis monitor ignores IOBYTE). A system
  SYSGEN'd with console=TTY: boots apparently-silent, talking to the
  serial port. `CONFIGUR` the installed system for CRT.

## The 16/8 5.25" rigid machine (`x1685s`) specifics

The 16/8 names the rigid unit "Expansion Box" id **0x21** (`rgd5`; the
RX024 floppy box is 0x20) at port 0xA6. At signon `ddskld` loads the unit's
driver from a port-mapped controller ROM (ports 0xB0-0xBF, B-register
indexed; header at 0xBF: `55AA` valid mark, length, RAM load address,
diagnostic entry; the image is followed by a **checksum byte** — the
loader's `ccs` sums the declared length to zero). The driver is
**sdvr** = `seltab+dphdpb+rigdpb+sa1403` linked at F360.

The surviving `sdvr.hex` (built on dev disk B23D13) carries the **8" 
geometry inline** (77 tracks / 26 spt in `sa1403.mac`); the source
assembles **byte-identical** to it, so a 5.25" build is a 4-literal edit
(77→40, 26→17) — see [`../source/rom-v50/SDVR-NOTES.md`](../source/rom-v50/SDVR-NOTES.md).
The real rgd5 box ROM is undumped.

**Status in MAME**: `x1685s` signs on, loads the (rebuilt) driver,
identifies floppy media over SASI, and **boots CP/M 2.2 from a rigid
partition (`LE`) to an interactive A:**. The floppy *cold boot* path is the
one open gap: the shared u36 eboot reads the boot sector as "track 0
sector 0", which the FDC path resolves but flat-LBA SASI cannot; real rgd5
boot floppies must have used the eboot's other branch — probe sectors
(trk1/trk2 sec8) carrying a `6B 66 70` signature plus a relocatable monitor
overlay (→F800) that performs the SASI boot. **No rgd5 boot floppy survives
in the collection**; that overlay is the missing artifact.

## Undumped / wanted

- the rgd5 (5.25" rigid unit) controller-box ROM;
- any real rgd5 5.25" boot floppy (would supply the boot-shim overlay and
  pin the 5.25"-over-SASI floppy format — currently modeled as 17 spt);
- `PART.COM` (the CP/M-80 partition utility);
- the 16/8's actual 10 MB 5.25" drive model.
