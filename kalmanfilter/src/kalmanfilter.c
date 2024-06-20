#include "kalmanfilter.h"
#include "stdio.h"

/********************* STATIC INTERFACE *********************/

void KalmanFilter_init(KalmanFilter* kalman) {
  static float A_data[9] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f
  };
  static float H_data[6] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 0.0f, 1.0f
  };
  static float Q_data[9] = {
    0.1f, 0.0f, 0.0f, 
    0.0f, 0.1f, 0.0f, 
    0.0f, 0.0f, 0.1f
  };
  static float R_data[4] = {
    0.1f, 0.0f, 
    0.0f, 0.1f
  };
  static float P_data[9] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f
  };
  static float K_data[6] = {
    0.0f, 0.0f, 0.0f, 
    0.0f, 0.0f, 0.0f, 
  };
  static float x_data[3] = {0.0f, 0.0f, 0.0f};
  static float z_data[2] = {0.0f, 0.0f};

  arm_mat_init_f32(&kalman->A, 3, 3, A_data);
  arm_mat_init_f32(&kalman->H, 2, 3, H_data);
  arm_mat_init_f32(&kalman->Q, 3, 3, Q_data);
  arm_mat_init_f32(&kalman->R, 2, 2, R_data);
  arm_mat_init_f32(&kalman->P, 3, 3, P_data);
  arm_mat_init_f32(&kalman->K, 3, 2, K_data); // K will be calculated in the update
  arm_mat_init_f32(&kalman->x, 3, 1, x_data);
  arm_mat_init_f32(&kalman->z, 2, 1, z_data);

  kalman->update = KalmanFilter_update;
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * UPDATE
 * =============================================================================== */
void KalmanFilter_update(KalmanFilter* kalman) {

  // Extract matrices from the struct
  arm_matrix_instance_f32 *A = &kalman->A;
  arm_matrix_instance_f32 *H = &kalman->H;
  arm_matrix_instance_f32 *Q = &kalman->Q;
  arm_matrix_instance_f32 *R = &kalman->R;
  arm_matrix_instance_f32 *P = &kalman->P;
  arm_matrix_instance_f32 *K = &kalman->K;
  arm_matrix_instance_f32 *x = &kalman->x;
  arm_matrix_instance_f32 *z = &kalman->z;

  // Define temporary matrices
  float y_data[2];     // Innovation or measurement residual
  float S_data[4];     // Innovation covariance
  float S_inv_data[4]; // Inverse of innovation covariance
  float temp1_data[6]; 
  float temp2_data[9]; 
  float I_data[9] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f
  };
  
  arm_matrix_instance_f32 y, S, S_inv, I, temp1, temp2;
  arm_mat_init_f32(&y, 2, 1, y_data);
  arm_mat_init_f32(&S, 2, 2, S_data);
  arm_mat_init_f32(&S_inv, 2, 2, S_inv_data);
  arm_mat_init_f32(&I, 3, 3, I_data);
  arm_mat_init_f32(&temp1, 3, 2, temp1_data);
  arm_mat_init_f32(&temp2, 3, 3, temp2_data);

  // Prediction step
  arm_mat_mult_f32(A, x, &temp2);       // x' = A * x
  arm_copy_f32(temp2.pData, x->pData, 3);
  arm_mat_mult_f32(A, P, &temp2);       // P' = A * P * A^T + Q
  arm_mat_trans_f32(A, &temp1);
  arm_mat_mult_f32(&temp2, &temp1, P);
  arm_mat_add_f32(P, Q, P);

  // Update step
  arm_mat_mult_f32(H, x, &temp1);       // y = z - H * x
  arm_mat_sub_f32(z, &temp1, &y);
  arm_mat_mult_f32(H, P, &temp1);       // S = H * P * H^T + R
  arm_mat_trans_f32(H, &temp2);
  arm_mat_mult_f32(&temp1, &temp2, &S);
  arm_mat_add_f32(&S, R, &S);
  arm_mat_inverse_f32(&S, &S_inv);      // S^-1
  arm_mat_mult_f32(P, &temp2, &temp1);  // K = P * H^T * S^-1
  arm_mat_mult_f32(&temp1, &S_inv, K);
  arm_mat_mult_f32(K, &y, &temp1);      // x = x + K * y
  arm_mat_add_f32(x, &temp1, x);

  arm_mat_mult_f32(K, H, &temp1);       // P = (I - K * H) * P
  arm_mat_sub_f32(&I, &temp1, &temp2);
  arm_mat_mult_f32(&temp2, P, P);
}
