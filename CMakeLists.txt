cmake_minimum_required(VERSION 3.25)
project(walker)

set(CMAKE_CXX_STANDARD 17)

add_executable(walker main.cpp scanner/Scanner.cpp scanner/Scanner.h scanner/Scanner.cpp scanner/ScanUtils.cpp scanner/ScanUtils.h lib/json.h scanner/StructureParser.cpp scanner/StructureParser.h)
