PC-MX2 ROM dumps, staged for MAME

All four boards the MAME 'pcmx2' driver needs are here as CRC-verified MAME
romsets, both zipped and as loose files under matching directory names.

CPUAP board (S26361-D333): the PC-MX2 boot monitor. MAME 'cpuap' device
(src/devices/bus/multibus/cpuap.cpp), selected with -slot6 cpuap,bios=X.
  cpuap.zip / cpuap/
     rev9  D333 Monitor Rev 9.0 (16.06.1988): d53 (b5eefb64) + d54 (3a3c6b6e)  [from Udo]
     rev3  D333 Monitor Rev 3   (09.12.1985): d55 (821e1e41) + d56 (0892ff90)  [oldcomputers-ddns.org]

STORAGER board (Interphase 3030 Storager): the mass-storage controller. MAME
'storager' device (src/devices/bus/multibus/storager.cpp), -slot1 storager,bios=X.
  storager.zip / storager/
     v260  Siemens MX-300/PC-MX2, MC68000P12 (default): 05820084260.u84 (43616528) + .u85 (206c064d)
     v180  MC68000L10:                                  58084180.u84 (38805bbf) + .u85 (e21001ce)
     sgic  SGI 026-0005-001 Rev C "Storager 3030":      05808423a.u84 (161e6a90) + .u85 (4c99e4b8)
     sgib  SGI Rev B: NO GOOD DUMP KNOWN, not included

SERAD board (S26361-D279): the serial-interface board. MAME 'serad' device
(src/devices/bus/multibus/serad.cpp).
  serad.zip / serad/
     d31  361d0279d031__e00422_tex.d31 (369f5fd1)

97801 terminal: the console. MAME 's97801' device/driver
(src/mame/siemens/s97801.cpp); romset under ../terminal-97801/roms/.
  terminal-97801/roms/s97801.zip
     d3 (aba8f4b7), d21 (b9b9df32), d23 (23b22a7d), d26 (fcf045d7)

Ready to use: drop the zips into MAME's rompath, then for example
     mame pcmx2                            (all board defaults)
     mame pcmx2 -slot6 cpuap,bios=rev3     (older boot monitor)
     mame s97801                           (the terminal, standalone)

Raw site downloads (pc-mx2_cpu_ap_*.bin, pc-mx2_serag_w2631-d279...bin, etc.)
are kept alongside; rev3 d55/d56 were verified byte-identical to MAME.
Firmware here that is NOT yet in the MAME driver (future work):
  omti-smc_5400vd-6011786.bin        OMTI 5400 SASI/disk-controller firmware (16 KB)
  pc-mx2_exelan_u54/u55_nx200*.bin   ExeLAN Ethernet board ROMs
