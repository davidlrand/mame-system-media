# Bringing the PC-MX2 back to life

*Deutsche Fassung: [BRINGUP.de.md](BRINGUP.de.md)*

*An abbreviated account of the 2026 MAME bring-up. No PC-MX2 hardware was
available at any point: no schematics, no logic analyzer, no oscilloscope.
The machine was reconstructed from board photos, ROM dumps, forty-year-old
floppy images, the German manual set, and reasoning.*

## Starting material

The ROMs were dumped in 2022 (CPUAP monitor Rev 3 and Rev 9.0, plus SERAD,
Storager, OMTI, ExeLAN and SERAG firmware) and preserved alongside IMD images
of the SINIX V2.0 distribution floppies, dated March–May 1986, at the
OldComputers archive. MAME already carried a skeleton `pcmx2` driver with the
NS32016/NS32082 CPU pair. Everything else (the SERAD serial I/O
processor, the Storager disk controller, the 97801 terminal) had to be built from the
firmware's own expectations: run the 8085 and 68000 firmware, watch what they
probe, and supply what the boards must have supplied.

## The climb

**The console.** The SERAD board (8085 + SCN2681 DUARTs) talks to the host
through a Multibus mailbox. Its firmware would not sync the console until a
clock error was found: the 8085 was being run at half its real rate through a
double division of the 20 MHz oscillator. One line fixed it, and for the first
time the kernel printed: `SINIX-M-C V2.0 (Rev. 266)`.

**The disk.** The Storager (68000-based intelligent controller) drives both
floppy and ESDI Winchester through IOPB mailboxes and a scatter-gather
register file. Its completion signalling had to be reverse-engineered from
the 68000 firmware itself; the decisive fix was a level-held interrupt keyed
to the firmware's own channel-descriptor queue, found by watching the 68000
spin on its completion word and tracing back what should have written it.

**"/: file system full".** With console and disk alive, SINIX booted, and
died: `/: file system full`, `panic: init died`, seconds into every boot.
This error consumed the bulk of the investigation. The install floppy's root
filesystem has exactly four free blocks; something was eating them at boot and
killing init. The hunt eliminated, by measurement, in order: disk-write
corruption, DMA placement, MMU translation, the swap subsystem (disabled by a
compiled guard, identical on real hardware), physical-memory sizing, and the
demand pager (whose faults turned out to be serviced perfectly). The
filesystem was faithful: the four blocks were being consumed by the kernel
core-dumping init; `SIGSEGV`, default disposition, `core()` written into a
nearly-full miniroot. The question became: why does init segfault on a
machine where this exact floppy boots?

**Reading the code.** The answer came from the archive, not the emulator. The
kernel and its companion `vmsymbols` namelist were extracted from the SINIX0
floppy's filesystem (a 4.2BSD-style FFS with V7-style directories, a very
Siemens hybrid), and `/etc/init` itself was pulled out and disassembled. Its
first action after `exec` is the C runtime storing `environ` through the
**Static Base register**, and SB held garbage. The binary carries its own
NS32000 module descriptor table at virtual address 0x20, inside its text:
correct SB = 0x1c00. The garbage value delivered instead was, to the byte, the
content of *physical* address 0x20, kernel memory.

**The CPU bug.** On RETT/RETI, the NS32000 reloads SB from the module
descriptor at MOD. The Series 32000 Programmer's Reference Manual specifies
the order exactly (RETT, p. 6-171): pop PC, pop MOD, pop PSR; *then* copy the
descriptor into SB, then adjust "the stack pointer newly selected" by the
restored PSR. The descriptor fetch belongs to the *destination* mode. MAME's
core fetched it in supervisor space; under SINIX's dual-space MMU
configuration, returning to user mode, that read kernel memory instead of the
process image, handing every user process a broken SB at every syscall
return. The kernel was flawless; the CPU model wasn't.

**The fix.** Two lines: issue the SB descriptor fetch with the restored PSR's
U bit, letting the NS32082's own dual-space logic choose the page table. The
whole MAME NS32000 fleet was regression-tested (ICM-3216, PC532, Opus and
PD-32 all still boot), and the PC-MX2 came up:

```
SINIX-M-C V2.0 (Rev. 266)
System 734k User 3362k
using 143 buffers containing 417792 bytes of memory

INSTALLATION EINES SINIX-SYSTEMS
Herzlich Willkommen zur Selbstinstallation Ihres SINIX-Systems
```

The memory figures match the real 4 MB machine exactly. Forty years after the
floppies were written, the installation dialog is waiting for its `j <CR>`.

## What the exercise proved

Every layer initially suspected (the filesystem, the pager, the MMU walk,
the disk DMA, the swap code) was ultimately *exonerated* by measurement, and
the fault was three layers below the symptom, in two lines of CPU semantics
documented plainly in a 1984 manual. The disk images were the schematics; the
emulator was the logic analyzer; the manuals were the ground truth. Software
preservation made hardware archaeology possible.
