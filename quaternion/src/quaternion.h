#ifndef _QUATERNION_H
#define _QUATERNION_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef struct Quaternion {
  double w;
  double x;
  double y;
  double z;
  void (*init)(struct Quaternion*);
  void (*normalise)(struct Quaternion*);
  void (*fromEuler)(struct Quaternion*, double, double, double);
} Quaternion;

Quaternion Quaternion_mul(Quaternion*, Quaternion*);
void Quaternion_init(Quaternion*);
void Quaternion_normalise(Quaternion*);
void Quaternion_fromEuler(Quaternion*, double, double, double);

#endif
