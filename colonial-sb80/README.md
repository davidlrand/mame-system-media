# Colonial Data Services SB-80

The **SB-80** (internal board name **CPU-80**) is a Z80A CP/M 2.2 single-board
computer made by Colonial Data Services Corp., Hamden, Connecticut, 1982–1984.
It is a headless, serial-console machine: Z80A @ 4 MHz, 64 KB DRAM, dual Zilog
SIO (console + aux), Zilog CTC, Z80 PIO, and a WD1793 floppy controller driving
8" drives. I wrote and maintained its CBIOS.

MAME driver: [`colonial/sb80.cpp`](https://github.com/mamedev/mame/pull/15417)
(PR #15417).

## Running it

Put the boot ROM where MAME expects the `sb80` romset, then mount a disk:

```sh
# bootrom.bin -> MAME's roms/sb80/bootrom.bin
./mame sb80 -flop1 disks/sb80-boot.mfi
```

This boots to the CP/M prompt:

```
SB 80 CP/M 2.2 61K Version 1.22   Dave Rand  04/15/84
A0>
```

- **Console** is Zilog SIO channel A on `-rs232a` (default terminal), **9600 baud**.
- **Aux / RDR: / PUN:** is SIO channel B on `-rs232b`, **300 baud at cold boot**.

### Serial aux gotcha (300 vs 9600)

If you use the aux port for file transfer (`PIP PUN:=file`, `PIP file=RDR:`),
note the baud asymmetry: the CBIOS programs the aux channel to **300 baud** at
cold boot, but MAME's connected serial devices (`pty`, `null_modem`, `terminal`)
default to **9600**. A mismatched pair produces framing garbage or silence.

Either side must be made to agree:

- Set the host device to 300 baud (Tab → Machine Configuration → the `rs232b`
  device's RX/TX baud), **or**
- Run a baud-setter on the CP/M side (e.g. `SET9600`) to reprogram the COM8116,
  then set the host device to 9600.

The console "just works" only because firmware 9600 happens to match MAME's
default 9600. (The driver carries `DEVICE_INPUT_DEFAULTS` to default `rs232b` to
the firmware's 300-baud cold-boot rate.)

## Contents

| File | Role | SHA-1 |
|---|---|---|
| `disks/sb80-boot.mfi` | Ready-to-run disk, all utilities (curated by me) | `c56aebe16c183961be43caa2f29ec18cca25279c` |
| `disks/sb80-boot.imd` | Same disk in ImageDisk (`.imd`) form, for non-MAME tools | `9d3e149c9e2a5f89226099d8cd8a5b1c02d588b1` |
| `disks/sb80-dr.td0` | Authentic original dump of my v1.22 CBIOS system disk | `7c2813b54959f79a2f44e62d4518ee2379ec7c7f` |
| `disks/sb80-1k.td0` | Colonial Data's own CP/M 2.2 (02.15) CBIOS, OEM customer disk | `e2636d6105f24e1e38ed4434d7207c79d70e2f2f` |
| `disks/sb80-004.imd` | My 8" recovery image (source of the extracted BIOS) | `2ec8381df3c7384ea3f4930f34bec5b4d87c177a` |
| `bootrom.bin` | 256-byte boot ROM — CRC32 `FA521576` | `32153db6f37e7f5666e89aa0b8712d79daafeb4b` |
| `docs/photos/` | Photos of my boards (S/N 1692 stock, S/N 1693 bank-switch mod) | — |

## Provenance & attribution

- **`sb80-dr.td0`** — from Don Maslin's CP/M disk archive, imaged 1993-02-09. It
  is the binary distribution of my v1.22 CBIOS ("SB 80 CP/M 2.2 61K Version 1.22
  Dave Rand 04/15/84"), matching the source to the day.
- **`sb80-1k.td0`** — also from Don Maslin's archive, same 1993-02-09 session.
  This is *Colonial Data's own* CP/M 2.2 (02.15) CBIOS for an OEM customer build
  ("CP/M 2.2 (02.15) COPYRIGHT 1984 COLONIAL DATA"), not mine. It is all-MFM
  8×1024 with no IBM cold-boot track, so it needs Colonial Data's own loader and
  will not cold-boot the stock `sb80` driver as-is; it is included for
  preservation. The user area carries a customer application (TMS) and its data
  files. See LICENSE for the third-party-material terms.
- **`sb80-004.imd`** — my own 8" recovery (2022); the BIOS and utility sources
  were extracted from it.
- **`sb80-boot.mfi`** — a clean bootable disk I formatted, sysgenned, and
  populated for emulation use.
- **`bootrom.bin`** — Colonial Data Services Corp boot ROM (1982). Third-party
  firmware from a defunct vendor, preserved here for emulation.
- **Photos** — my own hardware: production unit S/N 1692, and S/N 1693 with the
  256 KB DRAM bank-switching modification I designed.

The contemporary product review (BYTE, November 1982, p. 324) is **not**
re-hosted here — read it at the Internet Archive:
<https://archive.org/details/byte-magazine-1982-11/page/324/mode/2up>.
