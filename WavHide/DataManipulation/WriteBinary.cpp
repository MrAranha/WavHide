#include <fstream>
#include <iostream>
#include "WriteBinary.h"

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


void WriteBinary::WriteFile(int** file, int totalBytes, const char* path_to_file) {
    std::ofstream outFile(path_to_file, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    // Total number of bytes written based on bits
    for (int i = 0; i < totalBytes; i++) {
        unsigned char byte = 0;

        // Assuming each byte consists of 8 bits
        for (int bit = 0; bit < 8; bit++) {
            // Ensure we access valid indices
            if (i < totalBytes && bit < 8) {
                byte |= (file[i][7 - bit] << bit); // Combine bits into a byte
            }
        }

        // Write the byte to the output file
        outFile.write(reinterpret_cast<const char*>(&byte), sizeof(unsigned char));
    }

    outFile.close();  // Close the file
    std::cout << "File written successfully!" << std::endl;
}