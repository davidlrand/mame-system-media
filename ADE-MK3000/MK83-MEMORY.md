# MK-83 / MK3000 — working memory (MAME `mk83` bring-up)

Living summary; Dave updates as more surfaces. Companion files in this dir: `INDEX.md` (doc-file
index + driver facts), `mk3000-correspondence-and-site.md` (Enrico's email + MK3000 page),
and the MK83-1..8 PDFs / JPGs.

## What it is
The **MK83** is the Z80A motherboard of the **MK3000**, an **ADE Elettronica** (Palazzolo Milanese)
system, **1983**. The MK3000 was a *multi-format disk gateway*: its whole reason for existing was to
read floppies in a host of foreign formats and feed the data to an **IBM 370** mainframe (replacing
punch-card entry; used e.g. at CNR-CUCE Pisa). That multi-format mission is why the machine
re-configures drive geometry on the fly (and why a "plain" Big Board CP/M won't just boot it).

Lineage: a Ferguson **Big Board I** derivative (Xerox 820 / Big Board family), but **not** software-
compatible — see the F000-vs-F800 split below. (MAME driver currently credits "Scomar"; the site and
maintainer say **ADE Elettronica** — correct this.)

## Hardware (from MK3000 page + MK83 docs)
- **CPU** Z80A. **2× Z80A PIO** (printer ports / I-O / keyboard), **1× Z80A SIO/0** (2× RS232,
  console is SIO-B), **1× Z80A CTC** (timer + the clock H/K commands).
- **FDC: WD179x at 0x10-0x13** + **FDC9229BT** data separator. **Type unresolved: MK3000 page says
  FD1797; Enrico recalls FD1793.** (Big Board I was FD1771/SSSD; MK83 is the newer 179x.) Confirm
  from MK83-1/MK83-2 PDFs.
- **256K RAM, banked** (port 0x14 + PIO involved).
- Drive connectors: **IDC 50-pin (8")** and **IDC 34-pin (5.25")** — supports both sizes.
- Drives shipped: BASF AG 6106 5.25" (180KB SS/DD) + **Shugart SA465** 5.25" (720KB DS/DD).

## Firmware / memory model — KEY difference from Big Board
- Power-on: 16-byte ROM stub copies the EPROM into DRAM and jumps in. **Big Board → F000H; MK83 →
  F800H.** So the resident monitor sits higher and **all entry addresses are translated** up by 0x800.
  => MAME `mk83_mem` (ROM@0x0000, flat RAM, no banking) is wrong; re-disassemble the ROM at **basepc
  0xF000** (BIOS occupies the F800 half) and model the 256K banking.
- **System variables at FF00H** — and *the geometry config copied there varies per format* (tracks,
  sectors/track, SS/DS density). Enrico traced different "basic configurations" depending on what
  lands at FF00h — the heart of the multi-format behavior.
- **BIOS entry points (F800+):** COLD F800, WARM F803, CONST F806, CONIN F809, CONOUT F80C,
  CRTOUT F80F (mem-mapped video), SIOIN F812 / SIOST F815 / SIOOST F818, **SELECT F81B, HOME F81E,
  SEEK F821, RDSCT F824, WRSCT F827**.
- **Monitor commands** (`ROIGTFSMBDCUKHLP`): R read(unit,trk,sec) · B boot(unit) ·
  **U unit(tipo, ult.traccia, ult.sett, lung.sett)** = the geometry-set command · D/M/T/F/C mem ·
  G goto · I/O port · H/K clock · L line.
- **Drive select:** binary unit# in **PIO-A bits 4-5** (port 0x1C), plus port 0x14; ROM self-manages
  PIO-A bits 6-7. **DRQ→NMI** byte transfer, self-modifying handler at 0x0069 (Big-Board-style).

## Blockers to a working MAME machine
1. **No boot media exists anywhere** (Enrico received the board with none; never booted it; he could
   get the prompt by relocating traces to F800 but a Big-Board CP/M never fully loaded — needs the
   *original MK3000 software* / a native-format disk). Dave is still hunting.
2. Driver work: F000/F800 ROM map + 256K banking; mk83 PIO-A handler (drive-sel bits 4-5 + port 0x14);
   correct WD179x part + drive type/clock; geometry-config path (FF00H) for the multi-format reads.
3. Resolve FD1793 vs FD1797 from the hardware PDFs.

## Native disk geometry (to reconstruct a boot disk)
Read **MK83-6** (Lista-Formati-dischi) and **MK83-5a/5b** (format tables). Native = "Standard ADE"
in **5"-40trk, 5"-80trk, and 8"**; the catalog also lists IBM PC, Olivetti ETV, DEC Rainbow,
Superbrain, NCR, Monroe, L'EMMECI, etc. (density/side codes 0/1/2 per config).
