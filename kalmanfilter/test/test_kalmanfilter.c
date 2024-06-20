#include <stdio.h>
#include "arm_math.h"

int main() {
  printf("Starting test...\n");

  arm_matrix_instance_f32 A; 
  static float A_data[9] = {
    2.0f, 0.0f, 0.0f, 
    0.0f, 2.0f, 0.0f, 
    0.0f, 0.0f, 2.0f
  };

  arm_matrix_instance_f32 B;
  static float B_data[9] = {
    2.0f, 0.0f, 0.0f, 
    0.0f, 2.0f, 0.0f, 
    0.0f, 0.0f, 2.0f
  };

  arm_matrix_instance_f32 C;
  static float C_data[9];

  printf("Initialising...\n");
  arm_mat_init_f32(&A, 3, 3, A_data);
  printf("%dx%d\n", A.numRows, A.numCols);

  arm_mat_init_f32(&B, 3, 3, B_data);
  arm_mat_init_f32(&C, 3, 3, C_data);

  printf("Multiplying...\n");
  arm_mat_mult_f32(&A, &B, &C);

  printf("%f\t", C.pData[0]);
  printf("%f\t", C.pData[4]);
  printf("%f\t", C.pData[8]);

  return 0;
}
