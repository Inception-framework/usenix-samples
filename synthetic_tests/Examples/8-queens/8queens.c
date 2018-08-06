#include <stdio.h>

#define N 4

int scacchiera[N][N];

int num_sol = 0;

void powerset(int r, int c, int q);

int controlla();

void stampa();

int main() {
  __asm volatile("nop");
  int i, j;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      scacchiera[i][j] = 0;
  powerset(0, 0, 0);
#ifdef KLEE
  printf("Numero di soluzioni trovate: %d \n", num_sol);
#endif
}
void powerset(int r, int c, int q) {
  if (c >= N) {
    c = 0;
    r++;
  }
  if (r >= N) { // scacchiera finita !
    if (q != N)
      return;
    else if (controlla()) {
      num_sol++;
      stampa();
    }
    return;
  }
  // prova a mettere la regina su r,c
  scacchiera[r][c] = q + 1;
  // ricorri
  powerset(r, c + 1, q + 1);
  /* backtrack */
  scacchiera[r][c] = 0;
  // ricorri senza la regina su r,c
  powerset(r, c + 1, q);
  return;
}
int controlla(void) {
  __asm volatile("nop");
  int r, c, d, n;
  // controlla righe
  for (r = 0; r < N; r++) {
    for (c = n = 0; c < N; c++) {
      if (scacchiera[r][c] != 0)
        n++;
    }
    if (n > 1)
      return 0;
  }
  // controlla colonne
  for (c = 0; c < N; c++) {
    for (r = n = 0; r < N; r++) {
      if (scacchiera[r][c] != 0)
        n++;
    }
    if (n > 1)
      return 0;
  }
  // controlla diagonali
  for (d = 0; d < 2 * N - 1; d++) {
    n = 0;
    for (r = 0; r < N; r++) {
      c = d - r;
      if ((c >= 0) && (c < N))
        if (scacchiera[r][c] != 0)
          n++;
    }
    if (n > 1)
      return 0;
  }
  // controlla antidiagonali diagonali
  for (d = 0; d < 2 * N - 1; d++) {
    n = 0;
    for (r = 0; r < N; r++) {
      c = r - d + N - 1;
      if ((c >= 0) && (c < N))
        if (scacchiera[r][c] != 0)
          n++;
    }
    if (n > 1)
      return 0;
  }
  return 1;
}

void stampa() {
  int r, c;
#ifdef KLEE
  printf("\nsoluzione %d\n", num_sol);
  for (r = 0; r < N; r++) {
    for (c = 0; c < N; c++)
      if (scacchiera[r][c] != 0)
        printf("Q");
      else
        printf("*");
    printf("\n");
  }
#endif
}
