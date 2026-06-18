# PC532 hardware archive

Board design and fabrication files for the PC532 and related boards. The
original 1989 schematics ([`PC532_schematics.pdf`](PC532_schematics.pdf)) and
[PAL equations](../docs/PC532_PALs.pdf) (also in [`../docs/`](../docs/)) define
the board; this folder holds the CAD/Gerber sources, including the 2017-era
community reproductions that let the board be re-fabricated.

| File | What it is |
|---|---|
| [`PC532_schematics.pdf`](PC532_schematics.pdf) | The original **PC532 schematic** — 10 sheets, "532 Baby AT" rev 1D, © 1989 George Scolaro (companion to the PAL equations in `../docs/PC532_PALs.pdf`). |
| `pc532_eagle_2Jul17.zip` | Eagle CAD **source** for the PC532 — `pc532.sch` (schematic) + `pc532.brd` (board) + the PAL equations PDF (2 Jul 2017). |
| `pc532_eagle.zip` | Eagle-generated **Gerber / fabrication** output for the PC532 (top/bottom copper, inner layers G3L-G5L, soldermask, silk, drill). |
| `532_gerbers.zip` | A **KiCad** reproduction of the 532 board — `532.kicad_pcb` + Gerbers (Jun 2017). |
| `et532_gerbers.zip` | **ET532** Gerbers — a later RS-274X re-export (apertures flattened toward 0.254 mm; use `et532_pcb/` for accurate pad/trace sizes). |
| [`et532_pcb/`](et532_pcb/) | **ET532** authoritative photoplot output — the real April-1990 P-CAD **RS-274D** gerbers for both boards (`ether.gbr/`, `serial.gbr/`), the `ether.txt` **aperture table** (real round/square pad sizes) and the BOMs. The source for the renders below and for [`../docs/et532_hardware.md`](../docs/et532_hardware.md). |
| [`et532_schematic.pdf`](et532_schematic.pdf) | **ET532** schematic — 8 sheets, an unbuilt 532 variant (NS32532 + DP8390 Ethernet + dual SCC2698 = 16 serial lines), © 1988-90 George Scolaro. |
| [`et532_pals/`](et532_pals/) | **ET532** PAL equations — PALASM/TDL source: `DEC32` (address decode), `COPETH` (93C46 EEPROM + Ethernet handshake), `DRAMC`/`DRAMEN` (DRAM control), `WAIT` (wait states), © 1988-90 George Scolaro. The decode/timing reference behind the `et532` MAME driver and [`../docs/et532_hardware.md`](../docs/et532_hardware.md). |
| [`et532_ether_render.png`](et532_ether_render.png) | **Render of how the ET532 main board would have looked** (never built). 13.4 × 5.2 in ("532 Baby AT"): NS32532 + DP8390/DP8391 Ethernet + two SCC2698 octal UARTs + the 532SC edge connector; silk title "ET532 Ethernet/Serial Interface". Composited from `et532_pcb/` — real pad/trace sizes. |
| [`et532_ser_render.png`](et532_ser_render.png) | **Render of the ET532 serial card** — 6.0 × 3.6 in: 16 × MC145406 RS-232 transceivers + the J1–J18 port connectors; silk "© 1990 G.Scolaro · ET532 · Rev 1.0". Rendered at the same pixels-per-inch as the main board, so the two are to scale. |
| `render_gerber.py` | The RS-274D rasterizer that produced the two renders from `et532_pcb/` — pen-state plotter format, the `ether.txt` aperture table (round/square pads), green soldermask + gold pads + white silk + pad-derived holes. Python + Pillow + numpy. |
| `mini386_basic.zip` | The **mini386** project (separate, related) — an FPGA/CPLD design in Verilog (UART, SDRAM, SPI, `cpld_top.v`), 2014. Not the PC532 itself; included here as part of the Scolaro/Rand hardware lineage. |

## Provenance / licensing

The original PC532 schematics and PALs are © 1988-1990 George Scolaro (see
`../docs/`). The 2017 Eagle/KiCad reproductions and Gerbers are later
community/author re-creations of the Scolaro/Rand design. Preserved here as
hardware documentation; see the repository `LICENSE` (§2).
