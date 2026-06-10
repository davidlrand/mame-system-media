# Xerox 16/8 — 5.25" boot-disk format and reconstruction

> **SOLVED (2026-06-09).** `x1685` boots Balcones CP/M 2.2 to an interactive
> `A>` from the reconstructed disk in [`../boot-disk/`](../boot-disk/)
> (`x1685-cpm22-boot.imd`). The final, verified layout is in the
> **"The solved layout"** section at the end of this document; the sections
> between record the (partly superseded) investigation that led there and are
> kept for provenance.

Once the RX024 controller path and the disk login work (see
[`16-8-rx024-controller.md`](16-8-rx024-controller.md)), `x1685` reaches the
**cold-boot loader**: `eboot` reads the cold-start loader from **track 0, sector
0** (the Expansion-Box-II `c.eb2` convention) at the detected MFM density, then
loads CCP+BDOS and the BIOS. To finish the boot it needs a disk in the 16/8's own
5.25" layout. This note records the format and the search for an existing one.

## The native 16/8 5.25" format

The boot loader's own format table (`source/rom-v50/qfs.mac`) accepts these
5.25" geometries, **all MFM** (no FM track 0):

| records/track | physical | role |
|---|---|---|
| 36 | **9 × 512** | the native 16/8 / IBM CP/M-86 format |
| 32 | 8 × 512 | |
| 34 | 17 × 256 | base-820-II-style (also accepted) |

