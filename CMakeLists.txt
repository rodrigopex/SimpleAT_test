cmake_minimum_required (VERSION 2.8)
project (SimpleATTest)
include_directories(SimpleAT/)
add_executable(SimpleATTest main.c Stub.c ${CMAKE_SOURCE_DIR}/SimpleAT/SimpleAT.c)
