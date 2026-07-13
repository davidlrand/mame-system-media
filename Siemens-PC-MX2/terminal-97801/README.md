# Siemens 97801 terminal

The 97801 is the Siemens block terminal the PC-MX2 (and the wider
Transdata/9780 family) was sold with. It is **not** a generic RS-232 glass
TTY: SINIX addresses it with a proprietary protocol, including host-sent
keyboard-table downloads (the keyboard layout is switched by escape sequences
the OS sends at login, recovered from the SINIX floppies during the MAME
bring-up); a VT100 on the serial port will not produce a working system
console.

The terminal is a computer in its own right — two, counting its keyboard: a
**SAB8031** microcontroller (Siemens' MCS-51, ROM-less, 12 MHz from the 24 MHz
clock module) executing the D21/D26 firmware EPROMs, an **SCN2672B** AVDC with
an SCB2673-class attribute plane and the D23 character generator (512 glyphs),
an **SCN2661B** EPCI for the host serial link, and a detached keyboard whose
own **MAB 8035HL** (MCS-48) runs the K111 firmware and talks to the 8031's
on-chip UART over a serial pair; the fourth and fifth CPU architectures in a
full PC-MX2 emulation.

Board photos: [`../docs/images/97801_board_12.jpg`](../docs/images/97801_board_12.jpg),
[`../docs/images/97801_board_DSCN4117.jpg`](../docs/images/97801_board_DSCN4117.jpg).
The detached keyboard's own controller board, an MCS-48 family 8035
microcontroller (a Philips `MAB 8035HL`) running the K111 EPROM (an Intel
`D2732A` at position D3, matching the ROM filename) off a 5.760 MHz crystal, is
shown in
[`../docs/images/97801_keyboard.webp`](../docs/images/97801_keyboard.webp).

## ROMs (`roms/`)

| File | Role | CRC32 | SHA1 |
|---|---|---|---|
| `010_d21__0118_04.d21` | SAB8031 firmware EPROM (D21) | `b9b9df32` | `9a3ba060ebcf00b1ed9112493a1d73212c04d8e5` |
| `010_d23__0118_03.d23` | character generator, 512 glyphs (D23) | `23b22a7d` | `649abcfde9752f427ec7d1efdc013a4f01dc271c` |
| `010_d26__0118_04.d26` | SAB8031 firmware EPROM (D26) | `fcf045d7` | `4a98e7d2d98272970d627ce5c10e9572b87293d1` |
| `p26361_k111_v1_3.d3` | K111-V1 keyboard firmware (International variant, keyboard 97801-111) | `aba8f4b7` | `970a45b509081603e25319e1bbf0f7941f91a056` |

These files are packaged as the MAME `s97801` romset in
[`roms/s97801.zip`](roms/s97801.zip), plus the keyboard device romset
[`roms/s97801_kbd.zip`](roms/s97801_kbd.zip) (the K111 dump, which the
`s97801_kbd` LLE device loads); place both in the rompath for `mame s97801`
(standalone terminal) or `mame pcmx2` (as the console).

## MAME status

The `s97801` device is a **fully working low-level emulation** and is the
SINIX system console: the SAB8031 runs its original D21/D26 firmware
unmodified, driving the SCN2672B AVDC, the SCB2673-class attribute plane and
the D23 character generator, with the SCN2661B EPCI carrying the host link (the
SINIX `SS97` setting: 38400 baud, 7O1, XON/XOFF). SINIX boots to the
installation dialog and on to the login screen through it (see the screenshots
in the top-level [README](../README.md)), with the proprietary block-terminal
protocol, the SGR attribute handling, the D23 glyph-index remapping, and the
host-downloaded keyboard tables all reproduced.

Input is MAME's natural keyboard, and the detached keyboard is itself **full
LLE** (the `s97801_kbd` device): an emulated MAB 8035HL executes the K111
controller ROM (`p26361_k111_v1_3.d3`) unmodified, scanning the recovered
16x8 key matrix and speaking its firmware-timed serial link, so the
self-test/status/identify handshake, the "800105" ident string, the bell, and
the shift/control resolution are all the real firmware's behaviour. (For
natural-keyboard typing, the input ports advertise the host-effective
characters — each key's firmware code as the terminal's power-on table
recodes it — so what you type is what SINIX receives; the physical
International cap legends are kept in the key names.) (The K111
code is MCS-48, matching the 8035 on the board — its init loop clears exactly
the 8035's 64 bytes of internal RAM; an independent re-dump confirms the EPROM
byte-for-byte, CRC32 `aba8f4b7`. This dump is **S26361-K111-V1**, the
International layout variant of the 1985-generation keyboard `97801-111` —
the layout lives in the keyboard firmware, so y/z and some punctuation follow
the International caps against the terminal's German power-on default table,
exactly as the real pairing would; the national variants V2-V10 are different
K111-Vn EPROMs, dumps wanted.) A neat
finding from the link analysis: terminal and keyboard crystals pair at a fixed
1.92 ratio (terminal baud = f_cpu/18432 via `TH1=D0h`, keyboard baud =
f_kbd/9600), and both photographed generations satisfy it exactly — the D311
board's 24 MHz (CPU 12 MHz) pairs with a 6.25 MHz keyboard at 651 baud, while
this keyboard's photographed 5.760 MHz pairs at 600 baud with the 11.0592 MHz
of the later D238 board (its 44.2368 MHz module / 4). MAME emulates the
matched D311 pair.

Packaged as a generic RS-232 terminal, `s97801` can serve as the console for
any MAME host, but its reason for being is the PC-MX2's SERAD port.
