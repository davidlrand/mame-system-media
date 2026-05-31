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

More machines will be added as their drivers land.

## Notes

- **Work on a copy.** MAME persists writes back to floppy images on exit and can
  alter or corrupt them. Copy a disk (or `chmod -w` it) before mounting if you
  want to preserve the original bits.
- Each machine folder has its own `README.md` with run instructions, provenance,
  and per-file attribution.
- Boot ROMs and third-party firmware bundled here are preserved for emulation;
  attribution is in each folder's README.
