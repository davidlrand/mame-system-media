#!/usr/bin/env python3
# Build a DSI-32 RAM image the way LOAD.EXE does (LOAD.C loadit()),
# for poking through the host window in a MAME test harness.
import struct, sys

START32K = bytes([0x6f,0xad,0x20,0xed,0xa7,0x00,0x20,0x17,0xa8,0x2c,0x7f,0x02])

def load_e32(image, path):
    d = open(path,'rb').read()
    execid, dirblk = struct.unpack_from('<hh', d, 0)
    heap_low, heap_high, stack_low, stack_high, mainmod, modcount, modaddr = \
        struct.unpack_from('<7i', d, 4)
    assert execid == 7700, f"{path}: execid {execid}"
    # general info to 0x2020
    image[0x2020:0x2030] = d[4:20]
    # main module's start address from its directory entry
    mdoff = dirblk*512 + (mainmod-1)*64
    def moddir(off):
        modnm = d[off:off+8]
        (modtype, strtaddr, slen, llen, clen, symlen, saddr, laddr, caddr,
         lblk, cblk, symblk, sbblk) = struct.unpack_from('<hiiiiiiiihhhh', d, off+8)
        return dict(modnm=modnm, strtaddr=strtaddr, slen=slen, llen=llen,
                    clen=clen, saddr=saddr, laddr=laddr, caddr=caddr,
                    lblk=lblk, cblk=cblk, sbblk=sbblk)
    realstart = moddir(mdoff)['strtaddr']
    # GMT table: file offset 32, 16 bytes per module -> modaddr
    gmt = bytearray(d[32:32+16*modcount])
    main_codead = struct.unpack_from('<i', gmt, (mainmod-1)*16+8)[0]
    struct.pack_into('<i', gmt, 12, realstart + main_codead)       # entry -> GMT[0].reserved (0x2C)
    struct.pack_into('<i', gmt, 16+12, mainmod-1)                  # main module no -> GMT[1].reserved
    print(f"  modaddr {modaddr:#x} dirblk {dirblk} modcount {modcount} heap {heap_low:#x}-{heap_high:#x}")
    image[modaddr:modaddr+len(gmt)] = gmt
    # module blocks
    for m in range(modcount):
        md = moddir(dirblk*512 + m*64)
        for blk, addr, ln in ((md['cblk'],md['caddr'],md['clen']),
                              (md['lblk'],md['laddr'],md['llen']),
                              (md['sbblk'],md['saddr'],md['slen'])):
            if blk > 0 and ln > 0:
                image[addr:addr+ln] = d[blk*512:blk*512+ln]
        if md['sbblk'] <= 0 and md['modnm'].startswith(b'$$$_COMM') and md['slen'] > 0:
            image[md['saddr']:md['saddr']+md['slen']] = bytes(md['slen'])
    return realstart + main_codead

image = bytearray(0x100000)  # 1MB: programs land as high as FC000
image[0:len(START32K)] = START32K
for path in sys.argv[1:-1]:
    entry = load_e32(image, path)
    print(f"{path}: entry {entry:06x}")
open(sys.argv[-1],'wb').write(image)
print(f"image: {len(image)} bytes -> {sys.argv[-1]}")
