#include "kalmanfilter.h"

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
    0.0f, 0.0f, 
    0.0f, 0.0f, 
    0.0f, 0.0f
  };
  static float x_data[3] = {0.0f, 0.0f, 0.0f};

  arm_mat_init_f32(&kalman->A, 3, 3, A_data);
  arm_mat_init_f32(&kalman->H, 2, 3, H_data);
  arm_mat_init_f32(&kalman->Q, 3, 3, Q_data);
  arm_mat_init_f32(&kalman->R, 2, 2, R_data);
  arm_mat_init_f32(&kalman->P, 3, 3, P_data);
  arm_mat_init_f32(&kalman->K, 3, 2, K_data); // K will be calculated in the update
  arm_mat_init_f32(&kalman->x, 3, 1, x_data);

  kalman->update = KalmanFilter_update;
}

/********************* PUBLIC INTERFACE *********************/

/* ===============================================================================
 * UPDATE
 * =============================================================================== */
void KalmanFilter_update(KalmanFilter* kalman, arm_matrix_instance_f32* z) {

  // Extract matrices from the struct
  arm_matrix_instance_f32 *A = &kalman->A;
  arm_matrix_instance_f32 *H = &kalman->H;
  arm_matrix_instance_f32 *Q = &kalman->Q;
  arm_matrix_instance_f32 *R = &kalman->R;
  arm_matrix_instance_f32 *P = &kalman->P;
  arm_matrix_instance_f32 *K = &kalman->K;
  arm_matrix_instance_f32 *x = &kalman->x;

  // Define intermediate matrices
  // TODO: extract initialisation of intermediate matrices for performance
  //  : Performance indicator measurements should probably be ran first
  //    to determine if this is even necessary.
  float y_data[2];        // Innovation or measurement residual
  float S_data[4];        // Innovation covariance
  float S_inv_data[4];    // Inverse of innovation covariance
  float x_pred_data[3];   // State prediction vector
  float P_pred_data[9];   // Covariance prediction matrix
  float H_trans_data[6];  // Measurement model transpose
  float temp2x1_data[2]; 
  float temp2x3_data[6]; 
  float temp3x2_data[6]; 
  float temp3x3_data[9]; 
  float I_data[9] = {     // Identity matrix
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f
  };

  /* ================================
   * Initialise intermediate matrices 
   * ================================ */

  arm_matrix_instance_f32 y, S, S_inv, x_pred, P_pred, H_trans, I;
  arm_mat_init_f32(&y, 2, 1, y_data);
  arm_mat_init_f32(&S, 2, 2, S_data);
  arm_mat_init_f32(&S_inv, 2, 2, S_inv_data);
  arm_mat_init_f32(&H_trans, 3, 2, H_trans_data);
  arm_mat_init_f32(&x_pred, 3, 1, x_pred_data);
  arm_mat_init_f32(&P_pred, 3, 3, P_pred_data);
  arm_mat_init_f32(&I, 3, 3, I_data);
  // Temporary matrices for calculation
  arm_matrix_instance_f32 temp2x1, temp2x3, temp3x2, temp3x3;
  arm_mat_init_f32(&temp2x1, 2, 1, temp2x1_data);
  arm_mat_init_f32(&temp2x3, 2, 3, temp2x3_data);
  arm_mat_init_f32(&temp3x2, 3, 2, temp3x2_data);
  arm_mat_init_f32(&temp3x3, 3, 3, temp3x3_data);
  
  /* ================================
   * Run calculations
   * ================================ */

  // Prediction step
  arm_mat_mult_f32(A, x, &x_pred);                // x' = A * x
  arm_mat_mult_f32(A, P, &P_pred);                // P' = A * P * A^T + Q
  arm_mat_trans_f32(A, &temp3x3);
  arm_mat_mult_f32(&P_pred, &temp3x3, P);
  arm_mat_add_f32(P, Q, &P_pred);

  // Update step
  arm_mat_mult_f32(H, x, &temp2x1);               // y = z - H * x'
  arm_mat_sub_f32(z, &temp2x1, &y);
  arm_mat_mult_f32(H, &P_pred, &temp2x3);         // S = H * P * H^T + R
  arm_mat_trans_f32(H, &H_trans);
  arm_mat_mult_f32(&temp2x3, &H_trans, &S);
  arm_mat_add_f32(&S, R, &S);
  arm_mat_inverse_f32(&S, &S_inv);                // S^-1
  arm_mat_mult_f32(&P_pred, &H_trans, &temp3x2);  // K = P * H^T * S^-1
  arm_mat_mult_f32(&temp3x2, &S_inv, K);
  arm_mat_mult_f32(K, &y, &temp3x2);              // x = x' + K * y
  arm_mat_add_f32(&x_pred, &temp3x2, x);
  arm_mat_mult_f32(K, H, &temp3x3);               // P = (I - K * H) * P
  arm_mat_sub_f32(&I, &temp3x3, &temp3x3);
  arm_mat_mult_f32(&temp3x3, &P_pred, P);
}
