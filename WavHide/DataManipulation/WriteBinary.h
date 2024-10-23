// WavHide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

class WriteBinary
{
public:
	static void WriteText(int** file, int bytes, const char* path_to_file);
	static void WriteJpeg(int** file, int bytes, const char* path_to_file);
	static void WriteWav(int** file, int bytes, const char* path_to_file, int sampleRate, short numChannels, short bitsPerSample);
};
