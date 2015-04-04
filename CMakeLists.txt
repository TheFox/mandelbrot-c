cmake_minimum_required(VERSION 2.8)
project(Mandelbrot)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_COPYRIGHT "Copyright (C) 2014 Christian Mayer <http://fox21.at>")
set(USE_OPENMP 1)
set(PROJECT_DEFINE_OPENMP_NUM_THREADS 8)
set(USE_OPENCL 0)
set(USE_OPENCL_BINARY 0)
set(USE_LLVM 0)
set(USE_MB_XY_GRID 1)
set(USE_LOG 0)
set(GRID 1)
set(PROJECT_DEFINE_GRID_STEP_PIXEL 50)
set(PROJECT_DEFINE_GRID_STEP_SIZE 5)
set(TEXT 1)
set(PROJECT_DEFINE_TEXT_OFFSET_X 5)
set(PROJECT_DEFINE_TEXT_OFFSET_Y 5)
set(PROJECT_DEFINE_COLOR_MIN 1)
set(PROJECT_DEFINE_COLOR_MAX 255)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
set(CMAKE_C_FLAGS "-Wall -funroll-loops -ffast-math")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib")


if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
	#set(USE_OPENMP Off)
endif()
if(USE_OPENMP)
	set(PROJECT_DEFINE_USE_OPENMP "#define USE_OPENMP 1")
endif()
if(USE_OPENCL)
	set(PROJECT_DEFINE_USE_OPENCL "#define USE_OPENCL 1")
endif()
if(USE_OPENCL_BINARY)
	set(PROJECT_DEFINE_USE_OPENCL_BINARY "#define USE_OPENCL_BINARY 1")
endif()
if(USE_MB_XY_GRID)
	set(PROJECT_DEFINE_USE_MB_XY_GRID "#define USE_MB_XY_GRID 1")
endif()
if(USE_LOG)
	set(PROJECT_DEFINE_USE_LOG "#define USE_LOG 1")
endif()
if(GRID)
	set(PROJECT_DEFINE_GRID "#define GRID 1")
endif()
if(TEXT)
	set(PROJECT_DEFINE_TEXT "#define TEXT 1")
endif()
configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_BINARY_DIR}/src/config.h" NEWLINE_STYLE LF)


file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/logs")
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/pics")
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/data")
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/tools")

