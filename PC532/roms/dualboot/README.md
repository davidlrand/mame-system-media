# pc532 dual-boot ROM

Burn the **proven RAM bootloader into the reset EPROM** so the pc532 boots an OS
straight off its SCSI controller — no DP8490-resident ROM-monitor handoff needed.

```
OLD:  reset -> ROM monitor --(DP8490 image-table load)--> RAM loader --(AIC6250)--> /unix
NEW:  reset -> THIS ROM (is the loader) --(AIC6250 or DP8490)--> OS
```

Dave's rule — **the controller picks the OS**:

| controller | ICU G7 | OS it boots | path |
|---|---|---|---|
| AIC6250  | G7 = 1 | System V (s5 fs `/unix`, COFF) | `sysv_boot()` = `../boot/main.c` |
| DP8490   | G7 = 0 | NetBSD (primary bootstrap)      | `netbsd_boot()` = `dp8490.c`   |

The ROM probes each controller for a live disk and boots whichever answers,
AIC6250/SysV first.

> **Status: the AIC6250 / System V path is tested and boots in MAME** — it probes
> the AIC6250, loads `/unix` off `0s0`, and runs UNIX System V Release 2.0 to the
> console (the runnable distribution disk is in [`../../disks/`](../../disks/)).
> The **DP8490 / NetBSD path is still static** — written from proven sources but
> not yet live-tested; its `[V-NBSD]` bootstrap parameters are placeholders. Items
> still needing a live test are tagged `[V…]` in the sources.

---

## Files

| file | what |
|---|---|
| `pc532romstart.s` | ROM reset prologue (AT&T `as`). CFG/MOD/INTBASE/SB, NS32202 G0 overlay flip + high-ROM-mirror handoff, console DUART init, `.data` ROM→RAM copy, `.bss` zero, then `jsr _main`. Faithful translation of the Culbertson monitor `resume532.s start::`. Also holds `_pputc` / `_jumpto` / trap handler. **Replaces `../boot/pc532bstart.s`.** |
| `bootsel.c` | `main()` — the dual-boot dispatcher: probe AIC6250 → `sysv_boot()`, else probe DP8490 → `netbsd_boot()`, else halt. |
| `aic_live.c` | `aic_live()` — bounded TEST-UNIT-READY to the AIC SysV target (id 1) via `../boot/dc.c`'s `do_scsi`. Needed because the SysV mount retries `-48` forever (`_dcopen`), so it would **hang** on an empty AIC bus without a bounded pre-probe. |
| `dp8490.c` | `dp_live()` + `netbsd_boot()` — NCR5380-class PIO driver (G7 = 0), modelled on the Culbertson monitor `scsi.c`. Reads the NetBSD primary bootstrap and jumps. |
| `rom.map` | linker map: `.text` → EPROM `0x10000000` (32KB); `.data` VMA in RAM `0x00200000` with its image (LMA) in ROM (`AT>rom`); `.bss` in RAM. |
| `mkrom.sh` | build recipe (run by hand on the cross host; reuses `../boot/{main.c,dc.c,conf.c}` + `lib2.a`). |

**Reused verbatim from `../boot/` (the proven loader):** `main.c` (COFF `/unix`
load, linked in as `sysv_boot` via `-Dmain=sysv_boot`), `dc.c` (AIC6250 `do_scsi`
+ `_dcopen/_dcstrategy/_dcclose`), `conf.c` (`_devsw/_dtab` device tables,
`bzero/bcopy`), and `lib2.a` (standalone s5 filesystem).

**What's here vs. not (redistribution).** The dual-boot ROM's own source —
`bootsel.c`, `aic_live.c`, `dp8490.c`, `coffbin.c`, `hexout.c`, `pc532romstart.s`,
`mkrom.sh`, `rom.map` — is included in full, along with the built `pcrom.bin`
(32 KB, CRC32 `ed5d7a78`). The reused `../boot/` loader (`main.c`/`dc.c`/`conf.c`)
and `lib2.a` derive from AT&T System V source and are **not** redistributed here,
so this folder documents the ROM and ships its binary but is not standalone-
buildable from these files alone.

---

## Memory map

```
0x10000000..0x10007fff  EPROM, 32KB   .text + initialised-.data image (LMA).
                                       Also OVERLAID at 0..0x7fff at reset until
                                       pc532romstart flips ICU G0 and hands off
                                       to this high mirror.
0x00200000              RAM   loader .data (VMA) — copied here from its ROM LMA
0x002xxxxx              RAM   loader .bss        — zeroed at reset
0x002c0000              RAM   loader stack top
0x00008000..~0x39000    RAM   the loaded SysV kernel (COFF text_start = 0x8000)
0x28000000              mc68681 DUART 0 (console, chA 9600 8N1)
0x30000000              shared SCSI window (AIC6250 if G7=1, DP8490 if G7=0)
0xfffffe00              NS32202 ICU (native byte offsets; G0 overlay, G7 SCSI sel)
```

