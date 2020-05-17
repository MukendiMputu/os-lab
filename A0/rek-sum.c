#include <stdio.h>

int sum_n(int);

char glob_uninit_char;
char glob_init_char = 'a';
short glob_uninit_short;
short glob_init_short = 0;
int glob_uninit_int;
int glob_init_int = 0;
long glob_uninit_long;
long glob_init_long = 1L;
double glob_uninit_double;
double glob_init_double = 1.0;



int main(int argc, char const *argv[]) {
  //printf("\nSumme von 1 bi %i: %i.\n", 5, sum_n(5));
  //printf("\nSumme von 1 bi %i: %i.\n", 50, sum_n(50));
  //printf("\nSumme von 1 bi %i: %i.\n", 500, sum_n(500));
  //printf("\nSumme von 1 bi %i: %i.\n", 5000, sum_n(5000));
  printf("\nSumme von 1 bi %i : %i.\n\n", 261829, sum_n(5));

  printf("Die Adressen der globalen Variable sind:\n");
  printf("\tglob_uninit_\e[32mchar\e[m: \e[32m%p\e[m\n", &glob_uninit_char);
  printf("\tglob_init_\e[32mchar\e[m: \e[32m%p\e[m\n", &glob_init_char);
  printf("\tglob_uninit_\e[33mshort\e[m: \e[33m%p\e[m\n", &glob_uninit_short);
  printf("\tglob_init_\e[33mshort\e[m: \e[33m%p\e[m\n", &glob_init_short);
  printf("\tglob_uninit_\e[34mint\e[m: \e[34m%p\e[m\n", &glob_uninit_int);
  printf("\tglob_init_\e[34mint\e[m: \e[34m%p\e[m\n", &glob_init_int);
  printf("\tglob_uninit_\e[35mlong\e[m: \e[35m%p\e[m\n", &glob_uninit_long);
  printf("\tglob_init_\e[35mlong\e[m: \e[35m%p\e[m\n", &glob_init_long);
  printf("\tglob_uninit_\e[31mdouble\e[m: \e[31m%p\e[m\n", &glob_uninit_double);
  printf("\tglob_init_\e[31mdouble\e[m: \e[31m%p\e[m\n\n", &glob_init_double);

  char loc_uninit_char;
  char loc_init_char = 'a';
  short glob_uninit_short;
  short glob_init_short = 0;
  int loc_uninit_int;
  int loc_init_int = 0;
  int glob_init_int = 0;
  long glob_uninit_long;
  long glob_init_long = 1L;
  double glob_uninit_double;
  double glob_init_double = 1.0;

  printf("Die Adressen der localen Variable in main() sind:\n");
  printf("\tloc_uninit_\e[32mchar\e[m: \e[32m%p\e[m\n", &loc_uninit_char);
  printf("\tloc_init_\e[32mchar\e[m: \e[32m%p\e[m\n", &loc_init_char);
  printf("\tloc_uninit_\e[33mshort\e[m: \e[33m%p\e[m\n", &glob_uninit_short);
  printf("\tloc_init_\e[33mshort\e[m: \e[33m%p\e[m\n", &glob_init_short);
  printf("\tloc_uninit_\e[34mint\e[m: \e[34m%p\e[m\n", &loc_uninit_int);
  printf("\tloc_init_\e[34mint\e[m: \e[34m%p\e[m\n", &loc_init_int);
  printf("\tloc_uninit_\e[35mlong\e[m: \e[35m%p\e[m\n", &glob_uninit_long);
  printf("\tloc_init_\e[35mlong\e[m: \e[35m%p\e[m\n", &glob_init_long);
  printf("\tloc_uninit_\e[31mdouble\e[m: \e[31m%p\e[m\n", &glob_uninit_double);
  printf("\tloc_init_\e[31mdouble\e[m: \e[31m%p\e[m\n", &glob_init_double);


  return 0;
}

int sum_n(int n) {
  int i = n;
    printf("Adresse von n: %p\n", &n);
  while (i > 0) {
    i += sum_n(i-1) ;
    printf("Adresse von i: %p\n", &i);
    break;
  }
  return i;
}

// Farben codes: 30=black, 31=red, 32=green, 33=yellow, 34=blue, 35=magenta, 36=cyan, 37=white

// 1. Ab dem Wert n = 261830 bekommt man ein core dumped, da das Programm versucht
// in einen Adressbereich zu schreiben, der ihm gar nicht zugeteilt wurde.
// Dies kommt zustande, weil der Stack für die Programmvariablen durch die Rekursion
// irgendwann voll wird.

// 2. Die Speicheradressen von zwei in der main() nacheinander angelegten Variablen
// vom Typ double haben einen Abstand von 8 Bytes. Dieser Abstand entsteht dadurch,
// dass beim Anlegen der ersten Variable, einen Speicherplatz von 8 Bytes ((32 Bits)
// - Systemabhängige Länge einer double Variable) reserviert wird. Und beim Anlegen
// der zweite wird dort fortgeführt, wo die erste Reservierung aufhörte.

// 3. Abhängig vom Speicherlayout landen global initialisierten Variablen auf dem
// Block-Storage-Segment oder auf dem Datasegment, welche sich im Bereich der niedrigen
// Adressen befinden. Und die localen Variablen landen auf dem Programmstack im
// Bereich der höheren Adressen.

// 4. Weil der Stackframe eines Programms in Richtung der niedrigen Adressen wächst,
// wird die Adresse einer localen Variable, die sich auf diesem Stack befindet, mit
// jedem rekursiven Aufruf neu angelegt, aber weiter tiefer (Richtung niedriger Adressen)
// im Programmstack.
// [In einigen Systemen - wie in meinem - wächst der Stack in die andere Richtung
// (gen höheren Speicheradressen), um Exploits (Stackoverflow) zu verhindern. ]
