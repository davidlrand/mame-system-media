# Toshiba T-200 / T-250 CP/M Disk Format Reference

Format definitions for **cpmtools**, **libdsk**, and **FlashFloppy / GOTEK**, derived directly from the CP/M BIOS source (`bios64.asm` rev 2.12, 1982). Includes literal sector-translate tables for tools that require explicit skewmap.

---

## Machines

| Machine | Drive form factor | Cylinders | Heads | Spindle | Data rate (MFM) | Data rate (FM) |
|---------|------------------|-----------|-------|---------|------------------|-----------------|
| T-250 | 8" full-height | 77 | 2 | 360 RPM | 500 kbps | 250 kbps |
| T-200 | 5.25" full-height | 35 | 2 | 300 RPM | 250 kbps | 125 kbps |

**Note**: T-200 uses **35 tracks**, not the more common 40. This is unusual.

Both machines:
- Boot track (cyl 0, head 0) is always **FM 128-byte sectors**, regardless of disk format
- Sector numbering on the physical media starts at **1** (BIOS sector-translate tables are 0-indexed internally)
- CP/M 2.2

---

## T-250 — 8 floppy formats

DPB literals are taken from `bios64.asm` lines 396-471 (T-250 conditional block).

| Type | Density | Sides | Sector size | Physical sectors/track | Records/track | CP/M block | Dir entries | Reserved tracks | Total usable |
|:----:|---------|:-----:|------------:|-----------------------:|--------------:|:----------:|------------:|----------------:|-------------:|
| 0 | SD (FM)  | 1 | 128  | 26 | 26 | 1 KB | 64  | 2 | 243 KB |
| 1 | SD (FM)  | 2 | 128  | 26 | 26 | 2 KB | 128 | 4 | 486 KB |
| 2 | DD (MFM) | 1 | 256  | 26 | 52 | 2 KB | 128 | 3 | 486 KB |
| 3 | DD (MFM) | 2 | 256  | 26 | 52 | 2 KB | 256 | 4 | 974 KB |
| 4 | DD (MFM) | 1 | 512  | 15 | 60 | 2 KB | 128 | 3 | 562 KB |
| 5 | DD (MFM) | 2 | 512  | 15 | 60 | 2 KB | 256 | 4 | 1124 KB |
| 6 | DD (MFM) | 1 | 1024 |  8 | 64 | 2 KB | 128 | 3 | 600 KB |
| 7 | DD (MFM) | 2 | 1024 |  8 | 64 | 2 KB | 256 | 4 | 1200 KB |

---

## T-200 — 8 floppy formats

DPB literals from `bios64.asm` lines 472-538 (T-200 conditional block).

| Type | Density | Sides | Sector size | Physical sectors/track | Records/track | CP/M block | Dir entries | Reserved tracks | Total usable |
|:----:|---------|:-----:|------------:|-----------------------:|--------------:|:----------:|------------:|----------------:|-------------:|
| 0 | SD (FM)  | 1 | 128  | 18 | 18 | 1 KB | 64  | 3 | 72 KB  |
| 1 | SD (FM)  | 2 | 128  | 18 | 18 | 1 KB | 64  | 6 | 144 KB |
| 2 | DD (MFM) | 1 | 256  | 18 | 36 | 1 KB | 64  | 3 | 144 KB |
| 3 | DD (MFM) | 2 | 256  | 16 | 32 | 1 KB | 64  | 6 | 256 KB |
| 4 | DD (MFM) | 1 | 512  |  9 | 36 | 1 KB | 64  | 3 | 144 KB |
| 5 | DD (MFM) | 2 | 512  |  9 | 36 | 2 KB | 128 | 4 | 296 KB |
| 6 | DD (MFM) | 1 | 1024 |  5 | 40 | 1 KB | 128 | 3 | 160 KB |
| 7 | DD (MFM) | 2 | 1024 |  5 | 40 | 2 KB | 128 | 4 | 330 KB |

**Note on type 3 (DD/DS 256B)**: only 16 sectors/track on the DS variant (vs. 18 on the SS variant). This is to maintain reliable timing margins on the second head.

