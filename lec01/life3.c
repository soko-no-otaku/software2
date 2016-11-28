#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];

void init_cells(FILE *init)
{
  int i, j;
  char c;

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      while ((c = fgetc(init)) == '\n') {
        /* 改行は読み飛ばす */
      }
      switch (c) {
        case '#':
          cell[i][j] = 1;
          break;
        case ' ':
          cell[i][j] = 0;
          break;
      }
    }
  }
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
      if (cell[i][j] == 1) {
        if (n == 2 || n == 3) {
          cell_next[i][j] = 1;
        } else {
          cell_next[i][j] = 0;
        }
      } else {
        if (n == 3) {
          cell_next[i][j] = 1;
        } else {
          cell_next[i][j] = 0;
        }
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
  FILE *init;

  if ((fp = fopen("cells.txt", "a")) == NULL) {
    fprintf(stderr, "error: cannot open cells.txt\n");
    return 1;
  }
  if ((init = fopen("init.txt", "r")) == NULL) {
    fprintf(stderr, "error: cannot open init.txt\n");
    return 1;
  }

  init_cells(init);
  print_cells(fp);

  for (gen = 1;; gen++) {
    printf("generation = %d, ", gen);
    printf("population = %d\n", count_population());
    update_cells();
    print_cells(fp);
  }

  fclose(fp);

  return 0;
}
