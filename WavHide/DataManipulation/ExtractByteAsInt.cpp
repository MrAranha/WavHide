#include "ExtractByteAsInt.h"

// Helper function to extract a value from the 2D int array (data) representing the WAV file's binary bits
unsigned int ExtractByteAsInt::Extract(int** data, int byteIndex, int byteCount) {
	unsigned int value = 0;

	// Iterate over each byte and construct the integer value
	for (int i = 0; i < byteCount; i++) {
		unsigned char byte = 0;
		for (int bit = 0; bit < 8; bit++) {
			byte |= (data[i][7 - bit] << bit); //combine bit into byte
		}
		value |= (byte << (i * 8));  // Shift and combine bytes into a single integer
	}

	return value;
}