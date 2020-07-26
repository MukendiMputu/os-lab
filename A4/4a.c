#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "4a.h"


char yet_allocated = 0;

void *nf_alloc(size_t size)
{
  // Der index soll ueber mehrere Funktionsaufrufe erhalten bleiben
  static size_t chunk_index = 0;
  size_t i = 0;
   if ( size ) {
     for (i = chunk_index; i <= NUM_CHUNKS; i++) {
       // if ((i == NUM_CHUNKS) && !yet_allocated)
       //   i = 0;

       // Wenn eine Lücke entdeckt wird...
       if (allocation_list[i].status == CHUNK_FREE) {

         // und wenn sie gross genug ist, beleg diese und aktualisiere status
          if ( size_to_chunks(size) <= allocation_list[i].length ) {
            allocation_list[i].status = CHUNK_ALLOCATED;
            allocation_list[i].length = size_to_chunks(size);
            //yet_allocated = 1;
            chunk_index = i;
            break;
          }

        } else if (allocation_list[i].status == CHUNK_ALLOCATED) {
            i += allocation_list[i].length;
            // if (i == chunk_index) {
            //
            // }
        }
     }

  	/*
  	 * Gibt die Startadresse des belegten Speicher zurück.
  	 */
  	return heap + chunk_index * CHUNK_SIZE;
  } else {
    return NULL;
  }

}
