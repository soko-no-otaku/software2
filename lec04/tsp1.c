#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <float.h>

#define WIDTH 70
#define HEIGHT 40
#define MAX_CITIES 1000
#define N 1000 // number of trials

typedef struct
{
  int x;
  int y;
} City;

City city[MAX_CITIES];
char map[WIDTH][HEIGHT];

int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

void draw_line(const int x0, const int y0, const int x1, const int y1)
{
  int i;
  const int n = max(abs(x1 - x0), abs(y1 - y0)) + 1;
  for (i = 0; i <= n; i++) {
    const int x = x0 + i * (x1 - x0) / n;
    const int y = y0 + i * (y1 - y0) / n;
    if (map[x][y] == ' ') map[x][y] = '*';
  }
}

void draw_route(const int n, const int *route)
{
  if (route == NULL) return;

  int i;
  for (i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = (i < n - 1) ? route[i+1] : route[0];
    draw_line(city[c0].x, city[c0].y, city[c1].x, city[c1].y);
  }
}

void plot_cities(FILE *fp, const int n, const int *route)
{
  int i, j, x, y;

  fprintf(fp, "----------\n");

  memset(map, ' ', sizeof(map));

  for (i = 0; i < n; i++) {
    char buf[100];
    sprintf(buf, "C_%d", i);
    for (j = 0; j < strlen(buf); j++) {
      const int x = city[i].x + j;
      const int y = city[i].y;
      map[x][y] = buf[j];
    }
  }

  draw_route(n, route);

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      const char c = map[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }

  fflush(fp);
}

double distance(const int i, const int j)
{
  const double dx = city[i].x - city[j].x;
  const double dy = city[i].y - city[j].y;
  return sqrt(dx * dx + dy * dy);
}

double total_distance(const int n, const int route[])
{
  double sum_d = 0;
  int i;
  for (i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = (i < n - 1) ? route[i+1] : route[0];
    sum_d += distance(c0, c1);
  }
  return sum_d;
}

int visited[MAX_CITIES];
double best_total_distance;
int best_route[MAX_CITIES];

/*
void search(const int i, const int n, int route[])
{
  int j;
  if (i == n) {
    // Compute the total distance
    double td = total_distance(n, route);
    if (td < best_total_distance) {
      best_total_distance = td;
      for (j = 0; j < n; j++) {
        best_route[j] = route[j];
      }
    }
    return;
  }

  for (j = 1; j < n; j++) {
    if (visited[j] == 1) continue;
    visited[j] = 1;
    route[i] = j;
    search(i + 1, n, route);
    visited[j] = 0;
  }
}
*/

double hc(const int n, int route[], double local_best_total_distance, int local_best_route[]) {
  double current_td = total_distance(n, route);

  int j;
  int s, t;
  for (s = 0; s < n; s++) {
    for (t = 0; t < s; t++) {
      int tmp = route[s];
      route[s] = route[t];
      route[t] = tmp;

      double td = total_distance(n, route);
      if (td < local_best_total_distance) {
        local_best_total_distance = td;
        for (j = 0; j < n; j++) {
          local_best_route[j] = route[j];
        }
      }

      route[t] = route[s];
      route[s] = tmp;
    }
  }
  if (local_best_total_distance < current_td) {
    for (j = 0; j < n; j++) {
      route[j] = local_best_route[j];
    }
    return hc(n, route, local_best_total_distance, local_best_route);
  } else {
    return local_best_total_distance;
  }
}

void shuffle(const int n, int route[]) {
  for(int from = 0; from < n; from++) {
    int to = rand() % n;
    int tmp = route[to];
    route[to] = route[from];
    route[from] = tmp;
  }
}

double solve(const int n, int route[])
{
  int i;

  for (i = 0; i < n; i++) {
    route[i] = i;
  }
  best_total_distance = DBL_MAX;
  for (int trial = 0; trial < N; trial++) {
    int local_best_route[MAX_CITIES];
    shuffle(n, route);
    double local_best_total_distance = hc(n, route, DBL_MAX, local_best_route);
    if (local_best_total_distance < best_total_distance) {
      best_total_distance = local_best_total_distance;
      for (int j = 0; j < n; j++) {
        best_route[j] = local_best_route[j];
      }
    }
  }

  for (i = 0; i < n; i++) {
    route[i] = best_route[i];
  }


  // Compute the total distance
  double sum_d = 0;
  for (i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = (i < n - 1) ? route[i+1] : route[0];
    sum_d += distance(c0, c1);
  }

  return sum_d;
}

void print_route(int n, int route[]) {
  int i;
  printf("route: ");
  for (i = 0; i < n - 1; i++) {
    printf("%d-", route[i]);
  }
  printf("%d\n", route[i]);
}

int main(const int argc, const char **argv)
{
  FILE *fp;
  const char *map_file = "map.txt";
  if ((fp = fopen(map_file, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", map_file);
    return 1;
  }

  if (argc != 2) {
    fprintf(stderr, "error: please specify the number of cities.\n");
    exit(1);
  }

  const int n = atoi(argv[1]);
  assert(n > 1 && n <= MAX_CITIES);

  int i;
  for (i = 0; i < n; i++) {
    city[i].x = rand() % (WIDTH - 5);
    city[i].y = rand() % HEIGHT;
  }

  plot_cities(fp, n, NULL);
  sleep(1);

  int route[MAX_CITIES];
  const double d = solve(n, route);

  printf("total distance = %f\n", d);
  print_route(n, route);
  plot_cities(fp, n, route);

  return 0;
}
