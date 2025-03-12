#pragma once
class ContentEvaluation {
public:
	static void ManipulateJpeg(int** file, int fileBytes, int byteIndex);
	static void WriteTextOnWav(int** file, int fileBytes, const char* textToWrite);
	static void WavSampleHalver(int** file, int fileBytes);
	static void ExtractMessageFromWav(int** file, int fileBytes);
};