cmake_minimum_required(VERSION 3.8)
project("meth-heads" VERSION 1.0.0 LANGUAGES CXX)


file(GLOB source_files *.?pp)
add_executable(${PROJECT_NAME} ${source_files})


find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)

target_link_libraries(  ${PROJECT_NAME}
                        sfml-window sfml-graphics)


#compiler/linker options
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

    target_compile_options(${PROJECT_NAME} PUBLIC /std:c++17 -DWIN32_LEAN_AND_MEAN /MP /W4 /w14254 /w14263 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14640 /w14826 /w14905 /w14906 /w14928)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    target_compile_options(${PROJECT_NAME} PUBLIC -DNDEBUG -O3 -lstdc++ -std=c++17 -Weverything -Wno-error -Wno-unused-command-line-argument -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-padded -Wno-weak-vtables -Wno-disabled-macro-expansion -Wno-deprecated -Wno-covered-switch-default -Wno-inconsistent-missing-destructor-override)
    
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

	target_compile_options(${PROJECT_NAME} PUBLIC -DNDEBUG -O3 -std=c++17 -lstdc++ -ffast-math -DBOOST_NO_AUTO_PTR -pedantic -Wall -Wextra -Wshadow -Wfloat-equal -Wundef -Wswitch-default -Wswitch-enum -Wunreachable-code -Weffc++ -Wunused-parameter -Wfatal-errors -Wstrict-null-sentinel -Wconversion -Wsign-conversion)

else()

    message(FATAL_ERROR " Unknwon Compiler: ${CMAKE_CXX_COMPILER_ID}")

endif()

