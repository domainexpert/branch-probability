// From http://lists.llvm.org/pipermail/llvm-dev/2017-May/113143.html

#define DIM 10
int A[DIM][DIM], B[DIM][DIM], C[DIM][DIM];

void MatMul(int Arows, int Acols, int Brows, int Bcols) {
  short i, j, k;

  for (i = 0; i < Arows; ++i) {
    for (j = 0; j < Bcols; ++j) {
      C[i][j] = 0;
      for (k = 0; k < Acols; ++k) {
	C[i][j] += A[i][k] * B[j][k];
      }
    }
  }
}

int main() {
  MatMul(DIM, DIM, DIM, DIM);
  return 0;
}
