#include <fstream>
#include <iostream>

typedef struct WAV_HEADER {
    char riffHeader[4];    // "RIFF"
    int wavSize;           // File size - 8 bytes
    char waveHeader[4];    // "WAVE"
    char fmtHeader[4];     // "fmt "
    int fmtChunkSize;      // Size of the fmt chunk (16 for PCM)
    short audioFormat;     // Audio format (1 for PCM)
    short numChannels;     // Number of channels (1 for mono, 2 for stereo)
    int sampleRate;        // Sample rate (44100, 48000, etc.)
    int byteRate;          // byteRate = sampleRate * numChannels * bitsPerSample / 8
    short blockAlign;      // blockAlign = numChannels * bitsPerSample / 8
    short bitsPerSample;   // Bits per sample (8, 16, 24, or 32)
    char dataHeader[4];    // "data"
    int dataSize;          // Number of bytes in the data section
} wav_hdr;