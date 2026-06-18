# PC532 community archives

The primary record of the PC532 — and the wider NS32k/pc532 community —
re-hosted here for preservation. All of it has been public for decades: Dave
Rand's lists at **bungi.com** since 1989 (FTP, then `www.bungi.com/pc532`), and
the **funet** PC532 FTP archive.

## `bungi.com/` — the PC532 mailing lists

Two lists Dave Rand ran at `daver`/`bungi.com`, 1989–1993+:

- **`pc532`** — the main (digest) list: `pcdig01.Z` … `pcdig76.Z` (76 digests,
  **Nov 1989 → Oct 1993**). `pcdig01` opens with Dave's 16-Nov-1989 *"PC532
  system"* post to `comp.sys.nsc.32k` — the message that started the project.
- **`pc532-src`** — the sources list: `pcsrc.Z`, a single mbox from 16-Sep-1990.
- `README`, `HEADER.html` — the original FTP/web index pages.

Format: Unix `compress` (`.Z`) plain-text mail (mbox + RFC-1153 digests).

- read: `gzip -dc pcdig01.Z` (or `zcat`); `uudecode` for posted attachments
- search: `zcat pcsrc.Z pcdig*.Z | grep -i <term>`

These are the community's complete primary record — kernel diffs, source posts
and design discussion (NetBSD, Minix, the monitor, the hardware) all live here.

## `ftp.funet.fi/` — the funet PC532 mirror

A mirror of the funet PC532 FTP archive (265 files): the NS32k cross-toolchain
bits, Bruce Culbertson's monitor/Minix material (`Culbertson/`), hardware docs
(`hardware-docs/` — et532, schematics, PALs, SCSI), and the Minix 1.3/1.5 ports
and diffs (`minix/`). Mixed provenance and licenses (GNU components are GPL;
Minix-era sources under their original terms) — a faithful preservation mirror
of already-public material.

Complements the design sources in `../hardware/` and `../docs/`.
