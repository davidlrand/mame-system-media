# Opus PM family — board details & MAME implementation notes

Companion to `opus5_unix_hd.chd`. Captures the real hardware and the
non-obvious decisions in the MAME driver (`src/devices/bus/isa/opus_pm.cpp`).

---

## 1. The hardware

Opus Systems "Personal Mainframe" — NS32000 UNIX coprocessor cards for the
IBM PC/AT. One board family, one host interface, one OS (Opus5 = Opus's port
of AT&T UNIX System V Release 2 v2):

| Driver        | Model       | CPU      | Bus        |
|---------------|-------------|----------|------------|
| `opus_pm100`  | Opus 108PM  | NS32016  | 8-bit ISA  |
| `opus_pm110`  | Opus 110PM  | NS32032  | 8-bit ISA (a 16-bit ISA version also shipped) |
| *(future)*    | Opus 300PM  | NS32532  | 16-bit ISA (on-chip MMU) |

**Naming across the product's life** (FCC grantee code **FTE = Opus Systems,
Cupertino CA** — the authoritative model list):
- *1985-86, the NS32016 & NS32032 boards:* board silkscreen **32.16 / 32.32**;
  FCC **32.16-2** / **32.32-4** (filed 1986); advertised as **Opus516** / **Opus532.32**
  (BYTE, 1985); named **108PM** / **110PM** in the 1987 100PM "Personal Mainframe" manual.
- *1989, rebranded PM line:* FCC **200PM** and **300PM** (the NS32532 board is the 300PM;
  the 200PM is the NS32032). Only four FCC filings exist: 32.16-2, 32.32-4, 200PM, 300PM.

**There was no NS32332 product** — no FCC filing, and cpu-ns32k.net notes only that
whether Opus built National's NS32332-based **SYS32/30** dev package is *unknown*. The
line went 32016 → 32032 → 32532. (The SPARCard 500 is an unrelated SPARC product.)

The MAME tags `opus_pm100` / `opus_pm110` cover the 32016 / 32032 boards.

**CPU module:** NS32016/32032 CPU + **NS32082 MMU** + **NS32081 FPU** +
**NS32201 TCU**. No boot ROM. No on-board peripherals — all I/O is the PC's.

**Memory:** 1 MB base array (256K DRAMs, parity) + refresh controller; a
piggyback brings it to 2 MB, or a 3 MB piggyback (110PM) to **4 MB max**.
The driver models the full 4 MB.

**Host interface (PC side):**
- A **64 KB ISA memory window**, jumpered to segment 8000/9000/A000/D000/E000
  (OPMON probes E000, D000, A000, 9000, 8000). It is **not a fixed page**: in
  the INITIALIZE state the address top 8 bits are forced to 0 (PC sees physical
  low memory — the download phase); after the **RUN** command they are forced to
  1, and the **32082 MMU re-vectors** them — so the window is MMU-translated,
  reaching the same pages as the slave's "DMA space" (virtual 0xFF0000).
- The window's top 16 bytes are the **control/status register file** (base+FFF0):
  `FFF0` STAT, `FFF1` EIRQ, `FFF2` RIRQ, `FFF3` INT, `FFF4` NMI, `FFF5` GO,
  `FFF6` RUN, `FFF7` RST. FFF1-FFF7 are address strobes (read or write triggers).
- Communication is via a **512-byte host comm page** in shared board memory plus
  those control ports. The PC can reset/interrupt the board; the board raises a
  jumpered PC IRQ (2/3/5/7) to request service.

**STAT bits:** `80` host→CPU int pending · `40` CPU→host int pending · `20`
parity err · `10` DMA abort · `08` st_init (CPU not running) · `04/02/01` =
CWT/TSO/CTTL live bus-state (TSO reads high except in a TCU cycle, CWT low only
during refresh, CTTL is the CPU clock).

**Slave-side (CPU physical) map:**
- `000000-3FFFFF` DRAM (4 MB). The comm page, host command ring and I/O buffers
  live in the first 64 KB; the kernel reaches them at virtual `FF0000` via the
  MMU. The kernel image is linked at 0 (first word doubles as the comm header).
- `800000/810000/820000/830000` = WAIT/STAT/C.ACK/C.IRQ (the MMU page tables
  address these); also as 512-byte pages at `FFF000/FFF200/FFF400/FFF600`.
- `R_C_IRQ` (FFF600): a write raises the CPU→host interrupt; **this firmware
  writes 0 to raise**, and only the host's RIRQ strobe clears the latch.

