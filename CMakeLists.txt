cmake_minimum_required(VERSION 2.6)
project(Mandelbrot)

set(Mandelbrot_VERSION_MAJOR 1)
set(Mandelbrot_VERSION_MINOR 0)
set(Mandelbrot_VERSION_PATCH 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")

MESSAGE(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
MESSAGE("")

file(GLOB_RECURSE Mandelbrot_SOURCES "src/*.c")
file(GLOB_RECURSE Mandelbrot_HEADERS "src/*.h")

find_path(STDIO_INCLUDE_PATH stdio.h)

set(Mandelbrot_INCLUDE_DIRS "")
foreach(_headerFile ${Mandelbrot_HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND Mandelbrot_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Mandelbrot_INCLUDE_DIRS)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Mandelbrot_INCLUDE_DIRS})
include_directories(${STDIO_INCLUDE_PATH})
include_directories("/opt/X11/include")
include_directories("/usr/local/Cellar/imlib2/1.4.6_1/include")
link_directories("/usr/local/Cellar/imlib2/1.4.6_1/lib")

add_definitions(-Wall)
add_definitions(-O3)

add_executable(mandelbrot ${Mandelbrot_SOURCES})
target_link_libraries(mandelbrot Imlib2)

add_executable(mandelbrot_test tests/mandelbrot_test.c)
target_link_libraries(mandelbrot_test Imlib2)


enable_testing()

add_test(NAME TestRuns COMMAND mandelbrot_test)


MESSAGE(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
MESSAGE(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
MESSAGE("")

MESSAGE(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
MESSAGE("")
