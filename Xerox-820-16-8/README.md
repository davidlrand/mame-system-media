# Xerox 820-II 16/8

![Xerox 16/8 running CP/M in MAME](xerox-16-8-cpm-mame.png)

This directory is the operating media and reference set for the **Xerox 16/8**:
a Xerox 820-II Z80 CP/M-80 system with the optional **"816 PC"** 8086
coprocessor card for CP/M-86 or MS-DOS. It also covers the related 820-II disk
subsystems used by the 16/8: 8" floppy, 5.25" floppy, SASI rigid disk, and the
Expansion Module II WD1002-05 ST-506 rigid/floppy unit.

This README is an operating guide: which MAME machine to run, which disk image
to mount, which monitor command to type, and how to move between CP/M-80 and
CP/M-86.

## Directory Layout

| Folder | Contents |
|---|---|
| [`roms/`](roms/) | Z80 monitor ROMs, 8086 board ROM, character generators, and keyboard ROMs, with CRC/SHA1 provenance. |
| [`disks/`](disks/) | Original TeleDisk and ImageDisk media sets. Use these as source media; work on copies. |
| [`boot-disk/`](boot-disk/) | Reconstructed and repaired bootable IMD/CHD images for MAME. Start here for normal use. |
| [`source/`](source/) | Extracted Balcones OS v5.0 ROM source, CP/M-86 BIOS material, 8086 ROM disassembly, and tools. |
| [`docs/`](docs/) | Architecture and disk subsystem references: keyboard, 8086 mailbox, RX024 5.25" controller, boot-disk layout, and SASI rigid system. |

## Before Running

Use a copy of any writable disk image. MAME writes floppy and hard-disk changes
back to mounted images on exit. The CHD examples in `boot-disk/` are stored as
`.chd.gz`; decompress a working copy before mounting it with MAME.

Examples below assume you are running MAME from `/Users/dlr/src/mame` and this
media tree is at:

```sh
/Users/dlr/src/github-davidlrand/mame-system-media/Xerox-820-16-8
```

Set a shell variable if you want shorter commands:

```sh
MEDIA=/Users/dlr/src/github-davidlrand/mame-system-media/Xerox-820-16-8
```

At the Xerox monitor prompt, the usual boot commands are:

| Monitor command | Meaning |
|---|---|
| `L` | Boot the default floppy device. |
| `LA` | Boot floppy drive/partition A. Used by the EM-II floppy path. |
| `LE` | Boot rigid partition E as CP/M drive A:. |

## Quick Start

### Xerox 16/8, 8" floppy (`x168`)

Boot the repaired 8" system disk:

```sh
./mame x168 -flop1 "$MEDIA/boot-disk/16-8sys8-boot.imd"
```

At the monitor prompt:

```text
L
```

This boots CP/M-80 to `A>`.

To start CP/M-86 on the 8086 card:

```text
LOAD86
86CON
```

The prompt changes to the CP/M-86 `a>` prompt. To return to CP/M-80:

```text
GOBACK
```

### Xerox 16/8, 5.25" floppy (`x1685`)

Boot the reconstructed 5.25" CP/M-80 disk:

```sh
./mame x1685 -flop1 "$MEDIA/boot-disk/x1685-cpm22-boot.imd"
```

At the monitor prompt:

```text
L
```

To use the CP/M-86-capable 5.25" boot disk instead:

```sh
./mame x1685 -flop1 "$MEDIA/boot-disk/x1685-cpm86-boot.imd"
```

Then boot with `L`, and at `A>` use:

```text
LOAD86
86CON
```

Use `GOBACK` at the CP/M-86 prompt to return to CP/M-80.

### Xerox 16/8, SASI rigid disk (`x168s`)

Prepare and boot the CP/M-86-capable rigid-disk CHD:

```sh
gunzip -k "$MEDIA/boot-disk/x1685s-cpm86.chd.gz"
./mame x168s -hard "$MEDIA/boot-disk/x1685s-cpm86.chd"
```

At the monitor prompt:

```text
LE
```

This boots rigid partition E as CP/M drive A:. To start CP/M-86:

```text
LOAD86
86CON
```

Use `GOBACK` to return to CP/M-80.

You can also mount a floppy in `-flop1` for file transfer while booting the rigid
disk:

```sh
./mame x168s \
  -hard "$MEDIA/boot-disk/x1685s-cpm86.chd" \
  -flop1 "$MEDIA/boot-disk/16-8sys8-boot.imd"
```

