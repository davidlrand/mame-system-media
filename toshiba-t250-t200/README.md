# Toshiba T-250 / T-200

Runnable media for the **Toshiba T-250** (8" DSDD) and **Toshiba T-200**
(5.25" DSDD) — 8085-based CP/M business computers, circa 1981. Both share the
same main board and peripheral set; only the floppy subsystem differs.

MAME driver: [`toshiba/t250.cpp`](https://github.com/mamedev/mame/pull/15372)
(PR #15372), systems `t250` and `t200`.

|  | T-200 | T-250 |
|---|---|---|
| CPU | Intel 8085A @ 5.333 MHz | Intel 8085A @ 5.333 MHz |
| RAM | 64 KB | 64 KB |
| Floppy | 5.25" DSDD, 35 trk, 300 RPM | 8" DSDD, 77 trk, 360 RPM |
| Display | 80×24 char, hardware inverse | 80×24 char, hardware inverse |
| Serial / printer | 8251 USART (CCM) · Centronics | 8251 USART (CCM) · Centronics |
| Hard disk (opt.) | SASI via 8155 host adapter | SASI via 8155 host adapter |

The boot ROM and CP/M 2.2 BIOS here are **not** Toshiba's stock firmware — I
wrote a replacement boot ROM and full BIOS from scratch as a reseller of these
machines in 1981–83. (Two disks, `T-250/Toshiba-original-v2.20` and
`T-200/t200-boot-v2.21`, *do* carry the stock Toshiba BIOS; see LICENSE.)

## Running it

```sh
# ROMs into MAME's ROM path
mkdir -p roms/t250 roms/t200
cp rom/t250boot.bin roms/t250/
cp rom/t200boot.bin roms/t200/

# boot a disk
mame t250 -window -flop1 disks/T-250/Toshiba-original-v2.20.mfi
mame t200 -window -flop1 disks/T-200/t200-boot-v2.21.mfi
```

Full serial / printer / SASI hard-disk invocations, the `newboot`+`newbios`
build-from-source pair, and CHD creation are in
[`docs/mame-usage.md`](docs/mame-usage.md). All disk formats (8 per machine)
are catalogued in [`docs/disk-formats.md`](docs/disk-formats.md).

## What's here

| Path | Content |
|---|---|
| [`rom/`](rom/) | `t250boot.bin`, `t200boot.bin` (match the MAME romset), `t250cg.bin` chargen |
| [`disks/T-250/`](disks/T-250/) | 6 boot disks: original v2.20, 2.14g, 2.12a demo, 1024-byte font disk, `newboot`/`newbios` source pair |
| [`disks/T-200/`](disks/T-200/) | v2.21 stock (+ ser-10133), v2.30a (512/1024-byte sectors), Trianex v3.0 |
| [`docs/`](docs/) | MAME usage guide + 16-format disk reference |
| [`docs/photos/`](docs/photos/) | Historical hardware photos (T-250 CRT board, front & back) |

Each disk ships as `.mfi` (MAME read/write) plus `.hfe` (GOTEK/FlashFloppy) or
`.imd` (cpmtools/libdsk), uncompressed. See [`disks/README.md`](disks/README.md)
for the per-disk rundown.

## Serial baud note

The CCM 8251 defaults to 19200 8N1, but the baud clock (1.9968 MHz) makes 19200
run at ~20800 — the clean rates are 9600 and below. **BIOS V2.21 has a serial
bug** (polls TxRDY without first enabling TxEN) that hangs serial output; use
**V2.30a or V2.32** for any upload/download/`PIP PUN:` workflow. Match both ends
of the link to the same rate (the on-disk `SET.COM` changes it).

## References

- T-200 product coverage — *Your Computer*, June 1982, p. 24:
  <https://archive.org/details/1982.06-your-computer-june-1982/page/24/mode/2up>
- Toshiba T-250 Functional Specification (September 1981) — full hardware
  reference, scanned from my personal archive (on the Internet Archive and
  bitsavers):
  <https://archive.org/details/bitsavers_toshibaT25nalSpecification198109_6869581>
- Toshiba T-250 User's Manual (1982) — scanned from my personal archive (on the
  Internet Archive and bitsavers):
  <https://archive.org/details/bitsavers_toshibaT25rsManual1982_12255177>

Both documents were scanned from my personal archive.

## Deeper archive

The companion repository
[`davidlrand/toshiba-t250-t200`](https://github.com/davidlrand/toshiba-t250-t200)
is the dedicated preservation home for these machines and will carry the CP/M
BIOS assembly source (`bios64.asm` and libraries) as it is published.

## Credits

- Hardware: Toshiba Corporation, 1981
- Replacement boot ROM + CP/M 2.2 BIOS, and the MAME driver: Dave L. Rand
