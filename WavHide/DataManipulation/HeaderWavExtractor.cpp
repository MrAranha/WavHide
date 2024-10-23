#include <iostream>
#include <vector>
#include "HeaderWavExtractor.h"

// Helper function to extract a value from the 2D int array (data) representing the WAV file's binary bits
unsigned int HeaderWavExtractor::ExtractBitAsInt(int** data, int byteIndex, int byteCount) {
    unsigned int value = 0;

    // Iterate over each byte and construct the integer value
    for (int i = 0; i < byteCount; i++) {
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; bit++) {
            byte |= (data[byteIndex + i][bit] << bit);  // Combine bits into a byte
        }
        value |= (byte << (i * 8));  // Shift and combine bytes into a single integer
    }

    return value;
}

// Function to extract WAV header information from the binary data (int** file)
void HeaderWavExtractor::ExtractWAVHeader(int** data, WAV_HEADER* header) {
    // Number of channels is located at byte offset 22, and is 2 bytes long
    header->numChannels = HeaderWavExtractor::ExtractBitAsInt(data, 22, 2);
    // Sample rate is located at byte offset 24, and is 4 bytes long
    header->sampleRate = HeaderWavExtractor::ExtractBitAsInt(data, 24, 4);
    // Bits per sample is located at byte offset 34, and is 2 bytes long
    header->bitsPerSample = HeaderWavExtractor::ExtractBitAsInt(data, 34, 2);
}