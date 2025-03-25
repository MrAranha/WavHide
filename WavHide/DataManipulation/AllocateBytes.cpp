#include "AllocateBytes.h";
#include <bitset>
#include <fstream>

using namespace std;

void AllocateBytes::Allocate(int** file, const char* path_to_file)
{
	std::ifstream fileTemp(path_to_file, std::ios::binary);

	char c;
	int i = 0;
	while (fileTemp.get(c))
	{
		unsigned long val = bits_in_byte(Byte(c)).to_ulong();
		std::bitset<8> binary(val);
		for (int j = 0; j < 8; j++)
		{
			//first val of file[i] is the msb
			file[i][7 - j] = binary[j];  // Store bits in reverse order (MSB first)
			//e.g: file[0] = { 0, 0, 0, 0, 0, 0, 0, 1 } where 1 is the least significant bit
		}
		i++;
	}
}