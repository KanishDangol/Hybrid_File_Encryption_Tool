#include <stdio.h>
#include <stdint.h>

#include "tables.h"

static void subBytes(uint8_t matrix[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      uint8_t byte = matrix[i][j];
      uint8_t column = byte % 16;
      uint8_t row = byte / 16;
      matrix[i][j] = sbox[row][column];
    }
  }
}

static void invSubBytes(uint8_t matrix[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      uint8_t byte = matrix[i][j];
      uint8_t column = byte % 16;
      uint8_t row = byte / 16;
      matrix[i][j] = inv_sbox[row][column];
    }
  }
}

static void shiftRows(uint8_t matrix[4][4]) {
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

static void invShiftRows(uint8_t matrix[4][4]) {
    uint8_t temp; // Temporary variable for swapping

    // Row 1: Shift right by 1 position (undo left shift by 1)
    temp = matrix[1][3];
    for (int j = 3; j > 0; j--) {
        matrix[1][j] = matrix[1][j - 1];
    }
    matrix[1][0] = temp;

    // Row 2: Shift right by 2 positions (swap first two with last two)
    temp = matrix[2][0];
    matrix[2][0] = matrix[2][2];
    matrix[2][2] = temp;

    temp = matrix[2][1];
    matrix[2][1] = matrix[2][3];
    matrix[2][3] = temp;

    // Row 3: Shift right by 3 positions (same as shifting left by 1)
    temp = matrix[3][0];
    for (int j = 0; j < 3; j++) {
        matrix[3][j] = matrix[3][j + 1];
    }
    matrix[3][3] = temp;
}

static uint8_t gmul(uint8_t a, uint8_t b) {
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

static void mixColumns(uint8_t block[4][4]) {
    for (int c = 0; c < 4; c++) {  // Process each column
        uint8_t a0 = block[0][c], a1 = block[1][c], a2 = block[2][c], a3 = block[3][c];

        block[0][c] = gmul(2, a0) ^ gmul(3, a1) ^ a2 ^ a3;  // 2*a0 + 3*a1 + 1*a2 + 1*a3
        block[1][c] = a0 ^ gmul(2, a1) ^ gmul(3, a2) ^ a3;  // 1*a0 + 2*a1 + 3*a2 + 1*a3
        block[2][c] = a0 ^ a1 ^ gmul(2, a2) ^ gmul(3, a3);  // 1*a0 + 1*a1 + 2*a2 + 3*a3
        block[3][c] = gmul(3, a0) ^ a1 ^ a2 ^ gmul(2, a3);  // 3*a0 + 1*a1 + 1*a2 + 2*a3
    }
}

static void invMixColumns(uint8_t block[4][4]) {
    for (int c = 0; c < 4; c++) {  // Process each column
        uint8_t a0 = block[0][c], a1 = block[1][c], a2 = block[2][c], a3 = block[3][c];

        block[0][c] = gmul(0x0E, a0) ^ gmul(0x0B, a1) ^ gmul(0x0D, a2) ^ gmul(0x09, a3);
        block[1][c] = gmul(0x09, a0) ^ gmul(0x0E, a1) ^ gmul(0x0B, a2) ^ gmul(0x0D, a3);
        block[2][c] = gmul(0x0D, a0) ^ gmul(0x09, a1) ^ gmul(0x0E, a2) ^ gmul(0x0B, a3);
        block[3][c] = gmul(0x0B, a0) ^ gmul(0x0D, a1) ^ gmul(0x09, a2) ^ gmul(0x0E, a3);
    }
}

static void printMatrix(uint8_t matrix[4][4]) {   
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2x ", matrix[i][j]);  
    }
  printf("\n");
  }
}

// RotWord - Rotate a 4-byte word left
static void rotWord(uint8_t word[4]) {
    uint8_t temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

// SubWord - Substitute each byte using the S-Box
static void subWord(uint8_t word[4]) {
    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i] / 16][word[i] % 16];
    }
}

// AES-256 Key Expansion
static void keyExpansion(uint8_t key[AES_256_KEY_SIZE], uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE]) {
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
static void printExpandedKey(uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE]) {
    for (int i = 0; i < AES_256_EXPANDED_KEY_SIZE; i++) {
        if (i % 16 == 0) printf("\nRound Key %d: ", i / 16);
        printf("%.2x ", expandedKey[i]);
    }
    printf("\n");
}

// Function to apply the AddRoundKey step
static void addRoundKey(uint8_t state[4][4], uint8_t roundKey[16]) {
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

void aes256_decrypt(uint8_t block[4][4], uint8_t key[32]) {
    uint8_t expandedKey[AES_256_EXPANDED_KEY_SIZE];

    // Expand the key for all 14 rounds
    keyExpansion(key, expandedKey);

    // Initial round key addition (Round 14 key)
    addRoundKey(block, expandedKey + (14 * 16));

    // 13 full rounds (in reverse order)
    for (int round = 13; round > 0; round--) {
        invShiftRows(block);
        invSubBytes(block);
        addRoundKey(block, expandedKey + (round * 16));
        invMixColumns(block);
    }

    // Final round (no invMixColumns)
    invShiftRows(block);
    invSubBytes(block);
    addRoundKey(block, expandedKey); // Round 0 key addition
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

  aes256_decrypt(block, key);
  
  printf("Decrypted Block:\n");
  printMatrix(block);

  return 0;

}
