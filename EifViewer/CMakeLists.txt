cmake_minimum_required(VERSION 3.10)

#project(eifconverter)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Local include for malloc.h on OSX
if(APPLE)
  include_directories(./osx/)
endif()
add_library(eif EifConverter.cpp ./EasyBMP/EasyBMP.cpp ./exoquant/exoquant.c)
add_executable(eifconverter  main.cpp)
target_link_libraries(eifconverter eif)
include_directories(../cxxopts/include)
include_directories(../CRCpp/inc)
include_directories(../utils)
include_directories(./EasyBMP)
include_directories(./exoquant)

install(TARGETS eifconverter)