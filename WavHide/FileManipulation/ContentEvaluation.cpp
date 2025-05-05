#include "ContentEvaluation.h"

#include <algorithm>

#include "../DataManipulation/ByteConverter.h"
#include "../DataManipulation/HeaderWavExtractor.h"
#include "../DataManipulation/ByteConverter.h"
#include <string.h>
#include <string>
#include <cmath>  // For log10
#include <bitset>  // For bitset functionality
#include <memory>

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
int** ContentEvaluation::WriteTextOnWav(int** file, int fileBytes, const char* textToWrite) {
	WAV_HEADER header;
    HeaderWavExtractor::ExtractWAVHeader(file, &header);

    // Calculate the sample size based on bit depth and number of channels
    int sampleSize = (header.bitsPerSample / 8) * header.numChannels;

	//TODO modify const char* textToWrite to a file** and find a way to prevent the hidden file do not exceeding the original
    // Ensure the message does not exceed 9999 bytes
    size_t messageLength = strlen(textToWrite) + 1;  // Including null terminator
    if (messageLength > 9999) {
        throw std::invalid_argument("Message exceeds maximum allowed size of 9998 bytes");
    }
    if (strlen(textToWrite) * 8 > fileBytes) {
        std::string textNumOfBytesToWrite = std::to_string((strlen(textToWrite) * 8));
        std::string maxCharacters = std::to_string(fileBytes / 8);
        throw std::invalid_argument("It is not Possible to write this message within the file, the message size (" + textNumOfBytesToWrite + ") surpasses the total of possible characters writable in file data.\n Maximum Characters: " + maxCharacters);
    }

    // FIRST 32 BYTES AFTER HEADER ARE DEDICATED TO STORE INSTRUCTIONS TO DECODE AT MAX A 4096-BIT RSA KEY (3600 BYTES MAX)
    int lastWrittenByte = (75 * 8 * sampleSize) * 8;  // Start writing from here

	// we do this to start writing by the lsb
	unsigned int currentBit = (std::bitset<sizeof(size_t) * 8>(messageLength).to_string()
	.substr(std::bitset<sizeof(size_t) * 8>(messageLength).to_string().find('1'))).length();
	currentBit--;

    int iterations = 0;

    // Calculate the total number of iterations based on the sampleSize
    int totalIterations = 32 * 8 * sampleSize;

    // Iterating down from lastWrittenByte towards 44 bytes, we stop exactly at byte 44.
    for (int i = lastWrittenByte; i >= 44 && iterations < totalIterations; i -= sampleSize) {

    	if (currentBit < 0) {
    		break;
    	}
    	for (int channel = 1; channel < header.numChannels + 1; channel++) {

            int byteIndex = i + (header.bitsPerSample / 8) * channel;
            unsigned int leastValuableByteInSample = byteIndex;

    		if (header.bitsPerSample/8 >= 2) {
    			unsigned int latestByte;
    			unsigned int lastByte;
    			int bytesPerSample = header.bitsPerSample / 8;
    			int iterations = 0;
    			for (int j = byteIndex; iterations < bytesPerSample ; j--) {
    				if (iterations > 0) {
    					latestByte = ByteConverter::ByteToInt(file, j, 1);
    					lastByte = ByteConverter::ByteToInt(file, (j - 1), 1);
    					if (iterations ==63) {

    					}
    					if (latestByte < lastByte) {
    						leastValuableByteInSample = byteIndex;
    					}
    				}
    				iterations++;
    			}

    		}

    		//easier to debug this way
    		std::string binaryString = std::bitset<sizeof(size_t) * 8>(messageLength).to_string().substr(std::bitset<sizeof(size_t) * 8>(messageLength).to_string().find('1'));
			char zero = '0';
    		char one = '1';
    		int currentBitValue = 0;
    		if (binaryString[currentBit] == one && binaryString.size() > currentBit) {
    			currentBitValue = 1;
    		}
    		else if (binaryString[currentBit] == zero && binaryString.size() > currentBit) {
    			currentBitValue = 0;
    		}
            // Write the bit to the least valuable byte in the sample
            file[leastValuableByteInSample][7] = currentBitValue;
			std::cout << file[leastValuableByteInSample][7] << std::endl;
    		if (currentBit == 0) {
    			break;
    		}
    		currentBit--;
			iterations++;  // Increment the iteration count
        }
    	if (currentBit == 0) {
    		break;
    	}
    	lastWrittenByte = i;
    }
	int remover = 0;
	//after the for we made we zero all LSB from 44 to the fist one
	for (int i = 44; i + header.numChannels * (header.bitsPerSample / 8) < lastWrittenByte; i += sampleSize) {
		for (int channel = 1; channel < header.numChannels + 1; channel++) {

			// Calculate the byte index for the current channel
			int byteIndex = i + (header.bitsPerSample / 8) * channel;

			unsigned int leastValuableByteInSample = byteIndex;
    		if (header.bitsPerSample/8 >= 2) {
    			unsigned int latestByte;
    			unsigned int lastByte;
    			int bytesPerSample = header.bitsPerSample / 8;
    			int iterations = 0;
    			for (int j = byteIndex; iterations < bytesPerSample ; j--) {
    				if (iterations > 0) {
    					latestByte = ByteConverter::ByteToInt(file, j, 1);
    					lastByte = ByteConverter::ByteToInt(file, (j - 1), 1);
    					if (latestByte < lastByte) {
    						leastValuableByteInSample = byteIndex;
    					}
    				}
    				iterations++;
    			}

    		}

			file[leastValuableByteInSample][7] = 0;
            std::cout << file[leastValuableByteInSample][7] << std::endl;
		}
		remover++;
	}

	unsigned int currentLetter = 0;
	unsigned int currentBitOfLetter = 0;
	//we write in only a single byte of each sample, that does help concealing the message
	// + 1 bytes because it is the next byte after our guidebytes
	for (int i = (75 * 8 * sampleSize) * 8 + 1; i + header.numChannels * (header.bitsPerSample / 8) < fileBytes; i += sampleSize) {
		for (int channel = 1; channel < header.numChannels + 1; channel++)
		{
			// Calculate the byte index for the current channel
			int byteIndex = i + (header.bitsPerSample / 8) * channel;

			unsigned int leastValuableByteInSample = byteIndex;
			if (header.bitsPerSample/8 >= 2) {
				unsigned int latestByte;
				unsigned int lastByte;
				int bytesPerSample = header.bitsPerSample / 8;
				int iterations = 0;
				for (int j = byteIndex; iterations < bytesPerSample ; j--) {
					if (iterations > 0) {
						latestByte = ByteConverter::ByteToInt(file, j, 1);
						lastByte = ByteConverter::ByteToInt(file, (j - 1), 1);
						if (latestByte < lastByte) {
							leastValuableByteInSample = byteIndex;
						}
					}
					iterations++;
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
	return file;
}

std::string ContentEvaluation::ExtractMessageFromWav(int** file, int fileBytes) {
    WAV_HEADER header;
    HeaderWavExtractor::ExtractWAVHeader(file, &header);

    // Calculate the sample size based on bit depth and number of channels
    int sampleSize = (header.bitsPerSample / 8) * header.numChannels;
	std::string bitsetResult;
	int totalIterations = 32 * 8 * sampleSize;
	int iterations = 0;
    int lastReadByte = (75 * 8 * sampleSize) * 8;  // Start reading from here
    // Assuming the first 32 bits after the header store the length of the message (based on your description)
    for (int i = lastReadByte; i >= 44 && iterations < totalIterations; i -= sampleSize) {
        for (int channel = 1; channel <= header.numChannels; channel++) {
            int byteIndex = i + (header.bitsPerSample / 8) * channel;

        	unsigned int leastValuableByteInSample = byteIndex;

        	if (header.bitsPerSample/8 >= 2) {
        		unsigned int latestByte;
        		unsigned int lastByte;
        		for (int j = byteIndex; j != byteIndex - header.bitsPerSample / 8; j--) {
        			latestByte = ByteConverter::ByteToInt(file, j, 1);
        			lastByte = ByteConverter::ByteToInt(file, j - 1, 1);
        			if (latestByte < lastByte) {
        				leastValuableByteInSample = byteIndex;
        			}
        		}

        	}
            // Extract the bit corresponding to the message length
        	std::cout << leastValuableByteInSample << std::endl;
            int bitValue = file[leastValuableByteInSample][7]; // Getting the 7th bit of each byte
        	bitsetResult += std::to_string(bitValue);
        	iterations++;
        }
    	lastReadByte = i;
    }
	//cleanup string, it is reversed, so any zeros on the left should hold no value
	// AKA removing trailingzeros
	if (bitsetResult.length() > 63) {
		bitsetResult = bitsetResult.substr(0, 63);
	}
	size_t lastOnePos = bitsetResult.find_last_not_of('0');
	bitsetResult.erase(lastOnePos +1);

	std::reverse(bitsetResult.begin(), bitsetResult.end());
	int fileSize = std::stoi(bitsetResult, nullptr, 2);
	std::string message;

	unsigned int currentBitOfByte = 0;
	int decodedLetters = 0;
	int currentByteBits[8] = {0};

	int start = (75 * 8 * sampleSize) * 8 + 1;

	for (int i = start; i < fileBytes && decodedLetters < fileSize -1; i += sampleSize) {
		for (int channel = 1; channel <= header.numChannels; channel++) {
			int byteIndex = i + (header.bitsPerSample / 8) * channel;

			unsigned int leastValuableByteInSample = byteIndex;
			if (header.bitsPerSample/8 >= 2) {
				unsigned int latestByte;
				unsigned int lastByte;
				int bytesPerSample = header.bitsPerSample / 8;
				int iterations = 0;
				for (int j = byteIndex; iterations < bytesPerSample ; j--) {
					if (iterations > 0) {
						latestByte = ByteConverter::ByteToInt(file, j, 1);
						lastByte = ByteConverter::ByteToInt(file, (j - 1), 1);
						if (latestByte < lastByte) {
							leastValuableByteInSample = byteIndex;
						}
					}
					iterations++;
				}

			}
			currentByteBits[currentBitOfByte++] = file[leastValuableByteInSample][7];
			if (currentBitOfByte == 8) {
				unsigned char ch = 0;
				for (int b = 0; b < 8; ++b) {
					ch |= (currentByteBits[b] << (7 - b));
				}
				message += static_cast<char>(ch);
				decodedLetters++;
				currentBitOfByte = 0;

				if (decodedLetters > fileSize) {
					break;
				}
			}
		}
		if (decodedLetters > fileSize) {
			break;
		}
	}

	return message;
}