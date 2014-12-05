# - Try to find OpenMP
# Once done this will define
#  OpenMP_FOUND - System has OpenMP
#  OpenMP_INCLUDE_DIRS - The OpenMP include directories
#  OpenMP_LIBRARIES - The libraries needed to use OpenMP
#  OpenMP_DEFINITIONS - Compiler switches required for using OpenMP

MESSAGE(STATUS "Find OpenMP header files")
find_path(OpenMP_HEADER_INCLUDE_PATH omp.h
	PATHS
		/usr/local/include
		/usr/lib/gcc/x86_64-linux-gnu/4.8/include
		/usr/lib/gcc/x86_64-linux-gnu/4.7/include
		/usr/lib/gcc/x86_64-linux-gnu/4.6/include
		#/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2/include
)

MESSAGE(STATUS "Find OpenMP library files")
find_library(OpenMP_LIBRARY_PATH gomp
	PATHS
		/usr/local/lib
		#/usr/lib/x86_64-linux-gnu
		/usr/lib/gcc/x86_64-linux-gnu/4.8
		/usr/lib/gcc/x86_64-linux-gnu/4.7
		/usr/lib/gcc/x86_64-linux-gnu/4.6
		#/usr/local/Cellar/gcc49/4.9.2/lib/gcc/x86_64-apple-darwin14.0.0/4.9.2
)

if(OpenMP_HEADER_INCLUDE_PATH AND OpenMP_LIBRARY_PATH)
	MESSAGE(STATUS "OpenMP found")
	set(OpenMP_FOUND On)
	set(OpenMP_INCLUDE_DIRS ${OpenMP_HEADER_INCLUDE_PATH})
	set(OpenMP_LIBRARIES ${OpenMP_LIBRARY_PATH})
else()
	MESSAGE(STATUS "OpenMP_HEADER_INCLUDE_PATH: ${OpenMP_HEADER_INCLUDE_PATH}")
	MESSAGE(STATUS "OpenMP_LIBRARY_PATH: ${OpenMP_LIBRARY_PATH}")
	set(OpenMP_FOUND Off)
	if(${OpenMP_FIND_REQUIRED})
		message(SEND_ERROR "OpenMP not found")
	endif()
endif()
