/*
 * coffbin.c -- carve a flat 32KB EPROM image from the COFF `pcrom' a.out.
 *
 * The SVR2 host has no objcopy.  This reads the COFF section headers natively
 * (host IS the ns32k that ld wrote the COFF on -- no byte-swap) and emits:
 *
 *   img[0 .. textsize)               = .text  (ROM @ rom_org)
 *   img[textsize .. textsize+dsize)  = .data IMAGE  (lands in ROM at _etext;
 *                                      pc532romstart copies it to the .data VMA)
 *   img[...]                         = 0xff pad to 32768 (.bss has no file bytes)
 *
 * Build/run on the host:  cc -o coffbin coffbin.c ; ./coffbin pcrom pcrom.bin
 */
#include <stdio.h>

struct filehdr {
	unsigned short f_magic, f_nscns;
	long  f_timdat, f_symptr, f_nsyms;
	unsigned short f_opthdr, f_flags;
};
struct scnhdr {
	char  s_name[8];
	long  s_paddr, s_vaddr, s_size, s_scnptr, s_relptr, s_lnnoptr;
	unsigned short s_nreloc, s_nlnno;
	long  s_flags;
};

/* On-disk COFF record sizes are FIXED by the format -- do NOT use sizeof().
 * This ns32k SVR2 COFF uses a 48-BYTE section header (standard 40 fields + 8
 * trailing bytes); verified from `od -x pcrom' (.data name@60, .bss@108,
 * .text@156 -> 48-byte stride).  We stride by 48 but read only the 40 std
 * fields into struct scnhdr. */
#define FILHSZ 20
#define SCNHSZ 48			/* on-disk section-header stride */
#define SCNFLD 40			/* standard fields to read into the struct */

#define ROMSZ 32768
char img[ROMSZ];

main(argc, argv)
int argc;
char **argv;
{
	FILE *f;
	struct filehdr fh;
	struct scnhdr sh;
	long textsize = 0;
	int i, got_text = 0, got_data = 0;

	if (argc < 3) { printf("usage: coffbin in.coff out.bin\n"); exit(1); }
	for (i = 0; i < ROMSZ; i++) img[i] = 0xff;

	f = fopen(argv[1], "r");
	if (f == NULL) { printf("cannot open %s\n", argv[1]); exit(1); }
	fread((char *)&fh, FILHSZ, 1, f);
	printf("magic=0%o nscns=%d opthdr=%d\n", fh.f_magic, fh.f_nscns, fh.f_opthdr);

	/* PASS 1: find .text size (== _etext offset where .data image goes) */
	for (i = 0; i < fh.f_nscns; i++) {
		fseek(f, (long)FILHSZ + fh.f_opthdr + (long)i * SCNHSZ, 0);
		fread((char *)&sh, SCNFLD, 1, f);
		if (strncmp(sh.s_name, ".text", 5) == 0) textsize = sh.s_size;
	}
	printf("textsize=%ld (data image goes at img offset %ld)\n", textsize, textsize);

	/* PASS 2: copy .text -> img[0], .data -> img[textsize] */
	for (i = 0; i < fh.f_nscns; i++) {
		long out = -1;
		fseek(f, (long)FILHSZ + fh.f_opthdr + (long)i * SCNHSZ, 0);
		fread((char *)&sh, SCNFLD, 1, f);
		printf("scn %-8.8s vaddr=0x%lx size=%ld scnptr=%ld\n",
		       sh.s_name, sh.s_vaddr, sh.s_size, sh.s_scnptr);
		if (strncmp(sh.s_name, ".text", 5) == 0) { out = 0;        got_text = 1; }
		else if (strncmp(sh.s_name, ".data", 5) == 0) { out = textsize; got_data = 1; }
		else continue;			/* .bss / .comment: no file bytes */
		if (sh.s_scnptr == 0 || sh.s_size == 0) continue;
		if (out + sh.s_size > ROMSZ) {
			printf("OVERFLOW: %.8s past 32KB\n", sh.s_name); exit(1);
		}
		{
			long save = ftell(f);
			fseek(f, sh.s_scnptr, 0);
			fread(&img[out], 1, (int)sh.s_size, f);
			fseek(f, save, 0);
		}
	}
	fclose(f);
	if (!got_text) { printf("no .text!\n"); exit(1); }

	f = fopen(argv[2], "w");
	if (f == NULL) { printf("cannot create %s\n", argv[2]); exit(1); }
	fwrite(img, 1, ROMSZ, f);
	fclose(f);
	printf("wrote %s (%d bytes); text@0 data@%ld%s\n",
	       argv[2], ROMSZ, textsize, got_data ? "" : " (NO .data)");
	exit(0);
}
