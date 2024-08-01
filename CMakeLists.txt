cmake_minimum_required(VERSION 3.19)
project(namedoptions VERSION 0.1 LANGUAGES CXX)

enable_testing()

add_subdirectory(namedoptions)
add_subdirectory(tests)

add_custom_target(namedoptionsdocs SOURCES LICENSE README.md)
