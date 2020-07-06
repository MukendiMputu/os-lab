#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "vorgabe.h"

void *work(void *arg)
{
	int my_num = *((int *)arg);

	printf("[K%d] Ich bin bereit zu kochen!\n", my_num);

    //Aufgabe b)

    for(int i=0; i<MEAL_PER_CHEF; i++)
    {
        //Zunächst wird der Kühlschrank reserviert (der kritische Bereich wird durch mutex lock gesperrt).
        pthread_mutex_init(&lock, NULL);
        pthread_mutex_lock(&lock);

        //Das Gericht soll aus MEAL_SIZE vielen Zutaten bestehen. Das Array mit den Informationen zu den Zutaten wird angelegt und das zu kochende Gericht wird generiert.
        ingredient_t *meal[MEAL_SIZE];
        get_meal(meal);

        //Der Koch holt sich alle benötigten Kisten direkt nacheinander.
        for(int j=0; j<MEAL_SIZE; j++)
        {
            sem_wait(&meal[j]->sem);
        }

        //Dannach wird der Kühlschrank wieder freigegeben (mutex unlock).
        pthread_mutex_unlock(&lock);

        //Die Zutaten werden verarbeitet.
        for(int k=0; k<MEAL_SIZE; k++)
        {
            sleep(meal[k]->time_needed);
        }
        //Schließlich werden die Kisten zurückgebracht (Semaphoren werden freigegeben).
        for(int l=0; l<MEAL_SIZE; l++)
        {
            sem_post(&meal[l]->sem);
        }
    }
	return NULL;
}
