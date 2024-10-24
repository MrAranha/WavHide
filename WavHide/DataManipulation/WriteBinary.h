// WavHide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "WavHeader.cpp"

// TODO: Reference additional headers your program requires here.

class WriteBinary
{
public:
	static void WriteText(int** file, int bytes, const char* path_to_file);
	static void WriteFile(int** file, int bytes, const char* path_to_file);
};
