# Xerox 820 Line — ROM Repository

A single comprehensive collection of the firmware for the Xerox 820 family:
820-I, 820-II (8" / 5.25" / SASI / low-profile-keyboard variants), 16/8
("820-II with the 816 PC 8086 board"), the RX/LPK u36 option-ROM line, and
related material (824 test-disk dumps, diagnostics, character generators,
keyboard controller).

ROMs recovered from the **Don Maslin 820-II disk archive** (bitsavers
`820ii_images`, disk IDs `BnnDnn` below) and the **MAME project ROM sets**
(`x820`, `x820ii`, `x168`, `x820kb`). Preserved for emulation and historical
reference. Firmware copyright **Balcones Computer Corporation / Xerox
Corporation** (820-II/16-8 monitor = the "Balcones Operating System",
R. Burns et al., BCC, 1982-84; RX/LPK u36 line = Rank Xerox).

Survey/provenance source of record:
`mame/x820disks/extracted-files/ROM-VERSIONS.md` (2026-06-10 survey of the 302
extracted Maslin disks). All CRC32s are plain zlib CRC32 of the raw binary.

## Layout

```
820-I/            factory / third-party / community monitor ROMs (u64+u63, 2x2K)
820-II/           Balcones monitor sets v4.00 ... v5.00 (u33-u35, 3x2K, + u36 option)
u36-rx-series/    the RX/LPK u36 option-ROM line, every recovered generation
16-8/             the 816 PC 8086 coprocessor boot ROM
hardware/         character generators + keyboard-MCU firmware (shared across models)
expansion-box/    documentation only - the RX024 / rgd5 box controller ROMs are UNDUMPED
824/              material from the Xerox 824 BIOS test disk (B18D9)
```

File naming: factory part numbers where known (`537p####`), MAME slot names
where the file fills (or fillable) a MAME ROM slot (`v400.u33`, `v404.u36`),
otherwise the original on-disk name (`rx006.com`, `patch.rom`).

## Master table

Validation status legend:
- **board dump** — read from a physical ROM (via the MAME ROM sets)
- **master-disk image** — binary ROM image recovered from a Balcones/Xerox master disk
- **master-disk build** — assembler output (.COM) recovered from a master disk
- **hex decode** — Intel-HEX text on a master disk, decoded here (method below)
- **source-rebuilt** — independently confirmed by assembling the recovered source
- **functionally validated** — boot-tested in MAME

### 820-I (u64 = 0000-07FF, u63 = 0800-0FFF)

| File | Machine | Version | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|---|
| `820-I/factory/x820v20.u64` | 820-I | Xerox Monitor v2.0 (21-Jul-1981) | 2048 | `2fc227e2` | `b4ea0ae23d281a687956e8a514cb364a1372678e` | MAME `x820` | board dump; boots in MAME |
| `820-I/factory/x820v20.u63` | 820-I | Xerox Monitor v2.0 | 2048 | `bc11f834` | `4fd2b209a6e6ff9b0c41800eb5228c34a0d7f7ef` | MAME `x820` | board dump; boots in MAME |
| `820-I/third-party/mxkx25a.u64` | 820-I | MICROCode SmartROM v2.3 | 2048 | `7ec5f100` | `5d0ff35a51aa18afc0d9c20ef99ff5d9d3f2075b` | MAME `x820` | board dump |
| `820-I/third-party/mxkx25b.u63` | 820-I | MICROCode SmartROM v2.3 | 2048 | `a7543798` | `886e617e1003d13f86f33085cbd49391b77291a3` | MAME `x820` | board dump |
| `820-I/third-party/p2x25a.u64` | 820-I | MICROCode Plus2 v0.2a | 2048 | `3ccd7a8f` | `6e46c88f03fc7289595dd6bec95e23bb13969525` | MAME `x820` | board dump |
| `820-I/third-party/p2x25b.u63` | 820-I | MICROCode Plus2 v0.2a | 2048 | `1e580391` | `e91f8ce82586df33c0d6d02eb005e8079f4de67d` | MAME `x820` | board dump |
| `820-I/third-party/xpro8u64.rom` | 820-I | "X-8 System Monitor v1.21" | 2048 | `8873a2d6` | `9f0ebefb6dc4d5aa27811ce03129bb7dc0204196` | B13D17 `XPRO8U64.ROM`, hex decode | master-disk image (not in MAME) |
| `820-I/third-party/xpro8u63.rom` | 820-I | "X-8 System Monitor v1.21" | 2048 | `2583a1d3` | `ff9ebf1d845d8aac74d8d36badbc45da58286a85` | B13D17 `XPRO8U63.ROM`, hex decode | master-disk image (not in MAME) |
| `820-I/community/rom0.com` | 820-I | Micro Cornucopia "XEROX 820 4 Mhz VER. 3.0", ORG 0 | 2816 | `2b6f7788` | `cff9b09c880db15887d9a1c70f8589cd610784e8` | B22D13 `ROM0.COM` | master-disk build; source on same disk (ROM1.MAC + modules) |
| `820-I/community/rom100.com` | 820-I | same, ORG 100h relocation | 2816 | `74ef22b5` | `ce0f804de436a551dfd6d1d7734f5303d7d9e263` | B22D13 `ROM100.COM` | master-disk build (differs from rom0 only in first KB — verified) |
| `820-I/community/newmon10.com` | 820-I + SWP DD board | SWP NEWMON 1.0 (7-Sep-83) | 2048 | `ef2400fa` | `902a95a1e419ccd4a062c95b3eabe6fd012a4cd7` | B22D13 `NEWMON10.COM` | master-disk build; source on same disk |
| `820-I/community/newmon15.com` | 820-I + SWP DD board | SWP NEWMON 1.5 (cal fix 15-Jan-84) | 2048 | `87e0ed0f` | `e93788a963dff3d53e57d9a5056d608aa5b9cd3a` | B22D13 `NEWMON15.COM` | master-disk build |

