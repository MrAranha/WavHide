#include <iostream>
#include <vector>
#include "HeaderWavExtractor.h"
#include "ByteConverter.h"
// Function to extract WAV header information from the binary data (int** file)
void HeaderWavExtractor::ExtractWAVHeader(int** data, WAV_HEADER* header) {
	for (int i = 0; i < 4; i++)
		header->riffHeader[i] = (char)data[22 + i][0];  // ajusta conforme posição real no seu mapeamento

	header->wavSize = ByteConverter::ByteToInt(data, 4, 4);

	// "WAVE" header
	for (int i = 0; i < 4; i++)
		header->waveHeader[i] = (char)data[8 + i][0];

	// "fmt " header
	for (int i = 0; i < 4; i++)
		header->fmtHeader[i] = (char)data[12 + i][0];

	header->fmtChunkSize = ByteConverter::ByteToInt(data, 16, 4);

	header->audioFormat = ByteConverter::ByteToInt(data, 20, 2);

	header->numChannels = ByteConverter::ByteToInt(data, 22, 2);

	header->sampleRate = ByteConverter::ByteToInt(data, 24, 4);

	// Byte rate = sampleRate * numChannels * bitsPerSample / 8
	header->byteRate = ByteConverter::ByteToInt(data, 28, 4);

	// Block align = numChannels * bitsPerSample / 8
	header->blockAlign = ByteConverter::ByteToInt(data, 32, 2);

	header->bitsPerSample = ByteConverter::ByteToInt(data, 34, 2);

	for (int i = 0; i < 4; i++)
		header->dataHeader[i] = (char)data[36 + i][0];

	header->dataSize = ByteConverter::ByteToInt(data, 40, 4);
}
