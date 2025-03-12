# CMakeList.txt : CMake project for WavHide, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (WavHide "WavHide.cpp" "WavHide.h" "DataManipulation/WriteBinary.cpp" "DataManipulation/WriteBinary.h" "DataManipulation/AllocateBytes.h" "DataManipulation/AllocateBytes.cpp" "DataManipulation/CountBytes.cpp" "DataManipulation/CountBytes.h" "BitManipulator.cpp" "BitManipulator.h" "DataManipulation/WavHeader.cpp"  "DataManipulation/HeaderWavExtractor.h" "DataManipulation/HeaderWavExtractor.cpp" "DataManipulation/ByteConverter.cpp" "DataManipulation/ByteConverter.h" "FileManipulation/ContentEvaluation.cpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WavHide PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.