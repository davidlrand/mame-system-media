# Xerox 820-II 16/8 Boot Media

This directory contains the reconstructed and repaired boot media that are ready
to use with the Xerox 820-II 16/8 family in MAME. For original source images and
development disks, see [`../disks/`](../disks/).

Use an absolute media path when running MAME from another directory:

```sh
MEDIA=/Users/dlr/src/github-davidlrand/mame-system-media/Xerox-820-16-8
```

The CHD examples are stored as `.chd.gz`. Decompress a working copy before
mounting a CHD with MAME.

## Bootable Images

| Image | MAME system | Mount as | Monitor command | Result |
|---|---|---|---|---|
| `16-8sys8-boot.imd` | `x168` | `-flop1` | `L` | 8-inch CP/M-80 system disk; includes the working CP/M-86 loader set. |
| `x1685-cpm22-boot.imd` | `x1685` | `-flop1` | `L` | 5.25-inch CP/M-80 boot disk. |
| `x1685-cpm86-boot.imd` | `x1685` | `-flop1` | `L` | 5.25-inch CP/M-80 boot disk with the CP/M-86 loader set added. |
| `x1685s-cpm86.chd.gz` | `x168s` | `-hard` after decompression | `LE` | Compressed SASI rigid-disk image with CP/M-86 tools installed. |
| `x820ii5-cpm22-rebuilt.imd` | `x168em` | `-flop1` | `LA` | Reconstructed EM-II/base 820-II-compatible CP/M-80 boot disk. |
| `x168em_floppy_build.imd` | `x168em` | `-flop1` | `LA` | Correct-and-proper EM-II CP/M-80 boot floppy; not present in the Maslin archive. |
| `x168em_cpm86_clean.chd.gz` | `x168em` | `-hard` after decompression | `LE` | Compressed EM-II ST-506 rigid-disk image with CP/M-86 support. |

## Run Commands

### 8-inch 16/8 (`x168`)

```sh
./mame x168 -flop1 "$MEDIA/boot-disk/16-8sys8-boot.imd"
```

At the monitor prompt:

```text
L
```

To start CP/M-86 on the 8086 card:

```text
LOAD86
86CON
```

To return from CP/M-86 to CP/M-80:

```text
GOBACK
```

### 5.25-inch 16/8 (`x1685`)

```sh
./mame x1685 -flop1 "$MEDIA/boot-disk/x1685-cpm22-boot.imd"
```

At the monitor prompt:

```text
L
```

For the CP/M-86-capable 5.25-inch image, mount `x1685-cpm86-boot.imd` instead
and use the same CP/M-86 commands:

```text
LOAD86
86CON
GOBACK
```

### SASI rigid-disk 16/8 (`x168s`)

```sh
gunzip -k "$MEDIA/boot-disk/x1685s-cpm86.chd.gz"
./mame x168s -hard "$MEDIA/boot-disk/x1685s-cpm86.chd"
```

At the monitor prompt:

```text
LE
```

Then use `LOAD86`, `86CON`, and `GOBACK` as above.

### EM-II / base 820-II-compatible floppy (`x168em`)

```sh
./mame x168em -flop1 "$MEDIA/boot-disk/x820ii5-cpm22-rebuilt.imd"
```

At the monitor prompt:

```text
LA
```

For the example EM-II ST-506 rigid disk:

```sh
gunzip -k "$MEDIA/boot-disk/x168em_cpm86_clean.chd.gz"
./mame x168em \
  -hard "$MEDIA/boot-disk/x168em_cpm86_clean.chd" \
  -flop1 "$MEDIA/boot-disk/x820ii5-cpm22-rebuilt.imd"
```

At the monitor prompt:

```text
LE
```

## Image Notes

`16-8sys8-boot.imd` is the repaired MAME-ready form of Don Maslin's
`16-8sys8.td0` 8-inch system disk. The repaired image trims duplicate sector
records from tracks that MAME's IMD loader rejects, while preserving the usable
sector data. This is the reference image for `x168` CP/M-80 and CP/M-86 use.

`x1685-cpm22-boot.imd` is a reconstructed 5.25-inch CP/M-80 system disk for
`x1685`. It combines the 5.25-inch track layout and data area with the v5-era
boot sector, BIOS, CCP, and BDOS needed by the 16/8 monitor path.

`x1685-cpm86-boot.imd` starts from `x1685-cpm22-boot.imd` and adds the CP/M-86
loader and utility set from `16-8sys8-boot.imd`: `LOAD86.COM`, `CPM86.COM`,
`86CON.COM`, `PIP.CMD`, `STAT.CMD`, `SUBMIT.CMD`, `HELP.CMD`, and
`GOBACK.CMD`.

`x1685s-cpm86.chd.gz` is the compressed distribution copy of a SASI rigid-disk
image prepared from a bootable CP/M-80 system and populated with the same
CP/M-86 loader set. Decompress it to `x1685s-cpm86.chd` before mounting it.

`x820ii5-cpm22-rebuilt.imd` is the reconstructed floppy boot image to use with
`x168em`. The original EM-II source disk is preserved as `../disks/emiidia5.td0`;
this rebuilt image is the convenient floppy boot form for MAME.

`x168em_floppy_build.imd` is a correct-and-proper EM-II boot floppy for
`x168em`. Unlike the other floppy images here, it does not derive from the
Maslin TeleDisk archive; it was built for MAME.

`x168em_cpm86_clean.chd.gz` is the compressed distribution copy of an EM-II
ST-506 rigid-disk image with CP/M-86 support installed. Decompress it to
`x168em_cpm86_clean.chd` before mounting it with `x168em -hard`.

## Construction Notes

Work on copies of these files. MAME persists floppy writes to mounted writable
images when it exits.

The 5.25-inch CP/M-80 disk uses 41 cylinders. Track 0 is FM 18 x 128 bytes;
data tracks are MFM 17 x 256 bytes. The double-sided image includes formatted
empty side-1 tracks because the donor directory is laid out for the double-sided
2K-block DPB.

For files extracted from `16-8sys8-boot.imd`, address sectors by sector ID, not
by physical record position. The source image contains repaired tracks, and a
position-based extraction can silently shift data after the first repaired track.
