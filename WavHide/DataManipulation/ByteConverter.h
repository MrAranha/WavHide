#pragma once
class ByteConverter {
public:
	static unsigned int ByteToInt(int** data, int byteIndex, int byteCount);
	static void DecrementByte(int** data, int byteIndex);
};