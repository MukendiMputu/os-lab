#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include "vorgabe.h"

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[K%d] Ich bin bereit zu kochen!\n", my_num);


	// Aufgabe a)

    //Der Koch soll MEAL_PER_CHEF mal Gerichte anrichten, bevor er seine arbeitet beendet.
    for(int i=0; i<MEAL_PER_CHEF; i++)
    {
        //Das Gericht soll aus MEAL_SIZE vielen Zutaten bestehen. Das Array mit den Informationen zu den Zutaten wird angelegt und das zu kochende Gericht wird generiert.
        ingredient_t *meal[MEAL_SIZE];
        get_meal(meal);

        //Die Zutatenkiste wird geholt (Semaphore wird belegt) und die Zutat wird verarbeitet (sleep)
        for(int j=0; j<MEAL_SIZE; j++)
        {
            sem_wait(&meal[j]->sem);
            sleep(meal[j]->time_needed);
        }
        //Nachdem alle Zutaten verarbeitet wurden, werden alle Kisten zurückgebracht (Semaphoren werden freigegeben).
        for(int k=0; k<MEAL_SIZE; k++)
        {
            sem_post(&meal[k]->sem);
        }
    }
	return NULL;
}
