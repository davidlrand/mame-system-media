# The Xerox G25 keyboard — interface & protocol

This documents the Xerox 820-II / 16-8 detached keyboard well enough to **build
a compatible one** (or a microcontroller emulation of one). It was reverse-
engineered from two sources:

1. the keyboard-handling code in the **820-II Technical Reference** monitor
   BIOS (the host side — how the Z80 expects to be talked to), and
2. **direct disassembly of the keyboard's own 8748 firmware**, `820iikey.bin`
   (CRC `8ea3b39b`, 1 KB), in [`../roms/keyboard/`](../roms/keyboard/).

Disassemble the firmware with the MAME `unidasm` tool:
`unidasm 820iikey.bin -arch mcs48`.

> Everything below is observed behaviour of the shipped ROM and BIOS. Addresses
> are 8748 ROM byte offsets unless prefixed with `$` for host (Z80) addresses.

## 1. Physical interface

The keyboard contains an **Intel 8748** single-chip MCU. It scans the key matrix,
debounces, translates to a code, and presents that code to the host over a
parallel byte bus plus a **strobe** line (`KBSTB`):

- **Data:** 8 bits, MCU port → host. The host reads it through a Z80 PIO
  (`kbpio`) at **I/O port `0x1E`** (PIO port B). The host BIOS reads the port and
  immediately complements it (`in a,($1E)` / `cpl`): the bus is wired inverted,
  so the value the BIOS works with equals the raw 8748 bus byte.
- **Strobe `KBSTB`:** MCU port 1 bit 4 → host PIO strobe input. The 8748:
  - asserts `KBSTB` **low** once the key byte is stable on the bus
    (`anl p1,#$EF` at offset `0x16C`), then
  - releases it **high** (`orl p1,#$10` at `0x172`).
- **Host latching:** the Z80 PIO is programmed MODE 1 (input). It **samples the
  data bus while the strobe is low** and **raises the keyboard interrupt on the
  strobe's rising (low→high) edge**. So the correct sequence a compatible
  keyboard must produce per keystroke is: *put byte on bus → drive strobe low
  (host samples) → drive strobe high (host interrupts)*. Holding the strobe
  static or pulsing it without fresh data will mis-latch.

This strobe polarity is the single most important detail and the easiest to get
backwards. (It was an inverted-polarity bug in MAME's model that this work
fixed: sampling on the wrong strobe level latches the *previous* byte, producing
a persistent one-character lag.)

## 2. Power-on "keyboard present" announcement

After reset the keyboard sends **one** byte, value **`0x9E`**, through the normal
strobe sequence, to tell the host a keyboard is attached. The 820-II monitor's
keyboard ISR (at `$F140` in relocated monitor RAM) does `in a,($1E)` / `cpl`,
then `cp $9E`: **only the exact code `0x9E`** takes the "gate open" branch — it
sets the keyboard-available flag and programs CTC channel 1 (`out ($19),$81`).
Any other first byte is treated as an ordinary keystroke and enqueued. So a
compatible keyboard must emit `0x9E` (not a key code) as its power-on hello.

## 3. Key codes

For ordinary keys the 8748 sends **7-bit ASCII** on the bus (bit 7 clear; the
host's `cpl` plus the inverted wiring cancel out, so internally the BIOS sees the
ASCII value directly). The translate path in the firmware:

- **Scan loop** begins at `0x01B`; it walks the matrix columns and finds the
  pressed key's index.
- **Translate** at `0x0F6` reads the modifier column (column 15) and branches:
  - `jb1` set → **Shift** → shifted table at `0x370`
  - `jb2` or `jb6` set → **Control** → control table at `0x3B8`
  - `jb0` set → **Caps Lock**
  - otherwise → base (unshifted) table at `0x328`

The base/shift/ctrl tables give a plain US-ASCII typewriter layout (letters,
digits, the usual punctuation, and a real `]` key — the shipped keyboard is
**not** bit-paired).

## 4. Two layouts in one ROM (the strap)

The same `820iikey.bin` supports **two** keyboard layouts, selected by a hardware
**strap** the firmware reads on test input T0 (8748 `jt0`):

- **Strap high (shipped / G25): standard US-ASCII layout.** The alternate
  remap below is bypassed. This is the layout the 820-II/16-8 actually ships
  with, and the one a G25-compatible keyboard should reproduce.
- **Strap low: an alternate bit-paired / Teletype (ANSI X3.23-style) layout.**
  The firmware overlays a remap table at ROM offset **`0x21C`** — twelve
  3-byte entries `[8748 scancode, unshifted ASCII, shifted ASCII]`:

  | scancode | unshifted | shifted |
  |---|---|---|
  | `0x32` | `2` | `"` |
  | `0x36` | `6` | `&` |
  | `0x37` | `7` | `'` |
  | `0x2F` | `8` | `(` |
  | `0x2E` | `9` | `)` |
  | `0x2D` | `0` | `0` |
  | `0x2C` | `-` | `=` |
  | `0x2B` | `^` | `~` |
  | `0x0C` | `@` | `` ` `` |
  | `0x0B` | `[` | `{` |
  | `0x11` | `;` | `+` |
  | `0x12` | `:` | `*` |

  This is the older bit-paired pairing (e.g. shift-`2` = `"`, shift-`6` = `&`,
  `;`/`+` and `:`/`*` on the same keys) for a different keyboard variant — it has
  no `]`/`}`/`_`. Don't apply it to a standard keyboard: it's the encoding for
  the alternate hardware, not a "fix" for the normal one.

## 5. Building a compatible keyboard — checklist

1. Scan the matrix; translate to **7-bit ASCII** (US layout) using Shift / Ctrl
   / Caps modifiers.
2. Drive the data byte onto the bus **inverted** relative to the host's `cpl`
   convention — i.e. the host should see the true ASCII after its complement.
   (Simplest: present true ASCII and let the cable/PIO inversion + BIOS `cpl`
   cancel, exactly as the original does.)
3. Per keystroke, strobe **low (host samples) → high (host interrupts)**.
4. At power-on, send the single byte **`0x9E`** through that same strobe
   sequence before any keys.
5. If you want the alternate bit-paired layout, tie the layout strap low and use
   the table in §4; otherwise leave it high for standard US-ASCII.


## The 16/8 low-profile keyboard (LPK) — a different animal

The low-profile 16/8 ships a **different keyboard** from the 8748 unit
documented above. It sends **matrix position codes** (not ASCII); the v5.0
monitor translates them through keytop tables in the boot ROM
(`u36.rx024.rom` at F800+: unshifted rows at `FB21/FB30/FB3D/FB4A`, shifted
at `FB88/FB97/FBA4/FBB1`). Shift/ctrl/lock travel as status bits, not codes.

The keytops are the **bit-paired (ASCII/JIS) layout**, *not* the
typewriter-paired layout of the 8748 keyboard:

```
unshifted:  \ 1 2 3 4 5 6 7 8 9 0 - ^     q..p @ [     a..l ; :     z../ 
shifted:    | ! " # $ % & ' ( ) = _ ~     Q..P ` {     A..L + *     Z..? 
```

So shift-2 = `"`, shift-7 = `'`, shift-8 = `(`, `*` lives on shift-`:`,
`+` on shift-`;` — and there are no `]`, `=`, or apostrophe keys. MAME's
`xerox_lpk` device HLEs this (position codes + these exact tables); its
natural-keyboard mappings were verified against the running monitor.
