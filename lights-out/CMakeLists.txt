cmake_minimum_required(VERSION 3.8)
project("lights-out" VERSION 1.0.0 LANGUAGES CXX)


file(GLOB source_files *.?pp)
add_executable(${PROJECT_NAME} ${source_files})


find_package(SFML 2.5 COMPONENTS window graphics audio REQUIRED)

target_link_libraries(  ${PROJECT_NAME}
                        sfml-window sfml-graphics sfml-audio)


#compiler/linker options
if(MSVC)

    target_compile_options(${PROJECT_NAME} PUBLIC /std:c++17 -DWIN32_LEAN_AND_MEAN /MP /W4 /w14254 /w14263 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14640 /w14826 /w14905 /w14906 /w14928)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL AppleClang)

    target_compile_options(${PROJECT_NAME} PUBLIC -DNDEBUG -O3 -lstdc++ -std=c++17 -Weverything -Wno-error -Wno-unused-command-line-argument -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-padded -Wno-weak-vtables -Wno-disabled-macro-expansion -Wno-deprecated -Wno-covered-switch-default -Wno-inconsistent-missing-destructor-override)
    
    option(ADDRESS_SANITIZER "Using Clang's Address Sanitizer" OFF)

    if(ADDRESS_SANITIZER)
	message(" *** Using Clang's Address Sanitizer ***")
    
        target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)

        target_link_libraries(${PROJECT_NAME} -fsanitize=address -fno-omit-frame-pointer)
    endif()

endif()
