# Xerox 820-II / 16-8 — "820+" CP/M BIOS source

Source for the Balcones **"XEROX 820+" ROM-resident BIOS** (the Z80/CP/M-80 side),
extracted from Don Maslin developer disk **B16D39** (`820ii_images` set) with the
head-grouped DS diskdef — see [`../README.md`](../README.md).

| File | What it is |
|---|---|
| `xbios.mac` | "XEROX 820+ BIOS Jump Table" (ROM-resident BIOS entry table). `Copyright 1981, Balcones Computer Corporation`. |
| `deblkr.mac` / `deblkr1.mac` | Sector **deblocker** (128↔physical sector blocking). |
| `chario.mac` | Console / character I/O. |
| `cwboot.mac` | Cold / warm boot. |
| `qfs.mac` | File-system helper. |
| `bios0/1.hex`, `cpsys0/1.hex`, `boot0/1.hex` | Assembled BIOS / CP/M-system / boot images (Intel HEX). |
| `makebios.sub`, `bildbios.sub` | M80/L80 assemble+link build scripts. |
| `*.rel` | M80 relocatable objects; `*.bak` editor backups. |

**Note on scope:** this is the **Z80 CP/M-80** BIOS. It contains no 8086 /
doorbell / mailbox code — on the 16/8 the CP/M-86 (8086) BIOS forwards I/O to the
Z80, and that inter-processor handoff lives in the loader (`load86`/`86con`/
`swap`) and the monitor's bank-switch/`$F033` + port-A0/A1 path, not here. See
[`../../docs/16-8-architecture.md`](../../docs/16-8-architecture.md).