No ICU and no on-board timer: the kernel runs non-vectored (`SETCFG [M,F]`); a
single NVI multiplexes clock ticks, I/O completions and request handshakes
through comm-page flags, with the PC's clock driver supplying time.

**Software:** the DOS host monitor **OPMON.EXE** loads OPSASH (the standalone
shell) or the Opus5 kernel into board memory through the window and serves all
disk/console I/O over the comm page.

---

## 2. MAME implementation notes (the parts that aren't obvious)

These are the decisions that took real digging — keep them in mind before
"simplifying" anything.

1. **Model the full 4 MB.** The CPU's power-up routine sizes RAM by writing a
   bank tag to the top word of every 512 KB bank across 0-4 MB and reading it
   back. Backing less than 4 MB leaves banks unmapped, sizing never resolves,
   and the Level-2 test runs away (millions of unmapped accesses).

2. **The host window is MMU-translated** (`window_phys()`): once running, a
   window access is virtual `0xFF0000|offset` translated by the 32082 — the same
   path the slave's DMASPACE takes — so host and slave share the comm page
   through one map. Physical-low only during INITIALIZE/download. This broke the
   multi-week "opmon: device index table overflow" wall. The monitor relocates
   the comm page to the **top** of physical memory (`0x3F0000` at 4 MB), and the
   cpu_map mirrors `0xFF0000 → 0x3F0000` to match.

3. **CFG_M gate** (in the shared `ns32000` core): the CPU consults the 32082 MMU
   only after `SETCFG` has set the M bit. While the MMU is idle/just-reset, every
   address is physical and the page tables are ignored regardless of a stale MSR.
   Without this, a slave released into stale state translated through a stale page
   table → opconfig MMU abort. This is a shared-CPU change, so it is gated by a
   `m_mmu_uses_cfg_m` flag (default on for the external-MMU cores 32016/32032/
   32332); the NS32532's on-chip MMU clears it and keeps gating internally via
   MSR/MCR, so `pc532` is unaffected.

4. **Reset before every GO/RUN + a reset-recovery hold** (`CPU_RESET_HOLD_US`,
   500 ms): each GO/RUN strobe first does a full `reset_card()` (MMU/FPU + every
   latch), reports the board *running* immediately (st_init clears, so OPMON's
   run/stop probe still detects the card), but holds the CPU in reset via
   `INPUT_LINE_RESET` for the recovery delay before its first fetch, then
   releases it from a clean PC=0 state. This fixes two things: the warm-restart
   Level-2 "overflow as if reset wasn't done" (stale board state — GO/RUN never
   reset the board before, only the RST strobe did), and the Level-1/Test-02
   failures (the resident kernel ran to its first C.IRQ before the host read
   status). Implementation lessons learned the hard way:
   - **Do NOT** hand-manage `SUSPEND_REASON_RESET` — it collides with MAME's
     built-in reset (every CPU powers up holding it) and the slave runs wild.
     Use `INPUT_LINE_RESET`.
   - **Do NOT** use `perfect_quantum` to make the hold prompt — it forces
     instruction-level interleave on the whole machine and it crawls.
   - The hold is a one-shot `emu_timer` (`start_cpu`) that releases via
     `set_running(true)`.

5. **A reset slave drives nothing:** `slave_reg_w` ignores writes while
   `!m_running` (a held CPU can't raise C.IRQ; covers a one-instruction reset slip).

6. **Family structure:** one base class (`isa8_opus_pm100_device`) with `m_cpu`
   as a generic `cpu_device` finder (the common code uses only generic CPU ops);
   the CPU is created per-variant in `device_add_mconfig`. `opus_pm110` is a thin
   derived class swapping NS32016 → NS32032. Opus5 is CPU-compatible across the
   two: **the same hard-disk image boots on both** `opus_pm100` and `opus_pm110`.

---

## 3. Provenance

The emulation was derived from the surviving Opus5 source distribution and the
OPMON driver kit, preserved by **Al Kossow / bitsavers.org**. Reference manual:
*800-00237-000 Opus 100PM User Manual* (1987). The installed disk image boots to
the banner `OPUS5/2.0v2: Opus5C3`.

Note on the SYS32: National Semiconductor released the **SYS32 reference design**
(schematic) well *before* the Opus board, so the Opus card was **derived from
National's design** — "productized from the SYS32/20 reference design" is correct.
(cpu-ns32k.net carries a 2017 secondhand claim that the SYS32/20 *package hardware*
was built by Opus, but that's contradicted by the timeline: contemporaries were
building NS32k boards from National's design in 1984 — e.g. DSI-32 prototypes 1984,
shipped 1985. National's reference design is the origin.)
