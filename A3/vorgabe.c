#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "vorgabe.h"

ingredient_t ingredients[INGREDIENT_NUM] = {{"Zwiebeln", 3, 3}, {"Möhren", 2, 2}, {"Nudeln", 2, 2}, {"Kartoffeln", 4, 2}, {"Gemüsebrühe", 1, 4}};
pthread_mutex_t lock;
volatile int restart;
pthread_t chefs[CHEF_NUM];

// Funktion, die ein zufälliges Gericht generiert und im als Argument
// angegebenen (d.h. von euch mit der richtigen Länge anzulegenden!)
// Array speichert
void get_meal(ingredient_t * meal_buffer[])
{
	ingredient_t *ingredients_left[INGREDIENT_NUM];
	for (int i = 0; i < INGREDIENT_NUM; i++){
		ingredients_left[i] = &(ingredients[i]);
	}

	for (int i = 0; i < MEAL_SIZE; i++){
		int rand_index = rand() % (INGREDIENT_NUM - i);
		meal_buffer[i] = ingredients_left[rand_index];
		for (int j = rand_index; j < INGREDIENT_NUM - i - 1; j++){
			ingredients_left[j] = ingredients_left[j + 1];
		}
	}
}

int main(void)
{
	srand(time(NULL));	// Zufallszahlengenerator initialisieren

	restart = 1;

	while (restart) {
		restart = 0;

		if (pthread_mutex_init(&lock, NULL)) {
			fprintf(stderr, "pthread_mutex_init failed");
			exit(1);
		}

		// Initialisieren der Semaphoren
		for (int i = 0; i < INGREDIENT_NUM; i++) {
        	if (sem_init(&(ingredients[i].sem), 0, ingredients[i].boxes)) {
            	fprintf(stderr, "sem_init failed");
            	exit(1);
        	}
    	}
		printf("[ ! ] Semaphoren initialisiert.\n");

		// Nummernvariablen zur Übergabe an die Threads anlegen
		int chef_nums[CHEF_NUM];
		for (int i = 0; i < CHEF_NUM; i++) {
			chef_nums[i] = i;
		}

		// Koch-Threads anlegen
		for (int i = 0; i < CHEF_NUM; i++) {
			if (pthread_create(&chefs[i], NULL, &work, &(chef_nums[i]))){
            	fprintf(stderr, "pthread_create failed");
				exit(1);
			}
		}

		// Koch-Threads einsammeln
		for (int i = 0; i < CHEF_NUM; i++) {
			if (pthread_join(chefs[i], NULL)) {
            	fprintf(stderr, "pthread_join failed");
				exit(1);
			}
		}

		printf("[ ! ] Alle Köche beendet.\n");

		if (pthread_mutex_destroy(&lock)) {
            fprintf(stderr, "pthread_mutex_destroy failed");
			exit(1);
		}

		//Zerstören der Semaphoren
		for (int i = 0; i < INGREDIENT_NUM; i++) {
        	if (sem_destroy(&(ingredients[i].sem))) {
				fprintf(stderr, "sem_destroy failed");
            	exit(1);
        	}
   		}

		printf("[ ! ] Semaphoren zerstört.\n");
	}

	printf("[!!!] Beende Programm.\n");

	return 0;
}
