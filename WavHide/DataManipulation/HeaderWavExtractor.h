#include "WavHeader.cpp"

class HeaderWavExtractor {
public:
	static unsigned int ExtractBitAsInt(int** file, int byteIndex, int byteCount);
	static void ExtractWAVHeader(int** data, WAV_HEADER* header);
};