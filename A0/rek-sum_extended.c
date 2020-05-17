#include <stdio.h>
#include <stdlib.h>

int sum_n(int);

int main(int argc, char const *argv[]) {
  if(argc > 2){
    printf("\e[31mFehler\e[m: Mehr als ein Argument eingegeben.\n");
    return 1;
  }else if (argc == 1) {
    printf("\e[31mFehler\e[m: Keine Obergrenze angegeben.\n");
    return 1;
  }

  if (strtoll(argv[1], NULL, 10) < 0) {
    printf("\e[31mFehler\e[m: Negative Zahl als Obergrenze angegeben.\n");
    return 1;
  }

    printf("\n\e[32mSumme\e[m von 1 bis \e[33m%s\e[m: %i.\n", argv[1], sum_n(strtoll(argv[1], NULL, 10)));
    return 0;
}

int sum_n(int n) {
  while (n > 0) {
    n += sum_n(n-1) ;
    break;
  }
  return n;
}
