// WavHide.cpp : Defines the entry point for the application.
//

#include "WavHide.h"
#include "DataManipulation/WriteBinary.h"
#include "DataManipulation/AllocateBytes.h"
#include "DataManipulation/CountBytes.h"
#include "DataManipulation/HeaderWavExtractor.h"
#include "DataManipulation/WavHeader.cpp"

using namespace std;

int main()
{
	CountBytes countBytes;
	const int fileBytes = countBytes.Count("C:\\Users\\Aranha\\Downloads\\wonderofudualchannel.wav");
	
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
	
	AllocateBytes::Allocate(file, "C:\\Users\\Aranha\\Downloads\\wonderofudualchannel.wav");

	WriteBinary::WriteWav(file, fileBytes, "C:\\Users\\Aranha\\Downloads\\testewavresult.wav");
	return 0;
}