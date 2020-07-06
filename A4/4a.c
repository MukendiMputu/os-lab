#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "4a.h"


 struct mem_info {
   int length;
   char status;
 } feld [64];

void *nf_alloc(size_t size)
{
	/*
	 * Diese Funktion sollt ihr implementieren.
	 */


	/*
	 * Der Laufindex
	 */
	 size_t chunk_index;



	/*
	 * Gibt die Startadresse des belegten Speicher zurück.
	 */
	return heap + chunk_index * CHUNK_SIZE;
}
