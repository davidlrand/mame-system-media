# ET532 boot monitor

The ET532 was **never fabricated**, so there is no EPROM to dump. This monitor is
**built from source** with the **Definicon DSI-32** NS32k toolchain, then loaded
by the MAME `et532` driver. Unlike the pc532 `.u44` files (real dumps), this is a
fresh bring-up of a board that never physically existed.

## Files

| File | What |
|---|---|
| `et532mon.a32` | the monitor — position-independent NS32k assembly |
| `payload.a32` | a tiny test payload (prints `Z`) that exercises the loaders |
| `et532_monitor.bin` | the assembled 64 KB EPROM image (CRC `250253af`) the driver loads |

## What it does

A small (~1.2 KB) position-independent monitor, reset entry at EPROM address 0:

- console banner + echo on the SCC2698 (chip 0 ch A, 8N1 @ 9600)
- **D** dump memory, **S** deposit, **G** go (jump)
- **L** load a 512-byte boot block over the console; **B** load block 0 over SCSI
  (DP8490, target 1) — both validate a signature+checksum header, copy the
  payload and jump to it
- after the banner it auto-tries the SCSI boot, falling through to the monitor if
  there is no bootable disk

**Boot block** (512 bytes): `+0` magic `0xe5320bb0`, `+4` load addr, `+8` start
addr, `+12` length, `+16` checksum (sum of payload bytes), `+20` payload.

## Build

With the Definicon DSI-32 toolchain (`emu` running `as.e32`):

```
emu as -a et532mon.a32                 # -> et532mon.o32 (DSI object)
# the .o32 has a header; the code section starts at file offset 0x200, and its
# length is the 16-bit little-endian field at offset 0x12:
LEN=$(python3 -c "d=open('et532mon.o32','rb').read();print(d[0x12]|d[0x13]<<8)")
dd if=et532mon.o32 of=et532mon.bin bs=1 skip=512 count=$LEN
# pad to the 64 KB EPROM:
python3 -c "d=open('et532mon.bin','rb').read();open('et532_monitor.bin','wb').write(d+b'\xff'*(0x10000-len(d)))"
```

The monitor is position-independent (immediate addresses, PC-relative branches),
so the code runs at base 0 regardless of link address — the DSI linker is not
needed; the `.o32` code section is used directly. Verify with
`unidasm et532_monitor.bin -arch ns32000 -basepc 0`.

See [`../../docs/et532_hardware.md`](../../docs/et532_hardware.md) §9 for the
monitor reference and the rest of the board.
