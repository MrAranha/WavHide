// WavHide.cpp : Defines the entry point for the application.
//

#include "WavHide.h"
#include <fstream>
#include <bitset>
#include <limits>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

//http://en.cppreference.com/w/cpp/language/type_alias
using Byte = unsigned char;

//http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<Byte>::digits;

//http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE>;


std::string read_bits(const char* path_to_file)
{
	std::string bitstring;
	std::ifstream file(path_to_file, std::ios::binary); // the ios declaration means it will open in binary mode

	char c;
	while (file.get(c))
	{
		bitstring += bits_in_byte(Byte(c)).to_string(); //makes as a string of 0 and 1
	}
	return bitstring;
}

int main()
{
	std::string teste = read_bits("C:\\Users\\Aranha\\Downloads\\teste.txt");
	cout << teste << endl;
	return 0;
}