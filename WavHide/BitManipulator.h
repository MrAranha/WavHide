// WavHide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <bitset>
#include <limits>

// TODO: Reference additional headers your program requires here.
using Byte = unsigned char;
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<Byte>::digits;
using bits_in_byte = std::bitset<BITS_PER_BYTE>;  // Alias for bit representation of a byte

class BitManipulator {
protected:
    // Converts a bitset to a byte
    Byte bitsetToByte(const bits_in_byte& bitset) const;

    // Converts an array of bits (e.g., int[8]) to a byte
    Byte bitsArrayToByte(const int* bitsArray) const;
};