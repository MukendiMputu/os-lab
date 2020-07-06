#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "4b.h"

int main(void)
{
	void *addr[16];
	int i = 0;

	printf("Aktuelles (leeres) Speicherlayout:\n");
	dump_memory();
	printf("Alloziere  10 kiB -> %p\n", addr[i++] = nf_alloc( 10 * 1024));
	dump_memory();
	printf("Gebe %p (10 kiB) frei\n", addr[0]);
	nf_free(addr[0]);
	dump_memory();
	printf("Alloziere  10 kiB -> %p\n", addr[i++] = nf_alloc( 10 * 1024));
	dump_memory();
	printf("Alloziere  10 kiB -> %p\n", addr[i++] = nf_alloc( 10 * 1024));
	dump_memory();
	printf("Gebe %p (10 kiB) frei\n", addr[1]);
	nf_free(addr[1]);
	dump_memory();
	printf("Alloziere  5 kiB -> %p\n", addr[i++] = nf_alloc( 5 * 1024));
	dump_memory();
	printf("Gebe %p (10 kiB) frei\n", addr[2]);
	nf_free(addr[2]);
	dump_memory();
	printf("Gebe %p (5 kiB) frei\n", addr[3]);
	nf_free(addr[3]);
	dump_memory();
	printf("Alloziere  10 kiB -> %p\n", addr[i++] = nf_alloc( 10 * 1024));
	dump_memory();
	printf("Alloziere 500 kiB -> %p\n", addr[i++] = nf_alloc(944 * 1024));
	dump_memory();
	printf("Alloziere   1   B -> %p\n", addr[i++] = nf_alloc(1));
	dump_memory();
	printf("Alloziere 200 kiB -> %p\n", addr[i++] = nf_alloc(200 * 1024));
	dump_memory();
	printf("Alloziere  40 kiB -> %p\n", addr[i++] = nf_alloc( 40 * 1024));
	dump_memory();
	printf("Alloziere   1   B -> %p\n", addr[i++] = nf_alloc(1));
	dump_memory();
	printf("Gebe %p (200 kiB) frei\n", addr[6]);
	nf_free(addr[6]);
	dump_memory();
	printf("Gebe NULL frei\n");
	nf_free(NULL);
	dump_memory();
	printf("Alloziere  20 kiB -> %p\n", addr[i++] = nf_alloc( 20 * 1024));
	dump_memory();

	return 0;
}
