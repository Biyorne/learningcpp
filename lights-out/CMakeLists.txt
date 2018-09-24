cmake_minimum_required(VERSION 3.8)
project("lights-out")

if(MSVC)
    add_definitions("-DWIN32_LEAN_AND_MEAN /MP /W4 /w14254 /w14263 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14640 /w14826 /w14905 /w14906 /w14928")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL AppleClang)
    add_definitions("-DNDEBUG -O3 -lstdc++ -Weverything -Wno-error -Wno-unused-command-line-argument -Wno-c++98-compat-pedantic -Wno-covered-switch-default -Wno-c++98-compat -Wno-global-constructors -Wno-exit-time-destructors -Wno-padded -Wno-weak-vtables -Wno-deprecated -Wno-double-promotion -Wno-missing-prototypes -Wno-string-conversion")
endif()

find_package(SFML 2.5 COMPONENTS window graphics audio REQUIRED)

file(GLOB source_files *.?pp)
add_executable(${PROJECT_NAME} ${source_files})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(  ${PROJECT_NAME}
                        sfml-window sfml-graphics sfml-audio)