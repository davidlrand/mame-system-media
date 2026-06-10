import sys
def parse_imd(data):
    # skip ASCII header comment up to 0x1A
    i = data.index(0x1A)+1
    tracks = {}
    while i < len(data):
        if i+5 > len(data): break
        mode,cyl,head,nsec,szc = data[i],data[i+1],data[i+2],data[i+3],data[i+4]
        i += 5
        ssize = 128 << szc
        smap = list(data[i:i+nsec]); i += nsec
        hflag = head
        if hflag & 0x40: i += nsec  # cyl map
        if hflag & 0x80: i += nsec  # head map
        h = hflag & 1
        secs = {}
        for s in range(nsec):
            t = data[i]; i += 1
            if t == 0: secs[smap[s]] = bytes([0xE5])*ssize
            elif t in (1,3,5,7): secs[smap[s]] = data[i:i+ssize]; i += ssize
            elif t in (2,4,6,8): secs[smap[s]] = bytes([data[i]])*ssize; i += 1
            else: raise SystemExit("bad sector type %d"%t)
        tracks[(cyl,h)] = (ssize, secs)
    return tracks

data = open(sys.argv[1],'rb').read()
tr = parse_imd(data)
maxcyl = max(c for (c,h) in tr)
SS = 256
out = bytearray()
order = sys.argv[3] if len(sys.argv)>3 else 'grouped'   # grouped | interleaved
def emit(cyl,h):
    if (cyl,h) not in tr: return
    ssize, secs = tr[(cyl,h)]
    for sid in sorted(secs):           # de-interleave: ascending sector ID
        d = secs[sid]
        if len(d) < SS: d = d + bytes([0xE5])*(SS-len(d))  # pad 128->256 (FM trk0)
        out.extend(d[:SS])
if order == 'grouped':
    for h in (0,1):
        for cyl in range(maxcyl+1): emit(cyl,h)
else:
    for cyl in range(maxcyl+1):
        for h in (0,1): emit(cyl,h)
open(sys.argv[2],'wb').write(out)
print("order=%s maxcyl=%d tracks=%d flatsize=%d"%(order,maxcyl,len(tr),len(out)))