`source/rom-v50/deblkr.mac` confirms the IBM lineage (*"IBM PC numbers them from
79 to 40"*, 512-byte host buffer). Every genuine 16/8 5.25" disk in hand
(`16-8cpm5`, `16-8dos5`, `16-8dev5`, `16-8utl5`) is **9 × 512 MFM**, e.g. cpm5
skew `[1,8,6,4,2,9,7,5,3]`. So the physical format is settled: **9 sectors of 512
bytes, MFM, double density**.

`source/cpm86-bios/cwboot.mac` gives the boot-track layout for the eb2 path:

- the **cold-start loader** at **sector 0**;
- **CCP + BDOS** = `(ccplen + bdosln)/128 = (0x800 + 0xE00)/128 = 44` records,
  starting at **sector 8**;
- then the **BIOS**.

(Records are 128 bytes, deblocked onto the 512-byte physical sectors — 4 records
per sector — by `deblkr.mac`.)

## Is there already a bootable disk? — full repository scan

Scanned all 390 IMD images under `~/src/mame/x820disks/` plus the converted 8-16
TeleDisk set, looking for **MFM track 0 with a sector 0** (the bootstrap
prerequisite). Results:

- **The real 16/8 5.25" disks** (`cpm5`, `dos5`, `dev5`, `utl5`, and variants) are
  9 × 512 MFM but **1-based** — sectors 1-9, **no sector 0**. They are CP/M-86
  *data* disks; they have no Z80 cold-start loader and do not boot CP/M-80.
- **Only three disks carry a sector 0**, all the Kaypro **10 × 512** MFM format
  (IDs `0,8,3,6,1,9,4,7,2,5`, IBM continuous side-numbering):
  `bitsavers/X820II.IMD`, `XPRO.IMD`, `XPROLST.IMD`. Under `x1685`:
  - `XPRO` / `XPROLST` **pass the login and reach the cold-boot loader** (no
    `A:Load error`) — proof that a 512-byte, sector-0 disk reads correctly — but
    their sector 0 is Emerald Microware XPRO (a Z80 card for the IBM PC) software,
    not a 16/8 bootstrap, so they stall instead of booting.
  - `X820II` fails earlier, on its track-2 login read (its sector 0 is a SYSGEN'd
    CCP).

**Conclusion:** no ready-made CP/M-80 boot disk exists in the collection. The
controller, login, and 512-byte sector-0 *read path* are all proven; what is
missing is purely the **content** — a real 16/8 cold-start loader and system laid
out on a correctly-formatted track 0.

## Reconstruction plan

Build a disk from the recovered Balcones binaries
(`source/cpm86-bios/`: `boot0.hex`, `cpsys0.hex`, `bios0.hex`) in the native
**9 × 512 MFM** geometry (the cpm5 skew is the reference):

1. **Track 0** carries a **sector 0** (unlike the 1-based data tracks): place
   `boot0` (the cold-start loader) there; CCP+BDOS (44 × 128-byte records) from
   sector 8; then `bios0`.
2. **Data tracks** 1-based (sectors 1-9), 9 × 512 MFM.
3. Honour the `cwboot.mac` sector-zero translate (`transz`) and the deblocker's
   record→sector mapping.

## The authoritative format: from the ROM's media selector

The ROM's disk-recognition code is the definitive source. The WDVR `SELECT`
command's media-format selector `smf` (`source/rom-v50/wd1797.mac`) recalibrates,
probes density (MFM then FM), then probes side-1 for two-sided media, and selects
a DPB from `fivdpb.mac`. The 5.25" DPB table defines **exactly two floppy
formats**:

| DPB | spt | sector | notes |
|---|---|---|---|
| `dpb5s` | 18 | **128** | FM, single density throughout |
| `dpb5d` | 17 (×2 records) | **256** | MFM data tracks, **track 0 single density (FM)** — flag byte `81h` |

So the CP/M-80 5.25" boot format is **`dpb5d`: FM 18 × 128 on track 0, MFM 17 ×
256 on data tracks** — *not* a 512-byte format. The 9 × 512 (`cpm5`) and 10 × 512
(`XPRO`) disks are the **8086 / IBM side**; the Z80 monitor's `smf` does not select
them. This matches the base-820-II 5.25" geometry; the only 16/8 difference is the
`c.eb2` **sector-0** boot convention (vs sector 1).

This is the format `seed16_8` uses (FM 18 × 128 track 0 with a sector 0, MFM 17 ×
256 data) — and `seed16_8` **passes the login**. boot0's 130 bytes are 128 of
loader code (`0x1600`–`0x167F`) plus two `00` padding bytes, so it fits one
128-byte track-0 sector.

## The cold-boot loader, disassembled from x1685's own u36 ROM

The recovered `boot0.hex`/`cwboot.mac` are from the *source-listing* build; x1685's
actual boot ROM is **`u36.rx024.rom`** (CRC `a7f1d677`, maps `F800`–`FFFF`, confirmed
by the menu strings landing at the right offsets), and **its boot region is a
different build** from the recovered source. Its Load-System routine has the
"Load error" string at `FFA5` and the error path at `FDC3`.

What is **established** (FDC trace + screen, independent of disassembly detail):

- the cold boot reads the system image from **MFM tracks 1 and 2, sector 8** (host
  sector 8) into a scratch buffer at **`ED80`** — *not* track-0 sector-0, and *not*
  via the recovered `boot0`;
- it then **rejects an `0xE5` first byte**: the routine does `ld a,(ED80); cp E5;
  jp z,FDC3` → "Load error". Confirmed empirically — `boot16_8_v2` has `E5` at
  track-2 sec8 and errors there; `seed16_8` has cpm22 data (non-E5) and clears that
  gate (then fails a later one);
- it **relocates** the loaded image into the `F800`/`F960`/`F0F8` region and patches
  jump vectors through a table at `(FA08)` — i.e. it expects a **SYSGEN'd boot
  image**, not loose CCP+BDOS.

**Caveat on the deeper disassembly:** the internal ROM-call targets (`F024`, `F01B`,
etc.) do *not* resolve cleanly against either u35 dump (`F344` is not the `xqdvr`
the symbol table claims; `F024` reads as a hex-digit converter and differs between
the two u35 images), so the exact step-by-step read/relocate sequence is **not yet
reliably reverse-engineered**. The three bullets above are solid; the byte-exact
image layout is not.

### Why each build failed (confirmed)

| disk | track2 sec8[0] | result |
|---|---|---|
| `boot16_8_v2`/`v3` | **`E5`** | FEF3 fires → **A: Load error** |
| `seed16_8` (cpm22 content) | `B4` (non-E5) | clears FEF3, fails a *later* gate → hangs |

My builds put the system on track 0 + left track 2 empty (`E5`); the loader never
reads track 0 and the `E5` at track 2 sec8 is exactly the byte FEF3 rejects.

## Open work: the loader content

The format is settled; the remaining problem is **content**. `seed16_8` reuses
cpm22's base-820-II system on its data tracks, so boot0 loads the wrong system
and stalls. A correct disk needs `boot0` at track-0 sector 0 and the recovered
**`cpsys0`** (CCP+BDOS, 44 records) laid out per `cwboot.mac` (system from sector
8), deblocked onto the `dpb5d` 256-byte data tracks (bios0 is ROM-resident). The
build is now fully specified by `fivdpb.mac` + `cwboot.mac` + `deblkr.mac`.

*(Earlier notes here speculated a 10 × 512 0-based "boot format" because XPRO
passes the login; that was a wrong turn — XPRO is a Kaypro disk and the 512-byte
formats are the 8086 side. The ROM's `fivdpb` is authoritative: 256-byte
`dpb5d`.)*

*(`seed16_8` — a cpm22-derived 256-byte experiment — is **not** a real 16/8 disk;
it was scaffolding for testing the controller path and should not be published as
media.)*


## The solved layout (2026-06-09, verified by a booting disk)

The decisive evidence was disassembling the **shipping warm boot** inside the
known-good 8" BIOS (`16-8sys8` track 1 sectors 23-26, the 1 KB image qfs loads
at `E980`). The recovered `cwboot.mac` listing is a *different revision* — its
`eb2wbt` sector-8 walk does **not** exist in the shipping build, and chasing it
cost several disk-build iterations.

