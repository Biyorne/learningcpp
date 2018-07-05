cmake_minimum_required(VERSION 3.5)
project("arrays")
file(GLOB source_files *.?pp)

if(MSVC)
else()

add_definitions("-O3 -std=c++17 -lstdc++")

endif()

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME})
