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