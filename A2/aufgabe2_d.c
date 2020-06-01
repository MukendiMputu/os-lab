#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

/*
 * Mukendi Mputu , Tobias Weimann , Ahmad Siar Latifi
 */


#define NUM_THREADS 1
#define ANZ_EINGAENGE 1
#define SLEEP_TIME 3


// Allgemeine Fehlerbehandlung der Systemaufrufe
#define handle_error(msg) \
              do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Fehlerbehandlung der Systemaufrufe mit Statuscode
#define handle_error_en(en, msg) \
              do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// Datenstruktur für die Thread Infos
struct thread_info {    /* Used as argument to thread_start() */
   pthread_t thread_id;        /* ID returned by pthread_create() */
   int       thread_num;       /* Application-defined thread # */
};

int status;
pthread_mutex_t lock;
pthread_cond_t canEnter;

static void *bezahlvorgang(void *);
static void *zutritt_gewaehren(void *);


int main(int argc, char const *argv[])
{
  int shared_kunden_anzahl = 42, tnum;
  struct thread_info *tinfo;
  pthread_t eingang[ANZ_EINGAENGE];
  pthread_attr_t attr;

  /* Initialisiert default Attribute für pthread_create */
  status = pthread_attr_init(&attr);
  if (status != 0)
     handle_error_en(status, "pthread_attr_init");

  pthread_cond_init(&canEnter, NULL);
  pthread_mutex_init(&lock, NULL);
  // alloziiert Speicherplatz für NUM_THREADS Threads von je sizeof
  // (struct thread_info) Bytes
  tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
  if (tinfo == NULL)
    handle_error("calloc");

  printf("\nKunden im Supermarkt: \e[32m%d\e[m.\n\n", shared_kunden_anzahl);

  // Threads für die 2 Eingaenge werden hier kreeiert
  for (size_t i = 0; i < ANZ_EINGAENGE; i++) {
    status = pthread_create(&eingang[i], &attr, &zutritt_gewaehren, &shared_kunden_anzahl);
    if (status != 0)
    handle_error_en(status, "pthread_create");
  }

   /* Für je eine Kasse ein Thread wird erzeugt */
   for (tnum = 0; tnum < NUM_THREADS; tnum++) {
       tinfo[tnum].thread_num = tnum + 1;

       status = pthread_create(&tinfo[tnum].thread_id, &attr,
                          &bezahlvorgang, &shared_kunden_anzahl);
       if (status != 0)
           handle_error_en(status, "pthread_create");
   }

    /* Destroy the thread attributes object */
    status = pthread_attr_destroy(&attr);
    if (status != 0)
       handle_error_en(status, "pthread_attr_destroy");

   /* warte bis alle Threads beendet sind*/
   for (tnum = 0; tnum < NUM_THREADS; tnum++) {
     status = pthread_join(tinfo[tnum].thread_id, NULL);
       if (status != 0)
          handle_error_en(status, "pthread_create");
    }

   // warten bis alle Threads für Eingaenge beendet sind
   for (size_t i = 0; i < ANZ_EINGAENGE; i++) {
     status = pthread_join(eingang[i], NULL);
     if (status != 0)
      handle_error_en(status, "pthread_create");
   }


  printf("\n\e[32m%d\e[m Kunden sind noch im Supermarkt.\n", shared_kunden_anzahl);
  pthread_mutex_destroy(&lock);
  free(tinfo);
  return 0;
}

// Routine, die von den jeweiligen Kassen (Thread) ausgeführen werden
// Da c default-mäßig pass-by-value ist, muss die Referenz übergeben werden
static void *
bezahlvorgang(void *param_anzahl_kunden)
{
  int *anzahl_kunden = param_anzahl_kunden;
  // int signalSent = 0;

  // Endlose Schleife, bis SIGKILL (CRTL-C) gesendet wurde.
  while (1) {
    // Kunden abrechnen und rausbitten
    if(pthread_mutex_lock(&lock))
      handle_error("mutex lock error");

    if (*anzahl_kunden > 0){
      printf("Bearbeite Kunden \e[32m%d\e[m ab...\n", *anzahl_kunden);
      sleep(SLEEP_TIME);
      *anzahl_kunden -= 1;
      printf("Kunde abgearbeitet und raus.\n\n");
    }

    // When 3 Kunden reingelassen werden können, sende das Signal
    if (*anzahl_kunden+3 < 42){
      status = pthread_cond_signal(&canEnter);
      if(status)
        handle_error_en(status, "pthread_cond_signal");
    }

    if(pthread_mutex_unlock(&lock))
      handle_error("mutex unlock error");

  }
  return anzahl_kunden;
}

// Routine, die von den jeweiligen Eingang-Threads ausgeführen werden
static void *
zutritt_gewaehren(void *param_anzahl_kunden)
{
  int *anzahl_kunden = param_anzahl_kunden;

  while (*anzahl_kunden > 0 ) {
    if(pthread_mutex_lock(&lock))
       handle_error("mutex lock error");

    // Warten bis das Signal gegeben wird, Kunden reinzulassen
    status = pthread_cond_wait(&canEnter, &lock);
    if(status)
      handle_error_en(status, "pthread_cond_wait");

    printf("\n[Signal empfangen]\n\n");
    // 3 neu Kunden werden reingelassen
    *anzahl_kunden += 3;
    printf("\n[Drei neu Kunden reingelassen]\n\n");

    if(pthread_mutex_unlock(&lock))
      handle_error("mutex unlock error");

    sleep(2);
  }

  return NULL;
}
