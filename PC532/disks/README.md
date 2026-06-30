# PC532 — boot disks (CHD)

Two runnable hard-disk images for the MAME `pc532` driver:

- **`pc532_netbsd-1.5.3.chd`** — NetBSD 1.5.3/pc532; boots via the ROM monitor over DP8490 SCSI.
- **`pc532_sysv.chd.gz`** — a binary UNIX System V Release 2.0 distribution; boots via the dual-boot ROM over AIC6250 SCSI. (gzip-compressed — `gunzip` it before mounting.)

---

## NetBSD 1.5.3

`pc532_netbsd-1.5.3.chd` — a runnable hard-disk image with **NetBSD 1.5.3/pc532**
installed, ready to boot from disk on the MAME `pc532` driver.

| | |
|---|---|
| OS | NetBSD 1.5.3 (GENERIC), the last NetBSD release supporting the PC532 |
| Geometry | ST225N, 2048 cyl × 16 heads × 32 sec, 512 B/sec = 512 MB (1048576 sectors) |
| Partitions | `a` = root (4.2BSD ffs), `b` = swap, `h` = PC532 boot images |
| CHD | v5, LZMA-compressed, 19 MB (512 MB logical); data SHA1 `b91b9065b683ab34ddb852ebd4ae165ddadba245` |

### Running

```
mame pc532 -hard pc532_netbsd-1.5.3.chd
```

At the ROM monitor prompt (`Command (? for help):`) type:

```
boot
```

The monitor loads the boot image from the disk, runs the NetBSD/pc532 secondary
boot (`>> NetBSD/pc532 Boot, Revision 1.1`), which loads `sd0a:/netbsd` and boots
the kernel. It comes up to single-user (`/etc/rc.conf is not configured`): press
**RETURN** at `Enter pathname of shell or RETURN for sh:` (answer `vt100` to the
terminal-type prompt) for a `#` shell. `root` is on `sd0a`, mounted ffs.

The console is the first serial port (`scn0`). For a headless/automated session,
drive it over a socket: `-serial0 null_modem -bitb socket.127.0.0.1:7000` and the
harness in `../tools/`.

### ⚠ Requires the ncr5380 SCSI fixes

This disk does **not** boot on stock MAME yet — the PC532's SCSI controller
emulation (`src/devices/machine/ncr5380.cpp`, shared NCR5380/DP8490 device) needs
two fixes, without which no SCSI disk I/O works:

1. **Phase-mismatch DRQ** — deassert DRQ when a SCSI phase change ends a DMA
   transfer (a host pseudo-DMA driver that polls `(DRQ && !PHASEMATCH)` for the
   end of a transfer otherwise spins; NetBSD's `ncr_pdma_out` "final SCI_DSR_DREQ"
   timeout).
2. **Self-reset interrupt** — a host-asserted bus reset (ICR R̅S̅T̅) must not
   raise a (self-)interrupt; otherwise the PC532 ROM monitor's SCSI driver aborts
   with `SCSI: reset error` and cannot read the disk.

Both are genuine bugs; a PR to mamedev is planned. Until then, build MAME with
those patches (David Rand's working tree).

### Provenance / build

Installed entirely inside MAME over the ROM monitor's serial `download`: the
NetBSD 1.5.3 `floppy-144.fs` install kernel was streamed in, a disklabel written,
and `base`/`etc`/`kern` extracted from a SCSI tape (SIMH `.tap` built from the
official NetBSD 1.5.3/pc532 binary sets) onto `sd0a`, then `bim` wrote the boot
block and `MAKEDEV all` populated `/dev`. NetBSD is BSD-licensed and freely
redistributable; the sets are the unmodified 2002 NetBSD 1.5.3/pc532 release.

---

## UNIX System V Release 2.0

`pc532_sysv.chd.gz` — a binary, runnable **UNIX System V Release 2.0 / NS32000**
distribution for the PC532: the native System V port (the ICM-3216 NS32000 SVR
lineage). `gunzip` it to `pc532_sysv.chd` before mounting.

| | |
|---|---|
| OS | UNIX System V Release 2.0 / NS32000 (native PC532 port) — **binary only** |
| Disk | Seagate ST-296 class, 84 MB, one SCSI drive on the AIC6250 (target 1) |
| Geometry | 1024 cyl × 6 heads × 27 sec, 512 B/sec = 84,934,656 bytes (165,888 sectors) |
| CHD | v5, uncompressed; 22 MB on disk (sparse from the 84.9 MB logical image) |
| Distributed as | `pc532_sysv.chd.gz` — gzip −9, 6.0 MB; SHA1 `5b0f7a17f1299f0189c0acb0dcbc01bbd85f208a` |
| `pc532_sysv.chd` | SHA1 `2fa17266dec764e288c3a6999e485af1b56515c2` (after `gunzip`) |

### Disk format

System V slices the single AIC6250 drive (disk 0) as:

| slice | mount | purpose |
|---|---|---|
| `0s0` | `/` | root filesystem |
| `0s1` | `/usr` | `/usr` filesystem |
| `0s2` | — | available (unallocated) |
| `0s3` | — | available (unallocated) |
| `0s5` | swap | swap |

The boot ROM loads `/unix` from `0s0`.

### Running

System V boots through the **dual-boot ROM** in
[`../roms/dualboot/`](../roms/dualboot/), not the stock monitor. That ROM probes
the SCSI controllers and boots the OS the live one implies — **AIC6250 → System V**
(loads `/unix` off `0s0`), DP8490 → NetBSD. Build MAME with the ROM added as a
BIOS option (one `ROM_REGION` line — see
[`../roms/dualboot/README.md`](../roms/dualboot/README.md)), then:

```
gunzip pc532_sysv.chd.gz
mame pc532 -bios dualboot -scsi:1 harddisk -hard2 pc532_sysv.chd
```

The disk goes on the **AIC6250** bus at target 1 (`-scsi:1 harddisk`); it is the
*second* harddisk image (`-hard2`), after the DP8490 bus's default `slot:0`
(`-hard1`). A bare `-hard` would attach it to the DP8490 bus instead, where the
ROM's AIC6250 probe would never see it.

The ROM prints `probe AIC6250... live -> System V`, loads `/unix` off `0s0`, and
the kernel boots to the System V console. For a headless/automated session use
`-serial0 null_modem -bitb socket.127.0.0.1:7000` + the `../tools/` harness, as
with NetBSD.

Also required, until it merges upstream: the **mc68681 transmitter fix**
([mamedev/mame#15564](https://github.com/mamedev/mame/pull/15564)) — without it the
SCN2681 console transmitter can deadlock under sustained output.

### Provenance / licensing

UNIX System V is AT&T-licensed; this is a **binary-only** runnable image — the
System V source is **not** redistributed here (nor anywhere in this repo). The
NS32000 System V port and this distribution disk are by David Rand. Work on a
copy — MAME persists writes back to the CHD on exit.
