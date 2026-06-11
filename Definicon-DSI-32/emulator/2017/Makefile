all: emu emu.lto

32016.o: 32016.c 32016.h defs.h
	cc -O3 -c 32016.c

Decode.o: Decode.c Decode.h defs.h
	cc -O3 -c Decode.c

emul32k.o: emul32k.c defs.h
	cc -O3 -c emul32k.c

mem32016.o: mem32016.c mem32016.h defs.h
	cc -O3 -c mem32016.c

emu: 32016.o Decode.o emul32k.o mem32016.o
	cc -o emu 32016.o Decode.o emul32k.o mem32016.o

emu.lto: 32016.c Decode.c mem32016.c emul32k.c
	gcc -O3 -flto -o emu.lto 32016.c Decode.c mem32016.c emul32k.c
