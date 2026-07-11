# Xerox 820-II 16/8 Source Material

This directory contains extracted source and disassembly material for the Xerox
820-II 16/8 firmware and operating-system support code.

## Contents

| Path | Contents |
|---|---|
| `rom-v50/` | Complete Balcones OS v5.0 RX Z80 monitor source tree from developer disk B23D13. |
| `8086/8086.u33.asm` | Disassembly of the 816 PC 8086 coprocessor boot ROM. |
| `cpm86-bios/` | Balcones XEROX 820+ Z80 CP/M-80 BIOS source from developer disk B16D39. |

## RX Monitor Source

`rom-v50/` is the source tree for the v5.0 Z80 boot monitor used by the 16/8.
The monitor lives in four 2 KB boot ROMs, `u33` through `u36`.

| File(s) | What it is |
|---|---|
| `xr.mac`, `xr.prn`, `xr.rel` | Top-level RX monitor source, listing, and M80 relocatable object. |
| `wd1797.mac` and related files | WD1797 floppy driver. |
| `sa1403.mac` and related files | Shugart SA1403D / SASI rigid-disk controller driver. |
| `dphdpb.mac`, `fivdpb.mac`, `rigdpb.mac` | Disk Parameter Headers and Disk Parameter Blocks. |
| `seltab.mac` | Drive-select table and sector translation data. |
| `driver.sub` | M80/L80 build script. |
| `u33-500.rom` through `u36-v18.rom` | Assembled boot ROM binaries from the development disk. |
| `rom500.com`, `yrom.com`, `do.com`, `if.com` | ROM build and burn helper utilities. |
| `*.hex` | Intel HEX driver images. |

The assembled ROM binaries extracted from B23D13 match the known MAME v5.0 ROM
dumps for the corresponding parts.

## Reading the Maslin 8-inch Source Disks

The Maslin 8-inch development disks are double-sided, double-density disks with
an FM track 0 and MFM data tracks. Logical tracks are grouped by head: all head-0
cylinders first, then all head-1 cylinders. They are not interleaved by cylinder.

To extract a disk for cpmtools, first de-interleave it to a grouped flat image:

```sh
python3 imd2flat.py B23D13.IMD b23d13.flat grouped
```

Use this cpmtools disk definition for double-sided 8-inch images:

```text
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

Example extraction:

```sh
cpmls -f x88grp b23d13.flat
cpmcp -f x88grp b23d13.flat 0:xr.mac xr.mac
```

For single-sided 8-inch images, use `tracks 77` with the same remaining fields.
The disk definition matches the firmware DPB in `dphdpb.prn`: 26 physical
256-byte sectors per track, 4K allocation blocks, 128 directory entries, and two
reserved boot tracks.
