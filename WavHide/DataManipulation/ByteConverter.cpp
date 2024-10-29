#include "ByteConverter.h"

// Helper function to extract a value from the 2D int array (data) representing the WAV file's binary bits
unsigned int ByteConverter::ByteToInt(int** data, int byteIndex, int byteCount) {
    unsigned int value = 0;

    // Iterate over each byte
    for (int i = 0; i < byteCount; i++) {
        unsigned char byte = 0;

        // Construct the byte from bits
        for (int bit = 0; bit < 8; bit++) {
            // Use the correct index to get the bit
            byte |= (data[byteIndex + i][7 - bit] << bit);
        }

        // Combine the byte into the total integer value
        value |= (byte << (i * 8));
    }

    return value;
}

void ByteConverter::DecrementByte(int** data, int byteIndex) {
    // Start from the least significant bit (7th bit in the array) and move left
    bool borrow = true;  // Initially, we want to subtract 1
    for (int bit = 7; bit >= 0; bit--) {
        if (data[byteIndex][bit] == 1 && borrow) {
            data[byteIndex][bit] = 0;  // Flip 1 to 0, no more borrow needed
            borrow = false;            // No more borrowing necessary
        }
        else if (data[byteIndex][bit] == 0 && borrow) {
            data[byteIndex][bit] = 1;  // Flip 0 to 1, continue borrowing
            borrow = true;             // Keep borrowing for the next bit
        }
    }
}