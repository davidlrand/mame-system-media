#!/usr/bin/env python3
# Relocate the Xerox CP/M 2.2C CCP+BDOS image (cpsys0/cpsys1 = the same
# code linked 0x100 apart) to CCP=DC00 for the MK-83 62k system.
# Bytes that differ by exactly +1 between the two links are address high
# bytes (the MOVCPM relocation-bitmap technique, proven on the x1685 disk).
import os, sys

SRC = os.path.expanduser(
    '~/src/github-davidlrand/mame-system-media/Xerox-820-16-8/source/cpm86-bios/')
TARGET_PAGE = 0xDC          # CCP base DC00

def readhex(p):
    mem = {}
    for line in open(p):
        line = line.strip()
        if not line.startswith(':'):
            continue
        n, a, t = int(line[1:3], 16), int(line[3:7], 16), int(line[7:9], 16)
        if t != 0:
            continue
        d = bytes.fromhex(line[9:9 + 2 * n])
        for i, b in enumerate(d):
            mem[a + i] = b
    lo, hi = min(mem), max(mem)
    return lo, bytes(mem.get(a, 0x00) for a in range(lo, hi + 1))

l0, b0 = readhex(SRC + 'cpsys0.hex')
l1, b1 = readhex(SRC + 'cpsys1.hex')
assert len(b0) == len(b1)
reloc = [i for i, (a, b) in enumerate(zip(b0, b1)) if a != b and (b - a) & 0xff == 1]
bad = [i for i, (a, b) in enumerate(zip(b0, b1)) if a != b and (b - a) & 0xff != 1]
assert not bad, bad
delta = (TARGET_PAGE - (l0 >> 8)) & 0xff
out = bytearray(b0)
for i in reloc:
    out[i] = (out[i] + delta) & 0xff
dst = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'ccpbdos_dc00.bin')
open(dst, 'wb').write(out)
print('%s: %d bytes, %d relocations, CCP entry %s, BDOS entry %s' %
      (dst, len(out), len(reloc), out[:3].hex(), out[0x806:0x809].hex()))
