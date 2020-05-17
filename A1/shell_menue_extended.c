#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


/*
 * Mukendi Mputu , Tobias Weimann , Ahmad Siar Latifi
 */

#define  EXIT_BEFEHLSLAENGE 4
#define  MAX_ANZAHL_ARGUMENTE 10
#define  MAX_ARGUMENTEN_LAENGE 10


// Explizite Definition der Hilfsfunktionen
int getline(char **lineptr, size_t *n, FILE *stream);
void clear(void);
int extractParameter(char *parameter, char *arg, char *argv[]);
char * beautifyArgVector(char *argv[]);

int main (int argc, char **argv)
{
	int status; 								// für den Exit-Status der Kindprozesse
	int wahl;										// für die User-Auswahl
	size_t len = 0;
	pid_t fork_return; 					// für die Elter-/Kindprozess Kontext Unterscheidung
	char *params = NULL;
	char *program = NULL; 			// für das auszuführende Programm
	char *eingabe = NULL; 			// für die rohe Usereingabe
	int anzahl_angegebener_params;
	char exit_cmd[EXIT_BEFEHLSLAENGE]; 	// wichtig die Länge zu praezisieren, um core dumps zu vermeiden
	char *argumente = malloc(MAX_ANZAHL_ARGUMENTE*MAX_ARGUMENTEN_LAENGE);	// für die rohe Argumenenliste
	char **temp_argv;
	char *argument_vektor[MAX_ANZAHL_ARGUMENTE];

	for (int i = 0; i < MAX_ANZAHL_ARGUMENTE; i++)
		argument_vektor[i] = malloc(MAX_ARGUMENTEN_LAENGE+1);


		// a) Einlesen der standardeingabe
	  do {
		printf("Bitte triff eine Auswahl\n\t[1=ps, 2=ls oder 3=date] : ");

		if (scanf("%d", &wahl))
		{
				clear(); // HACK um das Fehlverhalten von scanf herum.

				printf("Deine Auswahl: %d\n\n", wahl);

				switch (wahl) {
					case 1:
						program = "ps";
						argument_vektor[0] = program;
						extractParameter(params, argumente, argument_vektor);
						temp_argv = beautifyArgVector(argument_vektor);
					break;

					case 2:
						program = "ls";
						argument_vektor[0] = program;
						extractParameter(params, argumente, argument_vektor);
						beautifyArgVector(argument_vektor);
					break;

					case 3:
						program = "date";
						argument_vektor[0] = program;
						extractParameter(params, argumente, argument_vektor);
						beautifyArgVector(argument_vektor);
					break;

					default:
						printf("\n\e[31mFehler\e[m: ungültige Auswahl.\n");
						continue; // Überspring die restliche Ausführung und beginn neu Schleifenumgang
					break;
				}

				fork_return = fork();
				if (fork_return == 0) { // Code-Block des Kindprozesses
					printf("\nKindprozess PID: %i, Eltern PID: %i, Processgruppe: %i\n", getpid(), getppid(), getpgrp());
					/*char *argv[3] = {"ps", "f", NULL};
					execvp(program, &argv[0]);*/
					execvp(program, &argument_vektor[0]);
					perror("\e[31mExec Fehler\e[m: ");
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
				sleep(1);
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
	free(argumente);
	for (size_t i = 0; i < MAX_ANZAHL_ARGUMENTE; i++) {
		free(argument_vektor[i]);
	}
	return 0;
}

void clear (void)
{
	// consummiert das von scanf hinterlassene '\n'
  while ( getchar() != '\n' );
}

int extractParameter(char *parameter, char *arg, char *argv[]) {
	printf("Parameter? (default = ENTER): ");
	//scanf("%100s", argumente);
	int j; char *src;
	if (fgets(arg, sizeof(arg), stdin) != NULL){
		for (j = 1, src = arg; ; j++, src = NULL) {
			parameter = strtok(src, " \n");

			if (parameter == NULL)
				break;

			strncpy(argv[j], parameter, MAX_ARGUMENTEN_LAENGE);
		}

	}else {
		perror("\e[31mfgets Fehler\e[m: ");
		exit(1);
	}
	return j;
}

// Der ArgVektor muss ein NULL am Ende enthalten um das Ende der Parameterliste
// zu signalisieren.
char * beautifyArgVector(char *argv[]){
	int i = 0;
	while (argv[i] != NULL)
		i++;
	argv[i] = NULL;
	char *tempVect[i];
	for (size_t j = 0; j < i; j++) {
		strncpy(tempVect[j], argv[j], sizeof(argv[i]));
	}
	return &tempVect;
}
