# Xerox 16/8 — ROMs

Boot ROMs and firmware for the Xerox 16/8 (a Xerox 820-II with the "816 PC"
8086 coprocessor board). Preserved for emulation. CRCs/SHA1s below match the
MAME `x168` / `x820ii` / `x820kb` ROM definitions; the binaries are copied from
the MAME ROM set unless noted.

The 16/8's Z80 monitor is the **Balcones Operating System** ("Balcones Computer
Corporation" wrote the 820-II/16-8 firmware for Xerox, 1981–). Each boot ROM is
a 2 KB 2716-class part at `u33`–`u36`, mapped into the Z80's top 8 KB.

## `z80-monitor/` — Z80 boot monitor (two revisions)

### `v50/` — Balcones OS v5.0 "RX" (what the 16/8 ships with)
The 16/8 uses the Low Profile Keyboard, which only the v5.0 monitor decodes;
v4.04 echoes "what?" to LPK scancodes. This is the version to run the 16/8 with.

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `l5.u33.rom` | `a17af0f1` | `b1d9a151…` | = factory `537p10828` |
| `l5.u34.rom` | `c9f5182e` | `ac830848…` | = factory `537p10829` |
| `u35.5.0_537p10830.bin` | `278fa75f` | `f47cf9eb…` | **clean** u35. The commonly-circulated `l5.u35.rom` (`44c8dbf8`) is the *same* ROM with data bit 7 stuck high — a bad read. Use this one. |
| `u36.rx024.rom` | `a7f1d677` | `8c2a442f…` | RX v024; fitted for the low-profile keyboard |

The assembled source copies of these ROMs are in
[`../source/rom-v50/`](../source/rom-v50/) (`u33-500.rom` … `u36-v18.rom`);
they extract from disk B23D13 to `a17af0f1 / c9f5182e / 278fa75f / cda7f598`,
matching the factory `537p10828/29/30/31` dumps and independently confirming the
disk extraction.

### `v404/` — Balcones OS v4.04 (820-II, for reference)
Earlier monitor; changes sign-on to "Xerox 820-II". `u36` of this set is
undumped (it is keyboard-specific). Included for completeness/comparison.

| File | CRC32 | SHA1 |
|---|---|---|
| `537p3652.u33` | `7807cfbb` | `bd3cc5cc…` |
| `537p3653.u34` | `a9c6c0c3` | `c2da9d1b…` |
| `537p3654.u35` | `a8a07223` | `e8ae1ebf…` |

## `8086/` — 816 PC coprocessor boot ROM

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `8086.u33` | `ee49e3dc` | `a5f20c74…` | 4 KB. Internally "Xerox 816 PC". Reset vector at `FFFF0`, POSTs, then runs the Z80↔8086 mailbox dispatcher. Disassembly + protocol notes in [`../source/8086/`](../source/8086/) and [`../docs/16-8-architecture.md`](../docs/16-8-architecture.md). |

## `chargen/` — character generators

| File | CRC32 | SHA1 | Used by |
|---|---|---|---|
| `x820ii.u57` | `1a50f600` | `df4470c8…` | v4.04 set |
| `x820ii.u58` | `aca4b9b3` | `77f41470…` | v4.04 set |
| `u57.04.north.rom` | `eda727a2` | `292cd8a0…` | v5.0 set |
| `u58.03.north.rom` | `a2e514f3` | `8ac22dd0…` | v5.0 set |

## `keyboard/` — keyboard controller

| File | CRC32 | SHA1 | Notes |
|---|---|---|---|
| `820iikey.bin` | `8ea3b39b` | `3f05959f…` | Intel 8748 (1 KB) microcontroller firmware in the keyboard itself. It implements **two** selectable layouts — the standard plain-ASCII **G25** layout and a strap-selected bit-paired/Teletype layout. Reverse-engineered protocol + both encodings in [`../docs/g25-keyboard.md`](../docs/g25-keyboard.md). |
