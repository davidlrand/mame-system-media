# Disk contents

Full CP/M directory listings of every disk in [`disks/`](.), extracted with
cpmtools.

## Reading these disks with cpmtools

The filesystem is the standard 8" SSSD CP/M layout — use the stock `ibm-3740`
definition (77×26×128 FM, block 1024, 64 directory entries, skew 6, 2 system
tracks) against a raw (IMD-decoded, sector-ID-ordered) image.

One gotcha: cpmtools' boot-sector sniffing mis-parses images whose track 0
contains real boot code (every bootable disk here) and produces a garbage
directory. Blank tracks 0–1 (fill with `0xE5`) in a scratch copy first — the
system tracks are outside the filesystem, so listings and file extraction are
unaffected.

## BB1.imd

```
-.810
asm.com
bios.asm
boot.hex
boot.mac
boot.prn
boot.rel
cbios.asm
cbios.hex
copy.com
cpm56.com
ddt.com
deblock.asm
diskdef.lib
dump.asm
dump.com
ed.com
l80.com
load.com
movcpm.com
oboot.asm
pip.com
rom.prn
stat.com
submit.com
sysgen.com
xdir.com
xsub.com
```

## BB2.imd

```
-.811
asm.com
bios.asm
boot.mac
cbios.hex
cbios.mac
cbios.old
cbios.prn
cbios.rel
copy.com
cpm60.com
ddt.com
deblock.asm
diskdef.lib
dump.asm
dump.com
ed.com
findbad.com
init.com
l80.com
load.com
m80.com
movcpm.com
mpc.com
oboot.asm
pip.com
stat.com
submit.com
sysgen.com
xdir.com
xsub.com
```

## BB3.imd

```
-.812
asm.$$$
boot.asm
boot.hex
boot.prn
cbios.asm
cbios.hex
cbios.prn
crtout.asm
diskio.asm
init.asm
intsrv.asm
memory.asm
monitor.asm
read.me
rom.asm
rom.hex
rom.prn
```

## BB4.imd

```
-.813
asm.com
bios.asm
boot.asm
cbios.asm
cbios.hex
copy.com
cpm.com
cpm60.com
ddt.com
deblock.asm
diskdef.lib
dump.asm
dump.com
ed.com
findbad.com
init.com
load.com
movcpm.com
mpcinfo.doc
pip.com
rom.prn
stat.com
submit.com
sysgen.com
xdir.com
xsub.com
```

## BBUD01.imd

```
-.810
clock.art
clock.asm
clock.com
clock.prn
copy.doc
copyall.com
copyfast.com
cplus.doc
cplus.man
croweasm.com
croweasm.doc
dirx.com
dr.cpm
format.doc
format2.com
format3.com
format3.z80
format4.com
format4.z80
modem.lib
modem7.asm
modem7.com
modem7.doc
modem7.set
othello.com
othello.doc
p.cpm
p.doc
pr.com
setclk.asm
setclk.bak
setclk.com
setclk.doc
setclk.prn
```

## BBUD05.imd

```
-.810
-usrdisk.005
5-disk.doc
cat.com
cat.doc
cat.sub
cat1.sub
catme.sub
clock.com
clock.mac
crck3.com
crck3.lst
d.cpm
dif.com
dif.doc
dumpx.com
dumpx.doc
fast.com
fast.doc
fmap.com
format4.com
format4.hex
format4.z80
mast.cat
modem7a.com
modem7b.com
nolock.com
pacdefs.h
pacman.c
pacman.com
pacman.doc
pacmonst.c
pacutil.c
snoopy.txt
sq#usq.doc
sq.com
ssed.com
typesq.com
ucat.com
unload.com
usq.com
verify.com
verify.doc
wash.com
xdir.com
```

## BBUD07.imd

```
-.810
-catalog.307
abstract.307
changpfm.doc
checks.com
checks.doc
checks.z80
chngpfm.asm
cpfrmem.asm
cpfrmem.com
cptomem.asm
cpxxmem.hlp
crc.com
dumpf.c
dumpf.com
dumpf.hlp
exampl.chk
exampl.doc
exampl.nam
help.com
help.hlp
loadhex.c
loadhex.com
loadhex.hlp
rom.asm
termnl.asm
termrec.asm
termsnd.asm
termxxx.hlp
time.asm
time.hlp
```

