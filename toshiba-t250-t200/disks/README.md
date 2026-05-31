# Sample disk images

Bootable CP/M 2.2 disk images, uncompressed and ready to mount in MAME.

## Contents

### T-250 (8" DSDD)

| File | Description |
|------|-------------|
| [`T-250/Toshiba-original-v2.20.mfi`](T-250/Toshiba-original-v2.20.mfi) / [`.hfe`](T-250/Toshiba-original-v2.20.hfe) | Original Toshiba-shipped CP/M 2.20 boot disk |
| [`T-250/boot-2.14g.mfi`](T-250/boot-2.14g.mfi) / [`.hfe`](T-250/boot-2.14g.hfe) | Earlier BIOS revision 2.14g, with a substantial collection of CP/M utility programs |
| [`T-250/Toshiba-T250-demo.mfi`](T-250/Toshiba-T250-demo.mfi) / [`.hfe`](T-250/Toshiba-T250-demo.hfe) | Older BIOS revision 2.12a with a simple text-mode demo program that exercises the T-250's graphics capabilities |
| [`T-250/boot1024-font.mfi`](T-250/boot1024-font.mfi) / [`.hfe`](T-250/boot1024-font.hfe) | Modern BIOS revision 2.32, **1024-byte sector** format.  Includes a collection of font-loading programs (HP41, a sideways font, and others) plus `char.com`, the font editor that demonstrates real-time VPG static-RAM updates on the T-250 video hardware. |
| [`T-250/newboot.mfi`](T-250/newboot.mfi) / [`.hfe`](T-250/newboot.hfe) | Modern BIOS 2.32 boot disk with modified CCP (path search + archive integration).  Pair with `newbios` in `-flop2`.  See [Boot+BIOS pair](#bootbios-pair-newboot--newbios) below. |
| [`T-250/newbios.mfi`](T-250/newbios.mfi) / [`.hfe`](T-250/newbios.hfe) | BIOS 2.32 source disk, configured for the 32 MB CDC Finch hard drive.  Sources live in user-area 5 (`B5:`).  See [Boot+BIOS pair](#bootbios-pair-newboot--newbios) below. |

### T-200 (5.25" DSDD, 35-track)

| File | Description |
|------|-------------|
| [`T-200/t200-boot-v2.21.mfi`](T-200/t200-boot-v2.21.mfi) / [`.hfe`](T-200/t200-boot-v2.21.hfe) | Stock Toshiba-shipped CP/M BIOS V2.21 boot disk |
| [`T-200/toshiba-v2.21-original-ser-10133.mfi`](T-200/toshiba-v2.21-original-ser-10133.mfi) / [`.imd`](T-200/toshiba-v2.21-original-ser-10133.imd) | Second V2.21 distribution disk, serial 10133.  Same BIOS revision as the entry above with minor content differences. |
| [`T-200/boot-v2.30a-1024.mfi`](T-200/boot-v2.30a-1024.mfi) / [`.imd`](T-200/boot-v2.30a-1024.imd) | BIOS **V2.30a** boot disk — adds 1024-byte and 512-byte sector support beyond V2.21's 256-byte limit, giving higher-capacity disks (T-200 type 4-7 in the format table). |
| [`T-200/trianex-boot-v3.0.mfi`](T-200/trianex-boot-v3.0.mfi) / [`.hfe`](T-200/trianex-boot-v3.0.hfe) | Trianex V3.0 BIOS boot disk.  Trianex was a Canadian reseller of the Toshiba line; their BIOS added hard-drive support via their own interface and protocol (different from the SASI/ACB-4000 path used by other T-200 systems).  Boots cleanly under MAME; the HD formatter does not run because the Trianex disk-controller hardware is not yet emulated.  Trianex ceased operations around August 1985. |

Two formats per disk:
- **`.mfi`** — MAME Floppy Image.  Read AND write inside MAME; suitable for adding files, formatting, etc.
- **`.hfe`** — HxC Floppy Emulator flux-level format.  Use with GOTEK/FlashFloppy hardware or the HxC Floppy Emulator software.  Read AND write.
- **`.imd`** — Dave Dunfield's ImageDisk format.  Widely supported by vintage CP/M tooling (cpmtools, libdsk, IMDU).  Some disks (V2.30a, V2.21 ser-10133) ship in `.imd` rather than `.hfe` because their use case is emulation + filesystem inspection rather than Gotek hardware replay.

## Boot+BIOS pair: `newboot` + `newbios`

`newboot.mfi` and `newbios.mfi` are a working pair.  `newboot` is the boot disk; `newbios` carries the BIOS assembly source and tools so you can rebuild a customised BIOS without leaving the emulator.

### Launch

```sh
mame t250 -window -flop1 newboot.mfi -flop2 newbios.mfi \
   -sasi:0 dtc510 -hard1 tosh32.chd
```

The `-sasi:0 dtc510 -hard1 tosh32.chd` part is **required** if you intend to touch drives C:, D:, E: or F:.  `newboot` and `newbios` were built with hard-drive support compiled in (`hard equ 1`); accessing a hard-drive letter with no SASI controller attached will hang the BIOS in its `hconsel` selection loop.  Floppy-only use (`A:` and `B:`) doesn't need the SASI flags.

### What's on `newboot`

A fully working floppy-or-hard-drive boot.  Includes Dave's modified CCP with two extensions:

1. **Path search.**  When you type a command, the CCP walks back through user areas to `A0:` looking for `.COM` files.  No need to keep utilities duplicated across user areas.
2. **Archive lookup.**  After the path search, the CCP also looks inside `A0:progs.dir` / `progs.arc`.  This is a CP/M archive bundle managed by Dave's `arcdir` / `arcget` / `arccopy` / `arcdel` / `arctype` programs.  Files inside the archive are **directly executable** from the CCP prompt — no need to extract them first.

To list what's in the archive:

```
A> arcdir progs
```

### What's on `newbios`

The BIOS 2.32 source tree, configured for the 32 MB CDC Finch hard drive (`m32` build variant).  When mounted as drive `B:`, the sources are in user area 5 (`B5:`):

```
B> user 5
B5> dir
```

### Rebuilding the BIOS from source

The standard build sequence (run inside CP/M from `B5:`):

```
mac bios64 $pz sz -m         ; assemble bios64.asm
ddt shard2.com               ; load the writer stub
ibios64.hex                  ; ingest the .hex output
r4580                        ; relocate
g0                           ; run/finalize
save 50 shard2.com           ; persist as a fresh .COM
```

`shard2.com` is the tool that writes the new BIOS to a mounted floppy (drive `A:` or `B:`).

## Preparing the hard-disk image (32 MB CDC Finch)

`newboot` and `newbios` expect a 32 MB Finch-class drive at SCSI ID 0.  The BIOS sees four CP/M slices on it: `C:` (2.6 MB), `D:` (8 MB), `E:` (8 MB), `F:` (6 MB).

Create a blank image pre-filled with the CP/M empty-directory marker (`0xE5`) — this matches what Dave's original low-level format utility did on real hardware:

```sh
# 1. Build a 32 MB raw image full of 0xE5
python3 -c "
with open('tosh32.raw','wb') as f:
    f.write(b'\xe5' * (32 * 1024 * 1024))
"

# 2. Convert to a CHD with 256-byte sectors (CDC Finch sector size,
#    matches what the T-250 'not inch5' BIOS expects via hget's
#    64-long × 4-byte reads).  Geometry is arbitrary for emulation;
#    1024 × 8 × 16 × 256 B = exactly 32 MB.
chdman createhd -o tosh32.chd -i tosh32.raw -chs 1024,8,16 -ss 256

# 3. Verify
chdman info -i tosh32.chd | grep -E "Logical|Unit|CYLS"
#   Logical size:   33,554,432 bytes
#   Unit Size:      256 bytes
#                   CYLS:1024,HEADS:8,SECS:16,BPS:256.

# 4. (optional) delete the raw file
rm tosh32.raw
```

(For a T-200 5.25" Adaptec ACB-4000 setup, use `-ss 512` instead — the T-200 `inch5` BIOS uses 128-long × 4-byte reads = 512 B sectors.  `chdman` is shipped with MAME; it's in the same directory as the `mame` binary.)

Once the CHD is built, attach it as shown in the Launch command above.  CP/M will see four empty drives (C, D, E, F).  Use `PIP C:=A:*.COM` (or similar) to populate.

## Quick start

Launch in MAME:

```sh
mame t250 -window -flop1 T-250/Toshiba-original-v2.20.mfi
```

For real hardware via GOTEK/FlashFloppy, copy the `.hfe` to the USB stick.

## Format coverage

The MAME driver accepts a number of standard floppy formats: `.imd` (read-only), `.mfi` (read/write), and various MFM containers.  The HxC `.hfe` format is the most portable for both emulation and real-hardware replay via GOTEK.

`.imd` (Dave Dunfield's ImageDisk format) is widely supported by vintage CP/M tools.  MAME's `floptool` cannot write IMD; if you need it, convert via [Greaseweazle](https://github.com/keirf/greaseweazle):

```sh
gw convert T-250/Toshiba-original-v2.20.hfe T-250/Toshiba-original-v2.20.imd
```

Or via the HxC Floppy Emulator GUI's export function.

## Capturing fresh disks from real hardware

Use [Greaseweazle](https://github.com/keirf/greaseweazle):

```sh
# T-250 (8" DSDD)
gw read --drive=A --tracks=c=0-76:h=0-1 --format=ibm.scan disk.scp
gw convert disk.scp disk.imd
# or
gw convert disk.scp disk.hfe

# T-200 (5.25" DSDD)
gw read --drive=A --tracks=c=0-34:h=0-1 --format=ibm.scan disk.scp
gw convert disk.scp disk.imd
```

## Disk format reference

See [`../docs/disk-formats.md`](../docs/disk-formats.md) for the complete list of all 16 supported disk formats (8 per machine) with cpmtools / libdsk / FlashFloppy definitions.

## What's missing

- Hard-disk CHD samples — covered separately in the MAME usage guide; not bundled here.