---

## CP/M BIOS DPB fragments (verbatim)

Authoritative source of the parameter blocks. From `bios64.asm`:

### T-250 (IF NOT inch5)

```asm
dpbs1:    ;single density, single sided, 128B
    dw  26                  ; SPT (CP/M records per track)
    db  3,7,0               ; BSH=3 BLM=7 EXM=0 → 1KB allocation block
    dw  S1DSM-1,64-1        ; DSM, DRM
    db  11000000b,00000000b ; AL0, AL1 (directory allocation bitmap)
    dw  (64+3)/4            ; CKS (directory check vector size)
    dw  2                   ; OFS (reserved system tracks)

dpbs2:    ;single density, double sided, 128B
    dw  26
    db  4,15,1              ; BSH=4 BLM=15 EXM=1 → 2KB block
    dw  S2DSM-1,128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  2*2                 ; OFS=4

dpbd1:    ;double density, single sided, 256B
    dw  2*26                ; 52 records/track
    db  4,15,1
    dw  D1DSM-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  3

dpbd2:    ;double density, double sided, 256B
    dw  2*26
    db  4,15,0
    dw  D2DSM-1, 256-1
    db  11110000b,00000000b
    dw  (256+3)/4
    dw  2*2

dpbd3:    ;double density, single sided, 512B
    dw  4*15                ; 60 records/track
    db  4,15,0
    dw  D3DSM-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  3

dpbd4:    ;double density, double sided, 512B
    dw  4*15
    db  4,15,0
    dw  D4DSM-1, 256-1
    db  11110000b,00000000b
    dw  (256+3)/4
    dw  2*2

dpbd5:    ;double density, single sided, 1024B
    dw  8*8                 ; 64 records/track
    db  4,15,0
    dw  D5DSM-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  3

dpbd6:    ;double density, double sided, 1024B
    dw  8*8
    db  4,15,0
    dw  D6DSM-1, 256-1
    db  11110000b,00000000b
    dw  (256+3)/4
    dw  2*2
```

### T-200 (IF inch5)

```asm
dpbs1:    ;single density, single sided, 128B
    dw  18
    db  3,7,0               ; 1KB block
    dw  72-1,64-1
    db  11000000b,00000000b
    dw  (64+3)/4
    dw  3

dpbs2:    ;single density, double sided, 128B
    dw  18
    db  3,7,0
    dw  144-1,64-1
    db  11000000b,00000000b
    dw  (64+3)/4
    dw  6

dpbd1:    ;double density, single sided, 256B
    dw  2*18                ; 36 records/track
    db  3,7,0
    dw  144-1, 64-1
    db  11000000b,00000000b
    dw  (64+3)/4
    dw  3

dpbd2:    ;double density, double sided, 256B
    dw  2*16                ; 32 records/track (16 sectors)
    db  3,7,0
    dw  256-1, 64-1
    db  11000000b,00000000b
    dw  (64+3)/4
    dw  6

dpbd3:    ;double density, single sided, 512B
    dw  4*09                ; 36 records/track
    db  3,7,0
    dw  144-1, 64-1
    db  11000000b,00000000b
    dw  (64+3)/4
    dw  3

dpbd4:    ;double density, double sided, 512B
    dw  4*09
    db  4,15,1              ; 2KB block, EXM=1
    dw  148-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  4

dpbd5:    ;double density, single sided, 1024B
    dw  8*5                 ; 40 records/track
    db  3,7,0
    dw  160-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  3

dpbd6:    ;double density, double sided, 1024B
    dw  8*5
    db  4,15,1
    dw  165-1, 128-1
    db  11000000b,00000000b
    dw  (128+3)/4
    dw  4
```

---

## Sector-translate (skew) tables

These map *CP/M logical sector number → physical sector index on track* (0-based on both sides). The BIOS shares one table across each SS/DS pair, indexed by sector size:

