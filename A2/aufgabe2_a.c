#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

/*
 * Mukendi Mputu , Tobias Weimann , Ahmad Siar Latifi
 */


#define NUM_THREADS 5
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


// Routine, die von den jeweiligen Kassen (Thread) ausgeführen werden
// Da c default-mäßig pass-by-value ist, muss die Referenz übergeben werden
static void *
bezahlvorgang(void *param_anzahl_kunden)
{
  int *anzahl_kunden = param_anzahl_kunden;

  while ( *anzahl_kunden > 0 ) {
    // Kunden abrechnen und rausbitten
    printf("Bearbeite Kunden \e[32m%d\e[m ab...\n", *anzahl_kunden);
    *anzahl_kunden -= 1;
    sleep(SLEEP_TIME);
    printf("Kunde abgearbeitet und raus.\n\n");
  }

  return anzahl_kunden;
}

int main(int argc, char const *argv[])
{
  int shared_kunden_anzahl = 42, status;
  struct thread_info *tinfo;
  pthread_attr_t attr;
  int tnum;

  /* Initialize thread creation attributes */
  status = pthread_attr_init(&attr);
  if (status != 0)
     handle_error_en(status, "pthread_attr_init");


  // alloziiert Speicherplatz für NUM_THREADS Threads von je sizeof
  // (struct thread_info) Bytes
  tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
  if (tinfo == NULL)
    handle_error("calloc");

  printf("\nKunden im Supermarkt: \e[32m%d\e[m.\n\n", shared_kunden_anzahl);

   /* Für je eine Kasse ein Thread wird erzeugt */
   for (tnum = 0; tnum < NUM_THREADS; tnum++) {
       tinfo[tnum].thread_num = tnum + 1;

       // pthread_create() speichert die Thread-ID in die entspreched Variable
       // tinfo[tnum].thread_id
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


  printf("\n\e[32m%d\e[m Kunden sind noch im Supermarkt.\n", shared_kunden_anzahl);

  free(tinfo);
  return 0;
}
