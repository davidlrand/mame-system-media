# mame-system-media

Runnable media — disk images and boot ROMs — for the MAME drivers I've brought
up or modified. One folder per machine. Mount the disk in MAME and the system
boots.

This repo is intentionally lean: it holds only what you need to *run* a machine.
The original source code for the software I wrote for these machines (BIOS,
utilities, applications) lives in a separate, traditionally-organized source
archive — not here.

## Machines

| Folder | System | MAME driver |
|---|---|---|
| [`colonial-sb80/`](colonial-sb80/) | Colonial Data Services SB-80 (Z80 CP/M 2.2) | [mamedev/mame#15417](https://github.com/mamedev/mame/pull/15417) |
| [`toshiba-t250-t200/`](toshiba-t250-t200/) | Toshiba T-250 (8") / T-200 (5.25") CP/M (8085) | [mamedev/mame#15372](https://github.com/mamedev/mame/pull/15372) |
| [`bigboard2/`](bigboard2/) | Big Board II (Digital Research Computers, Z80 CP/M) | [mamedev/mame#15412](https://github.com/mamedev/mame/pull/15412) |
| [`trs80-model3-cpm/`](trs80-model3-cpm/) | TRS-80 Model III CP/M ("CP/M Cheap" mod, Z80) | patch in folder (not upstreamed) |
| [`Xerox-820-16-8/`](Xerox-820-16-8/) | Xerox 16/8 (820-II Z80 CP/M-80 + 8086 coprocessor, CP/M-86) | [mamedev/mame#15485](https://github.com/mamedev/mame/pull/15485) |
| [`Xerox-820-line-roms/`](Xerox-820-line-roms/) | Xerox 820 line ROM lineage (820, 820-II, 16/8; sources + recovered sets) | [mamedev/mame#15485](https://github.com/mamedev/mame/pull/15485) |
| [`ADE-MK3000/`](ADE-MK3000/) | ADE Elettronica MK3000 / MK-83 (Z80 CP/M, multi-format disk gateway) | [mamedev/mame#15489](https://github.com/mamedev/mame/pull/15489) |
| [`Definicon-DSI-32/`](Definicon-DSI-32/) | Definicon DSI-32 (NS32032 coprocessor ISA card for the IBM PC) | bring-up complete, PR pending |

More machines will be added as their drivers land.

The Xerox and Definicon folders are broader than runnable media — they are full
preservation bundles (ROMs or distribution software, firmware source, format and
interface documentation).

## Notes

- **Work on a copy.** MAME persists writes back to floppy images on exit and can
  alter or corrupt them. Copy a disk (or `chmod -w` it) before mounting if you
  want to preserve the original bits.
- Each machine folder has its own `README.md` with run instructions, provenance,
  and per-file attribution.
- Boot ROMs and third-party firmware bundled here are preserved for emulation;
  attribution is in each folder's README.
