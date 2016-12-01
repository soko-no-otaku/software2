#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 75
#define HEIGHT 40

const double G = 1.0;  // gravity constant

struct star
{
  double m;   // mass
  double x;   // position_x
  double y;   // position_y
  double vx;  // velocity_x
  double vy;  // velocity_y
};

struct star stars[] = {
  { 10.0,  0.0, 0.0, 0.0,  0.0 },
  {  1.0, 10.0, 0.0, 0.0,  1.0 },
  {  0.1, 20.0, 5.0, 0.2, -0.2 },
};

const int nstars = sizeof(stars) / sizeof(struct star);

void plot_stars(FILE *fp, const double t)
{
  int i;
  char space[WIDTH][HEIGHT];

  memset(space, ' ', sizeof(space));
  for (i = 0; i < nstars; i++) {
    const int x = WIDTH  / 2 + stars[i].x;
    const int y = HEIGHT / 2 + stars[i].y;
    if (x < 0 || x >= WIDTH)  continue;
    if (y < 0 || y >= HEIGHT) continue;
    char c = 'o';
    if (stars[i].m >= 1.0) c = 'O';
    space[x][y] = c;
  }

  int x, y;
  fprintf(fp, "----------\n");
  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++)
      fputc(space[x][y], fp);
    fputc('\n', fp);
  }
  fflush(fp);

  printf("t = %5.1f", t);
  for (i = 0; i < nstars; i++)
    printf(", stars[%d].x = %7.2f, stars[%d].y = %7.2f", i, stars[i].x, i, stars[i].y);
  printf("\n");
}

void update_velocities(const double dt)
{
  int i, j;
  for (i = 0; i < nstars; i++) {
    double ax, ay;
    ax = 0; ay = 0;
    for (j = 0; j < nstars; j++) {
      if (j != i) {
        double dr = sqrt(pow(stars[j].x - stars[i].x, 2) + pow(stars[j].y - stars[i].y, 2));
        ax += G * stars[j].m * (stars[j].x - stars[i].x) / pow(dr, 3);
        ay += G * stars[j].m * (stars[j].y - stars[i].y) / pow(dr, 3);
      }
    }
    stars[i].vx += ax * dt;
    stars[i].vy += ay * dt;
  }
}

void update_positions(const double dt)
{
  int i;
  for (i = 0; i < nstars; i++) {
    stars[i].x += stars[i].vx * dt;
    stars[i].y += stars[i].vy * dt;
  }
}

int main(int argc, char *argv[])
{
  const char *filename = "space.txt";
  FILE *fp;
  if ((fp = fopen(filename, "a")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", filename);
    return 1;
  }

  double dt = atof(argv[1]);
  const double stop_time = 400;

  int i;
  double t;
  for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
    update_velocities(dt);
    update_positions(dt);
    if (i % 10 == 0) {
      plot_stars(fp, t);
      usleep(200 * 1000);
    }
  }

  fclose(fp);

  return 0;
}
