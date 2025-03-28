﻿// WavHide.cpp : Defines the entry point for the application.
//

#include "WavHide.h"
#include "DataManipulation/WriteBinary.h"
#include "DataManipulation/AllocateBytes.h"
#include "DataManipulation/CountBytes.h"
#include "DataManipulation/HeaderWavExtractor.h"
#include "DataManipulation/WavHeader.cpp"
#include "DataManipulation/ByteConverter.h"
#include "FileManipulation/ContentEvaluation.h"

using namespace std;

int main()
{
	const char filePath[] = "/home/aranha/Documents/WavHideFiles/wonderofusinglechannvel.wav";
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
	WriteBinary::WriteFile(ContentEvaluation::WriteTextOnWav(file, fileBytes, "to be born"), fileBytes, "/home/aranha/Documents/WavHideFiles/resultteste.wav");
	//ContentEvaluation::ExtractMessageFromWav(file, fileBytes);
	return 0;
}