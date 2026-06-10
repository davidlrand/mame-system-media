# Xerox 16/8 — v5.0 ("RX") ROM source & 8086 disassembly

## What's here

`rom-v50/` — the complete source tree for the **Balcones Operating System v5.0**
Z80 boot monitor (the "RX" monitor the 16/8 ships with), recovered from the
original development floppy **B23D13**. This is the firmware that lives in the
four 2 KB boot ROMs `u33`–`u36`.

| File(s) | What it is |
|---|---|
| `xr.mac` / `xr.prn` / `xr.rel` | Top-level RX monitor: cold/warm boot, console, the CP/M-80 BIOS jump table, transient command processor. `.mac` = source, `.prn` = assembler listing (with addresses+opcodes), `.rel` = M80 relocatable object. |
| `wd1797.mac` … | Western Digital WD1797 floppy disk driver. |
| `sa1403.mac` … | Shugart SA1403D / SASI rigid-disk controller driver. |
| `dphdpb.mac` … | Disk Parameter Headers + Disk Parameter Blocks (all media formats). |
| `fivdpb.mac` / `rigdpb.mac` | 5.25" floppy and rigid-disk parameter blocks. |
| `seltab.mac` … | Drive-select table + the `Trn6` single-density sector translate table. |
| `driver.sub` | The M80/L80 build script that assembles and links the above. |
| `u33-500.rom` … `u36-v18.rom` | The four assembled boot ROM binaries, as stored on the dev disk. |
| `rom500.com`, `yrom.com`, `do.com`, `if.com` | ROM-burn / build helper utilities. |
| `*.hex` | Intel-HEX driver images (`wdvr.hex`, `sdvr.hex`). |

`8086/8086.u33.asm` — disassembly of the 8086 coprocessor ("816 PC") boot ROM.
See [`../docs/16-8-architecture.md`](../docs/16-8-architecture.md).

> `wd1797.mac` (+ `seltab`/`fivdpb`/`dphdpb`, linked at `0xF360` per
> `driver.sub`, image `wdvr.hex`) is also the **5.25" RX024 disk driver** — the
> code the low-profile 16/8's controller ROM loads at boot. See
> [`../docs/16-8-rx024-controller.md`](../docs/16-8-rx024-controller.md) for how
> the monitor loads and validates it, and how it's used to bring `x1685` up to
> the RX024 system menu under MAME.

All `.mac` and `.prn` files verified coherent end-to-end (no truncation, no
mis-ordered blocks). The four `.rom` binaries extract to CRCs that **match
MAME's known-good v5.0 dumps exactly** (see Provenance below) — independent
confirmation the extraction is byte-accurate.

## Provenance

- **Disk:** B23D13, from the Don Maslin Xerox 820-II disk archive mirrored on
  bitsavers (`bitsavers/.../820ii_images`, index `820ii_images.xls`). The raw
  image is preserved alongside this tree at
  [`../disks/maslin/B23D13.IMD`](../disks/maslin/).
- **Headers:** the source carries `Copyright (C) 1981 by Balcones Computer
  Corporation` — Balcones wrote the 820-II / 16/8 firmware for Xerox.
- **Extraction:** done from the raw IMD by this project (see below). Not
  previously published in extracted form, as far as I know.

## How the disk was read (the diskdef)

These 8" disks are **double-sided, double-density** with an FM track 0 and MFM
data — and the side layout is the subtle part. Xerox CP/M numbers logical tracks
as **all of head 0 first (cyl 0–76), then all of head 1 (cyl 0–76)**, *not* head-
interleaved per cylinder. Get that wrong and the directory plus small/low-block
files still read, but large multi-track files come out with mis-ordered or
erased-fill (`0xE5`) blocks. That cost me a wrong first attempt; the corrected
recipe:

1. **De-interleave to a flat image, grouped by head** with `imd2flat.py`:
   ```
   python3 imd2flat.py B23D13.IMD b23d13.flat grouped
   ```
   It sorts each track's sectors by ascending ID, pads the FM track-0 128-byte
   sectors to 256, and emits all head-0 cylinders followed by all head-1
   cylinders (1,025,024 bytes = 154 × 26 × 256).

2. **Read with this cpmtools diskdef** (`x88grp` — append to
   `/usr/local/share/diskdefs`):
   ```
   diskdef x88grp
     seclen    256
     tracks    154
     sectrk    26
     blocksize 4096
     maxdir    128
     skew      0
     boottrk   2
     os        2.2
   end
   ```
   ```
   cpmls -f x88grp b23d13.flat
   cpmcp -f x88grp b23d13.flat 0:xr.mac xr.mac
   ```

This diskdef is **confirmed against the firmware's own DPB** (`dphdpb.prn`,
`dpb8d` 8"-double-density-double-side entry): `spt 2*26=52` 128-byte records
(= sectrk 26 × seclen 256), `dsm 246`, `drm 127` (maxdir 128), `off 2`
(boottrk 2), directory-alloc `0C0h`, which gives blocksize 4096. `skew 0`
because the interleave is already carried in the physical sector IDs.

For **single-sided** 8" disks use `tracks 77`, the same other parameters
(`x88grp` with the grouped flat degenerates correctly).
