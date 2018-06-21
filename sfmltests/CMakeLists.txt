cmake_minimum_required(VERSION 3.5)
project("SFMLtests")
file(GLOB source_files *.?pp)

if(MSVC)
	set(SFML_PATH "C:/Program Files/SFML-2.4.2")
	set(SFML_BIN_DIR ${SFML_PATH}/bin)
    set(SFML_INCLUDE_DIR ${SFML_PATH}/include)
    set(SFML_LIBRARY_DIRS ${SFML_PATH}/lib)
    set(SFML_LIBRARIES sfml-system.lib sfml-window.lib sfml-graphics.lib sfml-audio.lib)

	set(TARGET_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Release)
	file(GLOB files ${SFML_BIN_DIR}/*)
    foreach(file ${files})
        file(COPY ${file} DESTINATION ${TARGET_BINARY_DIR})
    endforeach()
else()

add_definitions("-O3 -std=c++17 -lstdc++")

endif()

include_directories(${include_directories}
                    ${SFML_INCLUDE_DIR})

link_directories(   ${link_directories}
                    ${SFML_LIBRARY_DIRS} )

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(  ${PROJECT_NAME}
                        ${SFML_LIBRARIES})
