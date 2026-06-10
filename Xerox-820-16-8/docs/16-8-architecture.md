# Xerox 16/8 — system architecture & the Z80↔8086 link

The Xerox 16/8 is a **Xerox 820-II** (Z80, master) with the **"816 PC"** 8086
coprocessor board (the Expansion Module II / EM-II) added as a slave. The Z80
runs Balcones CP/M-80; the 8086 board runs **CP/M-86 or MS-DOS**. The two
processors communicate through a block of **shared RAM** with a
**doorbell-interrupt mailbox**.

This is a working map of that link, assembled from the 8086 boot ROM disassembly
([`../source/8086/8086.u33.asm`](../source/8086/)) and the Z80 boot/loader code.
It is the reference for emulating the coprocessor — and as of 2026-06 it is
enough to **boot CP/M-86 1.1F on the 8086 under MAME**, interactively (see
*Coprocessor emulation*, below).

## Processors and memory

- **Z80** — 820-II master. Its top 8 KB is the Balcones monitor ROM
  (`u33`–`u36`). Runs CP/M-80.
- **8086** — "816 PC". 4 KB boot ROM `8086.u33` (CRC `ee49e3dc`) mapped at
  **`0xFF000`–`0xFFFFF`** (covers the reset vector at `FFFF0`). Resident/shared
  RAM around **`0xF4000`–`0xFFFFF`**; main program RAM from `0x00000` up
  (128 KB base, 256 KB max). Runs CP/M-86 or MS-DOS.

### The bank window
The Z80 reaches the 8086's memory through a **bank window**: Z80 addresses
`0x0000`–`0xBFFF` map to 8086 physical `0xF4000`–`0xFFFFF`
(**8086 addr = Z80 offset + 0xF4000**). The window is switched in by **bit 7 of
the system "A" PIO** (`SYSPIO`, I/O port `0x1C`); the resident jump-table entry
`$F033` → `CRTLDIR` (`$F2A3`) block-moves through it, toggling that bit via
`CRTON` (`$F29C`) / `CRTOFF` (`$F293`) around an `LDIR`. So, for example, Z80
`0x4600` is 8086 `0xF8600` (the mailbox), and Z80 `0x8FF5` is 8086 `0xFCFF5`.

> Z80-side labels in this document (`CRTLDIR`, `CONFG`, `DDSKLD`, …) are the real
> Balcones symbols, verified against the v5.0 assembler listing
> [`../source/rom-v50/xr.prn`](../source/rom-v50/). The **8086-side** addresses
> (`FFB3F`, `FF949`, `FFCEF`, `[46xx]`, …) are from the
> [boot-ROM disassembly](../source/8086/) — that ROM's source did not survive, so
> those names are descriptive, not original.

## 8086 reset and POST

- **Reset `FFFF0`:** `EA 3F BB 00 F4` → `JMP FAR F400:BB3F` (= phys `FFB3F`).
  CS runs at `F400` thereafter.
- **`FFB3F`:** `CLI`, then a numbered **POST**. Each step writes its number to a
  fixed cell **`[461E]`** before running, so progress/failure is externally
  visible to the Z80. On failure it jumps to `FFBF7`, bumps **`[461F]`**, indexes
  an error-string table by step number, and signals it.

| step | test | failure string |
|---|---|---|
| 1–7 | CPU register / ALU (`5555/AAAA`, `idiv`,`xor`,`sub`,`and`) | "Bad CPU Register" / "Division Failure" / "ALU/REG/XOR/SUB/CPU Failure" |
| 8 | ROM CRC-16 (routine at `FFCF5`, expected sum in BP) | "CRC-16 Incorrect" |
| 9 | shared-RAM test | "Share RAM Failed" / "Shared Ram Init." |
| A–B | local scratch-RAM copy/verify (`F400:46A0`, `rep movsw`/`cmpsb`) | — |

On full pass **`[461E]` = `FFFF`** and the ROM prints **"Xerox 816 PC  Ok"**,
then falls into the mailbox loop.

## The mailbox / doorbell (the part you must emulate)

State lives in shared-RAM cells **`[4602]`–`[4614]`**:

