#!/usr/bin/env python3
# Build the MK-83 (ADE MK3000) bootable CP/M 2.2 disk image (IMD).
#
# Format: "Standard ADE 5.25-inch 40 tr." (Tabella Parametri sheet, 7/6/83):
#   40 cylinders, 1 side, MFM 250 kbps, 10 x 512-byte sectors, skew 2
#   (physical ID order 1,6,2,7,3,8,4,9,5,10), CP/M: 2K blocks, dir track 3,
#   128 entries, 3 reserved tracks.
#
# Layout:
#   trk0 sec1        boot loader (boot.z80, read by the monitor's B command)
#   trk0 sec2-10 +
#   trk1 sec1-4      CCP+BDOS (Xerox CP/M 2.2C relocated to DC00) + CBIOS (F200)
#   trk1 5-10, trk2  unused (E5)
#   trk3+            CP/M file system (dir blocks 0-1, then file data)

import sys, os, datetime

HERE = os.path.dirname(os.path.abspath(__file__))
CYLS, SPT, SSZ = 40, 10, 512
OFF = 3                      # reserved tracks
BLOCKSZ = 2048
INTERLEAVE = [1, 6, 2, 7, 3, 8, 4, 9, 5, 10]   # skew 2 physical ID order

def rd(p):
    with open(p, 'rb') as f:
        return f.read()

def pad(b, n, fill=0x00):
    assert len(b) <= n, (len(b), n)
    return b + bytes([fill]) * (n - len(b))

# ---------------------------------------------------------------- system
boot = rd(os.path.join(HERE, 'boot.bin'))
ccpbdos = pad(rd(os.path.join(HERE, 'ccpbdos_dc00.bin')), 0x1600)
cbios = rd(os.path.join(HERE, 'cbios.bin'))
assert all(b == 0 for b in cbios[0x400:]), 'CBIOS initialized data past F5FF'
cbios = pad(cbios[:0x400], 0x400)
system = ccpbdos + cbios                       # 0x1A00 = 13 sectors
assert len(system) == 13 * SSZ

# disk as track/sector dict, default E5
disk = {}                                      # (cyl, sec1based) -> 512 bytes
def put(cyl, sec, data):
    disk[(cyl, sec)] = pad(data, SSZ)

put(0, 1, pad(boot, SSZ))
for i in range(13):
    t, s = divmod(i + 1, SPT)                  # linear from trk0 sec2
    put(t, s + 1, system[i*SSZ:(i+1)*SSZ])

# ---------------------------------------------------------------- filesystem
# data area = tracks OFF..39 as a flat array of 2K blocks
data_tracks = CYLS - OFF
fs = bytearray(b'\xe5' * (data_tracks * SPT * SSZ))
nextblk = 2                                    # blocks 0,1 = directory
dirents = []

def add_file(name, ext, content):
    global nextblk
    recs = (len(content) + 127) // 128
    assert recs <= 128, 'file too big for one directory entry'
    nblk = (len(content) + BLOCKSZ - 1) // BLOCKSZ
    assert nblk <= 16
    blocks = list(range(nextblk, nextblk + nblk))
    nextblk += nblk
    for i, b in enumerate(blocks):
        chunk = pad(content[i*BLOCKSZ:(i+1)*BLOCKSZ], BLOCKSZ, 0x1a)
        fs[b*BLOCKSZ:(b+1)*BLOCKSZ] = chunk
    ent = bytes([0]) + pad(name.encode(), 8, 0x20) + pad(ext.encode(), 3, 0x20)
    ex = (recs - 1) // 128                     # 0 for everything we add
    ent += bytes([ex, 0, 0, recs if recs <= 128 else 128])
    ent += bytes(blocks) + bytes(16 - len(blocks))
    dirents.append(ent)
    print('  %-12s %5d bytes, %3d recs, blocks %s' % (name + '.' + ext, len(content), recs, blocks))

for fn in sys.argv[2:]:
    base = os.path.basename(fn)
    name, ext = os.path.splitext(base)
    add_file(name.upper(), ext[1:].upper(), rd(fn))

directory = b''.join(dirents)
assert len(directory) <= 2 * BLOCKSZ
fs[0:len(directory)] = directory

for t in range(data_tracks):
    for s in range(SPT):
        off = (t * SPT + s) * SSZ
        sec = bytes(fs[off:off+SSZ])
        if sec != b'\xe5' * SSZ:
            put(t + OFF, s + 1, sec)

# ---------------------------------------------------------------- IMD
out = bytearray()
out += ('IMD 1.18: %s\r\n' % datetime.datetime.now().strftime('%d/%m/%Y %H:%M:%S')).encode()
out += (b'MK-83 (ADE MK3000) CP/M 2.2 boot disk\r\n'
        b'Standard ADE 5.25" 40 tr.: 40x10x512 MFM SS, dir trk 3, 2K blocks\r\n'
        b'Boot from the MULTIPROG 2.00 monitor: U21,27,A,2 then B1\r\n')
out += b'\x1a'
for cyl in range(CYLS):
    out += bytes([5, cyl, 0, SPT, 2])          # mode 5 = 250 kbps MFM
    out += bytes(INTERLEAVE)                   # sector numbering map
    for sec in INTERLEAVE:
        data = disk.get((cyl, sec), b'\xe5' * SSZ)
        if data == bytes([data[0]]) * SSZ:
            out += bytes([2, data[0]])         # compressed: all one byte
        else:
            out += bytes([1]) + data           # normal data
open(sys.argv[1], 'wb').write(out)
print('wrote %s (%d bytes), %d used sectors' % (sys.argv[1], len(out), len(disk)))
