#include <iostream>
#include <vector>
#include "HeaderWavExtractor.h"
#include "ByteConverter.h"

// Function to extract WAV header information from the binary data (int** file)
void HeaderWavExtractor::ExtractWAVHeader(int** data, WAV_HEADER* header) {
	// Number of channels is located at byte offset 22, and is 2 bytes long
	header->numChannels = ByteConverter::ByteToInt(data, 22, 2);
	// Sample rate is located at byte offset 24, and is 4 bytes long
	header->sampleRate = ByteConverter::ByteToInt(data, 24, 4);
	// Bits per sample is located at byte offset 34, and is 2 bytes long
	header->bitsPerSample = ByteConverter::ByteToInt(data, 34, 2);
}