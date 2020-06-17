#ifndef __VORGABE_H__
#define __VORGABE_H__

#include <semaphore.h>

#define INGREDIENT_NUM 5	// Anzahl der verschiedenen Zutaten.
#define CHEF_NUM 6		// Anzahl der Köche.
#define MEAL_PER_CHEF 2	// Anzahl der Gerichte, die ein Koch zubereitet, bevor er sich beendet.
#define MEAL_SIZE 4		// Anzahl der Zutaten eines Gerichts.

typedef struct ingredient {	// Definiere Struktur für Zutaten mit "ingredient_t" als Alias.
	char name[256];		// Der Name dieser Zutat.
	int time_needed;	// Die Zeit in Sekunden, die zur Verarbeitung dieser Zutat benötigt wird.
	int boxes;		// Die anfängliche Anzahl der vorhandenen Kisten dieser Zutat.
	sem_t sem;		// Die noch verfügbare Kistenanzahl als Semaphore.
} ingredient_t;

// Vorgegebene Zutaten
extern ingredient_t ingredients[INGREDIENT_NUM];

// Funktion, die ein zufälliges Gericht generiert und im als Argument
// angegebenen (d.h. von euch mit der richtigen Länge anzulegenden!)
// Array speichert
void get_meal(ingredient_t * meal_buffer[]);

extern pthread_mutex_t lock;

extern volatile int restart;

extern pthread_t chefs[CHEF_NUM];

// a3_a.c / a3_b.c / a3_c.c

void *work(void *arg);

#endif