if(USE_LLVM)
	MESSAGE(STATUS "Use LLVM")
	
	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/tools/llvm")
		execute_process(COMMAND git clone https://github.com/clang-omp/llvm WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools")
		execute_process(COMMAND git clone https://github.com/clang-omp/compiler-rt llvm/projects/compiler-rt WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools")
		execute_process(COMMAND git clone -b clang-omp https://github.com/clang-omp/clang llvm/tools/clang WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools")
		execute_process(COMMAND svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools/llvm/projects")
		execute_process(COMMAND svn co http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools/llvm/projects")
		
	endif()

	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/llvm")
		file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ../tools/llvm/configure WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND make WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND make check-libcxx WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/clang WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/clang++ WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/llvm-ar WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/llvm-link WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/llvm-nm WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
		execute_process(COMMAND ln -s Debug+Asserts/bin/llvm-ranlib WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/llvm")
	endif()
	
	set(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/llvm/clang")
	set(CMAKE_CXX_COMPILER "${PROJECT_SOURCE_DIR}/llvm/clang++")
	set(CMAKE_AR "${PROJECT_SOURCE_DIR}/llvm/llvm-ar")
	set(CMAKE_LINKER "${PROJECT_SOURCE_DIR}/llvm/llvm-link")
	set(CMAKE_NM "${PROJECT_SOURCE_DIR}/llvm/llvm-nm")
	set(CMAKE_RANLIB "${PROJECT_SOURCE_DIR}/llvm/llvm-ranlib")
	
	include_directories("${PROJECT_SOURCE_DIR}/tools/llvm/projects/libcxx/include")
	#include_directories("${PROJECT_SOURCE_DIR}/tools/llvm/projects/compiler-rt/SDKs/darwin/usr/include")
	#include_directories("${PROJECT_SOURCE_DIR}/tools/llvm/projects/compiler-rt/SDKs/linux/usr/include")
endif()

file(GLOB_RECURSE PROJECT_SOURCES "src/*.c" "src/*.cpp")
#file(GLOB_RECURSE PROJECT_HEADERS "src/*.h" "src/*.hpp")

set(PROJECT_INCLUDE_DIRS "")
foreach(_headerFile ${PROJECT_SOURCES})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND PROJECT_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES PROJECT_INCLUDE_DIRS)

find_package(Imlib REQUIRED)
find_package(OpenMP REQUIRED)
if(USE_OPENCL)
	find_package(OpenCL REQUIRED)
	
	if(${OpenCL_FOUND})
		#execute_process(COMMAND git clone https://github.com/clang-omp/llvm WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tools")
	endif()
endif()

include_directories(${PROJECT_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}/src")
#include_directories("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1")
#include_directories("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/6.0/include")
#include_directories("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include")

# Imlib2 includes
include_directories(${X11_X11_INCLUDE_PATH})
include_directories(${Imlib_INCLUDE_DIRS})

# OpenMP includes
include_directories(${OpenMP_INCLUDE_DIRS})

# OpenCL includes
if(USE_OPENCL)
	include_directories(${OpenCL_INCLUDE_DIRS})
endif()


#add_definitions(-v)
#add_definitions(-fopenmp)

add_executable(mandelbrot src/mandelbrot.cpp src/functions.cpp)
target_link_libraries(mandelbrot ${Imlib_LIBRARIES})
target_link_libraries(mandelbrot ${OpenMP_LIBRARIES})
#target_link_libraries(mandelbrot OpenCL)

add_executable(mandelbrot_mt src/mandelbrot_mt.cpp src/functions.cpp)
target_link_libraries(mandelbrot_mt ${Imlib_LIBRARIES})

add_executable(mandelbrot_ci src/mandelbrot_ci.cpp src/functions.cpp)
target_link_libraries(mandelbrot_ci ${Imlib_LIBRARIES})

add_executable(mandelbrot_test tests/mandelbrot_test.cpp src/functions.cpp)
target_link_libraries(mandelbrot_test ${Imlib_LIBRARIES})
target_link_libraries(mandelbrot_test ${OpenMP_LIBRARIES})
#target_link_libraries(mandelbrot_test ${OpenCL_LIBRARIES})

if(USE_OPENMP)
	if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
		set_target_properties(mandelbrot mandelbrot_test PROPERTIES COMPILE_FLAGS "-fopenmp")
	endif()
	if(${CMAKE_C_COMPILER_ID} MATCHES "GNU")
		set_target_properties(mandelbrot mandelbrot_test PROPERTIES COMPILE_FLAGS "-fopenmp")
	endif()
endif()

if(USE_OPENCL AND OpenCL_FOUND)
	if(${CMAKE_C_COMPILER_ID} MATCHES "Clang")
		set_target_properties(mandelbrot mandelbrot_test PROPERTIES LINK_FLAGS "-framework OpenCL")
	endif()
	if(${CMAKE_C_COMPILER_ID} MATCHES "GNU")
		set_target_properties(mandelbrot mandelbrot_test PROPERTIES LINK_FLAGS "-lOpenCL")
	endif()
	
	execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M OUTPUT_STRIP_TRAILING_WHITESPACE)
	
	MESSAGE(STATUS "Compile kernel")
	#add_custom_target(kernel ALL ${OpenCL_COMPILER_PATH} -Os -emit-llvm-bc ${PROJECT_SOURCE_DIR}/src/kernel.cl -o kernel.s WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} SOURCES ${PROJECT_SOURCE_DIR}/src/kernel.cl)
	add_custom_target(kernel_ln ALL
		ln -sf ${PROJECT_SOURCE_DIR}/src/kernel.cl
	#	DEPENDS ${PROJECT_SOURCE_DIR}/src/kernel.cl
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	#	SOURCES ${PROJECT_SOURCE_DIR}/src/kernel.cl
	)
	
	add_custom_target(kernel ALL
		${OpenCL_COMPILER_PATH} -Os -emit-llvm-bc ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernel.cl -o kernel.s
		DEPENDS kernel_ln
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

#add_executable(test1 test1.cpp)


enable_testing()

#add_test(NAME StdRuns COMMAND mandelbrot)
#set_tests_properties(StdRuns PROPERTIES WILL_FAIL true)

#add_test(NAME MtRuns COMMAND mandelbrot_mt)
#set_tests_properties(MtRuns PROPERTIES WILL_FAIL true)

#add_test(NAME CiRuns COMMAND mandelbrot_ci)
#set_tests_properties(CiRuns PROPERTIES WILL_FAIL true)

add_test(NAME TestRuns COMMAND mandelbrot_test)

if(USE_OPENMP)
	add_test(NAME TestOmp1 COMMAND mandelbrot_test)
	set_tests_properties(TestOmp1 PROPERTIES PASS_REGULAR_EXPRESSION "omp_set_num_threads: 0x")

	add_test(NAME TestOmp2 COMMAND mandelbrot_test)
	set_tests_properties(TestOmp2 PROPERTIES PASS_REGULAR_EXPRESSION "omp_get_num_threads: 0x")

	add_test(NAME TestOmp3 COMMAND mandelbrot_test)
	set_tests_properties(TestOmp3 PROPERTIES PASS_REGULAR_EXPRESSION "omp_get_thread_num: 0x")

	add_test(NAME TestOmp4 COMMAND mandelbrot_test)
	set_tests_properties(TestOmp4 PROPERTIES PASS_REGULAR_EXPRESSION "OMP i=[0-9] ID=[1-9]")
endif()

MESSAGE("")

MESSAGE(STATUS "CMAKE Version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "PROJECT_SOURCE_DIR: '${PROJECT_SOURCE_DIR}'")
MESSAGE(STATUS "PROJECT_BINARY_DIR: '${PROJECT_BINARY_DIR}'")
MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'")
MESSAGE("")

#MESSAGE(STATUS "PROJECT_NAME: '${PROJECT_NAME}'")
#MESSAGE(STATUS "PROJECT_SOURCE_DIR: '${PROJECT_SOURCE_DIR}'")
#MESSAGE(STATUS "PROJECT_BINARY_DIR: '${PROJECT_BINARY_DIR}'")
#MESSAGE("")

#MESSAGE(STATUS "Imlib_FOUND: '${Imlib_FOUND}'")
#MESSAGE(STATUS "Imlib_INCLUDE_DIRS: '${Imlib_INCLUDE_DIRS}'")
#MESSAGE(STATUS "Imlib_LIBRARIES: '${Imlib_LIBRARIES}'")
#MESSAGE(STATUS "Imlib_DEFINITIONS: '${Imlib_DEFINITIONS}'")
#MESSAGE("")

#MESSAGE(STATUS "OpenMP_FOUND: '${OpenMP_FOUND}'")
#MESSAGE(STATUS "OpenMP_INCLUDE_DIRS: '${OpenMP_INCLUDE_DIRS}'")
#MESSAGE(STATUS "OpenMP_LIBRARIES: '${OpenMP_LIBRARIES}'")
#MESSAGE(STATUS "OpenMP_DEFINITIONS: '${OpenMP_DEFINITIONS}'")
#MESSAGE("")

#MESSAGE(STATUS "OpenCL_FOUND: '${OpenCL_FOUND}'")
#MESSAGE(STATUS "OpenCL_INCLUDE_DIRS: '${OpenCL_INCLUDE_DIRS}'")
#MESSAGE(STATUS "OpenCL_LIBRARIES: '${OpenCL_LIBRARIES}'")
#MESSAGE(STATUS "OpenCL_DEFINITIONS: '${OpenCL_DEFINITIONS}'")
#MESSAGE(STATUS "OpenCL_COMPILER_PATH: '${OpenCL_COMPILER_PATH}'")
#MESSAGE("")

#MESSAGE(STATUS "CMAKE_C_COMPILER: '${CMAKE_C_COMPILER}'")
#MESSAGE(STATUS "CMAKE_C_COMPILER_ID: '${CMAKE_C_COMPILER_ID}'")
#MESSAGE(STATUS "CMAKE_C_COMPILER_VERSION: '${CMAKE_C_COMPILER_VERSION}'")
#MESSAGE(STATUS "CMAKE_C_FLAGS: '${CMAKE_C_FLAGS}'")
#MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG_INIT: '${CMAKE_C_FLAGS_DEBUG_INIT}'")
#MESSAGE(STATUS "CMAKE_COMPILER_IS_GNUCC: '${CMAKE_COMPILER_IS_GNUCC}'")
#MESSAGE(STATUS "CMAKE_C_LINK_EXECUTABLE: '${CMAKE_C_LINK_EXECUTABLE}'")
#MESSAGE(STATUS "CMAKE_C_LINK_FLAGS: '${CMAKE_C_LINK_FLAGS}'")
#MESSAGE(STATUS "CMAKE_EXE_LINKER_FLAGS: '${CMAKE_EXE_LINKER_FLAGS}'")
#MESSAGE(STATUS "CMAKE_MODULE_LINKER_FLAGS: '${CMAKE_MODULE_LINKER_FLAGS}'")
#MESSAGE(STATUS "CMAKE_SHARED_LINKER_FLAGS: '${CMAKE_SHARED_LINKER_FLAGS}'")
#MESSAGE(STATUS "LINK_DIRECTORIES: '${LINK_DIRECTORIES}'")
#MESSAGE("")

#MESSAGE(STATUS "CMAKE_CXX_COMPILER: '${CMAKE_CXX_COMPILER}'")
#MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID: '${CMAKE_CXX_COMPILER_ID}'")
#MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: '${CMAKE_CXX_COMPILER_VERSION}'")
#MESSAGE(STATUS "CMAKE_CXX_FLAGS: '${CMAKE_CXX_FLAGS}'")
#MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG_INIT: '${CMAKE_CXX_FLAGS_DEBUG_INIT}'")
#MESSAGE(STATUS "CMAKE_COMPILER_IS_GNUCXX: '${CMAKE_COMPILER_IS_GNUCXX}'")
#MESSAGE(STATUS "CMAKE_CXX_LINK_EXECUTABLE: '${CMAKE_CXX_LINK_EXECUTABLE}'")
#MESSAGE(STATUS "CMAKE_CXX_LINK_FLAGS: '${CMAKE_CXX_LINK_FLAGS}'")
#MESSAGE("")

#MESSAGE(STATUS "CMAKE_AR: '${CMAKE_AR}'")
#MESSAGE(STATUS "CMAKE_LINKER: '${CMAKE_LINKER}'")
#MESSAGE(STATUS "CMAKE_NM: '${CMAKE_NM}'")
#MESSAGE(STATUS "CMAKE_OBJDUMP: '${CMAKE_OBJDUMP}'")
#MESSAGE(STATUS "CMAKE_RANLIB: '${CMAKE_RANLIB}'")
#MESSAGE("")

#MESSAGE(STATUS "CMAKE_PREFIX_PATH: '${CMAKE_PREFIX_PATH}'")
#MESSAGE(STATUS "CMAKE_MODULE_PATH: '${CMAKE_MODULE_PATH}'")
#MESSAGE(STATUS "CMAKE_BINARY_DIR: '${CMAKE_BINARY_DIR}'")
#MESSAGE(STATUS "CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
#MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'")
#MESSAGE(STATUS "CMAKE_COMMAND: '${CMAKE_COMMAND}'")
#MESSAGE(STATUS "CMAKE_CONFIGURATION_TYPES: '${CMAKE_CONFIGURATION_TYPES}'")
#MESSAGE(STATUS "CMAKE_FRAMEWORK_PATH: '${CMAKE_FRAMEWORK_PATH}'")
#MESSAGE(STATUS "CMAKE_APPBUNDLE_PATH: '${CMAKE_APPBUNDLE_PATH}'")
#MESSAGE(STATUS "CMAKE_SYSTEM_PREFIX_PATH: '${CMAKE_APPBUNDLE_PATH}'")
#MESSAGE(STATUS "CMAKE_SYSTEM_FRAMEWORK_PATH: '${CMAKE_SYSTEM_FRAMEWORK_PATH}'")
#MESSAGE(STATUS "CMAKE_SYSTEM_APPBUNDLE_PATH: '${CMAKE_SYSTEM_APPBUNDLE_PATH}'")
#MESSAGE("")
