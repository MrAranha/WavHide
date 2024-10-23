#include "countBytes.h"
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

unsigned int CountBytes::Count(const char* path_to_file)
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