- **8 doorbell interrupt vectors** at `FFCC5`–`FFCF4`. Each is just
  `inc word ptr cs:[46xx]` + `IRET`, counting interrupts into cells
  **`[4606]`–`[4614]`**. The POST installs them in the IVT (`FFA46`…); notably
  **`[3FC]` (INT `0FFh`) → `0BCEFh` (= `FFCEF`) → `inc [4612]`**, the cell the
  idle loop polls. `0FFh` is the bus-float vector you get when the doorbell
  asserts the 8086 `INTR` with no PIC driving the data bus during INTA — i.e. a
  bare interrupt **is** the doorbell. (Confirmed: ringing it bumps `[4612]`.)
- **Dispatcher** (main loop, `FF949`): walks a linked list of task-control
  blocks (the resident chain set up by POST; `ax`/`[4604]` = next), tests each
  TCB's flag byte (**bit 7 = ready**), and on a ready TCB does
  `jmp word ptr [bx+2]`. TCB types include a **block-move/DMA engine** (`FF989`,
  `movsw`/`movsb` from a mailbox source to an `es:di` destination — this is how
  a loaded image reaches low 8086 RAM, e.g. CP/M-86 at `0x1F000`) and a
  **task-switch** (`FF9FD`, sets `ss:sp`/`ds:bp` then `jmp far` — the jump into
  the loaded OS).
- **Ports:** the 8086 polls `in al,0FFh` (port `0xFF`) in a wait loop and uses
  DX-addressed high ports (`in ax,dx`, `dh |= 0FCh`). On the **Z80 side** the
  loader pokes the doorbell via I/O ports **`0xA0`/`0xA1`** (e.g. its ROM-present
  check does `out ($A1),0` / `ld hl,$8FF5` / `call $F033` / `in ($A0)` and
  expects a signature byte in `01..09`).

## Coprocessor emulation (MAME) — CP/M-86 boots

The 8086 CPU and this firmware are already in MAME. Four things make the
coprocessor fully come alive on the 8" 16/8 (`x168`); each was determined from
the boot ROM + the loader (`LOAD86.COM`) and confirmed by running it:

1. **Bank window** — map Z80 `0x4000`–`0xBFFF` to 8086 `0x4000+0xF4000` =
   `0xF8000`–`0xFFFFF` (linear; same `+0xF4000` rule as above), switched by
   `syapio` bit 7. This is what `$F033`/`CRTLDIR` moves data through.
2. **Doorbell** — a Z80 `OUT (0A1h)` asserts the 8086 maskable interrupt with
   vector `0FFh` (bus-float; no PIC). That vectors to `FFCEF` → `inc [4612]`.
3. **8086 run control** — `IN (0A0h)` = **assert** the 8086 `RESET` line (Stop);
   `IN (0A1h)` = **release** `RESET` (Start). Releasing reset runs the 8086 fresh
   from `FFFF0` — a full POST. This is the subtle one: the loader stops the 8086,
   stages RAM + commands, then starts it, and the *fresh POST* re-initializes the
   dispatcher's task blocks. (Modeling Stop/Start as suspend/resume — freezing the
   PC instead of resetting — leaves the dispatcher tables in a half-written state
   and it never boots.)
4. **Held at reset** — the 8086 starts **held in reset** at power-on and only
   runs when the Z80 first releases it (`IN 0A1h`). With this, the Z80's CP/M-80
   even reports itself correctly as the 16/8 ("Xerox 16/8 PC … CP/M-80 2.2F").

### The boot sequence (verified, interactive)
From a booted CP/M-80 on the 8" system disk:

1. **`LOAD86`** — builds the co-processor CP/M-80 (2.2F), DMA-loads the embedded
   CP/M-86 image into 8086 RAM (BDOS/CCP at `0x00000`+, BIOS at `0x1F000`), and
   starts the 8086.
2. **`86CON`** — switches the console over to the 8086.

Result on screen: **`Xerox 16/8 PC 256k CP/M-86 vers 1.1F`** and an `a>` prompt;
`DIR` lists the directory. The 8086 has no console hardware of its own — CONIN
and CONOUT travel through the mailbox to the Z80 (the I/O processor), which
`86CON` services; disk I/O likewise goes Z80↔8086 through the mailbox.

### 5.25" 16/8 (`x1685`)
The 5.25" machine boots through a separate **RX024 disk-controller ROM** that is
not in any known dump. Its driver *source* survives, though, on B23D13 — see
[`16-8-rx024-controller.md`](16-8-rx024-controller.md) for the reconstruction
(the boot reaches the RX024 system menu; the 5.25" media read is still being
worked out).
