#include <stdlib.h>
#include <stdio.h>
// Zusätzliche Header-Dateien hier!

#include "vorgabe.h"

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[K%d] Ich bin bereit zu kochen!\n", my_num);

	// *** HIER EUER CODE ***

	return NULL;
}
