# Xerox 820-II / 16-8 Expansion-Box Controller ROMs

The **rgd5 / EM-II box ROM is dumped**: [`537p3682.rom`](537p3682.md) — the
**WD1002-05** hard-disk/floppy driver, not a SASI/SA1403D ROM. The **RX024
(5.25" floppy) box ROM is undumped**. Nothing here should be mistaken for a dump
unless it is documented as one.

## The two boxes

- **RX024 box** — the v5.0-era 5.25" floppy expansion box. Its controller
  ROM is read by the host through ports 0xB0-0xBF (select latch at 0x1C);
  the v5.0 monitor's `ddskld` loader pulls the WDVR (WD1797) floppy driver
  out of it at boot.
- **rgd5 box / EM-II (Disk Expansion Module, product F89)** — the 5.25" rigid
  disk unit ("Expansion Box II", XR rev 500). Box ID 0x21 at port 0xA6,
  controller ROM at ports 0xB0-0xBF; `ddskld` loads its driver from it. The
  controller is a **Western Digital WD1002-05** (WD1010 Winchester+floppy combo,
  register-level task file at I/O ports **0xA8-0xAF**), driving a 5.25" ST-506
  rigid + 5.25" floppies. Its driver ROM is **dumped**:
  [`537p3682.rom`](537p3682.rom) / [`537p3682.md`](537p3682.md)
  (`DSKDRV`@`$F4C1`, matching the EM-II's `INIT.MAC`).

## What MAME uses instead

The MAME driver (`src/mame/xerox/xerox820.cpp`, `rx024_rom_r()` /
`rgd5_rom_r()`) serves **reconstructions** of these ROMs, generated from the
recovered Balcones driver source — not from dumps:

- `WDVR.HEX` / `WD1797.MAC` — the WD1797 physical floppy driver
  (v4.0-era build `6c4b0e06` on B16D35; v5.0-era `2cc863eb` on B16D40/B23D13)
- `SDVR.HEX` / `SA1403.MAC` — the SA1403D SASI physical driver
  (`11468ce3`, identical on B16D35/B16D40/B23D13)

The published copies of that source and the assembled driver images are in
[`../../Xerox-820-16-8/source/rom-v50/`](../../Xerox-820-16-8/source/rom-v50/)
(`sdvr.hex`, `wdvr.hex`, `sa1403.mac`, `sa1403-525.mac`, `wd1797.mac`,
`sdvr5.bin`, plus the linker pieces). The drivers are authentic period
Balcones code; only the box-ROM *packaging* around them (header/loader
glue actually burned into the box controller ROMs) is reconstruction.

If a real RX024 or rgd5 box surfaces, its controller ROM(s) should be dumped
and placed here, and the MAME reconstruction retired.
