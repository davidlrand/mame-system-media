# MK-83 documentation (Scomar / ADE Elettronica) — for MAME `mk83` bring-up

Source: http://www.vintagesbc.it/vintage-computer-board/collezione/mk-83/ (downloaded 2026-06-06).
No disk images are published anywhere on the site — documentation only. (The owner holds
original + copy disks per the 5a/5b tables but has not imaged them.)

## Files
| File | Contents |
|------|----------|
| MK83-1-Descrizione-Hardware-compresso.pdf | Hardware description (memory map / banking / I-O) |
| MK83-2-Schemi-compresso.pdf | **Schematics** — drive-select wiring, port 0x14, 256K banking |
| MK83-3-Manuale-Utente-compresso.pdf | User manual |
| MK83-4-Uso-CPM.pdf | CP/M usage + boot procedure |
| MK83-5a-Tabella-Formati-Dischi-Originale.pdf | Handwritten format catalog (originals) |
| MK83-5b-...-Copia.pdf | Same (copies) |
| MK83-6-Lista-Formati-dischi-compresso.pdf | **Disk-format list** — geometry params (read this to build a disk) |
| MK83-7-Pubblicita-e-costi.pdf | Period advertising / pricing |
| MK83-8-Listati-assembler-BSTAM.pdf | BSTAM assembler listings |
| MK83-Bios-Entry-points.jpg | BIOS jump table (typed) — see below |
| MK83-Bios-Cmd.jpg | Monitor command list (typed) — see below |
| MK83.jpg / MK83-2.jpg / MK83-cognetti.jpg | Board photos |

## Key facts extracted (for the driver)
- **Maker:** page says "MK-83 ADE Elettronica" (MAME driver currently credits "Scomar" — verify/correct).
- **256K RAM, banked.** Monitor/BIOS runs at **F000**, BIOS jump table at **F800** — so the current
  `mk83_mem` (ROM at 0x0000, RAM to 0xFFFF, no banking) is wrong; re-disassemble the ROM at basepc
  **0xF000** to read the BIOS (the F800-half) correctly.
- **BIOS entry points (F800+):** ENT00 COLD F800, WARM F803, CONST F806, CONIN F809, CONOUT F80C,
  CRTOUT F80F (memory-mapped video), SIOIN F812 / SIOST F815 / SIOOST F818 (SIO-B console),
  **SELECT F81B, HOME F81E, SEEK F821, RDSCT F824, WRSCT F827**.
- **Monitor commands** (set `ROIGTFSMBDCUKHLP`): R(ead) unit,track,sector · B(oot) unit ·
  **U(nit) tipo,ult.traccia,ult.sect,lung.sect** (configures drive geometry — the multi-format
  mechanism) · D/M/T/F/C mem · G goto · I/O port · H/K clock · L line.
- **Multi-format machine:** auto-configures to read IBM PC, Olivetti ETV, DEC Rainbow, Superbrain,
  NCR, Monroe, L'EMMECI, etc. Native = "Standard ADE" in **5"-40trk, 5"-80trk, and 8"** (so it
  supports both drive sizes; density/side codes 0/1/2 per config).
- **Floppy drive-select (from ROM):** binary unit# in **PIO-A bits 4-5** (port 0x1C), NOT the Big
  Board's bits 0-1; also drives port 0x14 and self-manages PIO-A bits 6-7. FDC at 0x10-0x13,
  DRQ->NMI byte transfer self-modifying the handler at 0x0069.

## Still needed to reach "working"
1. Driver: F000/F800 ROM mapping + 256K banking, mk83 PIO-A handler (drive-sel bits 4-5 + port 0x14),
   correct drive type/clock (5.25" 80trk likely native; supports 8").
2. **A boot disk** — none public. Read MK83-6 for the exact native geometry; reconstruct from file
   content if any surfaces, or get the owner to image an original.

## Added 2026-06-10 (provided by Enrico, vintagesbc.it — enrico@vintagesbc.it)
- docs/MK83 BIOS - Z80 DISASSEMBLER LISTING.doc — full BIOS disassembly listing (Word format); the key reference for the MAME mk83 driver work (drive select, ports, entry points)
- docs/MK83 - Uso CPM.pdf, docs/Mk83 - System Monitor Manuale Utente.pdf — CP/M usage + System Monitor user manuals (Italian; check overlap with MK83-3/MK83-4)
- schematics/MK-83 Schemi.rar + extracted Ridotte/MK3000-1..6.jpg — six schematic sheets (reduced-size scans, MK3000 numbering)
