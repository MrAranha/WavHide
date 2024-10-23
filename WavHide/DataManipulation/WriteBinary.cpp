#include <fstream>
#include <iostream>
#include "WriteBinary.h"
#include "WavHeader.cpp"

using namespace std;


//The purpose of this function is for testing
void WriteBinary::WriteText(int** file, int bytes, const char* path_to_file)
{
	std::ofstream outFile(path_to_file);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing." << std::endl;
		return;
	}
	for (int i = 0; i < bytes; i++) {
		for (int j = 0; j < 8; j++) {
			// << bitshift operator
			//this line will transform the byte [i] into a character once again
			int a = file[i][j];
			outFile << file[i][j];
		}
		outFile << "\n";
	}

	outFile.close();  // Close the file
	std::cout << "Text file written successfully!" << std::endl;
}

//Still need to adapt to a jpeg compiler, its not just binary data thrown into a file as a .txt
void WriteBinary::WriteJpeg(int** file, int bytes, const char* path_to_file)
{
}


//Same for Wave files
void WriteBinary::WriteWav(int** file, int bytes, const char* path_to_file, int sampleRate, short numChannels, short bitsPerSample)
{
	// Calculate total number of samples (bytes is the number of rows in data, each with 8 bits)
	int numSamples = bytes;

	//This just assures that we wont write in an unexisting file, usually if the file does not exist it will create it either way, but think of it as a safety measure 
	std::ofstream outFile(path_to_file);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing" << std::endl;
		return;
	}

	//We need to create the Wav header
	WAV_HEADER wavHeader;
	memcpy(wavHeader.riffHeader, "RIFF", 4);
	memcpy(wavHeader.waveHeader, "WAVE", 4);
	memcpy(wavHeader.fmtHeader, "fmt ", 4);
	memcpy(wavHeader.dataHeader, "data", 4);

	wavHeader.fmtChunkSize = 16;  // PCM chunk size
	wavHeader.audioFormat = 1;    // PCM format
	wavHeader.numChannels = numChannels;
	wavHeader.sampleRate = sampleRate;
	wavHeader.bitsPerSample = bitsPerSample;
	wavHeader.byteRate = sampleRate * numChannels * bitsPerSample / 8;
	wavHeader.blockAlign = numChannels * bitsPerSample / 8;
	wavHeader.dataSize = numSamples * numChannels * bitsPerSample / 8;  // Data size
	wavHeader.wavSize = wavHeader.dataSize + 36;  // Size of the file minus the first 8 bytes of the RIFF header

	// Write the WAV header to the file
	outFile.write(reinterpret_cast<const char*>(&wavHeader), sizeof(WAV_HEADER));

	// Write the audio data
    // Write the audio data
	for (int i = 0; i < bytes; i++) {
		unsigned char byte = 0;  // Variable to store the byte

		// Combine 8 bits from data[i] into one byte
		for (int bit = 0; bit < 8; bit++) {
			byte |= (file[i][bit] << bit);  // Combine bits into a byte
		}

		// Write the byte to the file
		outFile.write(reinterpret_cast<const char*>(&byte), sizeof(unsigned char));
	}


	outFile.close();  // Close the file
	std::cout << "WAV file written successfully!" << std::endl;
}