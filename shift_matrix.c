#include <stdio.h>

void printMatrix(int matrix[4][5]) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%d ", matrix[i][j]);
    }
  printf("\n");
  }
}

int main() {

  int matrix[4][5] = {
    { 0, 1, 2, 3, 4},
    { 0, 1, 2, 3, 4},
    { 0, 1, 2, 3, 4},
    { 0, 1, 2, 3, 4}
  };  
  int shiftedMatrix[4][5] = {0};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      int newIndex = j + i;
      shiftedMatrix[i][newIndex % 5] = matrix[i][j];
    }
  }

  printf("Matrix: \n");
  printMatrix(matrix);

  printf("Shifted Matrix: \n");
  printMatrix(shiftedMatrix);

  return 0;

}
