#define V_MATMULS 118
#define H_MATMULS 158
#define DIM_I 3
#define DIM_K 3
#define DIM_J 3

int main() {
  static elem_t edge_det[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  static elem_t im_matrix[3][3];
  static elem_t full_C[DIM_I][DIM_J] row_align(1);

  
  int v;
  int h;
  for (v = 0; v < V_MATMULS; v++) {
    for (h = 0; h < H_MATMMULS; h++) {
      //Fill the im_matrix with the correct 3x3 IA from the image
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            im_matrix[i][j] = image[v + i][h + j];        
        }
      }
      tiled_matmul_auto(DIM_I, DIM_J, DIM_K,
                    im_matrix, edge_det, NULL, full_C,
                    DIM_K, DIM_J, DIM_J, DIM_J, // striding factors
                    MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, MVIN_SCALE_IDENTITY, // mvin scaling factors
                    NO_ACTIVATION, ACC_SCALE_IDENTITY, 0, false, // activation, scale, relu6_shift, repeating_bias
                    false, false, // no transposing
                    false, false, // elem_t C, full bias width (doesn't matter since no bias added)
                    WS);
      //tiled_matmul_auto(matrix, edge_det);
    }
  }
}
