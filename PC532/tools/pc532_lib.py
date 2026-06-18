import socket, time, re, sys
ESC,CTLC,START=0x1b,0x03,ord(':')
def update_crc(crc,ch):
    for m in (0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01):
        crc=(crc<<1)|(1 if (m&ch) else 0)
        if crc&0x10000: crc^=0x11021
    return crc
def encode_image(path):
    data=open(path,'rb').read(); out=bytearray(); out.append(START)
    def q(b):
        if b==ESC or b==CTLC: out.append(ESC)
        out.append(b)
    n=len(data)
    for i in range(4): q((n>>(8*i))&0xff)
    crc=0
    for b in data: q(b); crc=update_crc(crc,b)
    for i in range(2): q((crc>>(8*i))&0xff)
    return bytes(out)
rx=bytearray(); conn=None; srv=None
def connect(port=7000):
    global conn,srv
    srv=socket.socket(); srv.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    srv.bind(('127.0.0.1',port)); srv.listen(1); srv.settimeout(30)
    sys.stderr.write("listening\n"); sys.stderr.flush()
    conn,_=srv.accept(); sys.stderr.write("connected\n"); sys.stderr.flush()
def pump(t):
    conn.settimeout(t)
    try:
        d=conn.recv(65536)
        if d: rx.extend(d)
    except (socket.timeout,OSError): pass
def expect(pat,timeout):
    end=time.time()+timeout
    while time.time()<end:
        if re.search(pat, rx.decode('latin1','replace')[-6000:]): return True
        pump(0.3)
    return False
def send(b):
    conn.settimeout(600); conn.sendall(b)
def sendslow(s):       # char-paced, with echo settling
    for ch in s.encode('latin1'):
        conn.sendall(bytes([ch])); time.sleep(0.03)
