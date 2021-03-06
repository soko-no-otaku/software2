#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define HEIGHT 200
#define WIDTH 200

int cell[HEIGHT][WIDTH];

void init_cells()
{
  int i, j;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = 0;
    }
  }

  cell[100][100] = 1;
}

void print_cells(FILE *fp)
{
  int i, j;

  fprintf(fp, "----------\n");

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      const char c = (cell[i][j] == 1) ? '#' : ' ';
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);

  sleep(1);
}

int count_adjacent_cells(int i, int j)
{
  int n = 0;
  int k, l;
  for (k = i - 1; k <= i + 1; k++) {
    if (k < 0 || k >= HEIGHT) continue;
    for (l = j - 1; l <= j + 1; l++) {
      if (k == i && l == j) continue;
      if (k != i && l != j) continue;
      if (l < 0 || l >= WIDTH) continue;
      n += cell[k][l];
    }
  }
  return n;
}

void update_cells()
{
  int i, j;
  int cell_next[HEIGHT][WIDTH];

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell_next[i][j] = 0;
      const int n = count_adjacent_cells(i, j);
      if (n == 1) {
        cell_next[i][j] = 1;
      } else {
        cell_next[i][j] = 0;
      }
    }
  }

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = cell_next[i][j];
    }
  }
}

int count_population() {
  int population = 0;
  int i, j;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      population += cell[i][j];
    }
  }
  return population;
}

int main()
{
  int gen;
  FILE *fp;

  if ((fp = fopen("cells.txt", "a")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }

  init_cells();
  print_cells(fp);

  for (gen = 1;; gen++) {
    printf("generation = %d, ", gen - 1);
    printf("population = %d\n", count_population());
    update_cells();
    print_cells(fp);
  }

  fclose(fp);

  return 0;
}