| Table | T-250 size & format     | T-200 size & format     |
|-------|--------------------------|--------------------------|
| stts  | SD 128B (types 0,1)      | SD 128B (types 0,1)      |
| sttd1 | DD 256B (types 2,3)      | DD 256B types 3 (DS)     |
| sttd2 | DD 512B (types 4,5)      | DD 512B (types 4,5)      |
| sttd3 | DD 1024B (types 6,7)     | DD 1024B (types 6,7)     |

### T-250 skew tables (verbatim from BIOS)

```asm
; stts — SD 128B, 26 sectors. Skew = 6.
stts:
    db 00,06,12,18,24,04,10,16,22,02,08,14,20
    db 01,07,13,19,25,05,11,17,23,03,09,15,21

; sttd1 — DD 256B, 52 records (26 sectors × 2). Skew-6 on physical sectors.
sttd1:
    db 00,01,06,07,12,13,18,19,24,25,30,31,36,37,42,43
    db 48,49,02,03,08,09,14,15,20,21,26,27,32,33,38,39
    db 44,45,50,51,04,05,10,11,16,17,22,23,28,29,34,35
    db 40,41,46,47

; sttd2 — DD 512B, 60 records (15 sectors × 4). Skew-4 on physical sectors.
sttd2:
    db 00,01,02,03,16,17,18,19,32,33,34,35,48,49,50,51
    db 04,05,06,07,20,21,22,23,36,37,38,39,52,53,54,55
    db 08,09,10,11,24,25,26,27,40,41,42,43,56,57,58,59
    db 12,13,14,15,28,29,30,31,44,45,46,47

; sttd3 — DD 1024B, 64 records (8 sectors × 8). Skew-3 on physical sectors.
sttd3:
    db 00,01,02,03,04,05,06,07,24,25,26,27,28,29,30,31
    db 48,49,50,51,52,53,54,55,08,09,10,11,12,13,14,15
    db 32,33,34,35,36,37,38,39,56,57,58,59,60,61,62,63
    db 16,17,18,19,20,21,22,23,40,41,42,43,44,45,46,47
```

### T-200 skew tables (verbatim from BIOS)

```asm
; stts — SD 128B, 18 sectors. Skew = 2.
stts:
    db 00,04,08,12,16,02,06,10,14
    db 01,05,09,13,17,03,07,11,15

; sttd1 — DD 256B (DS only, 32 records on 16 sectors × 2). Skew-2.
sttd1:
    db 00,01,08,09,16,17,24,25
    db 02,03,10,11,18,19,26,27
    db 04,05,12,13,20,21,28,29
    db 06,07,14,15,22,23,30,31

; sttd2 — DD 512B, 36 records (9 sectors × 4). Skew-2.
sttd2:
    db 00,01,02,03,16,17,18,19,32,33,34,35
    db 08,09,10,11,24,25,26,27,04,05,06,07
    db 20,21,22,23,36,37,38,39,12,13,14,15
    db 28,29,30,31

; sttd3 — DD 1024B, 40 records (5 sectors × 8). Skew-1 (sequential).
sttd3:
    db 00,01,02,03,04,05,06,07
    db 32,33,34,35,36,37,38,39
    db 24,25,26,27,28,29,30,31
    db 16,17,18,19,20,21,22,23
    db 08,09,10,11,12,13,14,15
```

---

## cpmtools diskdefs

cpmtools' `tracks` is **total tracks across all heads**. Skew is expressed as `skewtab` for non-sequential interleaves.