**The shipping CCP+BDOS walk** (disassembled at `EA69`+):

- `dmabas = D380` (constant); the walk covers logical 128-byte sectors with
  `DMA = D380 + s·128`, so **logical sector 1 ↦ D400 = CCP record 0**;
- DD disks start at **track 1, logical sector 1** (SD at track 0); FDC sector
  = `(s >> 1) + 1`, half = `s & 1`;
- live-verified memory map: **CCP = D400, BDOS = DC00, BIOS = E980**
  (jump table at **EA00**: cold `EAF1`, warm `EA69`, console vectors `F0xx`).

**Resulting disk layout** (41 cyl; FM 18×128 track 0; MFM 17×256 data):

| where | content |
|---|---|
| trk0 (FM), sector id **0** | the universal `qfs` boot sector (the RX024 eboot reads id 0; the base monitors read id 1 — the reconstructed disk carries it at **both**, so one disk boots `x1685` *and* `x820ii5`) |
| trk0 sector 3 | configuration sector (`lcp`) |
| trk1 sec n | CCP+BDOS records `2n-3` (lower half) and `2n-2` (upper) — sec 1 lower half unused, **sec 1 upper = CCP record 0** |
| trk2 sec 1-5 | records `2n+31` / `2n+32` (the BDOS tail) |
| trk2 sec 6-9 | the 1 KB BIOS block — sec 6's lower half doubles as the walk's final record (it lands at E980), so the qfs block and the walk interlock |
| trk3+ | directory + files (OFF = 3) |

**Content sources** (every byte accounted for):

- boot sector: `16-8sys8` trk0 sec1, verbatim (byte-identical on all the
  system disks except the serial string);
- BIOS: `16-8sys8` trk1 sec 23-26, verbatim — the BIOS is driver-agnostic
  (all disk I/O through the monitor's `xqdvr`), so the same image serves
  8" and 5.25" machines;
- CCP+BDOS: the recovered `cpsys0.hex`/`cpsys1.hex` pair. The two are the
  same code linked 0x100 apart, so **their byte-diff is a complete
  page-relocation bitmap** (1073 bytes, zero mismatches); applying +0xD4
  pages relocates to D400. Verified: the relocated CCP entry
  (`C3 5C D7`) is byte-identical to the working `cpm22` disk's.

**Boot-pair rule** (cost a day to learn): a boot sector and BIOS must come
from the *same lineage*. The shipping pair enters the BIOS at `EA00`
(xbios = 0x80 bytes); the pair rebuilt from the recovered `.mac`/`.rel`
sources enters at `E980` (its xbios is 0x34 bytes) *and* its modules are
revision-skewed against each other. Never mix; prefer the shipping binaries.

**Two-machine note**: this disk boots `x1685` (RX024 eboot, sector-0
convention) and `x820ii5` (base monitor, sector-1) — but **not** `x1685s`
(the SASI rigid machine): its floppies live behind the SASI controller in a
different addressing world. See
[`sasi-rigid-system.md`](sasi-rigid-system.md).
