# Siemens 97801 terminal

The 97801 is the Siemens block terminal the PC-MX2 (and the wider
Transdata/9780 family) was sold with. It is **not** a generic RS-232 glass
TTY: SINIX addresses it with a proprietary protocol, including host-sent
keyboard-table downloads (the keyboard layout is switched by escape sequences
the OS sends at login, recovered from the SINIX floppies during the MAME
bring-up); a VT100 on the serial port will not produce a working system
console.

The terminal is a computer in its own right: a **SAB8031** microcontroller
(Siemens' MCS-51, ROM-less, 12 MHz from a 24 MHz crystal) executing the D21/D26
firmware EPROMs, an **SCN2672B** AVDC with an SCB2673-class attribute plane and
the D23 character generator (512 glyphs), an **SCN2661B** EPCI for the host
serial link, and a detached serial keyboard on the 8031's on-chip UART; the
fourth CPU architecture in a full PC-MX2 emulation.

Board photos: [`../docs/images/97801_board_12.jpg`](../docs/images/97801_board_12.jpg),
[`../docs/images/97801_board_DSCN4117.jpg`](../docs/images/97801_board_DSCN4117.jpg).

## ROMs (`roms/`)

| File | Role | CRC32 | SHA1 |
|---|---|---|---|
| `010_d21__0118_04.d21` | SAB8031 firmware EPROM (D21) | `b9b9df32` | `9a3ba060ebcf00b1ed9112493a1d73212c04d8e5` |
| `010_d23__0118_03.d23` | character generator, 512 glyphs (D23) | `23b22a7d` | `649abcfde9752f427ec7d1efdc013a4f01dc271c` |
| `010_d26__0118_04.d26` | SAB8031 firmware EPROM (D26) | `fcf045d7` | `4a98e7d2d98272970d627ce5c10e9572b87293d1` |
| `p26361_k111_v1_3.d3` | K111 keyboard controller v1.3 | `aba8f4b7` | `970a45b509081603e25319e1bbf0f7941f91a056` |

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

Input is MAME's natural keyboard. The detached keyboard's MCU is modelled
behaviourally (it resolves shift, emits the 7-bit "Platz" codes, answers the
status/identify commands, and rings the bell), so the K111 controller ROM
(`p26361_k111_v1_3.d3`) is preserved here for reference and possible future
full LLE rather than executed.

Packaged as a generic RS-232 terminal, `s97801` can serve as the console for
any MAME host, but its reason for being is the PC-MX2's SERAD port.
