all:   load

32016.o: 32016.c 32016.h defs.h trap.h
	cc -O3 -c 32016.c

decode.o: decode.c decode.h defs.h trap.h
	cc -O3 -c decode.c

emul32k.o: emul32k.c defs.h
	cc -O3 -c emul32k.c

mem32016.o: mem32016.c mem32016.h defs.h
	cc -O3 -c mem32016.c

load: 32016.o decode.o emul32k.o mem32016.o
	cc -o load -lrt 32016.o decode.o emul32k.o mem32016.o

clean:
	rm -f load 32016.o decode.o emul32k.o mem32016.o
