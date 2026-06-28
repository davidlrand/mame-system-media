/*
 * hexout.c -- dump a binary file to the console as per-chunk-checksummed hex,
 * for pulling it off the icm3216hle host over the lossy serial tty.  Each 512B
 * chunk: "CK <idx> <nbytes> <hex...> <crc16>" where crc16 = BSD-rotate (== host
 * `sum -r').  The Mac side verifies each chunk's crc and merges good chunks
 * across retries.  Final "TOTAL <nbytes> <crc16>" for whole-file verification.
 *
 * Build/run on the host:  cc -o hexout hexout.c ; ./hexout pcrom.bin
 */
#include <stdio.h>

unsigned
rot(p, n)			/* BSD-rotate 16-bit checksum of n bytes at p */
char *p;
int n;
{
	unsigned crc = 0;
	int i;
	for (i = 0; i < n; i++) {
		crc = ((crc >> 1) | ((crc & 1) << 15));
		crc = (crc + (p[i] & 0xff)) & 0xffff;
	}
	return crc;
}

#define CH 512
char buf[CH];

main(argc, argv)
int argc;
char **argv;
{
	static char hx[] = "0123456789abcdef";
	FILE *f;
	int idx, n, i;
	long total = 0;
	unsigned tcrc = 0;

	f = fopen(argv[1], "r");
	if (f == NULL) { printf("ERR open %s\n", argv[1]); exit(1); }
	for (idx = 0; (n = fread(buf, 1, CH, f)) > 0; idx++) {
		printf("CK %d %d ", idx, n);
		for (i = 0; i < n; i++) {
			putchar(hx[(buf[i] >> 4) & 0xf]);
			putchar(hx[buf[i] & 0xf]);
		}
		printf(" %u\n", rot(buf, n));
		for (i = 0; i < n; i++) {
			tcrc = ((tcrc >> 1) | ((tcrc & 1) << 15));
			tcrc = (tcrc + (buf[i] & 0xff)) & 0xffff;
		}
		total += n;
	}
	printf("TOTAL %ld %u\n", total, tcrc);
	fclose(f);
	exit(0);
}