## Reset sequence (`pc532romstart.s`)

1. `lprw cfg` — enable I/F/M + caches (same CFG word as the monitor). `[V1]`
2. ~800µs settle delay, then NS32202 port setup: G0 (and G7) as outputs,
   ROM still overlaid at 0.
3. `jump @reset1` to the `0x10000000` high mirror, then flip G0 = 0 so RAM
   swaps in at address 0 (ROM stays at the high mirror). `[V2]`
4. Clear low RAM 0..stacktop; `bicpsrw` S+I (interrupt stack, ints off);
   set SP/FP, INTBASE, MOD, SB = 0. `[V3]`
5. `inttab_init` / `modtab_init` / `duart_init` (console chA 9600 8N1). `[V4]`
6. Copy `.data` image ROM(LMA)→RAM(VMA); zero `.bss`.
7. `jsr _main` → `bootsel.c`.

## Dual-boot dispatch (`bootsel.c`)

```
probe AIC6250 (aic_live)  -> live -> sysv_boot()   ; System V off the AIC6250
probe DP8490  (dp_live)   -> live -> netbsd_boot() ; NetBSD  off the DP8490
else halt
```

A `*_boot()` does not return on success (it jumps into the loaded OS).
Reverse the two if-blocks in `bootsel.c` to prefer NetBSD.

---

## Verify-on-test points

Tagged in the sources; collected here.

- **`[V1]`–`[V5]` (`pc532romstart.s`)** — CFG word, the G0-flip overlay timing,
  SB=0 global model, the mc68681 9600-baud `ACR=0x80`/MR-reset sequence, and the
  bare-`jump` kernel entry. All modelled on `resume532.s`; confirm on MAME.
- **`[V-MAP]` (`rom.map`, `mkrom.sh`)** — `.data AT>rom` / `LOADADDR()` and
  `objcopy -O binary` assume a GNU-ld-class National toolchain. If `ld` lacks
  `AT>`: link `.data` contiguously after `.text` in ROM (VMA = LMA in ROM) and
  hand-set `__datarom/__dataram/__dataend` for the runtime copy. If `objcopy`
  is absent: use the toolchain's S-record dump + a raw-image extractor.
- **`[V-NBSD]` (`dp8490.c`)** — the only logic **not** pinned to proven code:
  the NetBSD primary-bootstrap disk layout (`NBSD_TARGET`/`NBSD_BOOTSEC`/
  `NBSD_BOOTCNT`/`NBSD_LOADADDR`) and its entry point are placeholders. Pin them
  to the NetBSD/pc532 `bootxx` convention — i.e. match what Phil Nelson's
  autoboot ROM monitor already does to launch NetBSD off the DP8490 — before
  trusting the NetBSD path. The NCR5380 phase loop and G7=0 select **are**
  modelled on proven drivers.
- **`sysv_boot()` failure = spin, not fall-through.** `../boot/main.c` halts
  (`for(;;)`) on mount/open/magic failure rather than returning, so the
  "SysV boot failed → try NetBSD" fall-through in `bootsel.c` only triggers if
  `sysv_boot()` ever returns (it currently can't). Acceptable because
  `aic_live()` already gated on a live disk and Dave's invariant is AIC = SysV;
  revisit if a live-but-non-SysV AIC disk must degrade to NetBSD.

## MAME bring-up (when we can test)

1. Build on the cross host: `sh mkrom.sh` → `pcrom.bin` (32768 bytes).
2. Make it the pc532 reset EPROM — add a BIOS option in
   `src/mame/homebrew/pc532.cpp` pointing `ROM_REGION32_LE(0x8000 …)` at
   `pcrom.bin` (or hash-list it), and select it with `-bios`.
3. Run headless (per the mame skill — never bare, never `-window` backgrounded):
   ```
   SDL_VIDEODRIVER=dummy ./mame pc532 -bios <thisrom> \
       -hard <aic_sysv.chd> -video none -nothrottle -log
   ```
   - **AIC6250/SysV:** attach the System V s5 disk on the AIC6250 target.
     With this ROM the DP8490 `-hard1` handoff disk is **no longer needed** —
     the ROM reads `/unix` off the AIC6250 directly.
   - **DP8490/NetBSD:** attach a NetBSD disk on the DP8490 (after `[V-NBSD]`).
4. Console banner on success:
   `pc532 boot ROM (dual: AIC6250/SysV, DP8490/NetBSD)` →
   `probe AIC6250... live -> System V` → the loader's `LDR: load /unix` →
   `jump <entry>` → kernel.
