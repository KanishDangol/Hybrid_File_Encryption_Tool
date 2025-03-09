#include <stdio.h>

void printMatrix(int matrix[4][4]) {

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%d ", matrix[i][j]);
    }
  printf("\n");
  }
}

int main() {

  int matrix[4][4] = {
    { 0, 1, 2, 3 },
    { 0, 1, 2, 3 },
    { 0, 1, 2, 3 },
    { 0, 1, 2, 3 }
  };  
  int rightShiftedMatrix[4][4] = {0};
  int leftShiftedMatrix[4][4] = {0};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
     int newIndex = j + (4 - i);
     leftShiftedMatrix[i][newIndex % 4] = matrix[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int newIndex = j + i;
      rightShiftedMatrix[i][newIndex % 4] = leftShiftedMatrix[i][j];
    }
  }

  printf("Matrix: \n");
  printMatrix(matrix);

  printf("Left Shifted Matrix: \n");
  printMatrix(leftShiftedMatrix);

  printf("Right Shifted Matrix: \n");
  printMatrix(rightShiftedMatrix);

  return 0;

}
