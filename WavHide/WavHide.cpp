#include "WavHide.h"
#include "DataManipulation/WriteBinary.h"
#include "DataManipulation/CountBytes.h"
#include "DataManipulation/WavHeader.cpp"
#include "FileManipulation/ContentEvaluation.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "DataManipulation/AllocateBytes.h"

using namespace std;
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>


int main(int argc, char *argv[]) {
	/*
	int opt;
	while ((opt = getopt(argc, argv, "hvf:")) != -1) {
		switch (opt) {
			case 'h':
				printf("Usage: %s [-h] [-v] [-f filename]\n", argv[0]);
			exit(0);
			case 'v':
				printf("Verbose mode enabled.\n");
			break;
			case 'f':
				printf("Processing file: %s\n", optarg);
			break;
			default:
				fprintf(stderr, "Usage: %s [-h] [-v] [-f filename]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}*/

	const char filePath[] = "/home/aranha/Documents/WavHideFiles/resultteste.wav";
	const int fileBytes = CountBytes::Count(filePath);

	//first array = bytearray
	//second array = bits in byte
	//int file[][8];
	int** file = (int**)malloc(fileBytes * sizeof(int*));
	for (int i = 0; i < fileBytes; i++) {
		file[i] = (int*)malloc(8 * sizeof(int));

		for (int j = 0; j < 8; j++) {
			file[i][j] = j % 2;
		}
	}

	AllocateBytes::Allocate(file, filePath);


	/*std::ifstream textFile("/home/aranha/Documents/WavHideFiles/private_key.pem"); // Open in text mode (default)
	std::string content((std::istreambuf_iterator<char>(textFile)), std::istreambuf_iterator<char>());
	WriteBinary::WriteFile(ContentEvaluation::WriteTextOnWav(file, fileBytes, content.c_str()), fileBytes, "/home/aranha/Documents/WavHideFiles/resultteste.wav");
	//*/
	//Write result
	std::string SSHKEY = ContentEvaluation::ExtractMessageFromWav(file, fileBytes);
	std::ofstream out("/home/aranha/Documents/WavHideFiles/resultteste.pem");
	out << SSHKEY;
	out.close();
	return 0;
}