#include <stdio.h>
#include <stdint.h>

void printMatrix(int matrix[][2]) {   //int -> uint8_t

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      printf("%.2d ", matrix[i][j]);  //d -> x
    }
  printf("\n");
  }
}

int main() {

/*
  uint8_t sbox[2][2] = {
    { 0x63, 0x7c },
    { 0xca, 0x82 }
  };

  uint8_t block[2][2] = {
    { 0x00, 0x01 },
    { 0x10, 0x11 }
  };
*/
  int sbox[2][2] = {
    { 63, 77 },
    { 44, 55 }
  };

  int block[2][2] = {
    { 00, 01 },
    { 10, 11 }
  };

  printf("S-box: \n");
  printMatrix(sbox);  

  printf("Block: \n");
  printMatrix(block);  

  printf("Substituted Block: \n");

  return 0;

}
