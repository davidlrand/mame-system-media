97801_banner.bin -- "SIEMENS / 97801" block-letter banner (1797 bytes)
Solid blocks are reverse-video spaces (CSI 7m), positioned with CSI row;colH;
5x7 font, 2 columns/pixel, centred for the 80x24 screen.  Regenerate: the
generator lives in the 2026-07-13 session notes (banner gen, scratchpad).

Display on the emulated terminal (hero-shot recipe):
  mame s97801 -view "Terminal and Keyboard" -host_port pty
  # note the "Pty slave is /dev/ttysNNN" line, then from another shell:
  (stty raw; sleep 8; dd bs=64 if=97801_banner.bin) > /dev/ttysNNN
  # wait ~8 s first so the terminal finishes its power-up self-test;
  # F12 (MAME snapshot) or a camera does the rest.
On real hardware: send the file down the SS97 line at 38400 7O1.
