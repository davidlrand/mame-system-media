# TRS-80 Model III — CP/M ("CP/M Cheap" mod)

A stock **TRS-80 Model III** (Tandy/Radio Shack, 1980) is a cassette/ROM-BASIC
machine; its 14 KB of ROM sits at `0x0000–0x37FF`, leaving no contiguous low RAM
for CP/M. The **"CP/M Cheap"** hardware modification (Bill Brewer, *80
Microcomputing*, March 1983, p. 112) adds a small replacement address-decoder
watching **bit 0 of the output latch at port `0xEC`** (REMAP): with it set, RAM
is mapped over `0x0000–0xF7FF` and the keyboard/video block is relocated to
`0xF800`/`0xFC00`, giving CP/M a full low-memory TPA. I wrote the **"Alpha One"
CP/M 2.2 BIOS** ("Alpha One CP/M 2.2 56K Version 1.15") for the modified machine.

## The driver is not in stock MAME

This is my own hardware modification and is **not** upstreamed, so MAME has no
`trs80m3cpm` system out of the box. Build it from the patch in
[`driver/trs80m3cpm.patch`](driver/trs80m3cpm.patch) against a MAME source tree:

```sh
cd /path/to/mame
git apply /path/to/driver/trs80m3cpm.patch        # patches src/mame/trs/trs80m3.{cpp,h} + trs80m3_m.cpp
# then register the system: add a line "trs80m3cpm" under the
# "@source:trs/trs80m3.cpp" block in src/mame/mame.lst (right after "trs80m3")
make SOURCES=src/mame/trs/trs80m3.cpp REGENIE=1   # (add your other SOURCES as needed)
```

The patch adds a `trs80m3cpm` clone of `trs80m3` (same ROMs): the REMAP banking
(via `memory_view` on port `0xEC` bit 0), FDC auto-enable when `-flop1` carries
an image, and the port-`0xE4` NMI-status fix the BIOS requires (see note below).

## Running it

Boot CP/M from the 40-track system disk in drive A:

```sh
./mame trs80m3cpm -flop1 disks/model-iii-cpm.1.15.imd
```

```
Alpha One CP/M 2.2 56K Version 1.15
A0>
```

Mount the data disk as drive B as well. It is an **80-track** disk, so drive B
(`fdc1`) must be the quad-density type:

```sh
./mame trs80m3cpm -flop1 disks/model-iii-cpm.1.15.imd \
                  -flop2 disks/mycpm-model3.imd -fdc1 80t_qd
```

The BIOS auto-detects each drive's format, so a 40-track 10×512 disk in A and an
80-track 5×1024 disk in B coexist fine.

## Contents

| File | Role | SHA-1 |
|---|---|---|
| `disks/model-iii-cpm.1.15.imd` | Bootable CP/M system disk (drive A), 40-track SS, 10×512 MFM | `da0c6ba7a3793038cfea8cdf3cf40163deb3bc47` |
| `disks/mycpm-model3.imd` | CP/M data/programs disk (drive B), 80-track SS, 5×1024 MFM — not bootable | `d12d2f74584ec26e9e606c115c46045d161bfcc1` |
| `driver/trs80m3cpm.patch` | MAME source patch adding the `trs80m3cpm` system | — |

## Provenance & attribution

- **The CP/M BIOS** on `model-iii-cpm.1.15.imd` is mine — "Alpha One CP/M 2.2",
  written for the CP/M Cheap-modified Model III. The disk is a system disk I
  built/sysgenned for that machine.
- **`mycpm-model3.imd`** is a drive-B working disk of mine (utilities: an ARC
  archiver suite, an assembler, BASIC, etc.); its directory track is empty, so it
  does not boot — it is data only. Any third-party CP/M utilities it carries are
  preserved under the LICENSE §2 terms.
- **The "CP/M Cheap" modification** is by Bill Brewer, published in *80
  Microcomputing*, March 1983, p. 112. The article is **not** re-hosted here —
  read it at the Internet Archive:
  <https://archive.org/details/80-microcomputing-magazine-1983-03/page/n111/mode/2up>.
- **CP/M** is a product of Digital Research, Inc.
- The Model III ROMs are loaded from MAME's stock `trs80m3` romset (Tandy/Radio
  Shack); they are not redistributed here.
- **The TRS-80 Model III Technical Reference Manual** (Tandy/Radio Shack, 1981)
  is not bundled here — read it at the Internet Archive:
  <https://archive.org/details/model-iii-technical-reference-manual-1981>.

### A note on the port-`0xE4` fix

The patched driver clears bit 0 of the port-`0xE4` NMI-status read for
`trs80m3cpm` only. That bit is *undefined* on a stock Model III, but a local
hardware modification on my machine drove it to 0, and the BIOS's NMI handler
depends on it (it tests the status for exactly `0x7E`). Stock `trs80m3`/`m4`/`m4p`
are untouched.
