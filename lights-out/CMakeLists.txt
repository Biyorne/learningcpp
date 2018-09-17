cmake_minimum_required(VERSION 3.10)
project("lights-out")
file(GLOB source_files *.?pp)

if(MSVC)
    add_definitions("-DWIN32_LEAN_AND_MEAN /MP /W4 /w14254 /w14263 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14640 /w14826 /w14905 /w14906 /w14928")

	set(SFML_PATH "C:/Program Files/SFML-2.4.2")
	set(SFML_BIN_DIR ${SFML_PATH}/bin)
    set(SFML_INCLUDE_DIR ${SFML_PATH}/include)
    set(SFML_LIBRARY_DIRS ${SFML_PATH}/lib)
    set(SFML_LIBRARIES sfml-system.lib sfml-window.lib sfml-graphics.lib sfml-audio.lib)

	set(TARGET_BINARY_DIR_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Release)
	set(TARGET_BINARY_DIR_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(GLOB dll_files ${SFML_BIN_DIR}/*)
    foreach(file ${dll_files})  
        file(COPY ${file} DESTINATION ${TARGET_BINARY_DIR_RELEASE})
		file(COPY ${file} DESTINATION ${TARGET_BINARY_DIR_DEBUG})
    endforeach()
else()
    add_definitions("-DNDEBUG -O3 -lstdc++ -Weverything -Wno-error -Wno-unused-command-line-argument -Wno-c++98-compat-pedantic -Wno-covered-switch-default -Wno-c++98-compat -Wno-global-constructors -Wno-exit-time-destructors -Wno-padded -Wno-weak-vtables -Wno-deprecated -Wno-double-promotion -Wno-missing-prototypes -Wno-string-conversion")

    set(SFML_INCLUDE_DIR "-I/usr/include/SFML")
    set(SFML_LIBRARY_DIRS "/usr/lib")
    set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-audio)
endif()

include_directories(${include_directories}
                    ${SFML_INCLUDE_DIR})

link_directories(   ${link_directories}
                    ${SFML_LIBRARY_DIRS} )

add_executable(${PROJECT_NAME} ${source_files})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(  ${PROJECT_NAME}
                        ${SFML_LIBRARIES})
