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

## 16-8sys8-boot.imd (added 2026-06-10)

The genuine 16/8 8" system disk (Don Maslin's `16-8sys8.td0`, in `../disks/`),
converted to IMD and repaired for MAME: six tracks (8, 54, 66, 68, 72, 75) in
the raw TeleDisk conversion carried duplicate sector entries (42 declared on a
26-sector MFM track), which MAME's IMD loader rejects ("Incorrect layout");
the duplicates were trimmed. **This is the disk that boots x168 to CP/M-80
and, via `LOAD86` + `86CON`, to CP/M-86 1.1F** — its LOAD86 probes the 8086
boot ROM signature (0x0909 at 0xFFFFC), which the dumped 8086.u33 carries.
Note: the 816 SYSTEM DISK (B16D38) carries a *different* LOAD86 expecting an
earlier, undumped 8086-board ROM revision (9-byte ID 01..09 at 0xFCFF5) and
cannot start the 8086 with the available ROM.

## CP/M-86 media (added 2026-06-10)

### x1685-cpm86-boot.imd — 5.25" CP/M-86-capable boot disk (`x1685`)

`x1685-cpm22-boot.imd` plus the CP/M-86 tool chain, all taken from
`16-8sys8-boot.imd` (the proven pair for the dumped 8086.u33):
**LOAD86.COM, CPM86.COM, 86CON.COM** and **PIP/STAT/SUBMIT/HELP/GOBACK.CMD**.
To make room, M80.COM, L80.COM, HELP.TXT, HELP.COM and D.COM were deleted, and
the leftover 9×512 track (cyl 40 head 0, inherited from the donor) was
normalized to formatted-empty E5 MFM 17×256. All injected blocks live on
side 0. CPM86.COM is required: LOAD86's staged 2.2F system chains to it
(`CPM86?` at the new prompt if absent) to load the CP/M-86 OS into the 8086.

Recipe (`/tmp/c86build/inject.py` pattern): flatten the IMD to literal
sectors, address records by sector ID (trk = cyl, FDC sec = rec/2+1, half =
rec&1, 34 recs/track from cyl 3, 2K blocks, side 0 first), rewrite the
directory (2 dir blocks, EXM=1 so every file here is a single extent).

**Extraction warning**: `16-8sys8-boot.imd` carries damaged tracks (8, 54,
66, 68, 72, 75, 76 have duplicate/missing sector IDs; 12, 60, 70 each miss
one sector). Files must be extracted addressing sectors **by ID with a
uniform 26-sector track**, not by position in the track record — a
position-based read shifts everything after track 8 and yields silently
corrupt files (LOAD86 then crashes back to the ROM monitor). The injected
files were verified against a live-TPA dump of LOAD86 from a booted x168 and
against the B16D38 copies (PIP/STAT/SUBMIT.CMD byte-identical). All injected
files avoid the damaged sectors. ED.CMD spans the trk-12 hole and was left
off.

Verified (MAME headless, 2026-06-10): `x1685 -flop1` → `L` → CP/M 2.2C A> →
`LOAD86` → "Xerox 16/8 PC 60k CP/M-80 vers 2.2F #2-294" → `86CON` →
**"Xerox 16/8 PC 256k CP/M-86 vers 1.1F #2-294"** → `a>` DIR lists the disk,
`STAT` (STAT.CMD) runs ("A: RW, Free Space: 170k").

### x1685s-cpm86.chd — SA1004 rigid CHD with the CP/M-86 tool chain

`sa1004.chd` (bootable CP/M 2.2 system + tools on partition E) with the same
nine files PIPped onto E: from `16-8sys8-boot.imd`, done authentically inside
the emulation: `x168s -flop1 16-8sys8-boot(copy) -hard this.chd`, boot `L`,
then `PIP E:=A:<file>` one file at a time (ED.CMD skipped, damaged source),
`DIR E:` verified. The LPK keyboard's natural-keyboard map types `:`/`=`
correctly; the x820iis ASCII-keyboard HLE does not.

Verified (2026-06-10):
- **x168s** `-hard x1685s-cpm86.chd`, boot `LE` (rigid partition E → A:):
  `LOAD86` → 2.2F banner → `86CON` → **CP/M-86 1.1F**, `DIR` lists the rigid
  partition, `STAT` reports 3,304k free. Full CP/M-86 running from the rigid
  disk.
- **x1685s** same CHD, `LE`: boots CP/M-80 2.2 to A> (console comes up on
  TTY: per the installed system's IOBYTE; poke 0003=01 or CONFIGUR for CRT).
  `LOAD86` passes its 8086 sequence (stop, ROM signature 0909 at window
  BFFC, start) but the staged 2.2F system signs on garbled
  ("16/8 PC PXRZT\V^X`Zb\d^f") and drops back to the old system; `86CON`
  then reports "This software requires co-processor CP/M." The identical
  CHD rigid-booted on x168s works, so this is specific to the x1685s
  (rgd5/sdvr reconstructed-driver) personality, not the media — open issue.
