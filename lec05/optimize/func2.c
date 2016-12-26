#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "func1.h"

int main(const int argc, const char **argv)
{
  const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

  int i;
  const int dim = f_dimension();

  double *x = malloc(dim * sizeof(double));
  for (i = 0; i < dim; i++) {
    x[i] = 0;
  }

  printf("alpha = %f\n", alpha);

  const int iter = optimize(alpha, dim, x, f_gradient, f_value);

  printf("number of iterations = %d\n", iter);
  putchar('\n');
  printf("estimated average temperature in July at the top of Mt. Fuji\n");
  printf("= %4.2f degrees centigrade\n", x[0] * 3.776 + x[1]);

  free(x);

  return 0;
}
