cmake_minimum_required(VERSION 2.6)
project(Mandelbrot)

set(Mandelbrot_VERSION_MAJOR 1)
set(Mandelbrot_VERSION_MINOR 0)
set(Mandelbrot_VERSION_PATCH 0)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-4.9")
set(CMAKE_C_FLAGS "-Wall -O3 -funroll-loops -fopenmp")

#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-4.9")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#execute_process(COMMAND imlib2-config --cflags OUTPUT_VARIABLE IMLIB_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#execute_process(COMMAND imlib2-config --libs OUTPUT_VARIABLE IMLIB_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")



file(GLOB_RECURSE Mandelbrot_SOURCES "src/*.c" "src/*.cpp")
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

# Imlib2 includes
include_directories("/opt/X11/include")
include_directories("/usr/local/Cellar/imlib2/1.4.6_1/include")
link_directories("/usr/local/Cellar/imlib2/1.4.6_1/lib")

# OpenMP includes
include_directories("/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2/include")
link_directories("/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2")


#add_definitions(-v)

add_executable(mandelbrot ${Mandelbrot_SOURCES})
target_link_libraries(mandelbrot m)
target_link_libraries(mandelbrot Imlib2)

add_executable(mandelbrot_test tests/mandelbrot_test.cpp)
target_link_libraries(mandelbrot m)
target_link_libraries(mandelbrot_test Imlib2)


enable_testing()

add_test(NAME StdRuns COMMAND mandelbrot)
set_tests_properties(StdRuns PROPERTIES WILL_FAIL true)

add_test(NAME TestRuns COMMAND mandelbrot_test)




MESSAGE(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
MESSAGE(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
MESSAGE("")

MESSAGE(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG_INIT: ${CMAKE_C_FLAGS_DEBUG_INIT}")
MESSAGE("")

MESSAGE(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG_INIT: ${CMAKE_CXX_FLAGS_DEBUG_INIT}")
MESSAGE("")

MESSAGE(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
MESSAGE("")

#MESSAGE(STATUS "IMLIB_CFLAGS: ${IMLIB_CFLAGS}")
#MESSAGE(STATUS "IMLIB_LIBS: ${IMLIB_LIBS}")
#MESSAGE("")

#MESSAGE(STATUS "TARGET: ${TARGET}")
#MESSAGE(STATUS "TARGET_BASE: ${TARGET_BASE}")
#MESSAGE(STATUS "FLAGS: ${FLAGS}")
#MESSAGE("")