```
# T-250 (8" DSDD)

diskdef toshiba-t250-sd-ss
  seclen 128
  tracks 77
  sectrk 26
  blocksize 1024
  maxdir 64
  skewtab 0,6,12,18,24,4,10,16,22,2,8,14,20,1,7,13,19,25,5,11,17,23,3,9,15,21
  boottrk 2
  os 2.2
end

diskdef toshiba-t250-sd-ds
  seclen 128
  tracks 154
  sectrk 26
  blocksize 2048
  maxdir 128
  skewtab 0,6,12,18,24,4,10,16,22,2,8,14,20,1,7,13,19,25,5,11,17,23,3,9,15,21
  boottrk 4
  os 2.2
end

diskdef toshiba-t250-dd-ss-256
  seclen 256
  tracks 77
  sectrk 26
  blocksize 2048
  maxdir 128
  skewtab 0,3,6,9,12,15,18,21,24,1,4,7,10,13,16,19,22,25,2,5,8,11,14,17,20,23
  boottrk 3
  os 2.2
end

diskdef toshiba-t250-dd-ds-256
  seclen 256
  tracks 154
  sectrk 26
  blocksize 2048
  maxdir 256
  skewtab 0,3,6,9,12,15,18,21,24,1,4,7,10,13,16,19,22,25,2,5,8,11,14,17,20,23
  boottrk 4
  os 2.2
end

diskdef toshiba-t250-dd-ss-512
  seclen 512
  tracks 77
  sectrk 15
  blocksize 2048
  maxdir 128
  skewtab 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11
  boottrk 3
  os 2.2
end

diskdef toshiba-t250-dd-ds-512
  seclen 512
  tracks 154
  sectrk 15
  blocksize 2048
  maxdir 256
  skewtab 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11
  boottrk 4
  os 2.2
end

diskdef toshiba-t250-dd-ss-1024
  seclen 1024
  tracks 77
  sectrk 8
  blocksize 2048
  maxdir 128
  skewtab 0,3,6,1,4,7,2,5
  boottrk 3
  os 2.2
end

diskdef toshiba-t250-dd-ds-1024
  seclen 1024
  tracks 154
  sectrk 8
  blocksize 2048
  maxdir 256
  skewtab 0,3,6,1,4,7,2,5
  boottrk 4
  os 2.2
end
```

```
# T-200 (5.25" DSDD)

diskdef toshiba-t200-sd-ss
  seclen 128
  tracks 35
  sectrk 18
  blocksize 1024
  maxdir 64
  skewtab 0,4,8,12,16,2,6,10,14,1,5,9,13,17,3,7,11,15
  boottrk 3
  os 2.2
end

diskdef toshiba-t200-sd-ds
  seclen 128
  tracks 70
  sectrk 18
  blocksize 1024
  maxdir 64
  skewtab 0,4,8,12,16,2,6,10,14,1,5,9,13,17,3,7,11,15
  boottrk 6
  os 2.2
end

diskdef toshiba-t200-dd-ss-256
  seclen 256
  tracks 35
  sectrk 18
  blocksize 1024
  maxdir 64
  skewtab 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15,16,17
  boottrk 3
  os 2.2
end

diskdef toshiba-t200-dd-ds-256
  seclen 256
  tracks 70
  sectrk 16
  blocksize 1024
  maxdir 64
  skewtab 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15
  boottrk 6
  os 2.2
end

diskdef toshiba-t200-dd-ss-512
  seclen 512
  tracks 35
  sectrk 9
  blocksize 1024
  maxdir 64
  skewtab 0,4,8,2,6,1,5,3,7
  boottrk 3
  os 2.2
end

diskdef toshiba-t200-dd-ds-512
  seclen 512
  tracks 70
  sectrk 9
  blocksize 2048
  maxdir 128
  skewtab 0,4,8,2,6,1,5,3,7
  boottrk 4
  os 2.2
end

diskdef toshiba-t200-dd-ss-1024
  seclen 1024
  tracks 35
  sectrk 5
  blocksize 1024
  maxdir 128
  skewtab 0,4,3,2,1
  boottrk 3
  os 2.2
end

diskdef toshiba-t200-dd-ds-1024
  seclen 1024
  tracks 70
  sectrk 5
  blocksize 2048
  maxdir 128
  skewtab 0,4,3,2,1
  boottrk 4
  os 2.2
end
```

**Note**: cpmtools doesn't natively handle the mixed boot track (FM 128-byte) on an otherwise MFM disk. Use a separate process to image the boot track, or skip it with `-T` (treat boot as opaque blob).

---

## libdsk format definitions

Append these to `/etc/libdskrc` or `~/.libdskrc`.

