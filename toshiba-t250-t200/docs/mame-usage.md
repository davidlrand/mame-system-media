# Using the T-250 / T-200 ROMs and disks with MAME

The Toshiba T-250 and T-200 are emulated in upstream MAME by the `t250` and `t200` drivers in [`src/mame/toshiba/t250.cpp`](https://github.com/mamedev/mame/blob/master/src/mame/toshiba/t250.cpp).

## Prerequisites

1. **MAME** built from source or installed via a package manager. MAME 0.290 or later (driver added 2026).
2. **The ROM files** from [`../rom/`](../rom/) of this repo:
   - `t250boot.bin` — T-250 boot ROM
   - `t200boot.bin` — T-200 boot ROM

## ROM placement

MAME looks for ROMs in a `roms/<systemname>/` directory under its working directory or one of the configured ROM paths:

```sh
mkdir -p roms/t250 roms/t200
cp /path/to/this/repo/rom/t250boot.bin roms/t250/
cp /path/to/this/repo/rom/t200boot.bin roms/t200/
```

Verify with:
```sh
mame -verifyroms t250
mame -verifyroms t200
```

Both should report `romset t250 [or t200] is good`.

## Running with a floppy disk

Pick a bootable disk image from [`../disks/`](../disks/) or use one of your own. Both `.imd` (read-only) and `.mfi` (read-write) formats are supported.

```sh
# T-250 with bootable CP/M floppy
mame t250 -window -flop1 path/to/t250-cpm.imd

# T-200 with bootable CP/M floppy  
mame t200 -window -flop1 path/to/t200-cpm.imd
```

For best display on Mac, add:
```sh
-window -nomaximize -resolution 800x640
```

## Serial port

The CCM (Communications) board's 8251 USART is wired to the standard MAME RS-232 slot. Default baud is 19200, 8N1, but the on-disk `SET.COM` utility changes it. The cleanest exact rates on this hardware are 9600 and below (19200 actually runs at ~20800 due to the 1.9968 MHz baud clock).

### Pseudo-terminal (interactive, recommended)

```sh
mame t250 -window -flop1 boot.imd -rs232 pty
```

MAME announces the pty path on startup (e.g. `[:rs232:pty] Pty slave is /dev/ttysNNN`). From another terminal:

```sh
screen /dev/ttysNNN 9600
```

Type in the screen session — characters arrive on the T-250's serial RX. The T-250's serial TX appears in the screen session. Ctrl-A K exits screen.

### File transfer (one-way)

```sh
# Send a file out the serial port
mame t250 -window -flop1 boot.imd -rs232 null_modem -bitb host.txt
# host.txt MUST already exist (touch host.txt first)
# Then on CP/M: PIP HOST.TXT=A:SOURCE.TXT  (or similar)

# Feed data into the serial port
# Pre-populate host.txt before launching MAME
echo "data to receive" > host.txt
mame t250 -window -flop1 boot.imd -rs232 null_modem -bitb host.txt
# On CP/M: PIP CON:=RDR:  (reads from serial, displays on console; ^Z = EOF)
```

## Printer

Centronics parallel printer output goes to the host with `-prin1`:

```sh
mame t250 -window -flop1 boot.imd -prin1 printer.prn
# CP/M: PIP LST:=A:MYFILE.TXT  →  appends to printer.prn
```

## Hard disk (SASI)

The MAME driver includes an optional SASI hard-disk bridge for the original CDC Finch / Adaptec ACB-4000 controllers. Empty by default; attach with:

```sh
mame t250 -window -flop1 boot-with-hd-bios.imd \
   -sasi:0 harddisk -hard1 mydisk.chd
```

A blank CHD can be created with:
```sh
chdman createhd -o mydisk.chd -chs 153,4,32 -ss 256
```

Geometry must match what the BIOS computes from `hdtype` (see `bios64.asm`). The disk must then be formatted from within CP/M using the appropriate `FORMAT.COM` utility before drive C: becomes usable.

**Important**: The CP/M BIOS shipped with most of the sample disks here is the **floppy-only** build (`hard equ 0`). To use the SASI path you need a BIOS image rebuilt with `hard equ 1` (and matching DPH entries for drives C: onward).

## Creating new blank floppies

```sh
# 1. Create empty MFI for the right media type
floptool flopcreate mfi u8dsdd  newdisk.mfi   # 8" DSDD for T-250
floptool flopcreate mfi u525dsdd newdisk.mfi  # 5.25" DSDD for T-200

# 2. Mount in MAME as second drive
mame t250 -flop1 boot.imd -flop2 newdisk.mfi

# 3. Inside CP/M, format the second drive
A> FORMAT B:
   (Toshiba FORMAT utility prompts for one of the 8 layouts)

# Exit MAME — newdisk.mfi now has full Toshiba structure and is writable
```

See [`disk-formats.md`](disk-formats.md) for the complete list of supported formats.

## Keyboard

In MAME's "natural keyboard" mode (the default on macOS), most keys map sensibly:

| Host key | T-250 effect |
|---|---|
| Letters / digits | As typed |
| Shift + letter | Upper case |
| Ctrl + A..Z | Sends ASCII 0x01..0x1A (matches CP/M / WordStar) |
| Delete (Mac) | ASCII 0x08 (BS) → BIOS treats as destructive backspace |
| Forward Delete (Mac) | ASCII 0x7F (DEL/rubout) — different BIOS behavior |
| Return | ASCII 0x0D |
| Tab | ASCII 0x09 |

The unmarked arrow-cluster keys on the real keyboard aren't bound in MAME (their codes weren't well-documented on the physical hardware).

## Display

Hardware inverse video (XOR over the full 10x10 character cell) works as on real hardware. The character generator is the VPG static RAM loaded by the boot ROM — programs that reprogram VPG (e.g. chargen editors) see their changes on screen in real time.

## Audio

A 2 kHz keyboard buzzer (gated by bit 4 of the bank latch at port 0xC0) provides keyclicks, bell character, and program-driven tone. Comes through MAME's standard speaker output.

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| "MOUNT SYSTEM DISK" on T-200 boot | Drive motor not engaged | Driver bug — should be fixed in upstream; report if you see this |
| Distorted CPU memory display | Old driver version | Update to latest upstream MAME |
| Serial output is all NULs | Baud-rate mismatch (BIOS 19200 vs null_modem 9600 default) | Set both ends to 9600 (or use the SET.COM utility to match) |
| PIP hangs reading serial | No ^Z (EOF) in the input file | Append: `printf '\032' >> host.txt` |

## Reporting issues

If you find a bug in the MAME driver, report it at <https://github.com/mamedev/mame/issues> tagged with `toshiba/t250.cpp`. For BIOS issues or this repo's content, open an issue at <https://github.com/davidlrand/toshiba-t250-t200/issues>.
