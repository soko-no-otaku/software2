#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];

void init_cells()
{
  int i, j;

  srand(time(NULL));
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      cell[i][j] = rand() % 3;
    }
  }
}

void print_cells(FILE *fp)
{
  int i, j;

  fprintf(fp, "----------\n");

  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      char c;
      switch (cell[i][j]) {
        case 0:
          c = ' ';
          break;
        case 1:
          c = '.';
          break;
        case 2:
          c = '*';
          break;
      }
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);

  sleep(1);
}

int count_adjacent_children(int i, int j)
{
  int n = 0;
  int k, l;
  for (k = i - 1; k <= i + 1; k++) {
    if (k < 0 || k >= HEIGHT) continue;
    for (l = j - 1; l <= j + 1; l++) {
      if (k == i && l == j) continue;
      if (l < 0 || l >= WIDTH) continue;
      if (cell[k][l] == 1) n++;
    }
  }
  return n;
}

int count_adjacent_adults(int i, int j)
{
  int n = 0;
  int k, l;
  for (k = i - 1; k <= i + 1; k++) {
    if (k < 0 || k >= HEIGHT) continue;
    for (l = j - 1; l <= j + 1; l++) {
      if (k == i && l == j) continue;
      if (l < 0 || l >= WIDTH) continue;
      if (cell[k][l] == 2) n++;
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
      const int nc = count_adjacent_children(i, j);
      const int na = count_adjacent_adults(i, j);
      switch (cell[i][j]) {
        case 0:
          if (na == 2) {
            cell_next[i][j] = 1;
          } else {
            cell_next[i][j] = 0;
          }
          break;
        case 1:
          if (nc > 0 && (na == 2 || na == 3)) {
            cell_next[i][j] = 2;
          } else if (na == 2 || na == 3) {
            cell_next[i][j] = 1;
          } else {
            cell_next[i][j] = 0;
          }
          break;
        case 2:
          if (nc > 0) {
            cell_next[i][j] = 2;
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

  if ((fp = fopen("cells.txt", "a")) == NULL) {
    fprintf(stderr, "error: cannot open a file.\n");
    return 1;
  }

  init_cells();
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
