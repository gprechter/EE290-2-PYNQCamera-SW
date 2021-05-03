#define V_MATMULS 118
#define H_MATMULS 158

int main() {
  uint8_t edge_det[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  uint8_t im_matrix[3][3];
  
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
      //tiled_matmul_auto(matrix, edge_det);
    }
  }
}
