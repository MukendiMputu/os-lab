#include <stdio.h>
#include <pthread.h>
#include <errno.h>

// Allgemeine Fehlerbehandlung
#define handle_error(msg) \
              do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Fehlerbehandlung mit Statuscode
#define handle_error_en(en, msg) \
              do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char const *argv[])
{



  return 0;
}
