#include <stdio.h>

typedef struct
{
  char *loc;   // location name
  double alt;  // altitude (km)
  double temp; // temperature (centigrade)
  double snow; // amount of snowfall (m)
} Sample;

const Sample data[] = {
  { "Nozawa-onsen",   0.576,  22.3, 4.06 },
  { "Iiyama",         0.313,  23.3, 3.17 },
  { "Nagano",         0.4182, 23.8, 0.98 },
  { "Oomachi",        0.784,  21.1, 1.80 },
  { "Sugadaira",      1.253,  18.5, 1.93 },
  { "Karuizawa",      0.9991, 19.5, 0.36 },
  { "Matsumoto",      0.610,  23.6, 0.28 },
//  { "Nagawa",         1.068,  19.7 },
  { "Suwa",           0.7601, 22.7, 0.38 },
//  { "Nobeyama",       1.350,  18.4 },
//  { "Ina",            0.633,  22.7 },
//  { "Nagiso",         0.560,  22.3 },
  { "Iida",           0.5164, 23.9, 0.23 }
//  { "Minami-shinano", 0.407,  23.5 }
};

const int nsamples = sizeof(data) / sizeof(data[0]);

int f_dimension()
{
  return 3;
}

double f_value(const double x[])
{
  double E = 0;
  for (int i = 0; i < nsamples; i++) {
    E += (data[i].temp - (x[0] * data[i].alt + x[1] * data[i].snow + x[2]))
      * (data[i].temp - (x[0] * data[i].alt + x[1] * data[i].snow + x[2]));
  }
  return E;
}

void f_gradient(const double x[], double g[])
{
  g[0] = 0;
  g[1] = 0;
  g[2] = 0;
  for (int i = 0; i < nsamples; i++) {
    g[0] += -2 * (data[i].temp - (x[0] * data[i].alt + x[1] * data[i].snow + x[2])) * data[i].alt;
    g[1] += -2 * (data[i].temp - (x[0] * data[i].alt + x[1] * data[i].snow + x[2])) * data[i].snow;
    g[2] += -2 * (data[i].temp - (x[0] * data[i].alt + x[1] * data[i].snow + x[2]));
  }
}