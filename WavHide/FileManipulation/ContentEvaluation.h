#pragma once
#include <string>

class ContentEvaluation {
public:
	static void ManipulateJpeg(int** file, int fileBytes, int byteIndex);
	static int** WriteTextOnWav(int** file, int fileBytes, const char* textToWrite);
	static void WavSampleHalver(int** file, int fileBytes);
	static std::string ExtractMessageFromWav(int** file, int fileBytes);
};
