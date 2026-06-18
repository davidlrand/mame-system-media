#!/usr/bin/env python3
# Faithful ET532 board render from the authoritative RS-274D photoplot gerbers in
# et532_pcb/ plus the ether.txt aperture table (real round/square pad sizes, true
# trace widths).  Pen-state plotter format: D01 = pen down, D02 = pen up (modal),
# D03 = flash at the current point; bare coordinate blocks move/draw per pen state.
# Composites a realistic top-of-board image (green soldermask, gold pads, white
# silk, drilled holes).  Holes are sized from the pads (a ~constant annular ring):
# the gerbers carry no real per-hole sizes — every hole flashes one "drill target"
# aperture; true tool sizes live only in the P-CAD binary drill table.
# Requires: Python 3, Pillow, numpy.   Run from anywhere: python3 render_gerber.py
import os, re, numpy as np
from PIL import Image, ImageDraw

HERE = os.path.dirname(os.path.abspath(__file__))
SRC  = os.path.join(HERE, 'et532_pcb')

def load_apertures(path):
    aps = {}
    for line in open(path):
        m = re.search(r'\bD(\d+)\b\s+([RS])(\d+)\b', line)
        if m:
            aps[int(m.group(1))] = (m.group(2), float(m.group(3)))  # (shape, mils)
    return aps

def parse(path, aps):
    txt = open(path).read()
    prims = []
    cx = cy = 0.0
    cur = None
    pen = False                                   # False = up (move), True = down (draw)
    for tok in re.findall(r'[^*]*\*', txt):
        t = tok.strip().rstrip('*')
        if not t:
            continue
        msel = re.fullmatch(r'(?:G\d+)?D(\d+)', t)            # aperture select
        if msel and int(msel.group(1)) >= 10:
            cur = int(msel.group(1)); continue
        m = re.fullmatch(r'(?:G\d+)?(?:X(-?\d+))?(?:Y(-?\d+))?(?:D0([123]))?', t)
        if not m or (m.group(1) is None and m.group(2) is None and m.group(3) is None):
            continue
        has_xy = m.group(1) is not None or m.group(2) is not None
        nx = cx if m.group(1) is None else float(m.group(1))
        ny = cy if m.group(2) is None else float(m.group(2))
        op = m.group(3); ap = aps.get(cur)
        if op == '3':                             # flash at the coordinate
            cx, cy = nx, ny
            if ap: prims.append(('F', cx, cy, ap[0], ap[1]))
        elif op == '1':                           # pen down (+ draw if coords given)
            if has_xy and ap: prims.append(('L', cx, cy, nx, ny, ap[1]))
            cx, cy = nx, ny; pen = True
        elif op == '2':                           # pen up (+ move if coords given)
            cx, cy = nx, ny; pen = False
        else:                                     # bare coordinate: apply pen state
            if pen and ap: prims.append(('L', cx, cy, nx, ny, ap[1]))
            cx, cy = nx, ny
    return prims

def bounds(prims):
    xs, ys = [], []
    for p in prims:
        if p[0] == 'L': xs += [p[1], p[3]]; ys += [p[2], p[4]]
        else:           xs += [p[1]];        ys += [p[2]]
    return min(xs), min(ys), max(xs), max(ys)

def render_mask(prims, x0, y0, scale, W, H):
    img = Image.new('L', (W, H), 0); d = ImageDraw.Draw(img)
    def T(x, y): return ((x-x0)*scale, H-(y-y0)*scale)
    for p in prims:
        if p[0] == 'L':
            _, x1, y1, x2, y2, w = p
            wpx = max(1.0, w*scale); a = T(x1, y1); b = T(x2, y2)
            d.line([a, b], fill=255, width=int(round(wpx))); r = wpx/2.0
            for (px, py) in (a, b): d.ellipse([px-r, py-r, px+r, py+r], fill=255)
        else:
            _, x, y, shape, sz = p
            r = sz*scale/2.0; px, py = T(x, y)
            if shape == 'S': d.rectangle([px-r, py-r, px+r, py+r], fill=255)
            else:            d.ellipse([px-r, py-r, px+r, py+r], fill=255)
    return np.asarray(img) > 0

def render_holes(mask_prims, x0, y0, scale, W, H):
    img = Image.new('L', (W, H), 0); d = ImageDraw.Draw(img)
    for p in mask_prims:
        if p[0] != 'F': continue
        _, x, y, shape, sz = p
        hole = min(sz*0.70, max(12.0, sz-26.0))   # ~constant annular ring (mils)
        r = hole*scale/2.0; px = (x-x0)*scale; py = H-(y-y0)*scale
        d.ellipse([px-r, py-r, px+r, py+r], fill=255)
    return np.asarray(img) > 0

DPI = 210            # common pixels-per-inch for ALL boards, so the rendered PNGs
                     # are physically proportional (the serial card is smaller than
                     # the main board, and the images reflect that).

def board(name, aps, roles, out, ss=2):
    layers = {k: parse(v, aps) for k, v in roles.items()}
    allp = [p for ps in layers.values() for p in ps]
    x0, y0, x1, y1 = bounds(allp)
    m = 60.0; x0 -= m; y0 -= m; x1 += m; y1 += m
    wmm, hmm = x1-x0, y1-y0                       # extents in mils
    scale = (DPI/1000.0)*ss                       # px per mil (supersampled)
    W, H = int(wmm*scale), int(hmm*scale)
    print(f"{name}: {wmm/1000:.2f} x {hmm/1000:.2f} in  ->  {W//ss} x {H//ss} px")
    M = {k: render_mask(p, x0, y0, scale, W, H) for k, p in layers.items()}
    rgb = np.zeros((H, W, 3), np.float32)
    SUB=np.array([18,88,46]); COPPER=np.array([30,120,64])
    PAD=np.array([222,184,88]); SILK=np.array([238,238,232]); HOLE=np.array([16,16,18])
    rgb[:] = SUB
    if 'cu' in M:   rgb[M['cu']]   = COPPER
    if 'mask' in M: rgb[M['mask']] = PAD
    if 'silk' in M: rgb[M['silk']] = SILK
    if 'mask' in layers:
        rgb[render_holes(layers['mask'], x0, y0, scale, W, H)] = HOLE
    Image.fromarray(rgb.astype(np.uint8)).resize((W//ss, H//ss), Image.LANCZOS).save(out)
    print(f"  wrote {out}")

if __name__ == '__main__':
    aps = load_apertures(os.path.join(SRC, 'ether.txt'))
    board('ETHER (main)', aps, {
        'cu':   os.path.join(SRC, 'ether.gbr', 'ether1.gbr'),
        'mask': os.path.join(SRC, 'ether.gbr', 'ethersmk.gbr'),
        'silk': os.path.join(SRC, 'ether.gbr', 'etherslk.gbr'),
    }, os.path.join(HERE, 'et532_ether_render.png'))
    board('SER (serial)', aps, {
        'cu':   os.path.join(SRC, 'serial.gbr', 'ser1.gbr'),
        'mask': os.path.join(SRC, 'serial.gbr', 'sersmk.gbr'),
        'silk': os.path.join(SRC, 'serial.gbr', 'serslk.gbr'),
    }, os.path.join(HERE, 'et532_ser_render.png'))
