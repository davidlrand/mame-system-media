# DSI-32 boot disks

Ready-to-run host boot disks for the MAME `dsi32` ISA card, verified on
`mame ibm5160 -bios rev4 -isa4 "" -isa5 dsi32` (the empty isa4 removes
the XT hard-disk controller, which halts POST with no drive attached).

## dsi32-cpm86-boot.img — CP/M-86 1.1

Digital Research CP/M-86 1.1 for the IBM PC (1983), pruned to
essentials (PIP, STAT), with `LOAD.CMD`, `32IO.E32`, and `MON.E32`
(Definicon 32000 Monitor 1.0g) from the 1984 Concurrent distribution.

    mame ibm5160 -bios rev4 -isa4 "" -isa5 dsi32 -flop1 dsi32-cpm86-boot.img
    A>load mon
    Definicon 32000 Monitor 1.0g
    *

Format: the distribution is the 160K single-sided CP/M-86 format
(40 tracks, 8x512, 2K blocks, directory on track 1) carried in a
320K double-sided container with side 1 unformatted. The cpmtools
definition used to build it is in `cpm86-diskdefs`; work on the
160K side-0 view, then re-interleave.

## dsi32-dos-boot.img — PC-DOS 2.10

IBM PC-DOS 2.10 (1983), pruned to system + COMMAND.COM, with
`LOAD.EXE`, `LOADINT.EXE`, `32IO.E32`, and `MON.E32` (Monitor 1.0i)
from the 1984 DOS distribution.

`LOAD` polls the mailbox; `LOADINT` is the interrupt-driven variant
(IRQ2 from the board's DUART output port). Under single-tasking DOS
they behave identically; under Concurrent the interrupt version
yields the CPU to other processes while the 32032 computes. Both
are verified working in MAME.

    mame ibm5160 -bios rev4 -isa4 "" -isa5 dsi32 -flop1 dsi32-dos-boot.img
    A>load mon
    Definicon 32000 Monitor 1.0i
    *

PC-DOS is IBM-copyrighted; the OS files came from the MAME software
list set `pcdos21` (disk 1). To rebuild from scratch: copy that image,
`mdel` the utilities, `mcopy` the DSI files from `../software/`.

## dsi32-dos-1987.img — PC-DOS 2.10 + the 1987 generation

The same DOS base with Loader 2.24v, the 1987 32IO, monitor, and
DHRY.E32 (dhrystone 1.1, 500000 passes, compiled against the 1987
CLIB). `load dhry` runs the benchmark at true period speed:
468 seconds of 32032 time on the MAME card, versus ~440 seconds
implied by the official dhrystone-list figure for the real board
(1282-1315 dhrystones/second, NS32032-10, Green Hills) -- the model
runs within about 10-15%% of period hardware, slightly conservative.

## dsi32-work.img — toolchain data disk (FAT, not bootable)

The Definicon development tools for drive B: AS.E32, LN.E32,
NS32032.OPC, MON.E32, 32IO.E32, LOAD.EXE, PORTAB.H, and the
CC/AS/LN `.CMD` wrappers for Concurrent.

## Version pairing note

The 1984 binaries are matched: the 1984 `32IO.E32` serves `LOAD.EXE`
1.x and the 1984 monitors. Programs compiled against the later
(1986/2016 emulator-era) 32IO request service codes the 1984 loader
does not vector, and exit silently. Keep loader, kernel, and programs
from the same era.

PC-DOS halts at the date/time prompts: press Enter twice.
