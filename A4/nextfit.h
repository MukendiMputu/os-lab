#ifndef __NEXTFIT_H__
#define __NEXTFIT_H__

#include <stdlib.h> /* fuer size_t */

#define CHUNK_SIZE 4096
#define NUM_CHUNKS 64

#define CHUNK_FREE 0
#define CHUNK_ALLOCATED 1

/*
 * Informationen ueber einen zusammenhaengenden Speicherbereich.
 * Falls length == 0 beginnt hier kein Speicherbereich und der status-Wert
 * ist egal. Andernfalls gibt length die Länge in Blöcken (Chunks) an und
 * status bestimmt, ob es sich um einen freien (CHUNK_FREE) oder belegten
 * (CHUNK_ALLOCATED) Speicherbereich handelt.
 */
typedef struct {
	unsigned char status;
	size_t length;
} mem_info;

extern char heap[CHUNK_SIZE * NUM_CHUNKS];
extern mem_info allocation_list[NUM_CHUNKS];

size_t size_to_chunks(size_t bytes);
void dump_memory();

#endif
