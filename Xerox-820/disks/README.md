# Xerox 820 Family Disk Images

Disk images imported from `~/Downloads/xerox.7z`, organized by identified target
family and container format.

Per-image media sizes are listed in `MEDIA-SIZES.md`.

## Xerox 820-I

`820-I/teledisk/` contains `.td0` images with 820-I-style names, including system
and diagnostic disks. The directory contains both 5.25-inch and 8-inch media.
Use with MAME `x820`.

MAME boot-screen checks:

- `x820` - `820-I/teledisk/820sys5.td0`
- `x820` - `820-I/teledisk/sssd.td0`

## Xerox 820-II

`820-II/imagedisk/` contains 49 `.imd` images. ImageDisk headers and embedded
CP/M strings identify these as Xerox 820-II / Rank Xerox system, application,
language, communications, diagnostics, and accounting disks. These mount in MAME
with `x820ii`. The track maps identify the populated images as 8-inch media.

`820-II/teledisk/` contains `.td0` images with 820-II-style names. They are kept
in original TeleDisk form. The `*5.td0` names correspond to `x820ii5`; the
`*8.td0` names correspond to `x820ii`. The directory contains both 5.25-inch
and 8-inch media.

MAME boot-screen checks:

- `x820ii5` - `820-II/teledisk/8202cpm5.td0`
- `x820ii5` - `820-II/teledisk/8202sis5.td0`
- `x820ii5` - `820-II/teledisk/5sys-ii.td0`
- `x820ii5` - `820-II/teledisk/5dsys-ii.td0`
- `x820ii` - `820-II/imagedisk/Xerox 820-II CP-M v2.2 Rev 1.000 (1982)(Xerox)[Part Number 130S22203, Code 2Q82].imd`
- `x820ii` - `820-II/imagedisk/RX v2.2E CP-M (19xx)(-).imd`
- `x820ii` - `820-II/imagedisk/RX Base Reference System (19xx)(-)[Floppy Version].imd`
- `x820ii` - `820-II/imagedisk/RX Base Reference System (19xx)(-)[Rigid Version].imd`
- `x820ii` - `820-II/imagedisk/Accounting Plus v5.10 (19xx)(-)(Disk 1 of 13)(System Control Disk).imd`
- `x820ii` - `820-II/imagedisk/Xerox 820-II SC.COM etc (19xx)(-)[Master].imd`
- `x820ii` - `820-II/imagedisk/CP-M86 Diagnostics (19xx)(-).imd`
- `x820ii` - `820-II/imagedisk/UK System Checker (19xx)(-)(GB).imd`
- `x820ii` - `820-II/imagedisk/dBase II CPM v2.2 SD Master (19xx)(-).imd`

## Other Xerox-Labeled Media

`other-xerox/860/teledisk/` contains Xerox 860-labeled `.td0` images.

`other-xerox/1800/teledisk/` contains `1800-p.td0`.

## Related Xerox 16/8 Media

The Xerox 16/8 material from the same archive is filed under
`../Xerox-820-16-8/boot-disk/` and `../Xerox-820-16-8/disks/`, including:

- `16-8sys8-boot.imd`
- `x1685-cpm22-boot.imd`
- `x1685-cpm86-boot.imd`
- `x1685s-cpm86.chd`
- `x820ii5-cpm22-rebuilt.imd`
- `B16D38.IMD`, `B16D39.IMD`, `B17D11.IMD`, `B23D13.IMD`
- the `16-8*.td0`, `16-8cpm7.uue`, and `emiidia5.td0` set
