# Xerox 820-II 16/8 Source Disk Images

This directory preserves original source media. For normal MAME booting, use the
ready-to-run images in [`../boot-disk/`](../boot-disk/).

## TeleDisk Images

The `.td0` files are Don Maslin archive images from the Sydex/TeleDisk Xerox
set. MAME can mount `.td0` images for these systems; the repaired or
reconstructed images in `../boot-disk/` are preferred when a MAME-ready boot disk
is available.

| File | Format | Use |
|---|---|---|
| `16-8sys8.td0` | 8-inch | Original system disk source for `../boot-disk/16-8sys8-boot.imd`. |
| `16-8dev8.td0` | 8-inch | Development tools. |
| `16-8dos8.td0` | 8-inch | MS-DOS-side software for the 8086 card. |
| `16-8utl8.td0` | 8-inch | Utilities. |
| `16-8cpm5.td0` | 5.25-inch | Original CP/M-80 source media for the 5.25-inch 16/8 path. |
| `16-8dev5.td0` | 5.25-inch | Development tools. |
| `16-8dos5.td0` | 5.25-inch | MS-DOS-side software for the 8086 card. |
| `16-8utl5.td0` | 5.25-inch | Utilities. |
| `emiidia5.td0` | 5.25-inch | Original EM-II source disk; use `../boot-disk/x820ii5-cpm22-rebuilt.imd` for MAME booting. |

Provenance: Don Maslin's archive, Sydex/TeleDisk collection,
`ddrive/sydex/xerox`, with internal volume dates of 1991-11-27.

## Maslin ImageDisk Set

The `maslin/*.IMD` files are development and source disks from the Don Maslin
Xerox 820-II archive mirrored on bitsavers in `820ii_images`.

| File | Contents |
|---|---|
| `maslin/B23D13.IMD` | Balcones OS v5.0 ROM source work disk; extracted under `../source/rom-v50/`. |
| `maslin/B16D38.IMD` | 816 PC system disk. |
| `maslin/B16D39.IMD` | EM-II BIOS and CP/M-80 boot source. |
| `maslin/B17D11.IMD` | ROM source disk. |

These 8-inch disks use a head-grouped double-sided layout. The extraction method
and cpmtools disk definition are documented in [`../source/README.md`](../source/README.md).

## Handling Notes

Work on a copy before mounting a writable image. MAME persists floppy writes to
mounted images when it exits.
