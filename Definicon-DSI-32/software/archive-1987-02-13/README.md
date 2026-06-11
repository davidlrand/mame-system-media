# The 1987-02-13 archive

Dave Rand's working-state archive of the Definicon software as it
stood in his final week at the company — the latest known generation
of the DSI-32 system software. Files dated 1987-02-13 are the archive
proper:

- `LOAD.EXE` / `LOAD.CMD` — Loader 2.24v (the version line's
  "02/15/85" date string is stale; copyright 1985,1986), plus
  LOADMEM, LOADD, LOADDEB variants
- `32IO.E32` — the mature I/O kernel (serves the extended service
  set: environment, dostime — required by programs built against
  this generation's CLIB)
- `MON.E32`, `MON256.E32`, `MON2M.E32` — monitors for the memory
  variants
- `CC.E32`, `AS.E32`/`ASB`/`ASC`, `LN.E32`/`LNB`/`LNC`, `LIB.E32` —
  the self-hosted toolchain running on the card
- `CLIB.O32`, `CMAIN.O32` — the runtime library this generation's
  programs link against
- `*.arc` — period source archives (loader, assembler, linker, C /
  FORTRAN / Pascal language tools); later file dates reflect when
  they were unpacked from the original media
- `SQ`/`USQ`, `MEMTEST`, `VIRT`, utility `.CMD`/`.EXE` drivers

Later-dated files (2017-2022) are emulator-era additions: dhrystone
and test programs compiled with the 2016-2017 Series 32000 emulator
against the 1987 CLIB, plus extracted headers.

**Version pairing**: programs built against this CLIB require this
generation's loader and 32IO (their startup requests the environment
service, which loaders 1.18-2.00 do not vector). Conversely the 1985
distribution binaries run fine under the 1987 loader.
