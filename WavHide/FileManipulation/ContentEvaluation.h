#pragma once
class ContentEvaluation {
public:
	static void ManipulateJpeg(int** file, int fileBytes, int byteIndex);
	static void ManipulateWav(int** file, int fileBytes);
};