#pragma once
#include "../BitManipulator.h"

class CountBytes : BitManipulator {
public:
	static unsigned int Count(const char* path_to_file);
};