```
[toshiba-t250-sd-ss]
description=Toshiba T-250 8" SD/SS 128B sectors
cylinders=77
heads=1
sectors=26
secsize=128
secbase=1
datarate=SD
rwgap=42
fmtgap=80
fm=Y

[toshiba-t250-sd-ds]
description=Toshiba T-250 8" SD/DS 128B sectors
cylinders=77
heads=2
sectors=26
secsize=128
secbase=1
datarate=SD
rwgap=42
fmtgap=80
fm=Y
side1as0=N

[toshiba-t250-dd-ss-256]
description=Toshiba T-250 8" DD/SS 256B sectors
cylinders=77
heads=1
sectors=26
secsize=256
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N

[toshiba-t250-dd-ds-256]
description=Toshiba T-250 8" DD/DS 256B sectors
cylinders=77
heads=2
sectors=26
secsize=256
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N
side1as0=N

[toshiba-t250-dd-ss-512]
description=Toshiba T-250 8" DD/SS 512B sectors
cylinders=77
heads=1
sectors=15
secsize=512
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N

[toshiba-t250-dd-ds-512]
description=Toshiba T-250 8" DD/DS 512B sectors
cylinders=77
heads=2
sectors=15
secsize=512
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N
side1as0=N

[toshiba-t250-dd-ss-1024]
description=Toshiba T-250 8" DD/SS 1024B sectors
cylinders=77
heads=1
sectors=8
secsize=1024
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N

[toshiba-t250-dd-ds-1024]
description=Toshiba T-250 8" DD/DS 1024B sectors
cylinders=77
heads=2
sectors=8
secsize=1024
secbase=1
datarate=DD
rwgap=42
fmtgap=80
fm=N
side1as0=N

[toshiba-t200-sd-ss]
description=Toshiba T-200 5.25" SD/SS 128B sectors
cylinders=35
heads=1
sectors=18
secsize=128
secbase=1
datarate=SD
rwgap=12
fmtgap=23
fm=Y

[toshiba-t200-sd-ds]
description=Toshiba T-200 5.25" SD/DS 128B sectors
cylinders=35
heads=2
sectors=18
secsize=128
secbase=1
datarate=SD
rwgap=12
fmtgap=23
fm=Y
side1as0=N

[toshiba-t200-dd-ss-256]
description=Toshiba T-200 5.25" DD/SS 256B sectors
cylinders=35
heads=1
sectors=18
secsize=256
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N

[toshiba-t200-dd-ds-256]
description=Toshiba T-200 5.25" DD/DS 256B sectors (16 sectors/track)
cylinders=35
heads=2
sectors=16
secsize=256
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N
side1as0=N

[toshiba-t200-dd-ss-512]
description=Toshiba T-200 5.25" DD/SS 512B sectors
cylinders=35
heads=1
sectors=9
secsize=512
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N

[toshiba-t200-dd-ds-512]
description=Toshiba T-200 5.25" DD/DS 512B sectors
cylinders=35
heads=2
sectors=9
secsize=512
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N
side1as0=N

[toshiba-t200-dd-ss-1024]
description=Toshiba T-200 5.25" DD/SS 1024B sectors
cylinders=35
heads=1
sectors=5
secsize=1024
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N

[toshiba-t200-dd-ds-1024]
description=Toshiba T-200 5.25" DD/DS 1024B sectors
cylinders=35
heads=2
sectors=5
secsize=1024
secbase=1
datarate=DD
rwgap=12
fmtgap=23
fm=N
side1as0=N
```

**Note on libdsk**: the `datarate=SD/DD` fields refer to the bit-cell rate, not modulation. For 5.25" SD at 125 kbps, use `datarate=DD` (250 kbps cell rate) with `fm=Y` (FM modulation → half the bit rate). libdsk has no clean way to express the mixed-density boot track; use per-cylinder overrides or `dskconv` with custom logic.

---

## FlashFloppy / GOTEK (HxC / HFE)

FlashFloppy reads from any of several container formats. The most direct path for these formats:

### Recommended: HFE (HxC Free Encoding)

Generate from real media using **Greaseweazle** (you already have this — `/Users/dlr/src/Kicad/greaseweazle/`):

