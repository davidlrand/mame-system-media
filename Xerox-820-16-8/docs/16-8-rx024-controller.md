# Xerox 16/8 — the RX024 5.25" disk controller

The low-profile 16/8 (the common one — 5.25" drives, the G25 low-profile
keyboard, optional 5.25" rigid disk) boots its floppies through a controller the
v5.0 firmware calls **"RX v024"**. Internally the system treats this as the
**Expansion Box II** configuration. This note records how the v5.0 monitor
brings that controller up, and the multi-layer debugging that got `x1685` from
"no disk at all" to a working controller path. The controller's own ROM is *not*
in any dump I know of — but its **driver source survives** on the Balcones
development disk B23D13, so the path is reconstructable.

Everything here is read off the recovered v5.0 monitor source
([`../source/rom-v50/xr.mac`](../source/rom-v50/)) and confirmed by running the
sequence under MAME with the controller ROM reconstructed from that driver
source.

## How the monitor finds and loads the 5.25" driver (`ddskld`)

At cold start the monitor runs `ddskld` (boot-ROM offset `0x90`). The RX024
controller presents a small ROM in the **Z80 I/O space**, addressed with the B
register as the high byte and a fixed low byte — `in a,(c)` with `bc = 0xXXBF`
walks it. `ddskld`:

1. Reads a **validation word** from the top of that ROM (`0FFBFh`, `0FEBFh`) and
   checks it equals **`0x55AA`** (`valnum`). If not, it falls through to the
   low-profile-keyboard path and the controller is treated as absent.
2. Reads an **id byte** from **port `0A6h`** (`expbx`): `20h` = 5.25" floppy,
   `21h` = 5.25" rigid.
3. Sets the configuration byte accordingly — `flpy5id = 14h` =
   **`c.five` (5.25") | `c.eb2` (Expansion Box II)** — so the cold boot later
   takes the `eboot` (sector-0) path.
4. Reads a **driver length** and **load address** from the ROM header, copies the
   driver image into RAM at **`0xF360`** (block-by-block through ports `0B0h`+),
   and **checksums** it — the loaded bytes must sum to `0` (mod 256), else
   `Rom Error`.
5. Reads a **diagnostic entry address** from the ROM (`0F8BFh`, `0F7BFh`) and
   `jp`s to it. **Return convention: `NZ` = success** (proceed to keyboard
   init → sign-on → cold boot); **`Z` = disk error** (delay and retry via `prs`).

The loaded driver is the **WD1797 floppy driver** — exactly the source on B23D13:
`seltab + dphdpb + fivdpb + wd1797 + wdvr`, linked at `0xF360` (see
[`../source/rom-v50/driver.sub`](../source/rom-v50/) and `wd1797.mac` /
`wdvr.hex`). It drives the **same main-board FD1797** the 820-II already uses
(ports `0x10`–`0x13`, drive-select `0x1C`, density `0x30`/`0x31`).

## The controller, modelled as a real MAME device

In MAME the controller ROM is reconstructed as a proper port-mapped device (no
runtime synthesis): the genuine Balcones WDVR driver image (`wdvr.hex`, the
linked `seltab+dphdpb+fivdpb+wd1797+wdvr` at `0xF360`) wrapped in the EPROM
packaging the `ddskld` contract above describes — `valnum 0x55AA` at the `xxBF`
high ports, length / load-address / diagnostic-entry header, and the driver body
through the `0xB0`+ block ports, plus `expbx = 0x20` at port `0xA6`. This is
reconstruction of the EPROM *packaging* only; the driver bytes are the real,
unmodified Balcones code.

With that in place `x1685` passes `ddskld` validation, loads and checksums the
driver, sets `confg = 0x1C` (`c.five | c.eb2 | c.lpkb`), and reaches the **real
RX024 boot menu**:

```
  1984 Xerox Corp v 5.00 (RX v024)
L - Load System
H - Host Terminal
*
```

So the controller-bring-up half is sound. The 8" 16/8 (`x168`) boots CP/M to
`A>`; the 5.25" `x1685` reaches this menu and the work below carries it through
the disk login.

## The `A:Load error` — what it actually was (the long way round)

Pressing **`L`** (Load System) used to return **`A:Load error`**. The cause is
worth recording in full because it was *not* what it first looked like, and the
wrong theories cost a long detour.

**It was the head reading the empty side of the disk.** The loadable WDVR
driver's disk-login (`SELECT`, command `-1`/`0xFF`) examines the media for
density and drive type before it reads anything. On `x1685` the FD1797 was
reading **side 1**, which on a single-sided disk is blank — so no flux, no sync,
no density could be detected, and the login bailed. Dumping the raw track buffer
the floppy hands the FDC at cylinder 2 made it unambiguous:

```
x820ii5 (boots):   m_ss=0  cells=38956   ← side 0, full track
x1685   (failed):  m_ss=1  cells=0       ← side 1, EMPTY
```

The trigger was a side-effect of how `x1685` attaches its single drive. On the
16/8 the floppy lives in the RX024 expansion box, which routes the drive — so
the main-board system-PIO **drive-select bits do not mux it** the way they do on
a base 820-II. To model that, the `x1685` machine pins the FD1797 to `floppy0`.
But the v5.0 monitor's keyboard-ROM banking writes the system PIO continuously
with values like `0x3F`/`0xBF`, and **bit 2 of that port is the FD1797 side
select** — so once the drive was pinned, those banking writes flipped the head
to side 1. (On the base `x820ii5`, the same writes select the *second* drive, so
the boot drive's side is never disturbed — which is exactly why `x820ii5` worked
and `x1685` didn't.)

**Fix:** the expansion-box drive's side is not wired to the menu's bit 2, so the
machine pins side 0 (`kbpio_pa_w`: `ss_w(m_fdc_single_floppy ? 0 : BIT(data,2))`).
After it, `x1685` reads side 0 / full track, the `A:Load error` is gone, and the
density-detect login **completes** (the old SEEK + `0x1C`-verify + force-interrupt
retry loop disappears). `x820ii5 -bios v404 -flop1 cpm22 → A>` is unaffected.

### What it was NOT (recorded so the detours aren't repeated)

- **Not** a "boot-sector convention" (sector-0 vs sector-1) mismatch — an
  earlier theory, now disproven.
- **Not** the FDC clock. Measured: `m_fdc->unscaled_clock()` = 1.000 MHz on both
  `x1685` and `x820ii5`. (There is a cosmetic `16_MHz/16` vs `20_MHz/20`
  inconsistency between `machine_reset` and `kbpio_pa_w`; both resolve to 1 MHz.)
- **Not** an analog-PLL phase-lock pathology. The FD1797 uses the analog data
  separator (`fdc_pll.cpp`); instrumenting it showed a clean 2 µs frequency lock,
  and a deliberately injected half-bit-cell read-start phase shift changed
  nothing — correctly killing the phase theory. The flux looked "different" only
  because the head was on the empty side.

**Lesson:** when every measurable quantity is identical but behaviour differs,
dump the actual data (here, the track buffer) before theorising. That one log
line settled in minutes what clock/period/PLL analysis could not.

## The cold-boot loader and the 16/8 disk format

With the login fixed, `x1685` advances to the **cold-boot loader** — `eboot`
reads the cold-start loader from **track 0, sector 0** (the Expansion-Box-II
`c.eb2` convention; a base 820-II reads sector 1) at the detected (MFM) density,
then loads CCP+BDOS and the BIOS. This is the current frontier: the boot needs a
disk written in the **16/8's own boot layout**, which the surviving base-820-II
images are not.

The boot loader's own format table (`qfs.mac`) enumerates the 5.25" formats it
accepts — **all MFM**:

| record-spt | physical | note |
|---|---|---|
| `36` | 9 × 512 | IBM / CP/M-86 format |
| `32` | 8 × 512 | |
| `34` | 17 × 256 | base-820-II-style (also valid) |

and `deblkr.mac` notes the IBM side-numbering (*"IBM PC numbers them from 79 to
40"*) with a 512-byte host buffer. So the 16/8 5.25" disks moved to the **IBM
all-MFM family** the 8086 side uses — there is no FM track 0. A surviving cousin,
`X820II.IMD`, is exactly this family: **MFM, 512-byte, sector 0 present, IBM
continuous side-numbering** (side 0 = sectors 0-9, side 1 = 10-19) — but it is
the Kaypro **10**×512 variant, not the 16/8's 9×512, and its sector 0 holds a
SYSGEN'd CCP rather than a bootstrap, so it does not boot. It proves the format
is real, though.

`cwboot.mac` gives the boot-track layout (eb2 path): the cold loader at sector 0,
CCP+BDOS = `(ccplen+bdosln)/128 = 44` records starting at **sector 8**, then the
BIOS. **Next step:** build a real 16/8 boot disk from the recovered Balcones
binaries — `boot0` at track-0 sector-0, `cpsys0` (CCP+BDOS) deblocked from sector
8, `bios0` — in a valid all-MFM format (256×17 passes the login today; 9×512 is
the native IBM choice), rather than reusing a base-820-II track 0.

## Provenance / status

- **Controller ROM:** not dumped; not documented in the Technical Reference.
- **Driver:** recovered source on B23D13 (`rom-v50/wd1797.mac` et al.), Balcones,
  © 1981. `wdvr.hex` is the assembled floppy driver linked at `0xF360`.
- **Reconstruction:** the I/O-port ROM wrapper around the genuine driver is
  recreated from the `ddskld` read contract; the driver bytes themselves are the
  real Balcones code — reconstruction of the EPROM packaging only.
- **Status (2026-06-08):** controller bring-up and the disk login work on
  `x1685`; the cold-boot loader awaits a correctly-built 16/8 boot disk.

## Symbols

Addresses are the real Balcones labels from the v5.0 assembler listing
[`../source/rom-v50/xr.prn`](../source/rom-v50/) (M80 symbol table). Note that
`x1685`'s actual `u36.rx024` ROM is a *different* v5.0 build from the listing —
the boot/disk region (`$FA00`–`$FCFF`) differs — so the `$FAxx`/`$FCxx` addresses
below are the Typewriter-build references, not `x1685`'s; `u33`–`u35` (including
`ddskld` and `xqdvr` at `$F344`) are byte-identical and valid everywhere.

| Symbol | Addr | Role |
|---|---|---|
| `DDSKLD` | `0090` | load the 5.25" driver from the controller ROM |
| `VALNUM` | `55AA` | controller-ROM validation word |
| `EXPBX` | `00A6` | controller id port (`FLPY5`=`20`, `RGD5`=`21`) |
| `CCS` | `0124` | driver checksum (loaded bytes must sum to 0) |
| `TSTRET` | `00F4` | diagnostic return point (`NZ`=ok, `Z`=disk error→retry) |
| `LPKBD` | `00FD` | low-profile-keyboard init → `SIGNON` |
| `CONFG` | `F091` | configuration byte (`c.five`\|`c.eb2`\|`c.lpkb` = `0x1C`) |
| `SIGNON` | `FC52` | sign-on (the RX024 boot menu) |
| `EBOOT` | `FCDE` | Expansion-Box-II cold boot (reads track 0 **sector 0**) |
| `XQDVR` | `F344` | execute physical driver request (u35, valid on `x1685`) |
| `SELTAB` | `F360` | loaded driver base; `Dvrtab` at `F380` → `Dskdvr` at `F4B0` |

The WDVR `SELECT` (command `-1`) is the disk login that examines the media; the
`Dskdvr` entry table lives at the loaded-driver base `0xF360`.
