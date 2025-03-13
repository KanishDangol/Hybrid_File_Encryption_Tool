#include <stdio.h>
#include <stdint.h>

#define AES_256_KEY_SIZE 32   // 256-bit key = 32 bytes
#define AES_256_EXPANDED_KEY_SIZE 240  // 15 round keys * 16 bytes
#define AES_BLOCK_SIZE 16 // AES operates on 16-byte blocks

// Round constants for key expansion
static const uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

static const uint8_t sbox[AES_BLOCK_SIZE][AES_BLOCK_SIZE] = {
    { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
    { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
    { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
    { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
    { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
    { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
    { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
    { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
    { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
    { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
    { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
    { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
    { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
    { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
    { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
    { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    while (b) {
        if (b & 1) p ^= a;  // If LSB of b is 1, add a to result
        uint8_t hi_bit_set = a & 0x80; // Check if MSB is set
        a <<= 1; // Multiply by x (left shift)
        if (hi_bit_set) a ^= 0x1B; // Reduce if overflowed
        b >>= 1; // Move to the next bit
    }
    return p;
}

void mixColumns(uint8_t block[4][4]) {
    for (int c = 0; c < 4; c++) {  // Process each column
        uint8_t a0 = block[0][c], a1 = block[1][c], a2 = block[2][c], a3 = block[3][c];

        block[0][c] = gmul(2, a0) ^ gmul(3, a1) ^ a2 ^ a3;  // 2*a0 + 3*a1 + 1*a2 + 1*a3
        block[1][c] = a0 ^ gmul(2, a1) ^ gmul(3, a2) ^ a3;  // 1*a0 + 2*a1 + 3*a2 + 1*a3
        block[2][c] = a0 ^ a1 ^ gmul(2, a2) ^ gmul(3, a3);  // 1*a0 + 1*a1 + 2*a2 + 3*a3
        block[3][c] = gmul(3, a0) ^ a1 ^ a2 ^ gmul(2, a3);  // 3*a0 + 1*a1 + 1*a2 + 2*a3
    }
}

void printMatrix(uint8_t matrix[4][4]) {   
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2x ", matrix[i][j]);  
    }
  printf("\n");
  }
  return;
}

void subBytes(uint8_t matrix[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      uint8_t byte = matrix[i][j];
      uint8_t column = byte % 16;
      uint8_t row = byte / 16;
      matrix[i][j] = sbox[row][column];
    }
  }
  return;
}

void shiftRows(uint8_t matrix[4][4]) {
    uint8_t temp; // Temporary variable for swapping

    // Row 1: Shift left by 1 position
    temp = matrix[1][0];
    for (int j = 0; j < 3; j++) {
        matrix[1][j] = matrix[1][j + 1];
    }
    matrix[1][3] = temp;

    // Row 2: Shift left by 2 positions (swap first two with last two)
    temp = matrix[2][0];
    matrix[2][0] = matrix[2][2];
    matrix[2][2] = temp;

    temp = matrix[2][1];
    matrix[2][1] = matrix[2][3];
    matrix[2][3] = temp;

    // Row 3: Shift left by 3 positions (same as shifting right by 1)
    temp = matrix[3][3];
    for (int j = 3; j > 0; j--) {
        matrix[3][j] = matrix[3][j - 1];
    }
    matrix[3][0] = temp;
}

// RotWord - Rotate a 4-byte word left
void rotWord(uint8_t word[4]) {
    uint8_t temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

// SubWord - Substitute each byte using the S-Box
void subWord(uint8_t word[4]) {
    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i] / 16][word[i] % 16];
    }
}

// AES-256 Key Expansion
void keyExpansion(uint8_t key[AES_256_KEY_SIZE], uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE]) {
    int i, j;
    uint8_t temp[4];

    // Copy the original 32-byte key into the expanded key
    for (i = 0; i < AES_256_KEY_SIZE; i++) {
        expandedKey[i] = key[i];
    }

    // Generate remaining bytes
    for (i = AES_256_KEY_SIZE; i < AES_256_EXPANDED_KEY_SIZE; i += 4) {
        // Copy previous 4-byte word
        for (j = 0; j < 4; j++) {
            temp[j] = expandedKey[i - 4 + j];
        }

        // Apply key schedule core every 32 bytes
        if (i % AES_256_KEY_SIZE == 0) {
            rotWord(temp);
            subWord(temp);
            temp[0] ^= Rcon[(i / AES_256_KEY_SIZE) - 1]; // XOR with round constant
        }

        // Additional SubWord for AES-256 every 8th word
        if (i % AES_256_KEY_SIZE == 16) {
            subWord(temp);
        }

        // XOR with word from 32 bytes earlier
        for (j = 0; j < 4; j++) {
            expandedKey[i + j] = expandedKey[i + j - AES_256_KEY_SIZE] ^ temp[j];
        }
    }
}

// Print Expanded Key
void printExpandedKey(uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE]) {
    for (int i = 0; i < AES_256_EXPANDED_KEY_SIZE; i++) {
        if (i % 16 == 0) printf("\nRound Key %d: ", i / 16);
        printf("%.2x ", expandedKey[i]);
    }
    printf("\n");
}

// Function to apply the AddRoundKey step
void addRoundKey(uint8_t state[4][4], uint8_t roundKey[AES_BLOCK_SIZE]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            state[row][col] ^= roundKey[row + (col * 4)];
        }
    }
}

void aes256_encrypt(uint8_t block[4][4], uint8_t key[32]) {
  uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE];

  // Expand the key for all 14 rounds
  keyExpansion(key, expandedKey);

  // Initial round key addition (Round 0)
  addRoundKey(block, expandedKey);

  // 13 full rounds
  for (int round = 1; round < 14; round++) {
    subBytes(block);
    shiftRows(block);  // ShiftRows applied in-place
    mixColumns(block); // MixColumns applied in-place
    addRoundKey(block, expandedKey + (round * 16)); // Select correct subkey
  }

  // Final round (no MixColumns)
  subBytes(block);
  shiftRows(block);  // ShiftRows applied in-place
  addRoundKey(block, expandedKey + (14 * 16)); // Final key addition
}

int main() {

  uint8_t key[AES_256_KEY_SIZE] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

  uint8_t block[4][4] = {
    { 0x00, 0x44, 0x88, 0xcc },
    { 0x11, 0x55, 0x99, 0xdd },
    { 0x22, 0x66, 0xaa, 0xee },
    { 0x33, 0x77, 0xbb, 0xff }
};

  printf("Original Block: \n");
  printMatrix(block);  

  aes256_encrypt(block, key);

  printf("Encrypted Block:\n");
  printMatrix(block);

  return 0;

}
