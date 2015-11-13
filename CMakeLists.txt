cmake_minimum_required (VERSION 2.8)
project (FSloume)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package (SFML 2 COMPONENTS system window graphics)
include_directories (${SFML_INCLUDE_DIRS})
set (LIBS ${LIBS} ${SFML_LIBRARIES})

file (GLOB_RECURSE source_files src/*)

#	g++ -Wall -Wextra -pedantic -g -std=c++11 -lsfml-system -lsfml-window -lsfml-graphics main.cpp slime.cpp ball.cpp
add_executable (fsloume ${source_files})
target_link_libraries (fsloume ${LIBS})
