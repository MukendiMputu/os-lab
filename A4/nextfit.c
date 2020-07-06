#include <stdio.h>
#include <string.h>
#include <err.h>

#include "nextfit.h"

// Der Hauptspeicher
char heap[CHUNK_SIZE * NUM_CHUNKS];

/*
 * Die Allokationsliste gibt fuer den ersten Chunk jedes zusammenhaengenden
 * Speicherbereichs an, wie groß er ist (d.h. wie viele Chunks er umfasst)
 * und ob er reserviert oder frei ist. Zu Beginn ist der komplette
 * Speicher frei.
 */
mem_info allocation_list[NUM_CHUNKS] = {{CHUNK_FREE, NUM_CHUNKS}};

size_t size_to_chunks(size_t bytes)
{
	if ((bytes % CHUNK_SIZE) != 0)
		return bytes / CHUNK_SIZE + 1;
	return bytes / CHUNK_SIZE;
}

void dump_memory()
{
	size_t in_range = 0;
	size_t free_chunks = 0;
	unsigned char status = 0;
	putchar('|');
	for (size_t i = 0; i < NUM_CHUNKS; i++) {
		if (allocation_list[i].status > 1) {
			printf("\n [!] Konsistenzfehler in der Speicherverwaltung: "
					"allocation_list[%zu].status == %d != 0/1\n", i, allocation_list[i].status);
			exit(EXIT_FAILURE);
		}
		if (in_range && allocation_list[i].length) {
			printf("\n [!] Konsistenzfehler in der Speicherverwaltung: "
					"allocation_list[%zu].length == %zu != 0\n", i, allocation_list[i].length);
			exit(EXIT_FAILURE);
		}
		if (allocation_list[i].length) {
			if (i > 0 && status == CHUNK_FREE && status == allocation_list[i].status) {
				printf("\n [!] Konsistenzfehler in der Speicherverwaltung: "
						"allocation_list[%zu] ist frei und wurde nicht mit dem "
						"vorherigen (auch freien) Speicherbereich vereinigt\n", i);
			}
			status = allocation_list[i].status;
			in_range = allocation_list[i].length;
		} else if (!in_range) {
			printf("\n [!] Konsistenzfehler in der Speicherverwaltung: "
					"allocation_list[%zu].length == 0, hier muss aber ein neuer Bereich beginnen\n", i);
			exit(EXIT_FAILURE);
		}
		if (status == CHUNK_ALLOCATED) {
			putchar('#');
		} else {
			putchar('.');
			free_chunks++;
		}
		in_range--;
	}
	putchar('|');
	printf(" (%3zu KiB frei)\n", free_chunks * 4);
}
