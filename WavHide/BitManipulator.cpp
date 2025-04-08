#include <iostream>
#include <limits>
#include <fstream>
#include "BitManipulator.h"

// Type alias for Byte and defining bits per byte

// Base class to handle bit manipulation
// Converts a bitset to a byte
Byte BitManipulator::bitsetToByte(const bits_in_byte& bitset) {
    Byte result = 0;
    for (std::size_t i = 0; i < BITS_PER_BYTE; ++i) {
        result |= (bitset[i] << i);  // Shift and combine bits
    }
    return result;
}

// Converts an array of bits (e.g., int[8]) to a byte
Byte BitManipulator::bitsArrayToByte(const int* bitsArray) {
    Byte result = 0;
    for (std::size_t i = 0; i < BITS_PER_BYTE; ++i) {
        result |= (bitsArray[i] << i);  // Combine bits directly
    }
    return result;
}