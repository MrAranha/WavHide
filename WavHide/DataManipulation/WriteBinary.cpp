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

void WriteBinary::WriteFile(int** file, int bytes, const char* path_to_file) {
    std::ofstream outFile(path_to_file, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing" << std::endl;
        return;
    }

    for (int i = 0; i < bytes; i++) {
        unsigned char byte = 0;
        for (int bit = 0; bit < 8; bit++) {
            byte |= (file[i][7 - bit] << bit);
        }
        outFile.write(reinterpret_cast<const char*>(&byte), sizeof(unsigned char));
    }

    outFile.close();  // Close the file
    std::cout << "File written successfully!" << std::endl;
}
