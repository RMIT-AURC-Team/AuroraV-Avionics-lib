#include "quaternion.h"

void Quaternion_init(Quaternion* q) {
  q->w = 1.0;
  q->x = 0.0;
  q->y = 0.0;
  q->z = 0.0;
  q->fromEuler = Quaternion_fromEuler;
  q->normalise = Quaternion_normalise;
}

/********************* STATIC INTERFACE *********************/

/* ===============================================================================
 * MULTIPLY
 * - Multiplies two quaternions (Hamilton product)
 * - Stores the result in the provided result quaternion
 * =============================================================================== */
Quaternion Quaternion_mul(Quaternion* q, Quaternion* p) {
  Quaternion result;
  Quaternion_init(&result);
  result.w = q->w * p->w - q->x * p->x - q->y * p->y - q->z * p->z;
  result.x = q->w * p->x + q->x * p->w + q->y * p->z - q->z * p->y;
  result.y = q->w * p->y - q->x * p->z + q->y * p->w + q->z * p->x;
  result.z = q->w * p->z + q->x * p->y - q->y * p->x + q->z * p->w;
  return result;
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * FROM EULER
 * - Initializes the quaternion from Euler angles (roll, pitch, yaw)
 * - Converts angles to radians and halves them
 * - Computes the quaternion components based on the provided angles
 * =============================================================================== */
void Quaternion_fromEuler(Quaternion* q, double roll, double pitch, double yaw) {
  // Convert angles to radians and halve them
  roll *= 0.5f * M_PI / 180.0f;
  pitch *= 0.5f * M_PI / 180.0f;
  yaw *= 0.5f * M_PI / 180.0f;

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
void Quaternion_normalise(Quaternion* q) {
  // Compute the magnitude of the quaternion
  float magnitude = sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

  // Normalise the quaternion components
  if (magnitude > 1.0f) {
    float invMag = 1.0f / magnitude;
    q->w *= invMag;
    q->x *= invMag;
    q->y *= invMag;
    q->z *= invMag;
  }
}
