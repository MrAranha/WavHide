# CMakeList.txt : CMake project for WavHide, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (wavhide "wavhide.cpp" "DataManipulation/WriteBinary.cpp" "DataManipulation/WriteBinary.h" "DataManipulation/AllocateBytes.h" "DataManipulation/AllocateBytes.cpp" "DataManipulation/CountBytes.cpp" "DataManipulation/CountBytes.h" "BitManipulator.cpp" "BitManipulator.h" "DataManipulation/WavHeader.cpp"  "DataManipulation/HeaderWavExtractor.h" "DataManipulation/HeaderWavExtractor.cpp" "DataManipulation/ByteConverter.cpp" "DataManipulation/ByteConverter.h" "FileManipulation/ContentEvaluation.cpp"  )
set_target_properties(wavhide PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
)

# Install rule
install(TARGETS wavhide DESTINATION /bin)