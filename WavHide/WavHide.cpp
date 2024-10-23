// WavHide.cpp : Defines the entry point for the application.
//

#include "WavHide.h"
#include <fstream>
#include <bitset>
#include <limits>
#include <string>
#include <iostream>
#include <algorithm>

#include "DataManipulation/WriteBinaryToTextFile.h"

using namespace std;

//http://en.cppreference.com/w/cpp/language/type_alias
using Byte = unsigned char;

//http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<Byte>::digits;

//http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE>;


unsigned int countBytes(const char* path_to_file)
{
	std::string bitstring;
	std::ifstream file(path_to_file, std::ios::binary); // the ios declaration means it will open in binary mode

	char c;
	int bytes = 0;
	while (file.get(c))
	{
		bitstring += bits_in_byte(Byte(c)).to_string(); //makes as a string of 0 and 1
		bytes++;
	}
	return bytes;
}

void allocateBytes(int** file, const char* path_to_file)
{
	std::ifstream fileTemp(path_to_file, std::ios::binary);

	char c;
	int i = 0;
	while (fileTemp.get(c))
	{
		//TODO
		//I suspect my dumbass stored the bits backwards, must investigate
		unsigned long val = bits_in_byte(Byte(c)).to_ulong();
		std::bitset<8> binary(val);
		for (int j = 0; j < 8; j++)
		{
			file[i][j] = binary[j];  // Store each bit as an integer (0 or 1)
		}
		i++;
	}
}


int main()
{
	const int fileBytes = countBytes("C:\\Users\\Aranha\\Downloads\\teste.txt");
	
	//first array = bytearray
	//second array = bits in byte
	//int file[][8];
	int** file = (int**)malloc(fileBytes*sizeof(int*));
	for (int i = 0; i < 8; i++) {
		file[i] = (int*)malloc(sizeof(int*) * 8);
	}
	
	allocateBytes(file, "C:\\Users\\Aranha\\Downloads\\teste.txt");

	WriteBinaryToTextFile::Write(file, fileBytes, "C:\\Users\\Aranha\\Downloads\\output.txt");
	return 0;
}