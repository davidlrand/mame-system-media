# pc532 MAME serial harness

Drives the pc532 ROM monitor over a TCP socket bridge (the canonical
"host + serial line + download program" install path, done in emulation).

## Run MAME with the serial console on a socket
    cd ~/src/mame
    SDL_VIDEODRIVER=dummy ./mame pc532 -hard <disk.chd> \
        -serial0 null_modem -bitb socket.127.0.0.1:7000 -video none -nothrottle

## Drive it (host side)
`pc532_lib.py` provides: connect(), send(b), sendslow(s) [char-paced, REQUIRED
for command arguments — a bulk send right after a big download drops the args],
expect(pat,timeout), encode_image(path) [monitor download format].

Monitor facts (verified): default radix = HEX; `download <hexaddr>` then stream
`:`+LE32-len+ESC-quoted-data+LE16-CCITT-CRC, then CR for "CRC ok" status.

## NetBSD 1.5.3 boot recipe (WORKS -> root shell)
    download 260000                      (hex)
    <stream floppy-144.fs>  CR           -> "CRC ok, length = 1474560"
    run 3be020                           (char-paced!)
    >> NetBSD/pc532 Boot, Revision 1.1 ; answer: md0a:/netbsd.gz
    -> NetBSD 1.5.3 kernel boots, sd0 detected, root on md0a, "# " shell.
