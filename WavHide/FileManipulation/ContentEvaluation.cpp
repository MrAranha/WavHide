#include "ContentEvaluation.h"
#include "../DataManipulation/ByteConverter.h"
#include "../DataManipulation/HeaderWavExtractor.h"
#include "../DataManipulation/ByteConverter.h"
#include <string.h>
#include <string>
#include <cmath>  // For log10
#include <bitset>  // For bitset functionality

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
	//TODO
	//VERIFICAR SE MessageLength ESTÁ SENDO ESCRITO CORRETAMENTE
    WAV_HEADER header;
    HeaderWavExtractor::ExtractWAVHeader(file, &header);

    // Calculate the sample size based on bit depth and number of channels
    int sampleSize = (header.bitsPerSample / 8) * header.numChannels;

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

    unsigned int currentBit = 0;
    int iterations = 0;

    // Calculate the total number of iterations based on the sampleSize
    int totalIterations = 32 * 8 * sampleSize;

    // Iterating down from lastWrittenByte towards 44 bytes, we stop exactly at byte 44.
    for (int i = lastWrittenByte; i >= 44 && iterations < totalIterations; i -= sampleSize) {

    	if (currentBit >= sizeof(messageLength)*8) {
    		break;
    	}
		//TODO
    	//CHECK IF DUAL CHANNEL ITERATIONS ARE WORKING
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
    		currentBit++;
        }
    	lastWrittenByte = i;
        iterations++;  // Increment the iteration count
    }
	int remover = 0;
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
	for (int i = (75 * 8 * sampleSize) * 8; i + header.numChannels * (header.bitsPerSample / 8) < fileBytes; i += sampleSize) {
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
/*char* ContentEvaluation::ExtractMessageFromWav(int** file, int fileBytes) {
	char* message = {};
	WAV_HEADER header;
	HeaderWavExtractor::ExtractWAVHeader(file, &header);

	// Calculate the sample size based on bit depth and number of channels
	int sampleSize = (header.bitsPerSample / 8) * header.numChannels;


    int iterations = 0;
	int currentBit = 0;
    // Calculate the total number of iterations based on the sampleSize
    int totalIterations = 32 * 8 * sampleSize;
	char byteSizeCodedMessage[4];
	int fileSizeBit[8];
    // Iterating down from lastWrittenByte towards 44 bytes, we stop exactly at byte 44.
    for (int i = (75 * 8 * sampleSize) * 8; i >= 44 && iterations < totalIterations; i -= sampleSize) {

		//TODO
    	//CHECK IF DUAL CHANNEL ITERATIONS ARE WORKING
    	for (int channel = 1; channel < header.numChannels + 1; channel++) {
            int byteIndex = i + (header.bitsPerSample / 8) * channel;
            unsigned int leastValuableByteInSample = 0;

            // Find the least valuable byte in the sample
            for (int j = 0; j < header.bitsPerSample / 8; j++) {
                if (j > 0) {
                    unsigned int latestByte = ByteConverter::ByteToInt(file, byteIndex, 1);
                    unsigned int lastByte = ByteConverter::ByteToInt(file, byteIndex - 1, 1);
                    if (latestByte < lastByte) {
                        leastValuableByteInSample = byteIndex;
                    }
                }
            }
    		if (iterations % 8 == 0 && iterations != 0) {

    			int value = 0;

    			// Convert little-endian bit array to integer
    			for (int i = 0; i < 8; i++) {
    				value |= (fileSizeBit[i] << i);
    			}

    			// Ensure it's within ASCII digit range (0-9)
    			if (value < 0 || value > 9) {
    				throw std::invalid_argument("No Steganography identified on this file");
    			}
    			byteSizeCodedMessage[(iterations/8) - 1] = static_cast<char>(value);
    			memset(fileSizeBit, 0, sizeof(fileSizeBit));
    		}


            // Write the bit to the least valuable byte in the sample
            fileSizeBit[currentBit] = file[leastValuableByteInSample][7];

        }
        iterations++;  // Increment the iteration count
		if (currentBit == 7) {
			currentBit = 0;
			continue;
		}
    	currentBit++;
    }
	for (int i = 0; i < 4; i++) {
		std::cout << byteSizeCodedMessage[i] << std::endl;
	}
	return message;
}
*/

std::string ContentEvaluation::ExtractMessageFromWav(int** file, int fileBytes) {
    WAV_HEADER header;
    HeaderWavExtractor::ExtractWAVHeader(file, &header);

    // Calculate the sample size based on bit depth and number of channels
    int sampleSize = (header.bitsPerSample / 8) * header.numChannels;
	std::string bitsetResult;
    // Reading the message length from the first few bytes
    int messageLengthBits = 0;
    unsigned int currentBitOfNum = 0;
    unsigned int currentNum = 0;
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
        		for (int j = byteIndex; j > j - header.bitsPerSample / 8; j--) {
        			latestByte = ByteConverter::ByteToInt(file, j, 1);
        			lastByte = ByteConverter::ByteToInt(file, j - 1, 1);
        			if (latestByte < lastByte) {
        				leastValuableByteInSample = byteIndex;
        			}
        		}

        	}
            // Extract the bit corresponding to the message length
            int bitValue = file[leastValuableByteInSample][7]; // Getting the 7th bit of each byte
        	bitsetResult += std::to_string(bitValue);
        	iterations++;
        }
    	lastReadByte = i;
    }

    // Now, read the actual message content
    std::string message = "";
    currentNum = 0;
    currentBitOfNum = 0;

    // Iterate over the bytes where the message is stored
    for (int i = (75 * 8 * sampleSize) * 8; i < fileBytes && i < 0/*TODO TROCAR ESSE ZERO PELO RESULT TRANSFORMADO EM INT*/; i += sampleSize) {
        for (int channel = 1; channel <= header.numChannels; channel++) {
            int byteIndex = i + (header.bitsPerSample / 8) * channel;

            unsigned char byteValue = 0;
            for (int j = 0; j < 8; j++) {
                byteValue |= (file[byteIndex][7 - j] << j); // Reconstruct the byte (MSB to LSB)
            }

            // Append the byte to the message
            message += byteValue;

            if (message.length() >= 0) {
                return message;
            }
        }
    }

    throw std::invalid_argument("Message length exceeds available space in the file.");
}