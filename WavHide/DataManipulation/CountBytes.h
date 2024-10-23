#pragma once
#include "../BitManipulator.h"

class CountBytes : BitManipulator {
public:
	unsigned int Count(const char* path_to_file);
};