## BBUD09.imd

```
-.810
9-disk.doc
adv.com
advi.dat
advi.ptr
advt.dat
advt.ptr
cleanup.com
crck3.com
crck3.lst
e8bios.hex
e8bios.mac
eabios.hex
eprom.com
eprom.mac
modmon.com
modmon.mac
```

## BBUD13.imd

```
-.810
13-disk.doc
at.com
at.doc
crcklist.crc
cursor.asm
cursor.com
cursor.doc
ex14.asm
ex14.com
ex14.doc
ex14.sub
ex14.tst
makfcb.c
movpatch.asm
pippat.asm
remove.c
sort.c
sort.com
sort.doc
sort.let
tsort.c
tsort.sub
umpire.com
umpire.doc
xmon.com
xmon.doc
xmon.mac
xmonmac.doc
zesource.doc
zsidfix.doc
zzsource.asm
zzsource.com
```

## BBUD14.imd

```
-.810
14-disk.doc
aliens.com
aliens.doc
asm.su$
asm.sub
attrtest.int
attrtest.pas
baud12.com
baud12.mac
bdscio2.h
box.com
box.mac
ccpatch.asm
crcklist.crc
dumpx.doc
format.com
grafdemo.bas
init120.com
init30.com
initsio.mac
listset.com
menu.com
pip.com
print2.com
print2.mac
prog32.com
prog32.mac
program.com
readrom.com
readrom.mac
setclk.com
smodem.doc
smodem2.com
smodem2.mac
```

## BBUD15.imd

```
-catalog.315
abstract.315
bios.c
ccall.z80
chop.c
cp.c
crc.com
crypt.com
crypt.doc
default.$$$
dump.c
dump.com
edit.com
edit.doc
edit.hlp
entab.c
entab.com
is.c
matt.doc
ms.c
ms.com
portio.c
rtw.c
rtw.com
ted.com
ted.doc
tinyplan.asc
tinyplan.ccd
tinyplan.doc
trunc.c
trunc.com
tthelp0.dat
tthelp1.dat
tthelp2.dat
tthelp3.dat
tthelp4.dat
tthelp5.dat
ttkeybd.dat
ttype.bas
ttypexa.dat
ttypexb.dat
ttypexc.dat
ttypexd.dat
ttypexe.dat
ttypexf.dat
ttypexg.dat
ttypexh.dat
ttypexi.dat
ttypexj.dat
wrap.c
wrap.com
wrap.doc
ypt
```

## BBUD17.imd

```
-.810
-catalog.317
abs.c
abstract.317
call.mac
call.rel
cc.def
cc1.c
cc11.c
cc12.c
cc13.c
cc2.c
cc21.c
cc22.c
cc3.c
cc31.c
cc32.c
cc33.c
cc4.c
cc41.c
cc42.c
crc.com
d.com
dtoi.c
iolib.mac
iolib.rel
itod.c
itou.c
itox.c
left.c
lib.c
lib.rel
libl.h
makes.sub
out.c
printf.c
sign.c
smallc.doc
smallc2.doc
smc.com
stdiol.h
strcmp.c
utoi.c
xtoi.c
```

## ADVENT.imd

```
-.810
adv.com
advi.dat
advi.ptr
advt.dat
advt.ptr
copy.com
dump.com
ed.com
load.com
pip.com
stat.com
submit.com
sysgen.com
xdir.com
xsub.com
```

## GAMES.imd

```
aliens.com
copy.com
ddt.com
dump.com
ladder.com
ladder.dat
mload.com
pacman3a.com
pip.com
quatris.com
quatris.sco
stat.com
xdir.com
```

## PASCAL.imd

```
blinken.bak
blinken.com
blinken.pas
blinken2.bak
blinken2.com
blinken2.pas
blinken3.bak
blinken3.com
blinken3.pas
copy.com
ddt.com
dump.com
hello.com
hello.pas
mload.com
pip.com
porter.com
porter.pas
stat.com
tinst.com
tinst.dta
tinst.msg
turbo.com
turbo.msg
turbo.ovr
xdir.com
```

## PORTS.imd

```
copy.com
ddt.com
dump.com
mload.com
pip.com
porter.com
stat.com
xdir.com
```

