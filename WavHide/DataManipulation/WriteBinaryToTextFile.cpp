#include <fstream>
#include <iostream>
#include "WriteBinaryToTextFile.h"

using namespace std;


//The purpose of this function is for testing
void WriteBinaryToTextFile::Write(int** file, int bytes, const char* path_to_file)
{
	std::ofstream outFile(path_to_file);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing." << std::endl;
		return;
	}
	for (int i = 0; i < bytes; i++) {
		unsigned char character = 0;  // To store the converted character
		for (int j = 0; j < 8; j++) {
			// << bitshift operator
			//this line will transform the byte [i] into a character once again
			character |= (file[i][j] << j);
		}
		// Write the character to the file
		outFile << character;
	}

	outFile.close();  // Close the file
	std::cout << "Text file written successfully!" << std::endl;
}