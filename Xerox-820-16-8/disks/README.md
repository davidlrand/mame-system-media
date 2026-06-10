# Xerox 16/8 — disk images

Two sets, from two different archives.

## `*.td0` — TeleDisk images (5.25" and 8")

The standard 16/8 software set, as Sydex **TeleDisk** `.td0` images.

| File | Side | Contents |
|---|---|---|
| `16-8cpm5.td0` | 5.25" | CP/M-80 boot (the `x1685` 5.25" machine) |
| `16-8dev5.td0` | 5.25" | development tools |
| `16-8dos5.td0` | 5.25" | MS-DOS (the 8086 side of the 16/8 runs **MS-DOS**, not CP/M-86) |
| `16-8utl5.td0` | 5.25" | utilities |
| `16-8sys8.td0` | 8" | system / CP/M-80 boot (the `x168` 8" machine) |
| `16-8dev8.td0` | 8" | development tools |
| `16-8dos8.td0` | 8" | MS-DOS |
| `16-8utl8.td0` | 8" | utilities |

**Provenance:** Don Maslin's archive, in the Sydex/TeleDisk collection under
`ddrive/sydex/xerox`:
<https://ia801903.us.archive.org/view_archive.php?archive=/9/items/don_maslin_archive/maslin_archive.zip>
(`maslin_archive.zip` at `archive.org/details/don_maslin_archive`). TeleDisk
images with internal volume dates of 1991-11-27.

**Note:** MAME does not mount `.td0` directly — convert to `.imd` or `.mfi`
first, e.g. with Dave Dunfield's `TD02IMD`, or `samdisk`/`HxC`. On the 8" `x168`
the `16-8sys8` disk boots Balcones CP/M-80 to `A>` with `-bios v50`. The 5.25"
`x1685` does not yet boot in MAME (its CP/M boot reads the MFM track 0 of
`16-8cpm5` in FM and loops — an open driver issue, not a media problem).

## `maslin/*.IMD` — Don Maslin archive (ImageDisk)

Development/source disks from the **Don Maslin Xerox 820-II archive** mirrored on
bitsavers (`bitsavers/.../820ii_images`, spreadsheet index `820ii_images.xls`).
ImageDisk `.IMD` format. These are the disks that made the bring-up possible.

| File | Contents |
|---|---|
| `B23D13.IMD` | **v5.0 ROM source work disk** — the full Balcones OS v5.0 source + the four assembled boot ROMs. Extracted to [`../source/rom-v50/`](../source/rom-v50/). |
| `B16D38.IMD` | "816 PC" system disk (8086 / CP/M-86 era) |
| `B16D39.IMD` | EM-II BIOS + boot source (`DEBLKR1.MAC`, `CHARIO.MAC`, `CWBOOT.MAC`, `BIOS0-1.HEX`, `CPSYS0-1.HEX`) |
| `B17D11.IMD` | ROM source (DUCC) |

**Reading them:** these are double-sided 8" disks with a head-grouped track
layout; see [`../source/README.md`](../source/README.md) for the `imd2flat.py`
tool and the verified cpmtools `x88grp` diskdef. Some Maslin 8" images have 78
cylinders, one past MAME's 77-track 8" maximum; MAME tolerates the extra track
(it is ignored on load).

---

**Work on a copy.** MAME persists writes back to mounted floppy images on exit
and can alter or corrupt them. Copy or `chmod -w` an image before mounting if you
want to keep the original bits.
