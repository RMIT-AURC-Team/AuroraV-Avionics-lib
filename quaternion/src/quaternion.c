#include "quaternion.h"

void Quaternion_init(Quaternion *q) {
  q->w = 1.0;
  q->x = 0.0;
  q->y = 0.0;
  q->z = 0.0;
  //
  q->fromEuler = Quaternion_fromEuler;
  q->normalise = Quaternion_normalise;
}

/********************* STATIC INTERFACE *********************/

/* ===============================================================================
 * MULTIPLY
 * - Multiplies two quaternions (Hamilton product)
 * - Stores the result in the provided result quaternion
 * =============================================================================== */
Quaternion Quaternion_mul(Quaternion *q, Quaternion *p) {
  Quaternion result;
  Quaternion_init(&result);
  result.w = (q->w * p->w) + (-q->x * p->x) + (-q->y * p->y) + (-q->z * p->z);
  result.x = (q->w * p->x) + (q->x * p->w) + (q->y * p->z) + (-q->z * p->y);
  result.y = (q->w * p->y) + (-q->x * p->z) + (q->y * p->w) + (q->z * p->x);
  result.z = (q->w * p->z) + (q->x * p->y) + (-q->y * p->x) + (q->z * p->w);
  return result;
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * FROM EULER
 * - Initializes the quaternion from Euler angles (roll, pitch, yaw)
 * - Converts angles to radians and halves them
 * - Computes the quaternion components based on the provided angles
 * =============================================================================== */
void Quaternion_fromEuler(Quaternion *q, float roll, float pitch, float yaw) {
  // Convert angles to radians and halve them
  roll  *= 0.5f * M_PI / 180.0f;
  pitch *= 0.5f * M_PI / 180.0f;
  yaw   *= 0.5f * M_PI / 180.0f;

  // Calculate the trigonometric functions of half angles
  float cr = cosf(roll);
  float sr = sinf(roll);
  float cp = cosf(pitch);
  float sp = sinf(pitch);
  float cy = cosf(yaw);
  float sy = sinf(yaw);

  // Compute the quaternion components
  q->w = cr * cp * cy + sr * sp * sy;
  q->x = sr * cp * cy - cr * sp * sy;
  q->y = cr * sp * cy + sr * cp * sy;
  q->z = cr * cp * sy - sr * sp * cy;
}

/* ===============================================================================
 * NORMALISE
 * - Normalises the quaternion to unit length
 * =============================================================================== */
void Quaternion_normalise(Quaternion *q) {
  // Compute the magnitude of the quaternion
  float test = (q->w * q->w) + (q->x * q->x) + (q->y * q->y) + (q->z * q->z);

  // Normalise the quaternion components
  if (test > 1.0f) {
    float invMag = 1.0f / sqrtf(test);
    //
    q->w *= invMag;
    q->x *= invMag;
    q->y *= invMag;
    q->z *= invMag;
  }
}

/* ===============================================================================
 * FROTATEVECTOR3D
 * -
 * =============================================================================== */
void Quaternion_fRotateVector3D(Quaternion *q, float *v) {
  Quaternion qVec, qConj;

  // Initialise vector quaternion
  qVec.w = 0;
  qVec.x = v[0];
  qVec.y = v[1];
  qVec.z = v[2];

  // Initialise conjugate
  qConj.w = q->w;
  qConj.x = -q->x;
  qConj.y = -q->y;
  qConj.z = -q->z;

  // Apply quaternion rotation to vector
  Quaternion temp = Quaternion_mul(q, &qVec);
  Quaternion r    = Quaternion_mul(&temp, &qConj);

  // Update vector components
  v[0] = r.x;
  v[1] = r.y;
  v[2] = r.z;
}
