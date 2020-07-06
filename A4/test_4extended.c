#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "4b.h"
#include "4extended.h"

#define soft_assert(expr) \
	if (!(expr)) { \
		printf("%s:%d: Assertion %s failed\n", __FILE__, __LINE__, #expr); \
	}

int main(void)
{
	char *addr[16];
	int i = 0;

	printf("Aktuelles (leeres) Speicherlayout:\n");
	dump_memory();
	
	printf("Alloziere  10 kiB -> %p\n", addr[i] = bf_alloc( 10 * 1024));
	*addr[i] = 'A';
	i++;
	dump_memory();
	
	printf("Alloziere  10 kiB -> %p\n", addr[i] = bf_alloc( 10 * 1024));
	*(addr[i]) = 'A';
	i++;
	dump_memory();
	
	printf("Alloziere  8 kiB -> %p\n", addr[i] = bf_alloc( 8 * 1024));
	*(addr[i]) = 'A';
	i++;
	dump_memory();
	
	printf("Alloziere  20 kiB -> %p\n", addr[i] = bf_alloc( 20 * 1024));
	*(addr[i]) = 'A';
	i++;
	dump_memory();
	
	printf("Gebe %p (10 kiB) frei\n", addr[0]);	
	bf_free(addr[0]);
	*addr[0] = 'F';
	dump_memory();
	
	printf("Gebe %p (8 kiB) frei\n", addr[2]);	
	bf_free(addr[2]);
	*addr[2] = 'F';
	dump_memory();
	
	printf("Alloziere  20 kiB -> %p\n", addr[i] = bf_alloc( 20 * 1024));
	*(addr[i]) = 'A';
	i++;
	dump_memory();
	
	printf("Alloziere 500 kiB -> %p\n", addr[i++] = bf_alloc( 500 * 1024));
	
	printf("Alloziere  15 kiB -> %p\n", addr[i] = bf_alloc( 15 * 1024));
	*addr[i] = 'A';
	i++;
	dump_memory();
	
	printf("Gebe %p (20 kiB) frei\n", addr[4]);	
	bf_free(addr[4]);
	*addr[4] = 'F';
	dump_memory();
	
	printf("Alloziere  6 kiB -> %p\n", addr[i] = bf_alloc( 6 * 1024));
	*addr[i] = 'A';
	i++;
	dump_memory();
	
	printf("Alloziere 0 B -> %p\n", addr[i] = bf_alloc( 0 ));
	
	return 0;
}