### Xerox 820-II, SASI rigid disk (`x820iis`)

`x820iis` is the 820-II SASI rigid-disk system without the 8086 coprocessor. Use
it for CP/M-80 SASI testing and disk maintenance.

```sh
./mame x820iis -hard <sasi-rigid.chd> -flop1 "$MEDIA/boot-disk/16-8sys8-boot.imd"
```

Use `L` to boot the floppy or `LE` to boot rigid partition E, depending on the
image you are testing. See [`docs/sasi-rigid-system.md`](docs/sasi-rigid-system.md)
for the SASI partitioning and install flow.

### Xerox 16/8 Expansion Module II (`x168em`)

`x168em` is the 16/8 with the Expansion Module II: the 8086 card plus a WD1002-05
5.25" ST-506 rigid/floppy controller and the 537P3682 box ROM.

Boot the reconstructed EM-II CP/M-80 floppy:

```sh
./mame x168em -flop1 "$MEDIA/boot-disk/x820ii5-cpm22-rebuilt.imd"
```

At the monitor prompt:

```text
LA
```

To boot the example ST-506 rigid image, decompress a working copy, mount it with
`-hard`, and use `LE` at the monitor prompt:

```sh
gunzip -k "$MEDIA/boot-disk/x168em_cpm86_clean.chd.gz"
./mame x168em \
  -hard "$MEDIA/boot-disk/x168em_cpm86_clean.chd" \
  -flop1 "$MEDIA/boot-disk/x820ii5-cpm22-rebuilt.imd"
```

```text
LE
```

The original EM-II floppy image is also preserved at
[`disks/emiidia5.td0`](disks/emiidia5.td0). Use the reconstructed image in
[`boot-disk/`](boot-disk/) for ordinary booting and maintenance work.

`LOAD86`, `86CON`, and `GOBACK` are the CP/M-86 control commands when the mounted
system disk contains the required 8086-side files and BIOS support.

## Media Reference

### Bootable Images

| File | Use with | Boot command | Result |
|---|---|---|---|
| [`boot-disk/16-8sys8-boot.imd`](boot-disk/16-8sys8-boot.imd) | `x168` | `L` | CP/M-80 from 8" floppy; includes the working `LOAD86`/`86CON` path for CP/M-86. |
| [`boot-disk/x1685-cpm22-boot.imd`](boot-disk/x1685-cpm22-boot.imd) | `x1685` | `L` | CP/M-80 from reconstructed 5.25" floppy. |
| [`boot-disk/x1685-cpm86-boot.imd`](boot-disk/x1685-cpm86-boot.imd) | `x1685` | `L` | CP/M-80 from 5.25" floppy with CP/M-86 tools installed. |
| [`boot-disk/x1685s-cpm86.chd.gz`](boot-disk/x1685s-cpm86.chd.gz) | `x168s` | `LE` | Compressed SASI rigid image; decompress to `x1685s-cpm86.chd` before mounting. |
| [`boot-disk/x820ii5-cpm22-rebuilt.imd`](boot-disk/x820ii5-cpm22-rebuilt.imd) | `x168em` | `LA` | CP/M-80 from reconstructed EM-II 5.25" floppy. |
| [`boot-disk/x168em_cpm86_clean.chd.gz`](boot-disk/x168em_cpm86_clean.chd.gz) | `x168em` | `LE` | Compressed EM-II ST-506 rigid image with CP/M-86 support; decompress before mounting. |
| [`disks/emiidia5.td0`](disks/emiidia5.td0) | `x168em` | `LA` | Original EM-II 5.25" floppy source image. |

### Original TeleDisk Set

The `disks/*.td0` files are the original TeleDisk software set. MAME can mount
`.td0` directly for these machines.

| File | Media | Contents |
|---|---|---|
| [`disks/16-8sys8.td0`](disks/16-8sys8.td0) | 8" | 16/8 CP/M-80 system disk. |
| [`disks/16-8dev8.td0`](disks/16-8dev8.td0) | 8" | Development tools. |
| [`disks/16-8dos8.td0`](disks/16-8dos8.td0) | 8" | 8086-side DOS media. |
| [`disks/16-8utl8.td0`](disks/16-8utl8.td0) | 8" | Utilities. |
| [`disks/16-8cpm5.td0`](disks/16-8cpm5.td0) | 5.25" | 16/8 CP/M-80 system disk. |
| [`disks/16-8dev5.td0`](disks/16-8dev5.td0) | 5.25" | Development tools. |
| [`disks/16-8dos5.td0`](disks/16-8dos5.td0) | 5.25" | 8086-side DOS media. |
| [`disks/16-8utl5.td0`](disks/16-8utl5.td0) | 5.25" | Utilities. |
| [`disks/emiidia5.td0`](disks/emiidia5.td0) | 5.25" | Expansion Module II CP/M-80 boot floppy. |

