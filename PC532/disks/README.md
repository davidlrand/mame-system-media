# PC532 — NetBSD 1.5.3 boot disk (CHD)

`pc532_netbsd-1.5.3.chd` — a runnable hard-disk image with **NetBSD 1.5.3/pc532**
installed, ready to boot from disk on the MAME `pc532` driver.

| | |
|---|---|
| OS | NetBSD 1.5.3 (GENERIC), the last NetBSD release supporting the PC532 |
| Geometry | ST225N, 2048 cyl × 16 heads × 32 sec, 512 B/sec = 512 MB (1048576 sectors) |
| Partitions | `a` = root (4.2BSD ffs), `b` = swap, `h` = PC532 boot images |
| CHD | v5, LZMA-compressed, 19 MB (512 MB logical); data SHA1 `b91b9065b683ab34ddb852ebd4ae165ddadba245` |

## Running

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

## ⚠ Requires the ncr5380 SCSI fixes

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

## Provenance / build

Installed entirely inside MAME over the ROM monitor's serial `download`: the
NetBSD 1.5.3 `floppy-144.fs` install kernel was streamed in, a disklabel written,
and `base`/`etc`/`kern` extracted from a SCSI tape (SIMH `.tap` built from the
official NetBSD 1.5.3/pc532 binary sets) onto `sd0a`, then `bim` wrote the boot
block and `MAKEDEV all` populated `/dev`. NetBSD is BSD-licensed and freely
redistributable; the sets are the unmodified 2002 NetBSD 1.5.3/pc532 release.
