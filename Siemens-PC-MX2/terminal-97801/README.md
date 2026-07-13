# Siemens 97801 terminal

The 97801 is the Siemens block terminal the PC-MX2 (and the wider
Transdata/9780 family) was sold with. It is **not** a generic RS-232 glass
TTY: SINIX addresses it with a proprietary protocol, including host-sent
keyboard-table downloads (the keyboard layout is switched by escape sequences
the OS sends at login, recovered from the SINIX floppies during the MAME
bring-up); a VT100 on the serial port will not produce a working system
console.

The terminal is a computer in its own right — two, counting its keyboard: a
**SAB8031** microcontroller (Siemens' MCS-51, ROM-less, 11.0592 MHz from the
22.1184 MHz clock module) executing the D21/D26 firmware EPROMs, an **SCN2672B** AVDC with
an SCB2673-class attribute plane and the D23 character generator (512 glyphs),
an **SCN2661B** EPCI for the host serial link, and a detached keyboard whose
own **MAB 8035HL** (MCS-48) runs the K111 firmware and talks to the 8031's
on-chip UART over a serial pair; the fourth and fifth CPU architectures in a
full PC-MX2 emulation.

Board photos: [`../docs/images/97801_board_d253.jpg`](../docs/images/97801_board_d253.jpg)
(**the ROM-dump source**: Plamen Mihaylov's W26361-D253 second-revision board, whose
`010/0118`-labelled D26/D21/D23 EPROMs are the images below, with its 22.1184 MHz
clock module), [`../docs/images/97801_board_DSCN4117.jpg`](../docs/images/97801_board_DSCN4117.jpg)
(Udo Möller's earlier W26361-D311 revision, 24.000 MHz), and
[`../docs/images/97801_board_12.jpg`](../docs/images/97801_board_12.jpg) (the still-later
D238 gate-array generation). The complete unit is
[`../docs/images/97801_terminal_with_keyboard.webp`](../docs/images/97801_terminal_with_keyboard.webp).
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
K111-Vn EPROMs, dumps wanted.)

## Board revisions and the emulated pairing

The terminal firmware times its keyboard UART at f_CPU/18432 (`TMOD=21h`,
`TH1=D0h`, the only reload it ever programs), and the K111 keyboard firmware
times its bit clock at f_KBD/9600 — so a terminal and keyboard interoperate
exactly when their crystals sit at the fixed ratio **f_CPU = 1.92 × f_KBD**.
Every photographed board revision satisfies it:

| Logic board | Clock modules | CPU clock | Matching keyboard crystal | Link | Unit |
|---|---|---|---|---|---|
| W26361-**D311** (1st rev.) | 24.000 + 4.9152 MHz | 12.000 MHz (÷2) | 6.25 MHz (inferred) | 651.04 Bd | Udo Möller's board |
| W26361-**D253** (2nd rev.) | **22.1184** + 4.9152 MHz | **11.0592 MHz** (÷2) | **5.760 MHz** (photographed) | **600.0 Bd** | **Plamen Mihaylov's — the ROM-dump source** |
| W26361-**D238** (gate-array gen.) | 44.2368 + 3.6864 MHz | 11.0592 MHz (80C31, ÷4) | 5.760 MHz | 600.0 Bd | Plamen Mihaylov's later unit |

**MAME emulates the D253 pairing**, because that is where both dumps
physically come from: the `010/0118`-labelled D26/D21/D23 EPROMs sit in
Plamen's D253 board (see `97801_board_d253.jpg` — 8031 at 22.1184/2 =
11.0592 MHz, video dot clock 22.1184 MHz, ≈57 Hz frame), and the K111-V1
keyboard EPROM comes from that unit's companion keyboard with its 5.760 MHz
crystal — one owner's matched, consistent set, linked at exactly 600 baud.

The pairing is not just numerology, and it is not covered by ordinary baud
tolerance: cross-clocking the two generations in the emulator (651-baud
terminal against the 600-baud keyboard, both ends running their original
firmware) makes the power-up handshake fail outright — the ≈8.5% rate gap
accumulates to ~0.7 bit periods by data bit 7, and neither receiver resyncs
mid-frame. Matched, the full handshake (`$2D`/`$2E` self-tests → `$AA`,
`$2F` ident → `"800105"`, `$2A` status → `$DC`) completes about 1.7 s after
power-on, and the terminal firmware sets its keyboard-OK flag — verified in
emulation against the terminal firmware's own acceptance test.

Packaged as a generic RS-232 terminal, `s97801` can serve as the console for
any MAME host, but its reason for being is the PC-MX2's SERAD port.