### Maslin ImageDisk Set

The `disks/maslin/*.IMD` files are source and development disks from the Don
Maslin Xerox 820-II archive. They are most useful for extracting source and
system components rather than as day-to-day boot media.

| File | Contents |
|---|---|
| [`disks/maslin/B23D13.IMD`](disks/maslin/B23D13.IMD) | Balcones OS v5.0 ROM source and assembled boot ROMs. |
| [`disks/maslin/B16D38.IMD`](disks/maslin/B16D38.IMD) | 816 PC system disk. |
| [`disks/maslin/B16D39.IMD`](disks/maslin/B16D39.IMD) | EM-II BIOS and boot source material. |
| [`disks/maslin/B17D11.IMD`](disks/maslin/B17D11.IMD) | ROM source disk. |

## CP/M-86 Command Sequence

On a CP/M-80 system disk with CP/M-86 support installed:

1. Boot CP/M-80 to `A>`.
2. Run `LOAD86` to load the 8086-side CP/M-86 system.
3. Run `86CON` to transfer the console to CP/M-86.
4. At the CP/M-86 `a>` prompt, run `GOBACK` to return to CP/M-80.

The sequence is:

```text
A>LOAD86
A>86CON
a>GOBACK
```

## System Notes

### Keyboards

The 16/8 uses the Xerox low-profile keyboard. In MAME, natural keyboard input is
mapped so normal host keys produce the expected typewriter-paired characters,
including Ctrl-letter combinations.

The X928 ASCII keyboard used by other 820-II configurations is documented in
[`docs/x928-keyboard.md`](docs/x928-keyboard.md).

### 5.25" Reconstructed Boot Disk

The reconstructed `x1685` and `x168em` 5.25" boot disks are documented in
[`boot-disk/README.md`](boot-disk/README.md) and
[`docs/16-8-boot-disk-format.md`](docs/16-8-boot-disk-format.md). Use the
reconstructed images in [`boot-disk/`](boot-disk/) for ordinary MAME booting.

### SASI Rigid Disk

The SASI rigid system, partition layout, and install flow are documented in
[`docs/sasi-rigid-system.md`](docs/sasi-rigid-system.md). The important monitor
command for a rigid partition boot is `LE`.

### Expansion Module II

The EM-II path uses MAME system `x168em`, floppy media on `-flop1`, and an
optional ST-506 CHD on `-hard`. Boot the reconstructed EM-II floppy with `LA`;
boot the rigid partition with `LE`.

## Provenance

- ROMs: MAME `x168`, `x820ii`, and `x820kb` ROM sets; hashes are recorded in
  [`roms/README.md`](roms/README.md).
- TeleDisk media: Don Maslin archive, Sydex/TeleDisk collection
  (`ddrive/sydex/xerox` in `don_maslin_archive` on archive.org).
- ImageDisk media: Don Maslin Xerox 820-II images, mirrored on bitsavers.
- Balcones OS v5.0 firmware source: extracted from Maslin disk B23D13.
- Keyboard, 8086 mailbox, RX024, SASI, and EM-II notes: recovered and documented
  as part of this project.

The firmware source carries `Copyright (C) 1981 by Balcones Computer Corporation`.
The material here is preserved for emulation and historical/technical reference.

## References

- [`docs/16-8-architecture.md`](docs/16-8-architecture.md) - 8086 card, shared RAM, and CP/M-86 handoff.
- [`docs/16-8-boot-disk-format.md`](docs/16-8-boot-disk-format.md) - reconstructed 5.25" boot disk format.
- [`docs/16-8-rx024-controller.md`](docs/16-8-rx024-controller.md) - RX024 5.25" controller reconstruction.
- [`docs/sasi-rigid-system.md`](docs/sasi-rigid-system.md) - SASI rigid-disk adapter, partitions, and install flow.
- [`docs/x928-keyboard.md`](docs/x928-keyboard.md) - X928 ASCII keyboard interface.
- [`source/README.md`](source/README.md) - extracted source disks and tools.