### 820-II monitor sets (u33 = 0000-07FF, u34 = 0800-0FFF, u35 = 1000-17FF, u36 option = 1800-1FFF)

| File | Machine | Version | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|---|
| `820-II/v4.00/v400.u33` | 820-II | Balcones OS v4.00 (6/18/82) | 2048 | `0d1bcaa8` | `a6ac83f8584d19f7a08e666cb5d4b62620d7d3c0` | B16D35 `ROM400.COM` split (see method) | master-disk build; fills MAME NO_DUMP `v400.u33` |
| `820-II/v4.00/v400.u34` | 820-II | Balcones OS v4.00 | 2048 | `f1df9e29` | `79f38880c3aed9ddf2ccba4ddb11128586dc9c25` | B16D35 `ROM400.COM` split | master-disk build; fills MAME NO_DUMP `v400.u34` |
| `820-II/v4.00/v400.u35` | 820-II | Balcones OS v4.00 | 2048 | `68357ed7` | `78498542f4d8506edf5f2b3b9ed0fde3fd72f85b` | B16D35 `ROM400.COM` split | master-disk build; fills MAME NO_DUMP `v400.u35` |
| `820-II/v4.01/v401.u33` | 820-II | Balcones OS v4.01 (~Jul 1982) | 2048 | `fe9fa596` | `194162b3d063b2d1bcad03d0bee51dabce2d1985` | B17D7 `U33.HEX` ("820-II ROM IMAGES MASTER"), hex decode | master-disk image; fills MAME NO_DUMP `v401.u33`; rebuildable from XROM.MAC (REV 401, 4 source copies + B11D7 PRN/REL) |
| `820-II/v4.01/v401.u34` | 820-II | Balcones OS v4.01 | 2048 | `d3137de3` | `d1de4e11f29799b2024af0412415a07984e58f3a` | B17D7 `U34.HEX`, hex decode | master-disk image; fills MAME NO_DUMP `v401.u34` |
| `820-II/v4.01/v401.u35` | 820-II | Balcones OS v4.01 | 2048 | `bf3096fb` | `136f6b1cf2cd93f0b908688394675ef69883f47b` | B17D7 `U35.HEX`, hex decode | master-disk image; fills MAME NO_DUMP `v401.u35` |
| `820-II/v4.01/patch.rom` | 820-II w/ v4.0x ROMs | BCC v4.01 field-patch option ROM | 2048 | `733d2ebd` | `56c8ff415af7e414521d777a95af7e3355bff68a` | B16D32 `PATCH.ROM` ("4.01 ROM patch from BCC") | master-disk image; u36-socket option ROM (55AAh magic) that RAM-patches the v4.01 WD-1797 driver stack-overflow bug; source PATCH.MAC + transmittal memo on same disk |
| `820-II/v4.02/u33.4.02.rom` | 820-II | Balcones OS v4.02 | 2048 | `d9eb668e` | `6acbef96e4e6526c58e068b7849fb9cce2ea2a10` | MAME `x820ii` | board dump; boots in MAME |
| `820-II/v4.02/u34.4.02.rom` | 820-II | Balcones OS v4.02 | 2048 | `62181209` | `2238aec096d19af9307bb294532f66f53dd7dfc3` | MAME `x820ii` | board dump |
| `820-II/v4.02/u35.4.02.rom` | 820-II | Balcones OS v4.02 | 2048 | `e22fbf6d` | `6c162f79d42611176b0f1c0e8a4eeb07492beca1` | MAME `x820ii` | board dump |
| `820-II/v4.02/u36.rx11.4.02.rom` | 820-II (RX) | RX ver 011 (15-Sep-82) | 2048 | `b6a239ce` | `330d28fa8ec006d48d948b1c5e714ffced88fe90` | MAME `x820ii` | board dump; source generation = B16D34 RX1984.MAC ver 011 |
| `820-II/v4.03-partial/p10600.rom` | 820-II | v4.03 candidate u33 (537P10600?) | 2048 | `cf09c8c2` | `12354fe08fccb388c45a914dc36d6f2161365217` | B17D7 `P10600.ROM`, hex decode | master-disk image; differs from v4.01 u33 by 5 bytes (verified); no known board dump to compare |
| `820-II/v4.03-partial/p10601.rom` | 820-II | u34 (537P10601?) | 2048 | `a9c6c0c3` | `c2da9d1bf0da96e6b8bfa722783e411d2fe6deb9` | B17D7 `P10601.ROM`, hex decode | master-disk image; **byte-identical to the dumped v4.04 `537p3653.u34`** (verified) |
| `820-II/v4.04/537p3652.u33` | 820-II / 16-8 | Balcones OS v4.04 ("Sign-on message change") | 2048 | `7807cfbb` | `bd3cc5cc5c59c84a50747aae5c17eb4617b0dbc3` | MAME `x820ii`/`x168` | board dump; boots in MAME |
| `820-II/v4.04/537p3653.u34` | 820-II / 16-8 | Balcones OS v4.04 | 2048 | `a9c6c0c3` | `c2da9d1bf0da96e6b8bfa722783e411d2fe6deb9` | MAME `x820ii`/`x168` | board dump |
| `820-II/v4.04/537p3654.u35` | 820-II / 16-8 | Balcones OS v4.04 | 2048 | `a8a07223` | `e8ae1ebf2d7caf76771205f577b88ae493836ac9` | MAME `x820ii`/`x168` | board dump |
| `820-II/v4.04/v404.u36` | 820-II LP / 16-8 | RX ver **016** (27-Sep-83) — best-candidate stand-in for v4.04's exact-ship u36 | 2048 | `97047d38` | `f36506635653736b8d754d2c04f608180602b5a2` | B17D7 `U36.ROM`, hex decode | master-disk image; **functionally validated 2026-06-10: boots v4.04 set, LPK keyboard verified, standard keyboard correctly gated** |
| `820-II/v5.00/537p10828.u33.5.0.bin` | 820-II / 16-8 | Balcones OS v5.00 ("Expansion Box II") | 2048 | `a17af0f1` | `b1d9a151ed4558f49b3cdc1adbf348b54da48877` | MAME `x820ii` (= `l5.u33.rom`) | board dump; source-rebuilt (B16D40/B23D13 XR.MAC, = B23D13 `U33-500.ROM` and `ROM500.COM[0:0x800]`); boots in MAME |
| `820-II/v5.00/537p10829.u34.5.0.bin` | 820-II / 16-8 | Balcones OS v5.00 | 2048 | `c9f5182e` | `ac830848614cea984c849a42687ea2944d6765d9` | MAME `x820ii` (= `l5.u34.rom`) | board dump; source-rebuilt |
| `820-II/v5.00/537p10830.u35.5.0.bin` | 820-II / 16-8 | Balcones OS v5.00 | 2048 | `278fa75f` | `f47cf9eb30366211280f93a8460523fcc53eebe9` | B23D13 `U35-500.ROM` / `ROM500.COM[0x1000:0x1800]` | **master-disk image** — the only clean u35; both circulating board reads are bad (see `bad-dumps/`); source-rebuilt; boots in MAME |
| `820-II/v5.00/537p10831.u36.5.0.bin` | 820-II LP / 16-8 | LPKYBD ver **018** (Jan-1984) | 2048 | `cda7f598` | `08ffd18959e1708136076c82486b8d121a04fa23` | MAME `x820ii` | board dump; source-rebuilt (B16D40 LPKYBD.MAC ver 018; = B23D13 `U36-V18.ROM`) |
| `820-II/v5.00/bad-dumps/537p10830.u35.bad-3bit.bin` | — | v5.00 u35, bad read | 2048 | `cc4e1c2b` | `375bbed76d9088dec82b9599cd810727d3e605f3` | former MAME BAD_DUMP | 3 single-bit errors vs `278fa75f` (offsets 0x12/0x32/0x39); kept as board-read evidence corroborating the clean image |
| `820-II/v5.00/bad-dumps/l5.u35.rom` | — | v5.00 u35, bad read | 2048 | `44c8dbf8` | `cba925b425a7a5ca68dc9fed10ea33e100704bf4` | former MAME BAD_DUMP (`x168`) | same ROM with data bit 7 stuck high; kept as board-read evidence |

