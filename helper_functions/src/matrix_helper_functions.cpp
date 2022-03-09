#ifndef MATRIX_HELPER_FUNCTIONS_CPP
#define MATRIX_HELPER_FUNCTIONS_CPP

#include "../matrix_helper_functions.h"

// Zero a double matrix.
void zero_matrix(double* matrix, int rows, int cols){
  for (int i = 0; i < rows*cols; i++){
    matrix[i] = 0;
  }
}

#endif
