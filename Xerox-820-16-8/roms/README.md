# Xerox 820-II 16/8 ROMs

Boot ROMs and firmware for the Xerox 820-II 16/8 family. The CRC and SHA1 values
below match the MAME ROM definitions for the corresponding systems unless noted.

The 16/8 Z80 monitor is the Balcones Operating System. Each Z80 boot ROM is a
2 KB 2716-class part at `u33` through `u36`, mapped into the top 8 KB of the Z80
address space.

## Z80 Monitor ROMs

### `z80-monitor/v50/` - Balcones OS v5.0 RX

Use this monitor set for the 16/8 systems. It supports the Low Profile Keyboard
used by the 16/8.

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `l5.u33.rom` | `a17af0f1` | `b1d9a151...` | Factory `537p10828`. |
| `l5.u34.rom` | `c9f5182e` | `ac830848...` | Factory `537p10829`. |
| `u35.5.0_537p10830.bin` | `278fa75f` | `f47cf9eb...` | Clean `u35`; use this instead of the commonly circulated bad read `l5.u35.rom`. |
| `u36.rx024.rom` | `a7f1d677` | `8c2a442f...` | RX v024, fitted for the low-profile keyboard. |

The assembled source copies are in [`../source/rom-v50/`](../source/rom-v50/).
The B23D13 source disk extracts matching `u33`, `u34`, and `u35` binaries, which
confirms the source-disk extraction.

### `z80-monitor/v404/` - Balcones OS v4.04

Earlier 820-II monitor set, included for reference and comparison.

| File | CRC32 | SHA1 |
|---|---|---|
| `537p3652.u33` | `7807cfbb` | `bd3cc5cc...` |
| `537p3653.u34` | `a9c6c0c3` | `c2da9d1b...` |
| `537p3654.u35` | `a8a07223` | `e8ae1ebf...` |

## 8086 Coprocessor ROM

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `8086/8086.u33` | `ee49e3dc` | `a5f20c74...` | 4 KB 816 PC boot ROM. Required by the CP/M-86 loader path used by `LOAD86` and `86CON`. |

Disassembly and protocol notes are in [`../source/8086/`](../source/8086/) and
[`../docs/16-8-architecture.md`](../docs/16-8-architecture.md).

## Character Generators

| File | CRC32 | SHA1 | Used by |
|---|---|---|---|
| `chargen/x820ii.u57` | `1a50f600` | `df4470c8...` | v4.04 set. |
| `chargen/x820ii.u58` | `aca4b9b3` | `77f41470...` | v4.04 set. |
| `chargen/u57.04.north.rom` | `eda727a2` | `292cd8a0...` | v5.0 set. |
| `chargen/u58.03.north.rom` | `a2e514f3` | `8ac22dd0...` | v5.0 set. |

## Keyboard Controller

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `keyboard/820iikey.bin` | `8ea3b39b` | `3f05959f...` | Intel 8748 firmware for the X928 ASCII keyboard. |

The keyboard protocol and layouts are documented in [`../docs/x928-keyboard.md`](../docs/x928-keyboard.md).