### 820-II diagnostics set (537P60xx — recovered, identity unverified)

Found on B13D17 alongside the X-PRO material; **not in the ROM-VERSIONS.md
survey and not in MAME** — included as a clearly-marked extra. The code ROMs
carry an 820-II diagnostic menu ("Memory Test / Graphics Test / Comm Loop
Test / Crt Test", "Dead BitMap Display", f1-f8 soft keys) and a 1983
Balcones-Computer copyright (interleaved string). u58 decodes byte-identical
to the standard 820-II u58 chargen — which both authenticates the set and
identifies **537P2849 as the factory part number of `x820ii.u58`**.

| File | Version | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|
| `820-II/diagnostics-537p60xx/537p6022.u33` | diag code | 2048 | `b1d681cb` | `6cdb82a0e0be0b549539e9ce1d2c496696c76166` | B13D17 `537P6022.U33`, hex decode | master-disk image, unverified |
| `820-II/diagnostics-537p60xx/537p6023.u34` | diag code | 2048 | `82da9fb8` | `b546c887e9ac1c8de51270192ce16df3218318f8` | B13D17 `537P6023.U34`, hex decode | master-disk image, unverified |
| `820-II/diagnostics-537p60xx/537p6024.u35` | diag code | 2048 | `4262070a` | `7ca3e55fc39a85557efd5bfe25195465096d814a` | B13D17 `537P6024.U35`, hex decode | master-disk image, unverified |
| `820-II/diagnostics-537p60xx/537p6027.u36` | diag code | 2048 | `c4094708` | `cf422d90ee178b600e5ea0e7e0f161c8249823c9` | B13D17 `537P6027.U36`, hex decode | master-disk image, unverified |
| `820-II/diagnostics-537p60xx/537p6025.u57` | chargen variant | 2048 | `fe009eab` | `be3f41f1f885bfc0051330cc966b7f809fffcf55` | B13D17 `537P6025.U57`, hex decode | master-disk image, unverified (differs from both known u57s) |
| `820-II/diagnostics-537p60xx/537p2849.u58` | chargen | 2048 | `aca4b9b3` | `77f41470b0151945b8d3c3a935fc66409e9157b3` | B13D17 `537P2849.U58`, hex decode | **= MAME `x820ii.u58` byte-exact** (cross-validated) |

### u36 RX/LPK option-ROM line (every recovered generation)

The u36 socket holds the Rank Xerox option ROM (`RX1984.MAC`, later
`LPKYBD.MAC`): keyboard/CRT/screen-print/printer overlays, and from ver 012
the position-encoded Low Profile Keyboard handler (keyboard spec 156P82508).
Layout: version byte at offset 0x7FA (from ver 012; earlier builds carry FF
there), `FF AA 55 <checksum>` trailer at 0x7FC-0x7FF — verified on every
image below.

| File | Version | Date | Pairs with | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|---|---|
| `u36-rx-series/v006/rx006.com` | ver 006 | 18-Jun-82 | XR v4.00 | 2048 | `c2d3cddb` | `f7f108ac301e1889b6ca7a27d7da738e63c26f08` | B16D29 `RX006.COM` | master-disk image; source RX1984.MAC ver 006 on B16D29/B16D31 |
| `u36-rx-series/v011-rx11/u36.rx11.4.02.rom` | ver 011 | 15-Sep-82 | XR v4.02 | 2048 | `b6a239ce` | `330d28fa8ec006d48d948b1c5e714ffced88fe90` | MAME `x820ii` | board dump; source = B16D34 RX1984.MAC ver 011 |
| `u36-rx-series/v012/lpkybd.com` | ver 012 (first LPK) | 19-Jan-83 | XR 4.01/4.02 only | 2048 | `53a2b703` | `d733ae472442f8a7243d9337a7875c424f59af5f` | B16D30 `LPKYBD.COM` | master-disk image; cross-checked against B16D30 `LPKYBD.HEX` (identical except the trailer checksum byte, 00 in the HEX); source + PRN on same disk |
| `u36-rx-series/v016/u36-v016.rom` | ver 016 | 27-Sep-83 | XR v4.04 | 2048 | `97047d38` | `f36506635653736b8d754d2c04f608180602b5a2` | B17D7 `U36.ROM`, hex decode | same bytes as `820-II/v4.04/v404.u36`; functionally validated 2026-06-10 (see above) |
| `u36-rx-series/v018/537p10831.u36.5.0.bin` | ver 018 | 20-Jan-1984 | XR v5.00 | 2048 | `cda7f598` | `08ffd18959e1708136076c82486b8d121a04fa23` | MAME `x820ii` | board dump; source-rebuilt (B16D40 LPKYBD.MAC) |
| `u36-rx-series/rx024/u36.rx024.rom` | ver 024 | 1984? | XR v5.00 (16/8) | 2048 | `a7f1d677` | `8c2a442f3a691f2e181a640d65f767ce3b51d711` | MAME `x168` | board dump; **no source or disk image recovered** (LPKYBD vers 019-024 absent from the Maslin set); boots the 16/8 in MAME |

### 16-8

| File | Machine | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|
| `16-8/8086/8086.u33` | 16/8 "816 PC" 8086 board | 4096 | `ee49e3dc` | `a5f20c74fc53f9d695d8894534ab69a39e2c38d8` | MAME `x168` | board dump; boots in MAME; disassembly + mailbox protocol in `../Xerox-820-16-8/source/8086/` |

### Shared hardware

| File | Used by | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|---|
| `hardware/chargen/x820.u92` | 820-I | 2048 | `b823fa98` | `ad0ea346aa257a53ad5701f4201896a2b3a0f928` | MAME `x820` | board dump |
| `hardware/chargen/x820ii.u57` | 820-II v4.0x | 2048 | `1a50f600` | `df4470c80611c14fa7ea8591f741fbbecdfe4fd9` | MAME `x820ii` | board dump |
| `hardware/chargen/x820ii.u58` | 820-II v4.0x (= part 537P2849) | 2048 | `aca4b9b3` | `77f41470b0151945b8d3c3a935fc66409e9157b3` | MAME `x820ii` | board dump; part number established via B13D17 `537P2849.U58` |
| `hardware/chargen/u57.04.north.rom` | 820-II/16-8 v5.0 | 2048 | `eda727a2` | `292cd8a0dc6699c3a2091b20c0fc63d97a266fbf` | MAME `x820ii` | board dump |
| `hardware/chargen/u58.03.north.rom` | 820-II/16-8 v5.0 | 2048 | `a2e514f3` | `8ac22dd0cf0324a857718adf67b41912864893a3` | MAME `x820ii` | board dump |
| `hardware/keyboard/820iikey.bin` | 820-II/16-8 standard (G25) keyboard | 1024 | `8ea3b39b` | `3f05959f54a558b273567b1b4f0c7cdf46d8d9bf` | MAME `x820kb` | board dump (8748 MCU inside the keyboard); implements both the plain-ASCII G25 layout and a strap-selected bit-paired layout — protocol notes in `../Xerox-820-16-8/docs/g25-keyboard.md` |

The **Low Profile Keyboard** (spec 156P82508, position-encoded, 97 keystations
US / 100 RX) contains its own — **undumped** — microcontroller; MAME's
`xerox_lpk` device is a high-level emulation reconstructed from the LPKYBD.MAC
keystation tables. No LPK ROM file exists to include here.

### 824 material (from the "824 BIOS TEST DISK ... HORTON 12/23/81", B18D9)

| File | Size | CRC32 | SHA1 | Origin | Validation |
|---|---|---|---|---|---|
| `824/537p3682.rom` | 4096 | `88f30a00` | `54d205e5a7ed80feb7cd06eddc863110fc655dde` | B18D9 `537P3682.ROM`, hex decode | master-disk image; the independent copy on B13D16 decodes byte-identical (verified); unidentified 4K Xerox part, possible 824 connection |
| `824/8749.rom` | 2048 | `b5d2efe5` | `cb79f1ab72a9dad090b91f78e3b431b1b15531c6` | B18D9 `8749.ROM`, hex decode | master-disk image; 8749 MCU dump — possible keyboard-MCU variant (the 820-II keyboard is a 1K 8748) |

## Extraction methods

### ROM400.COM / ROM500.COM container split
Balcones distributed each 3-chip monitor build as a single raw 6144-byte image
(also wrapped as `YROM.COM` = 256-byte CP/M loader stub + the same image —
verified for B16D35). Split at 0x800 boundaries: bytes 0x0000-0x07FF = u33,
0x0800-0x0FFF = u34, 0x1000-0x17FF = u35.

**Method authentication:** `B23D13/ROM500.COM` split this way reproduces the
three physically-dumped v5.0 ROMs `537p10828/29/30`
(`a17af0f1`/`c9f5182e`/`278fa75f`) **byte-exactly** (re-verified during this
build). The v4.00 set here is the same split applied to `B16D35/ROM400.COM`
(sign-on in-image: `820-II v 4.00`, `(c) 1982 Xerox Corp`).

### Intel-HEX decode
The `.HEX`/`.ROM` text files were decoded with a tolerant Intel-HEX reader
(type-00 data records placed by address, type-01 EOF, per-record checksums
verified). Recorded anomalies — none affect the data:

- **CP/M ^Z slack:** most `.ROM` files end with a `^Z`-truncated 65th line
  followed by one garbage line (sector-slack from the CP/M filesystem, after
  the complete 64 records of a 2K image). Skipped; data coverage was complete
  in every file.
- **Missing EOF record:** none of the B17D7/B13D17/B18D9 files carry a type-01
  record (lost with the ^Z truncation). Harmless.
- **Non-zero base address:** several files were saved from a PROM-burner/CP/M
  buffer rather than address 0 — `P10600/P10601/U33-type .ROM` files and
  `8749.ROM` at base 0x0100 (the CP/M TPA), `XPRO8U63.ROM` at 0x0900,
  `B13D16/537P3682.ROM` at 0x0100, `LPKYBD.HEX` at 0x1800 (the u36 map
  address). The base was subtracted; full 2048/4096-byte coverage confirmed
  in every case, and every file with an independent reference decoded to the
  expected hashes (v401 set, v016 = MAME `v404.u36` byte-exact, P10601 =
  `537p3653.u34` byte-exact, 537P3682 cross-copy identical, LPKYBD.HEX =
  LPKYBD.COM except checksum byte).

## Lineage

```
820-I  (u64/u63, 2x2K)
  Xerox Monitor v1.0 (1980, R. Smith)               [UNDUMPED - MAME NO_DUMP]
  Xerox Monitor v2.0 (21-Jul-1981, L. Tran)         [820-I/factory]
   |- Micro Cornucopia 4MHz "VER. 3.0" (J. Mayhugh, Apr-1984)  [820-I/community]
   |- SWP NEWMON 1.0/1.5 (Sep-83/Jan-84), DDMON replacement    [820-I/community]
   |- "X-8 System Monitor v1.21" (third party)                 [820-I/third-party]
   '- MICROCode SmartROM v2.3 / Plus2 v0.2a                    [820-I/third-party]

820-II / 16-8  Balcones OS (R. Burns, BCC), u33-u35 (3x2K) + u36 option (2K)
  0.00-0.10 (Mar 1982) -> 1.01-1.90 -> 3.00/3.01 -> 3.90 (5/26/82)
  -> 4.00 (6/18/82)   [820-II/v4.00 - from ROM400.COM]
  -> 4.01             [820-II/v4.01 - from B17D7 hex; + BCC field patch]
  -> 4.02             [820-II/v4.02 - board dumps; no source recovered]
  -> 4.03             [820-II/v4.03-partial - u33 candidate only]
  -> 4.04 = rev 404 "Sign-on message change"   [820-II/v4.04 - 537p3652-54]
  -> 4.90/4.91 (5.25" driver loader, power-up diagnostics)
  -> 5.00 = "Expansion Box II"                 [820-II/v5.00 - 537p10828-31]

u36 RX/LPK option ROM (Rank Xerox: RX1984.MAC -> LPKYBD.MAC)
  001 (4-Jun-82, for XR 3.90) ... 006 (18-Jun-82, for XR 4.00)  [v006]
  -> 011 (15-Sep-82)                                            [v011-rx11]
  -> 012 (19-Jan-83) first Low-Profile-Keyboard support         [v012]
  -> 013/014 (1983, nationalized tables)                        [no image]
  -> 015 (23-Aug-83, sign-on "Xerox" - the v4.04 change)        [no image]
  -> 016 (27-Sep-83)                                            [v016]
  -> 017 (4-Nov-83, labeled on B16D40, superseded)              [no image]
  -> 018 (20-Jan-84) = 537P10831                                [v018]
  -> 024 (16/8 v5.0 ship)                                       [rx024 - dump only]
```

## Still undumped / missing

- **820-I Xerox Monitor v1.0** (`x820v10.u63/u64` — MAME NO_DUMP).
- **v4.04's exact-ship u36** — RX ver 016 (`v404.u36` here) is the
  best-candidate stand-in: ver 015 made the same "Xerox" sign-on change as XR
  rev 404, ver 016 follows it, and the next monitor release is 5.0. It is
  functionally validated against the 537p3652-54 set but is not a dump of a
  socketed v4.04-era part.
- **`v500.u36`** — the u36 actually shipped with v5.00 820-IIs (MAME NO_DUMP);
  ver 018 (537P10831) is the closest recovered build.
- **RX vers 019-024 source** — `u36.rx024.rom` exists only as a board dump.
- **v4.03 u34(?)/u35/u36** — only the u33 candidate (`p10600.rom`) and the
  u34 (= v4.04 u34) were recovered; without a u35 the set's sign-on version
  cannot be confirmed, so the MAME `v403` slots stay NO_DUMP.
- **v4.02 and v4.03 sources**, and a v4.04 source snapshot (its history
  survives inside the v5.0 XR.MAC).
- **The expansion-box controller ROMs** — see `expansion-box/README.md`.
- **The Low Profile Keyboard MCU** (spec 156P82508).
- Possibly related, recovered but unidentified: `537P3655-3661.ROM` (seven
  sequential 8K Xerox parts on B17D7, no strings, no MAME matches — left in
  the extraction, not included here pending identification).

## Transcribed revision histories

### Balcones Operating System for the Xerox 820-II — full REV history

From XROM.MAC (B1D1, uppercase edition, through 401) merged with the v5.0
XR.MAC (B16D40/B23D13) for 402-500. Verbatim except line-prefix `;` trimming.

```
                 Balcones Operating System for XEROX 820-II
              Copyright 1982 (C) Balcones Computer Corporation
                            All rights reserved

  Genesis:

  In the beginning there was no order, only chaos.  Then came
  the Big Board.  This expanding cloud of hot gas contained the
  fundamental matter necessary to produce a Xerox copy of itself.

  Version 0.00    March 2, 1982.  Robert Burns, Bcc.
      The 820-2/4 monitor sources converted to M80 format.

  Version 0.01    March 3, 1982   Robert Burns, Bcc.
      The code placement was reorganized to conserve valuable RAM.
      In particular, the CRT driver was reworked extensively.  It
      now executes directly out of the ROM in the screen bank.
      There ain't no such thing as a free lunch.  (wait states)
      The G command scans arguments to HL, DE, BC for the callee.
      It also prints the A register and the HL registers on return.
      The Input and Output commands were repaired for scroll port.
      A debug loader is provided for check-out in banked RAM.

  Version 0.02    March 4, 1982.  Robert Burns, Bcc.
      The CRT driver will allow interrupts during screen processing.
      A fast CRT driver was provided that does not save/restore the
      registers.  CP/M does not require this time consuming service.
      The upper 4k of RAM is tested on reset.

  Version 0.03    March 5, 1982.  Robert Burns, Bcc.
      Several Entry Point Vectors were installed to access a
      standard physical disk driver executrix.  They provide
      a complete, dynamic logical to physical device mapping.
      The 820 disk entry vectors were replaced by an Emulator
      that interfaces to this logical to physical disk mapper.
      The standard CP/M deblocker also knows how to employ these
      drivers.  1k of code space was reserved within the RAM
      Resident Monitor for the actual physical drivers, which
      are assembled and linked independently.

  Version 0.04    March 9, 1982.  Robert Burns, Bcc.
      An escape table with insert line, delete line, and sundry
      attribute control functions was installed.  Change message text
      as specified by C. Key.  In and Out commands accept 16 bit
      port addresses.  Command to select 7 or 8 bit data from keyboard.

  Version 0.05    March 10, 1982.  Robert Burns, Bcc.
      Background screen print with Xon/Xoff protocol added.  Insert and
      delete character provided.  The configuration byte will provide the
      calling program with information about the attached peripherals
      and software flags indicating 7 or 8 bit keyboard data, etc.
      An Options statement was added that allows certain less important
      routines to not be included at assembly time, since ROM space
      is tight.  Provided a user accessible 1 second interrupt jump vector.

  Version 0.06    March 12, 1982.  Robert Burns, Bcc.
      Corrected Disk Parameter Tables for variable rigid partitions.
      Made boot loader read into temporary buffer so big sectors
      Don't clobber the TPA.

  Version 0.07    March 13, 1982.  Robert Burns, Bcc.
      Added high level language interface to Daytim, Getsel, and Getcon
      Centralized CRT RAM turn on/off

  Version 0.08    March 13, 1982.  Robert Burns, Bcc.
      Set escape table as an option, with all video control codes
      as control characters, except cursor positioning.  Set
      typewriter/terminal mode as an option.  Corrected configuration
      check to disable drive select if WD 1797 disk board is installed.

  Version 0.09    March 14, 1982.  Robert Burns, Bcc.
      Added configurable options: Xon/Xoff, hardware handshake,
      step rate for WD 1797.  Use keyboard data mask to mask
      video output data so that WordStar files display properly
      when typed, if keyboard passes only 7 bits.  Cursor modified.
      Added rigid disk boot from any partition.

  Version 0.10    March 14, 1982.  Robert Burns, Bcc.
      Burned first ROM

  Version 1.0x    March 18, 1982...
      Boot command now automatically switches drive with A:
      Won't jump into an E5 sector,
      Enabled disk read/write commands, and
      Dug gold mine in video driver.

REV DEFL 101
REV DEFL 102
REV DEFL 110      ;ROM burned
REV DEFL 111      ;new WD driver
REV DEFL 112      ;baud rate command, II/IV
REV DEFL 120      ;ROM Burned
REV DEFL 121      ;line feed burys gold
REV DEFL 122      ;made LDIRS macro
REV DEFL 130      ;new disk drivers
REV DEFL 131      ;fixed single density selection
REV DEFL 140      ;burned ROM
REV DEFL 150      ;transient area code macros
REV DEFL 151      ;transient commands
REV DEFL 160      ;Burned ROM with faith
REV DEFL 161      ;Faith didn't help
REV DEFL 170      ;coming down the wire
REV DEFL 171      ;reworked double sided code
REV DEFL 172      ;fixed SA1403 DS
REV DEFL 173      ;installed help text
REV DEFL 174      ;fixed 1797 DS
REV DEFL 175      ;rigid partitions
REV DEFL 176      ;write enabled floppy if rigid par bad
REV DEFL 177      ;CRC, config, more gold for the Goose
REV DEFL 178      ;almost Final ROM Prior to QA Testing
REV DEFL 179      ;adjusted SIO mask for S. T.
REV DEFL 180      ;this is the one that burned 'em up
REV DEFL 181      ;fix DD configure
REV DEFL 300      ;This was IT
REV DEFL 182
REV DEFL 183      ;parallel printer, copyright, iobyte
REV DEFL 190      ;disk drivers at same exec address
REV DEFL 300      ;This is really it, Charlie
REV DEFL 301      ;proof that there is always another change
REV DEFL 399      ;SA1403D error recovery, AS31A rev 02
REV DEFL 398      ;WD1797 density select error recovery
REV DEFL 397      ;idle / soft error report
REV DEFL 396      ;Western Digital cleanup, removed fill, ramt
REV DEFL 395      ;eat keyboard error message
REV DEFL 394      ;auto boot A:
REV DEFL 393      ;fix auto boot
REV DEFL 392      ;no auto boot
REV DEFL 391      ;fix SA select failure, WD ready time, 10 msec
REV DEFL 390      ;fix SA floppy format code
REV DEFL 400      ;handle dead DTC controller during density sel
REV DEFL 401      ;corrected WD driver stack overflow
--- (above this line: XROM.MAC v4.01; below: only in the v5.0 XR.MAC) ---
rev DEFL 402      ; DTR settings restored to 820 mode
rev DEFL 403      ;Disabled RTS on comm port
rev DEFL 404      ;Sign-on message change
rev DEFL 490      ;5.25" disk driver loader
rev DEFL 491      ;added power up diagnostics
rev DEFL 500      ;Expansion Box II
```

Note: the 399-390 block is in descending file order (each fix was inserted
above the previous), so a file "ending" at 390 assembles as v3.90. The
effective version of a source file is the **last** `REV DEFL` that assembles.

### RX1984 / LPKYBD (u36 option ROM) — full ver history

From LPKYBD.MAC (B16D40, ver 018). Header:

```
  LPKYBD is the position encoded keyboard handler dedicated
      for the 820-II and 16/8 product family.
  1983 XEROX COPYRIGHT

  RX1984 is the stand alone rom addition to the Xerox 820-II
      monitor. It is called once during monitor restart
      and at that time it patches the monitor in ram to
      call the rx modifications to k/b,crt,Screenprint
      and printer routines.
      It then moves in its own SIGNON overlay and jumps into it.

      This SIGNON in addition to selecting the disk driver also
      moves into ram (in the spare driver area) translation
      tables and code for k/b and printer routines (crt is run
      out of rom).

      There is also a RX BOOT overlay which is selected instead
      of the Xerox one. This loads the national translation
      tables from disk and then calls the Xerox BOOT.

  ver 001   4th June 1982
      First release. Compatible with XR Ver. 3.90
  ver 002   8th June 1982
      Fixed relocation bug.
  ver 003   9th June 1982
      Corrected missing bracket in signon message.
      Correct boot source address.
      Keyboad vector equate corrected.
      Printer handler patch put in sioout instead of vector table
          to catch monitor calls to sioout.
  ver 004   10th June 1982
      Corrected missing bracket in signon message - again
      Changed rigid sector equates to 29 and 30.
      Corrected fill of empty rom with -1s.
  ver 005   15th June 1982
      Corrected bug in SIGNON that switches of rom.
      Removed 'typewriter' from SIGNON prompt.
  ver 006   18th June 1982
      Release for XR ver 4.00.
      Modified signon to pick up XR ver. no. from the XR signon code.
  ver 007   22nd July 1982
      Correct bug in k/b interrupt service routine which doesn't
      zero the command byte in flag after a position byte.
      Rewrite the k/b interrupt service routine to use shift
      lock key as a caps lock.
  ver 008   24th August 1982
      Changes made to accommodate 4.02 ROM
      All changes were to memory references.  No logic changes made.
  ver 009   2nd Sept. 1982
      Changes made to printer logic to accomodate 620 printer tables.
      Printer table expanded.
      CRT driver modified so that no translation is done if the
      graphics attribute is set.
  ver 010   10th Sept. 1982
      Correction to 'graphics translation inhibit' fix
      to account for attributes enabled mode.
  ver 011   15th Sept. 1982
      Include fix to ensure the screenprint function is as
      accurate as possible.
  ver 012   January 19, 1983
      Modified keyboad interrupt service routine to work with the
      position encoded low profile keyboard with 97 key stations
      (156P82508 keyboard specification) for the US or 100 key
      stations for RX.  The interrupt handler contains:
      o  Key station translation tables
      o  Up stroke truncation code
      o  Code to truncate the lock, shift, and ctrl characters &
         expansion for 1 character
      o  Alpha lock code for the 26 alphabets
      o  Shift lock byte
      o  Repeat code for 15 characters & expansion for 4 characters
      o  Redefinabe speed for the repeat keys
      o  Minimum code for the mouse(pointer)
      o  Minimum upstroke code
      o  Signon and boot overlay adjusted to work with 4.01 & 4.02 ONLY
      o  Typewritter is not dropped from SIGNON page
      o  Majority of keyboard and printer code remains ROM resident
         to save on limited RAM space of X'F800'-X'FA07'
      o  An additonal sector is used on track zero to store the
         increase table sizes
      o  KYBD jump table appended to monitor vector table for use by
         COMM KYBD handler
      o  Printer and font exception hanler loaded on if nationalized
         tables are on system disc
      o  Address table of fourth ROM tables
  ver 013   March 15, 1983
      Include the following tables in the boot load from the Nationalized
      System double density system disc:
      Alpha lock expansion / Shift lock variable / Repeat speed /
      Repeat key / Control key inhibit / Upstroke / Mouse.
      Support monitor revision levels between 4.01 & below 5.00
  ver 014   July 20, 1983
      Corrects MICE2 subroutine maximum limit checking when the maximum
      x or y position exceeds the value of 255.
      [followed by the list of machine-dependent patch addresses:
       KEYSRV+10 f14a, CMDTAB fada/faf0, KEY5 f185, CONFG f091,
       AVAILB ff3c-ff3d, PNEXT fc3d(4.01)/fc45(4.02),
       PRMPT0 fa95(4.01)/fa9d(4.02), jump-table spares f06c-f07a,
       SIOOUT f0f8, FASTCRT+18 f30f, MILLI+50 f22f]
  ver 015   August 23, 1983
      change sign on message to read Xerox instead of 820-II
  ver 016   September 27, 1983
      The keyboard translations for the '=ENTER' key are changed from
      X'BD', X'BD', & X'FE' to X'0D', X'0D', & X'3D' for the unshifted,
      shifted, & control plus translations.  The Marketers requires this
      change for the Multiplan spreadsheet applications to reduce the
      awkwardness of reaching over for the return key from the key pad.
  ver 017   November 4, 1983
      The once machine dependent address locations of version 14 are now
      independent. [Defines the table-driven XR interface: CONFG, LPK3/LPK4
      CMDTAB patches to RXBOOT, LPK2 KEYSRV patch to LPKYBD, LPK6 monitor
      prompt, AVAILB, BOOT overlay address, LPK5 FASTCRT patch to RXCRT,
      SIOOUT patch to RXSIOO, LPK7 MILLI patch to SCRPRT; plus a monitor
      jump-table extension (TEMP / LPK1 / KEY2 / KEY5 / PNEXT / PRMPT0)
      appended when a low profile keyboard is present.]
  ver 018   January 20, 1983 [sic - follows Nov 1983, so 1984]
      The equate C.SASI was changed from 06h to 40h.
```

### 820-I monitor source header (B22D13/ROM1.MAC)

```
  820 SYSTEM MONITOR ROM, NON-RELOCATABLE VERSION
  Russell Smith        2-August-1980
  Long Tran           28-May-1981
                      21-July-1981 (ver. 2.0)
  Jim Mayhugh         28-April-1984 (4Mhz mods)
```

### SWP NEWMON header (B22D13/NEWMON.MAC)

```
  7-Sep-83
  Software Publishers Xerox 820-I Monitor
  Fixed bug in calendar routine (15-Jan-84)
  - Several commands have been removed from the version 2.0
    including T,V,G,X,F,M,C and D
  [DDMON replacement; M80 =DDMON / L80 /P:100,DDMON...; must fit below F800]
```

## Related repositories

- `../Xerox-820-16-8/` — the 16/8-focused tree: runnable disks, recovered
  source (XR.MAC v5.0 baseline, LPKYBD, the SDVR/WDVR disk drivers, 8086
  disassembly), and docs. Its `roms/` subset is superseded by this repository.
- MAME drivers: `src/mame/xerox/xerox820.cpp`, `x820kb.cpp`, `xerox_lpk.cpp`.

## MAME functional test results 2026-06-10

Headless MAME (Dave's tree), repo ROMs installed into the rompath under the
MAME slot names, `-seconds_to_run` + Lua autoboot posting keys via the natural
keyboard and dumping video RAM. Boot media: 8" = Maslin B1D18 (Xerox 60k CP/M
2.2C, carries DDT.COM); 5.25" = `../Xerox-820-16-8/boot-disk/x1685-cpm22-boot.imd`;
16/8 = 16-8SYS8 (track-8-repaired copy — the raw IMD is rejected by MAME's
loader, a media issue, not ROM). Pass = boots CP/M **and** loads a program
from disk. 820-I pass bar = monitor sign-on + keyboard echo (no bootable
820-I single-density image available).

| Set | Machine / `-bios` | Sign-on observed | CP/M boot | Program load | Result |
|---|---|---|---|---|---|
| v4.00 | `x820ii -bios v400` | `820-II v 4.00   1982 Xerox Corp` | 2.2C 8" | DDT VERS 2.2 | **PASS** |
| v4.01 | `x820ii -bios v401` | `820-II v 4.01   1982 Xerox Corp` | 2.2C 8" | DDT VERS 2.2 | **PASS** |
| v4.02 + RX011 | `x820ii -bios v402` | `820-II v 4.02   1982 Xerox Corp(RX v011)` | no (note 1) | — | PARTIAL |
| v4.02 + RX011 | `x820iilp -bios v402` | same | no (note 1) | — | PARTIAL |
| v4.04 + v016 u36 | `x820ii -bios v404` | `Xerox  v 4.04   1983 Xerox Corp (v016)` | no — std kb gated by LPK u36, expected | — | EXPECTED |
| v4.04 + v016 u36 | `x820iilp -bios v404` | `Xerox  v 4.04   1983 Xerox Corp (v016)` | 2.2C 8" | DDT VERS 2.2 | **PASS** |
| v5.00 (no u36) | `x820ii -bios v50` | `Xerox  v 5.00   1982 Xerox Corp` | 2.2C 8" | DDT VERS 2.2 | **PASS** |
| v5.00 + v018 u36 | `x820iilp -bios v50v018` | `Xerox  v 5.00   1983 Xerox Corp (v018)` | 2.2C 8" | DDT VERS 2.2 | **PASS** |
| v5.00, 5.25" | `x820ii5 -bios v50` | `Xerox  v 5.00   1982 Xerox Corp` | 2.2C 5.25" | DDT VERS 2.2 | **PASS** |
| v5.00 + rx024 (16/8) | `x168 -bios v50` | `  1984 Xerox Corp v 5.00 (RX v024)` | 60k 2.2C 8" | DIR + STAT.COM | **PASS** |
| 820-I v2.0 | `x820 -bios v20` | `...XEROX 820  VER. 2.0...` | n/a | monitor `D` dump runs, full echo | **PASS** (820-I bar) |
| SmartROM v2.3 | `x820 -bios smart23` | `Monitor v2.3 for Xerox 820   1985 MICROCode Consulting` (+ live clock) | n/a | echo verified | **PASS** (820-I bar) |
| Plus2 v0.2a | `x820 -bios plus2` | `Plus2 0.2a   1986 MICROCode` | n/a | echo verified | **PASS** (820-I bar) |

Notes:

1. **v4.02/RX011 keyboard:** under the RX v011 u36 overlay, keystrokes from
   the standard G25 ASCII keyboard are consumed without echo (CR reaches the
   command parser → `what?`); on the LPK machine the position codes garble
   (RX011 predates LPK support, ver 012+). CP/M cannot be loaded from either
   MAME keyboard, so only the monitor/banner is validated. The RX keyboard
   patch likely expects a byte stream neither MAME keyboard produces;
   same family as the documented v50v018 standard-keyboard gating.
2. Implicitly exercised in the runs above: `hardware/chargen/*`,
   `hardware/keyboard/820iikey.bin` (every x820/x820ii/x820ii5 run),
   `16-8/8086/8086.u33` (x168 run), `u36-rx-series/` v016 (= `v404.u36`),
   v018 (= `537p10831.u36.5.0.bin`), rx024.
3. Not testable without MAME source changes — no BIOS slot or no machine:
   820-I community/third-party images without slots (`xpro8u63/u64`,
   `rom0.com`/`rom100.com`, `newmon10/15.com`), the v4.01 `patch.rom` and
   `rx006.com`/`lpkybd.com` u36 options (no u36 slot under v400/v401, and
   v402's slot is bound to RX011), the v4.03-partial set (MAME `v403` slots
   stay NO_DUMP), the diagnostics 537p60xx set, the 824 material, and the
   expansion-box ROMs (undumped). 820-I Monitor v1.0 remains undumped.
