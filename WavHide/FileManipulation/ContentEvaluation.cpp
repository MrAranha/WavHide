#include "ContentEvaluation.h"
#include "../DataManipulation/ByteConverter.h"
#include "../DataManipulation/HeaderWavExtractor.h"

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

void ContentEvaluation::ManipulateWav(int** file, int fileBytes) {
    WAV_HEADER header;
    HeaderWavExtractor::ExtractWAVHeader(file, &header);

    // Calculate the sample size based on bit depth and number of channels
    int sampleSize = (header.bitsPerSample / 8) * header.numChannels;

    //Process each sample in the file, starting after the header (44 bytes)
    //For now only halves the samples, but theorethically you could hide info into it
    for (int i = 44; i < fileBytes; i += sampleSize) {
        for (int channel = 0; channel < header.numChannels; channel++) {
            // Calculate the byte index for the current channel
            int byteIndex = i + (header.bitsPerSample / 8) * channel;

            if (header.bitsPerSample == 16) {
                // 16-bit signed sample
                int16_t sampleValue = 0;

                // Read the sample, considering the LSB is the 7th bit
                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << bit);  // Set bit in LSB
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 1][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (8 + bit)); // Set bit in MSB
                    }
                }

                // Halve the sample value
                sampleValue /= 2;

                // Write the modified value back, respecting the LSB
                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << bit)) {
                        file[byteIndex][0] |= (1 << (7 - bit)); // Set bit in LSB
                    }
                    else {
                        file[byteIndex][0] &= ~(1 << (7 - bit)); // Clear bit in LSB
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (8 + bit))) {
                        file[byteIndex + 1][0] |= (1 << (7 - bit)); // Set bit in MSB
                    }
                    else {
                        file[byteIndex + 1][0] &= ~(1 << (7 - bit)); // Clear bit in MSB
                    }
                }
            }
            else if (header.bitsPerSample == 8) {
                // 8-bit unsigned sample
                uint8_t sampleValue = static_cast<uint8_t>(file[byteIndex][0]);
                sampleValue /= 2; // Halve the sample value
                file[byteIndex][0] = static_cast<int>(sampleValue);
            }
            else if (header.bitsPerSample == 24) {
                // 24-bit signed sample
                int32_t sampleValue = 0;

                // Read the 3 bytes of the 24-bit sample
                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << bit);  // Set bit in the first byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 1][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (8 + bit)); // Set bit in the second byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 2][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (16 + bit)); // Set bit in the third byte
                    }
                }

                // Halve the sample value
                sampleValue /= 2;

                // Write the modified value back, respecting the LSB
                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << bit)) {
                        file[byteIndex][0] |= (1 << (7 - bit)); // Set bit in first byte
                    }
                    else {
                        file[byteIndex][0] &= ~(1 << (7 - bit)); // Clear bit in first byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (8 + bit))) {
                        file[byteIndex + 1][0] |= (1 << (7 - bit)); // Set bit in second byte
                    }
                    else {
                        file[byteIndex + 1][0] &= ~(1 << (7 - bit)); // Clear bit in second byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (16 + bit))) {
                        file[byteIndex + 2][0] |= (1 << (7 - bit)); // Set bit in third byte
                    }
                    else {
                        file[byteIndex + 2][0] &= ~(1 << (7 - bit)); // Clear bit in third byte
                    }
                }
            }
            else if (header.bitsPerSample == 32) {
                // 32-bit signed sample
                int32_t sampleValue = 0;

                // Read the 4 bytes of the 32-bit sample
                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << bit);  // Set bit in first byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 1][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (8 + bit)); // Set bit in second byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 2][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (16 + bit)); // Set bit in third byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (file[byteIndex + 3][0] & (1 << (7 - bit))) {
                        sampleValue |= (1 << (24 + bit)); // Set bit in fourth byte
                    }
                }

                // Halve the sample value
                sampleValue /= 2;

                // Write the modified value back, respecting the LSB
                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << bit)) {
                        file[byteIndex][0] |= (1 << (7 - bit)); // Set bit in first byte
                    }
                    else {
                        file[byteIndex][0] &= ~(1 << (7 - bit)); // Clear bit in first byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (8 + bit))) {
                        file[byteIndex + 1][0] |= (1 << (7 - bit)); // Set bit in second byte
                    }
                    else {
                        file[byteIndex + 1][0] &= ~(1 << (7 - bit)); // Clear bit in second byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (16 + bit))) {
                        file[byteIndex + 2][0] |= (1 << (7 - bit)); // Set bit in third byte
                    }
                    else {
                        file[byteIndex + 2][0] &= ~(1 << (7 - bit)); // Clear bit in third byte
                    }
                }

                for (int bit = 0; bit < 8; bit++) {
                    if (sampleValue & (1 << (24 + bit))) {
                        file[byteIndex + 3][0] |= (1 << (7 - bit)); // Set bit in fourth byte
                    }
                    else {
                        file[byteIndex + 3][0] &= ~(1 << (7 - bit)); // Clear bit in fourth byte
                    }
                }
            }
            // You can add handling for other bit depths if needed
        }
    }
}
