PC-MX2 ROM dumps — staged for MAME

CPUAP board (S26361-D333) = the PC-MX2 boot monitor. In MAME this is the
'cpuap' device romset (src/devices/bus/multibus/cpuap.cpp).

  cpuap.zip  — COMPLETE, CRC-verified MAME romset, BOTH BIOS revisions:
     rev9  D333 Monitor Rev 9.0 (16.06.1988): d53 (b5eefb64) + d54 (3a3c6b6e)  [from Udo]
     rev3  D333 Monitor Rev 3   (09.12.1985): d55 (821e1e41) + d56 (0892ff90)  [oldcomputers-ddns.org]
  cpuap/     — the same four files as loose files (MAME's exact names).

  Ready to use: drop cpuap.zip (or cpuap/) into MAME's rompath, then
     mame pcmx2 -bios rev9     (or -bios rev3)

Raw site downloads (pc-mx2_cpu_ap_*.bin etc.) kept alongside; rev3 d55/d56
were verified byte-identical to MAME. Other firmware here is NOT yet in MAME
(future driver work):
  omti-smc_5400vd-6011786.bin   OMTI 5400 SASI/disk-controller firmware (16 KB)
  pc-mx2_exelan_u54/u55_nx200*.bin   ExeLAN Ethernet board ROMs
  pc-mx2_serag_w2631-d279...bin      D279 serial-interface board ROM