```sh
# T-250 8" DD/DS 256B
gw read --device=/dev/cu.usbmodemGWxxx \
        --drive=A --tracks=c=0-76:h=0-1 \
        --format=ibm.scan \
        t250-disk.hfe

# T-200 5.25" DD/DS 256B  
gw read --device=/dev/cu.usbmodemGWxxx \
        --drive=A --tracks=c=0-34:h=0-1 \
        --format=ibm.scan \
        t200-disk.hfe
```

Use `ibm.scan` for auto-detection of mixed-density (boot track FM, rest MFM). Greaseweazle handles this transparently.

### Alternative: IMG with FF.CFG

If using a flat IMG file with FlashFloppy, the `FF.CFG` (or filename-based metadata) needs:

```
# T-250 DD/DS 256B
cyls = 77
heads = 2
secs = 26
bps = 256
rpm = 360
rate = 500
interleave = 1   # skew is in the data layout

# T-200 DD/DS 256B
cyls = 35
heads = 2
secs = 16
bps = 256
rpm = 300
rate = 250
interleave = 1
```

**Important caveat**: a flat IMG file *cannot* express the boot track being a different density. For boot-loadable disks, you **must** use HFE (or the equivalent SCP/IPF/DSK-with-track-metadata formats). FlashFloppy's HFEv3 handles per-track density override correctly.

### Naming convention for FlashFloppy

Place files in the USB stick's root or a folder. FlashFloppy auto-selects based on the filename extension. Suggested names:

```
TOSHIBA/T250-CPM-2.20.HFE
TOSHIBA/T250-CPM-2.32.HFE
TOSHIBA/T200-CPM.HFE
TOSHIBA/T200-UTILS.HFE
```

---

## Producing master disk images

### From the real machine (preservation)

Greaseweazle at maximum density to capture raw flux:

```sh
# T-250 — read at 500 kbps cell rate, capture all 77 cyls × 2 heads
gw read --drive=A --tracks=c=0-76:h=0-1 \
        --format=ibm.scan --revs=4 \
        master-t250.scp

# Convert flux to HFE
gw convert master-t250.scp master-t250.hfe

# Convert flux to .img (loses boot track metadata)
gw convert master-t250.scp master-t250.img --format=ibm.scan
```

For T-200, change `c=0-34`. Use `--diskdefs` if you have custom format definitions.

### To create a blank formatted disk in MAME

```sh
# Use chdman or floptool
floptool create dsk toshiba-t250-dd-ds-256 blank.dsk

# Or in MAME, mount an unformatted blank and let CP/M FORMAT.COM do it
./mame t250 -flop1 boot.imd -flop2 blank.dsk
# At CP/M: FORMAT B:
```

---

## Cross-reference

| File | Lines | Content |
|------|-------|---------|
| `b/b1/5/bios64.asm` | 396-471 | T-250 DPBs (`dpbs1`...`dpbd6`) |
| `b/b1/5/bios64.asm` | 472-538 | T-200 DPBs |
| `b/b1/5/bios64.asm` | 166-280 | Sector translate tables (`stts`, `sttd1`-`sttd3`) and dispatch (`stable`, `spttab`, `typtab`, `dpbtab`) |
| `b/b1/5/equate.lib` | 120-137 | DSM equate calculations |
| `b/b1/5/diskdef.lib` | — | CP/M DPB macro definitions (Digital Research standard) |

---

## Provenance & caveats

- Derived from `bios64.asm` rev 2.12, revision date 1982-04-22, release date 1982-02-08, by Dave Rand.
- Authoritative when in doubt: the BIOS source. DPB literals override equate calculations.
- T-200 type 3 (DD/DS 256B) has only 16 sectors/track — different from the SS variant's 18. Same physical disk format.
- Skew tables are **non-sequential and machine-specific**. Always use the literal byte arrays above when defining for tools that support `skewtab`.
- Boot track (cyl 0, head 0) is always **FM 128-byte sectors with 26 sectors** (T-250) or **18 sectors** (T-200) regardless of the rest of the disk's format. Many tools don't handle mixed-density correctly; preserve with raw-flux capture.

---

*Document generated from BIOS source analysis. Provided as-is. Suggested corrections welcome.*
