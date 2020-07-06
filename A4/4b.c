#include "4b.h"
#include <stdio.h>

void nf_free(void *addr)
{
	// Diese Funktion sollt ihr implementieren


	/*
	 * chunk_index ist der Index des Chunks, in dem addr liegt.
	 */
	size_t chunk_index = ((char*)addr - heap) / CHUNK_SIZE;
}
