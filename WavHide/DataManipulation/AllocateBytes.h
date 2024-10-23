#pragma once
#include "../BitManipulator.h"

class AllocateBytes : BitManipulator {
public:
	static void Allocate(int** file, const char* path_to_file);
};