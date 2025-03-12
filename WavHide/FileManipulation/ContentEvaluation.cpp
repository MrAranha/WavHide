#include "ContentEvaluation.h"
#include "../DataManipulation/ByteConverter.h"
#include "../DataManipulation/HeaderWavExtractor.h"
#include "../DataManipulation/ByteConverter.h"
#include <string.h>
#include <string>

/*
the int** data array alone cannot extract meaningful RGB information from a JPEG file. Here's why:

	Data Structure: The array is 2D, but JPEG data is compressed and not stored as direct RGB values.

	Compression: JPEG uses techniques like Huffman coding, making pixel data inaccessible directly.

	Decoding Process: To get RGB values, you must:
		Parse headers for compression details.
		Decode compressed data, involving steps like Huffman decoding.

	Intermediate Steps: Additional structures and functions are needed for decoding.

In summary, you can use an int** array for pixel data after decoding, but you must first decode the JPEG file.
*/
void ContentEvaluation::ManipulateJpeg(int** file, int fileBytes, int byteIndex)
{
	//
	for (int i = 0; i < fileBytes; i++)
	{
		//unsigned int currentByteVal = ByteConverter::Extract(file, i, 1);

	}
}

void ContentEvaluation::WriteTextOnWav(int** file, int fileBytes, const char* textToWrite) {
	WAV_HEADER header;
	HeaderWavExtractor::ExtractWAVHeader(file, &header);

	// Calculate the sample size based on bit depth and number of channels
	int sampleSize = (header.bitsPerSample / 8) * header.numChannels;
	//Process each sample in the file, starting after the header (44 bytes)
	//For now only halves the samples, but theorethically you could hide info into it

	if (strlen(textToWrite) * 8 > fileBytes)
	{
		std::string textNumOfBytesToWrite = std::to_string((strlen(textToWrite) * 8));
		std::string maxCharacters = std::to_string(fileBytes / 8);
		throw std::invalid_argument("It is not Possible to write this message within the file, the message size (" + textNumOfBytesToWrite + ") surpasses the total of possible characters writable in file data.\n Maximum Characters: " + maxCharacters);
	}

	unsigned int currentLetter = 0;
	unsigned int currentBitOfLetter = 0;
	//we write in only a single byte of each sample, that does help concealing the message
	for (int i = 44; i + header.numChannels * (header.bitsPerSample / 8) < fileBytes; i += sampleSize) {
		for (int channel = 0; channel < header.numChannels; channel++) 
		{
			// Calculate the byte index for the current channel
			int byteIndex = i + (header.bitsPerSample / 8) * channel;
			
			//Searches for the least valuable byte in a sample to alter, this reduces noise in the resulting file
			unsigned int leastValuableByteInSample = 0;
			for (int i = 0; i < header.bitsPerSample / 8; i++)
			{
				if (i > 0)
				{
					unsigned int latestByte = ByteConverter::ByteToInt(file, byteIndex, 1);
					unsigned int lastByte = ByteConverter::ByteToInt(file, byteIndex - 1, 1);
					if (latestByte < lastByte)
					{
						leastValuableByteInSample = byteIndex;
					}
				}
			}
			unsigned char byteValue = static_cast<unsigned char>(textToWrite[currentLetter]);  // Get the byte value
			int currentLetterByte[8];

			// Extract bits of the byte and store in the array (MSB first, LSB last)
			for (int i = 0; i < 8; i++) {
				currentLetterByte[7 - i] = (byteValue >> i) & 1;  // Extract bit i and store at index (7 - i)
			}

			file[leastValuableByteInSample][7] = currentLetterByte[currentBitOfLetter];

			if (currentBitOfLetter != 7)
			{
				currentBitOfLetter++;
				continue;
			}
			currentLetter++;
			currentBitOfLetter = 0;
		}
		if (currentLetter > strlen(textToWrite))
		{
			break;
		}
	}

}

//Can only halves 8bit and 16bits files (bitsPerSample)
void ContentEvaluation::WavSampleHalver(int** file, int fileBytes) {
	WAV_HEADER header;
	HeaderWavExtractor::ExtractWAVHeader(file, &header);

	int bytesPerSample = header.bitsPerSample / 8;
	int sampleSize = bytesPerSample * header.numChannels;

	// Start processing after the 44-byte header
	for (int i = 44; i + sampleSize <= fileBytes; i += sampleSize) {
		for (int channel = 0; channel < header.numChannels; channel++) {
			int byteIndex = i + (channel * bytesPerSample);

			int32_t sampleValue = 0; // Use 32-bit int for safety

			// --- Read sample from bitwise structure ---
			for (int b = 0; b < bytesPerSample; b++) {
				for (int bit = 0; bit < 8; bit++) {
					if (file[byteIndex + b][7 - bit]) { // LSB-first
						sampleValue |= (1 << (b * 8 + bit));
					}
				}
			}
			// --- Halve the sample ---
			sampleValue /= 2;

			// --- Write back the halved sample ---
			for (int b = 0; b < bytesPerSample; b++) {
				for (int bit = 0; bit < 8; bit++) {
					if (sampleValue & (1 << (b * 8 + bit))) {
						file[byteIndex + b][7 - bit] = 1; // Set bit
					}
					else {
						file[byteIndex + b][7 - bit] = 0; // Clear bit
					}
				}
			}
		}
	}
}
