# For use on HPLWBC to produce rom version

OBJ=	resume532.o debugger.o debugutil.o disasm.o ioutil.o \
	init532.o dblib.o pf.o newreg.o vaddr.o download.o scsi.o
SRC0=	debugger.c disasm.c 
SRC1=	ioutil.c debugutil.c \
	init532.c pf.c newreg.c vaddr.c download.c scsi.c
SRC2=	resume532.s dblib.s \
	dasm.h das32k.h debugger.h machine.h \
	version Makefile

GCCDIR = /usr/local/bin
GCCLIB = /usr/local/lib/32k

CC = $(GCCDIR)/gcc
LD = $(GCCLIB)/ld
AS = $(GCCLIB)/as
INCL = -I. -I../../include -I..
CFLAGS = -O -c

# LSC		1 if compiling with Lightspeed C on the Mac
# GCC		1 if compiling with GCC (can use const)
# STANDALONE	1 if compiling ROM version
# UNIX		1 if compiling to run under UNIX
DCL = -DLSC=0 -DGCC=1 -DSTANDALONE=1 -DUNIX=0

.c.o:
	$(CC) $(CFLAGS) $(DCL) $(INCL) $*.c

.s.o:
	$(AS) $*.s

# ROM version, text loads at 0, data at 0xa000
#	$(LD) -o rom_db -D a000 $(OBJ) version.o
rom_db: $(OBJ)
	sed -e "s/XXX/`date`/" version > version.c
	$(CC) $(CFLAGS) $(DCL) $(INCL) version.c
	$(LD) -o rom_db -T 10000000 -D 1000 $(OBJ) version.o

image.hex: rom_db
	dd if=rom_db of=image bs=1024 skip=1
	intelhex < image > image.hex

# RAM version, text at 0x200000, data following text
ram_db: $(OBJ)
	sed -e "s/XXX/`date`/" version > version.c
	$(CC) $(CFLAGS) $(DCL) $(INCL) version.c
	$(LD) -o ram_db -T 200000 $(OBJ) version.o

db_shar: $(SRC0) $(SRC1) $(SRC2)
	shar $(SRC0) > Shar/db_shar0
	shar $(SRC1) > Shar/db_shar1
	shar $(SRC2) > Shar/db_shar2
