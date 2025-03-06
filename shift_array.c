#include <stdio.h>

void printArray(int arr[]) {

  for (int i = 0; i < 5; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {

  int array[5] = { 0, 1, 2, 3, 4 };
  int shiftedArray[5] = {0};

  for (int j = 0; j < 5; j++) {
    if (j == 4) {
      shiftedArray[0] = array[j];
    } else {
      shiftedArray[j + 1] = array[j];
    }
  }

  printf("Original array: ");
  printArray(array);
  
  printf("Shifted array: ");
  printArray(shiftedArray);

  return 0;
}
