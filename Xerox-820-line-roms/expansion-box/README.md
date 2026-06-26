# Xerox 820-II / 16-8 Expansion-Box Controller ROMs

The 16/8's 5.25" disk expansion is the **DEM (Disk Expansion Module / "Expansion
Box II" / EB2)** — a **Western Digital WD1002-05** controller (WD1010
Winchester+floppy combo), driven register-level through a task file at I/O ports
**0xA8-0xAF**. Its box ROM is **dumped**: [`537p3682.rom`](537p3682.md), the
unified WD1002-05 driver (floppy on drive units 0-3, rigid on 4-7).

## DEM configurations

One WD1002-05 box, one box ROM (`537p3682`), two drive complements. The v5.0
monitor's `ddskld` reads the box ID at port 0xA6 and loads the driver from the
box's paged ROM (ports 0xB0-0xBF) identically for both; the ID only records
whether a rigid is present:

- **flpy5** — floppy-only DEM. Box ID **0x20** → config `flpy5id` **0x14**
  (`c.five`+`c.eb2`); two 5.25" floppies.
- **rgd5** — rigid + floppy DEM ("16/8 5.25" rigid unit"). Box ID **0x21** →
  config `rgd5id` **0x24** (`c.rgd5`+`c.eb2`); one 5.25" ST-506 rigid (Shugart
  "712", 11 MB) + one 5.25" floppy.

`537P3682`'s `SELECT` dispatches by drive number (<4 → floppy via `SMF`, >=4 →
rigid); `DSKDRV`@`$F4C1` matches the DEM's `INIT.MAC`. See
[`537p3682.md`](537p3682.md). `537p3682` is dumped from the rgd5-config material;
a floppy-only DEM may ship a stripped build, but none has been dumped separately.

## Related drivers (separate subsystems, not the DEM)

- **WDVR / `WD1797.MAC`** — the WD1797 driver for the **main-board** floppies
  (8", and the non-EB2 5.25" daughterboard). Not an expansion box.
- **SDVR / `SA1403.MAC`** — the Shugart **SA1403D** SASI driver for the **820-II
  8" 8 MB rigid box** (`c.sasi` config) — a different expansion box, box ROM
  undumped.

Source for both is published in
[`../../Xerox-820-16-8/source/rom-v50/`](../../Xerox-820-16-8/source/rom-v50/).
