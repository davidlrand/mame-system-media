# MK3000 reconstructed CP/M 2.2 boot disk

`mk83-cpm22.imd` boots the MAME `mk83` machine (ADE MK3000, Scomar
MULTIPROG 2.00 monitor) to an interactive CP/M 2.2 `A>` prompt.  No
original MK3000 media is known to survive; this disk was reconstructed
2026-06-10 from the machine's own documentation and ROM, by the same
byte-provenance method as the Xerox-820-16-8 boot disk.

## Booting

```
mame mk83 -flop2 mk83-cpm22.imd
```

The screen is blank at power-on (console select): press RETURN, then at
the `m>` prompt:

```
U21,27,A,2
B1
```

`U` sets the disk geometry (tipo 21 = 5.25" MFM 12 ms step, last track
27h, last sector 0Ah, length code 2 = 512 bytes); `B1` boots unit 1, the
BASF 6106 5.25" drive.  The monitor's B command reads track 0 sector 1 to
0080 and jumps to it — there is **no** automatic density probe in the B
path (the FCEB read-address probe is only a jump-table service, F839), so
the `U` command must be given first.  Banner: `MK-83 62k CP/M vers 2.2`.

## Disk format — "Standard ADE 5.25-inch 40 tr."

From the Tabella Parametri sheet dated 7/6/83 (MK83-6, page 2):
40 cylinders, 1 side, MFM 250 kbps, 10 x 512-byte sectors, skew 2
(baked into the physical ID order 1,6,2,7,3,8,4,9,5,10), 12 ms step.
CP/M: 2K blocks, directory on track 3 (3 reserved tracks), 128 entries.
DPB: SPT=40, BSH=4, BLM=15, EXM=1, DSM=91, DRM=127, AL0=C0, CKS=32, OFF=3.

Layout:
- trk0 sec1: boot loader (`boot.z80`)
- trk0 sec2-10 + trk1 sec1-4: CCP (DC00) + BDOS (E400) + CBIOS (F200)
- trk3+: file system (dir blocks 0-1, then file data)

## Byte provenance

- **Boot loader + CBIOS**: written new for this disk (`boot.z80`,
  `cbios.z80`, assembled with zmac).  Thin CBIOS over the MULTIPROG
  monitor's RAM-resident services (F800 jump table: CONST F806, CONIN
  F809, CONOUT F80C, SELECT F81B, SEEK F821, RDSCT F824, WRSCT F827),
  with read-modify-write deblocking through a one-sector cache.  The
  monitor stays resident at F7F7-FD8A; CP/M occupies 0100-F5FF.
- **CCP+BDOS**: Xerox CP/M 2.2C #2-294 (`cpsys0.hex`/`cpsys1.hex` from
  ../../Xerox-820-16-8/source/cpm86-bios/), relocated to DC00 via the
  cpsys0-vs-cpsys1 relocation bitmap (1073 relocation bytes, 0
  mismatches) — the proven x1685 technique.
- **Utilities** (STAT, PIP, ED, SUBMIT, XSUB, ZSID): standard DRI
  CP/M 2.2 binaries from the Xerox 820 disk extraction.
- Rebuild: `python3 mkdisk.py mk83-cpm22.imd <COM files...>` after
  `zmac -o boot.bin boot.z80; zmac -o cbios.bin cbios.z80` (needs
  `ccpbdos_dc00.bin`, see mkdisk.py / the MK83-MEMORY notes).

## Validated in MAME (2026-06-10)

Boot banner -> `A>`; DIR; STAT *.* (correct sizes, 144k free); PIP
C.COM=STAT.COM; SAVE 8 T.BIN; warm boots after each program; power-cycle
with the written-back image: DIR shows the new files and the PIP-copied
binary runs (`C T.BIN` reports 16 recs / 2k — exactly the SAVE 8).
