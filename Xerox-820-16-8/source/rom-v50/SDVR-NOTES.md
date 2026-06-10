# sdvr — the loadable SASI driver (and the 5.25" rebuild)

`sdvr.hex` is the loadable SASI disk driver for the rigid-disk ("Expansion
Box" id 0x21) configuration: `seltab + dphdpb + rigdpb + sa1403` linked at
F360 (see `driver.sub`). It serves the floppies (LUN 0-2) *and* the rigid
partitions (LUN 3) through the SASI host adapter at ports 0x10-0x13.

Two findings (2026-06):

1. **This build carries the 8-inch drive geometry inline** — `sa1403.mac`
   hardcodes 77 tracks and 26 sectors/track (the side-fold at `cp 77`, the
   LBA multiply `ld de,26`: "compute sector-26-1+(Track+1)*26", 1-based
   sectors). It is the 8" unit's driver. The 5.25" unit's ROM (undumped)
   carried a 5.25"-constant build.
2. **The source is authoritative**: assembling `sa1403.mac` with M80 and
   linking against the original `seltab/dphdpb/rigdpb.rel` reproduces
   `sdvr.hex` **byte-identically**.

Therefore the 5.25" driver is reconstructable as a four-literal edit:

- `sa1403-525.mac` — `sa1403.mac` with 77→40 and 26→17 (SA450-class
  40-track drives, 17 sectors/track; the 17-spt choice for SASI floppies is
  provisional — no 5.25"-over-SASI media survives to confirm it);
- `sdvr5.bin` — the resulting 1040-byte binary
  (`L80 /P:F360,SELTAB,DPHDPB,RIGDPB,SA14035,SDVR5/N/E`).

When packaged into the emulated controller-box ROM, the image needs a
**trailing checksum byte** (the loader's `ccs` routine 8-bit-sums the
header-declared length to zero) — for `sdvr5.bin` that byte is `0x8B`
(for the original `sdvr.hex`, `0x13`), and the declared length includes it
(0x411).

MAME's `x1685s` uses exactly this reconstruction and boots CP/M from a
rigid partition with it. See `../../docs/sasi-rigid-system.md`.
