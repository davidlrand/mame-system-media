# Reconstructed Xerox 16/8 (x1685) 5.25" CP/M-80 boot disk

`x1685-cpm22-boot.imd` boots MAME's `x1685` to a fully interactive CP/M 2.2 A>
prompt (first achieved 2026-06-09). `x820ii5-cpm22-rebuilt.imd` is the same
system laid for the base 820-II (cross-validation; also boots).

## Provenance / construction
- Track structure + data tracks: `820-II_CPM22.IMD` (b1d18-family 5.25" base
  disk; 41 trk, FM 18×128 trk0 + MFM 17×256).
- Boot sector: the universal qfs loader taken verbatim from `16-8sys8.imd`
  trk0 sec1 (byte-identical on cpm22 except the serial string). On the eb2
  disk it sits at FM sector id 0 (ids renumbered 0-17; the RX024 eboot reads
  sector 0).
- BIOS (1 KB at trk2 sectors 6-9): `16-8sys8.imd` trk1 sectors 23-26 verbatim
  (the shipping, driver-agnostic v5.0-era BIOS; all disk I/O via the monitor
  xqdvr, so the same image serves 8" and 5.25").
- CCP+BDOS: the recovered Balcones `cpsys0.hex`/`cpsys1.hex` pair (B16D39).
  The two are the same code linked 0x100 apart; their byte-diff yields the
  relocation bitmap (1073 bytes, 0 mismatches), applied as +0xD4 pages
  → CCP at D400, BDOS at DC00 (live-verified addresses).

## The decoded layout (shipping warm boot, disassembled from the sys8 BIOS)
dmabas = D380; walk = logical 128-byte sectors, DD disks start track 1
sector 1 (logical s ↦ DMA D380+s·128, FDC sector = (s>>1)+1, half = s&1):
- trk1 sec n: [rec 2n-3][rec 2n-2] (sec1 lower half unused, upper = CCP rec 0)
- trk2 sec n (1-5): [rec 2n+31][rec 2n+32]
- trk2 sec6 lower = the BIOS's first 128 bytes (the walk's last record lands
  at E980, so the qfs BIOS block and the walk interlock on this sector).

## Notes

- The boot sector is present at track-0 ids **0 and 1**, so the x1685 disk
  also boots the base machines (`x820ii5`): the RX024 eboot reads id 0, the
  base monitors read id 1.
- This disk does **not** boot `x1685s` (the SASI rigid machine) — its
  floppies live behind the SASI controller, a different addressing world
  (no id-0 sector, 17 spt). See `../docs/sasi-rigid-system.md`.
- Work on a copy: MAME writes in-session floppy changes back to the
  mounted image on exit.

## 2026-06-10: double-sided update

The disk is now built **double-sided** (formatted-empty E5 side-1 tracks,
MFM 17 x 256 ids 1-17). The donor `cpm22` disk's directory is allocated with
**2K blocks** — the double-sided DPB — proving the original was a DS disk
dumped single-sided. Without formatted side-1 tracks the monitor's media
selector logs the disk single-sided, selects the 1K DPB, and every program
load lands at half its true offset ("Bdos Err on K:/P: select" while DIR
still works). With them, DDT/INIT/MOVCPM load and run. Note the original
side-1 *content* is lost in the dump; any donor file whose blocks lived on
side 1 reads as E5 blanks.
