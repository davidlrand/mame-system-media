# Boot ROMs and chargen ROM

Binary dumps from the actual chips on the T-200 and T-250 main boards.

| File | Size | CRC32 (zlib) | SHA1 | Purpose |
|------|-----:|--------------|------|---------|
| `t250boot.bin` | 4096 | `bf752b07` | `57fc571dab686bd0eb01c62061153b58b8a90161` | T-250 boot ROM at 0x0000-0x0FFF |
| `t200boot.bin` | 4096 | `a73f4647` | `f8f2eab78f993b0f086dfa995f3b9681d99fc08a` | T-200 boot ROM at 0x0000-0x0FFF |
| `t250cg.bin` | 1024 | `0770d6b7` | `810b6a7e739356662abc8b158922e39fac9df743` | Character generator ROM (separate chip on the video subsystem) |

The two boot ROM hashes match the `ROM_LOAD` entries in upstream MAME's `src/mame/toshiba/t250.cpp`.

## Chargen ROM note

`t250cg.bin` is a separate physical chip on the video board, but the MAME driver does not load or use it. During boot the BIOS copies a default character set into VPG static RAM (the 6845 sources glyphs from VPG, not from this ROM), and the data the BIOS writes appears to be embedded in the boot ROM itself rather than read from this chargen chip. The chip is preserved here as a hardware artifact — its exact role on real hardware (perhaps a fallback or an alternate font for the optional video subsystem variant) hasn't been fully characterized.

## Format

Plain binary, byte-addressable. The 8085 is little-endian internally but ROM contents are flat byte streams.

## Usage with MAME

```sh
mkdir -p roms/t250 roms/t200
cp t250boot.bin roms/t250/
cp t200boot.bin roms/t200/
# t250cg.bin is not required by the current MAME driver
```

Verify:
```sh
mame -verifyroms t250
mame -verifyroms t200
```

Both should report `romset is good`. See [`../docs/mame-usage.md`](../docs/mame-usage.md) for the complete setup.
