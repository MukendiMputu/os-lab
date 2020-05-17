#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


/*
 * Mukendi Mputu (190729), Tobias Weimann (210070), Ahmad Siar Latifi (222962)
 */

#define  EXIT_BEFEHLSLAENGE 4

// Explizite Definition der Funktion getline
int getline(char **lineptr, size_t *n, FILE *stream);

int main (int argc, char **argv)
{
	int status; 								// für den Exit-Status der Kindprozesse
	pid_t fork_return; 					// für die Elter-/Kindprozess Kontext Unterscheidung
	int wahl;										// für die User-Auswahl
	char *program = NULL; 			// für das auszuführende Programm
	char *eingabe = NULL; 			// für die rohe Usereingabe
	char exit_cmd[EXIT_BEFEHLSLAENGE]; 					// es ist sehr wichtig die Länge zu bestimmen, um core dumps zu vermeiden
	size_t len = 0;

		// a) Einlesen der standardeingabe
	  do {
		printf("Bitte triff eine Auswahl\n\t[1=ps, 2=ls oder 3=date] : ");

		if (scanf("%d", &wahl))
		{
				printf("Deine Auswahl: %d\n\n", wahl);

				switch (wahl) {
					case 1:
						program = "ps";
					break;

					case 2:
						program = "ls";
					break;

					case 3:
						program = "date";
					break;

					default:
						printf("\n\e[31mFehler\e[m: ungültige Auswahl.\n");
						continue; // Überspring die restliche Ausführung und beginn neu Schleifenumgang
					break;
				}

				fork_return = fork();
				if (fork_return == 0) { // Code-Block des Kindprozesses
					printf("\nKindprozess PID: %i, Eltern PID: %i, Processgruppe: %i\n", getpid(), getppid(), getpgrp());
					execlp(program, program, NULL);
					perror("\e[31mFehler\e[m: exec");
				  exit(1);

				} else if(fork_return > 0) { // Code-Block des Elternprozesses
					// Kein Kindprozess bleibt zurück! Warte auf alle!
					wait(&status);
					program = NULL; // 'Reseten' des Buffers für die nächste Ausführung
					printf("\nElternprozess (%i): Mein Kind ist mit dem Status ", getppid());
					if (status) {
						printf("\e[31m%i\e[m zurückgekehrt.\n\n", status);
					} else {
						printf("\e[32m%i\e[m zurückgekehrt.\n\n", status);
					}

				} else {
					printf("Ein Fehler ist beim Forken aufgetreten.\n");
				}

		} else if (scanf("%4s", exit_cmd)) {
			if (strcmp(exit_cmd, "exit") == 0) {
				printf("Beenden...\n");
				sleep(2);
				return 0;
			} else {
				printf("\n\e[31mFehler\e[m: ungültiger Befehl.\n\n");
			}
		} else {
			printf("Wahl is: %d, exit_cmd is: %s\n", wahl, exit_cmd);
			printf("\e[31mFehler\e[m: ungültiges Parameter.\n\n");
		}

		// Blocken solange von der standardeingabe gelesen wird
	} while(getline(&eingabe, &len, stdin) != -1);

	free(eingabe);
	free(program);
	return 0;
}
