// WavHide.cpp : Defines the entry point for the application.
//

#include "WavHide.h"
#include "DataManipulation/WriteBinaryToTextFile.h"
#include "DataManipulation/AllocateBytes.h";
#include "DataManipulation/CountBytes.h";

using namespace std;

int main()
{
	CountBytes countBytes;
	const int fileBytes = countBytes.Count("C:\\Users\\Aranha\\Downloads\\teste.txt");
	
	//first array = bytearray
	//second array = bits in byte
	//int file[][8];
	int** file = (int**)malloc(fileBytes*sizeof(int*));
	for (int i = 0; i < 8; i++) {
		file[i] = (int*)malloc(sizeof(int*) * 8);
	}
	
	AllocateBytes::Allocate(file, "C:\\Users\\Aranha\\Downloads\\teste.txt");

	WriteBinaryToTextFile::Write(file, fileBytes, "C:\\Users\\Aranha\\Downloads\\output.txt");
	return 0;
}