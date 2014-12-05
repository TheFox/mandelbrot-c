cmake_minimum_required(VERSION 2.6)
project(Mandelbrot)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(DEBUG 1)
set(USE_OPENMP 1)
set(USE_MB_XY_GRID 1)
set(GRID 1)
set(PROJECT_DEFINE_GRID_STEP_PIXEL 50)
set(PROJECT_DEFINE_GRID_STEP_SIZE 5)
set(TEXT 1)
set(PROJECT_DEFINE_TEXT_OFFSET_X 5)
set(PROJECT_DEFINE_TEXT_OFFSET_Y 5)
set(PROJECT_DEFINE_COLOR_MIN 1)
set(PROJECT_DEFINE_COLOR_MAX 255)


set(CMAKE_C_COMPILER "/usr/local/bin/gcc-4.9")
set(CMAKE_C_FLAGS "-Wall -O3 -funroll-loops -fopenmp")

set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-4.9")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


if(DEBUG)
	set(PROJECT_DEFINE_DEBUG "#define DEBUG")
endif()
if(USE_OPENMP)
	set(PROJECT_DEFINE_USE_OPENMP "#define USE_OPENMP")
endif()
if(USE_MB_XY_GRID)
	set(PROJECT_DEFINE_USE_MB_XY_GRID "#define USE_MB_XY_GRID")
endif()
if(GRID)
	set(PROJECT_DEFINE_GRID "#define GRID")
endif()
if(TEXT)
	set(PROJECT_DEFINE_TEXT "#define TEXT")
endif()
configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h" NEWLINE_STYLE LF)



file(GLOB_RECURSE PROJECT_SOURCES "src/*.c" "src/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "src/*.h")

find_path(STDIO_INCLUDE_PATH stdio.h)

set(PROJECT_INCLUDE_DIRS "")
foreach(_headerFile ${PROJECT_HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND PROJECT_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES PROJECT_INCLUDE_DIRS)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_INCLUDE_DIRS})
include_directories(${STDIO_INCLUDE_PATH})

# Imlib2 includes
include_directories("/opt/X11/include")
include_directories("/usr/local/Cellar/imlib2/1.4.6_1/include")
link_directories("/usr/local/Cellar/imlib2/1.4.6_1/lib")

# OpenMP includes
include_directories("/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2/include")
link_directories("/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2")


#add_definitions(-v)
#add_definitions(-fopenmp)

add_executable(mandelbrot ${PROJECT_SOURCES})
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
