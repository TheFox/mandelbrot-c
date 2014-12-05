# - Try to find Imlib
# Once done this will define
#  Imlib_FOUND - System has Imlib
#  Imlib_INCLUDE_DIRS - The Imlib include directories
#  Imlib_LIBRARIES - The libraries needed to use Imlib
#  Imlib_DEFINITIONS - Compiler switches required for using Imlib

find_package(X11 REQUIRED)

MESSAGE(STATUS "Find Imlib2 header files")
find_path(Imlib_HEADER_INCLUDE_PATH Imlib2.h)

MESSAGE(STATUS "Find Imlib2 library files")
find_library(Imlib_LIBRARY_PATH Imlib2)

if(Imlib_HEADER_INCLUDE_PATH AND Imlib_LIBRARY_PATH)
	MESSAGE(STATUS "Imlib found")
	set(Imlib_FOUND On)
	set(Imlib_INCLUDE_DIRS ${Imlib_HEADER_INCLUDE_PATH})
	set(Imlib_LIBRARIES ${Imlib_LIBRARY_PATH})
else()
	MESSAGE(STATUS "Imlib_HEADER_INCLUDE_PATH: ${Imlib_HEADER_INCLUDE_PATH}")
	MESSAGE(STATUS "Imlib_LIBRARY_PATH: ${Imlib_LIBRARY_PATH}")
	set(Imlib_FOUND Off)
	if(${Imlib_FIND_REQUIRED})
		message(SEND_ERROR "Imlib not found")
	endif()
endif()
