# Xerox 820+ CP/M-80 BIOS Source

This directory contains Balcones XEROX 820+ ROM-resident BIOS source for the Z80
CP/M-80 side of the Xerox 820-II / 16/8 family. It was extracted from Don
Maslin developer disk B16D39 in the `820ii_images` set.

| File | What it is |
|---|---|
| `xbios.mac` | XEROX 820+ BIOS jump table. |
| `deblkr.mac`, `deblkr1.mac` | Sector deblocker code for 128-byte CP/M records and physical disk sectors. |
| `chario.mac` | Console and character I/O. |
| `cwboot.mac` | Cold and warm boot code. |
| `qfs.mac` | File-system helper. |
| `bios0.hex`, `bios1.hex` | Assembled BIOS images. |
| `cpsys0.hex`, `cpsys1.hex` | Assembled CP/M system images. |
| `boot0.hex`, `boot1.hex` | Assembled boot images. |
| `makebios.sub`, `bildbios.sub` | M80/L80 build scripts. |
| `*.rel` | M80 relocatable objects. |

## Scope

This is Z80 CP/M-80 BIOS source. CP/M-86 operation on the 16/8 uses the 8086
card together with the Z80 monitor and loader path. In normal use, start CP/M-86
from CP/M-80 with:

```text
LOAD86
86CON
```

Return to CP/M-80 with:

```text
GOBACK
```

Architecture notes for that path are in
[`../../docs/16-8-architecture.md`](../../docs/16-8-architecture.md).
