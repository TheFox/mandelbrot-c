# - Try to find OpenCL
# Once done this will define
#  OpenCL_FOUND - System has OpenCL
#  OpenCL_INCLUDE_DIRS - The OpenCL include directories
#  OpenCL_LIBRARIES - The libraries needed to use OpenCL
#  OpenCL_DEFINITIONS - Compiler switches required for using OpenCL

MESSAGE(STATUS "Find OpenCL header files")
find_path(OpenCL_HEADER_INCLUDE_PATH NAMES OpenCL/opencl.h CL/cl.h)

MESSAGE(STATUS "Find OpenCL library files")
find_library(OpenCL_LIBRARY_PATH OpenCL)



if(OpenCL_HEADER_INCLUDE_PATH AND OpenCL_LIBRARY_PATH)
	MESSAGE(STATUS "Find OpenCL compiler")
	
	#find_path(OpenCL_COMPILER_PATH NAMES openclc PATHS ${OpenCL_LIBRARY_PATH} ${OpenCL_HEADER_INCLUDE_PATH})
	
	#file(GLOB_RECURSE OpenCL_COMPILER_PATH RELATIVE ${OpenCL_LIBRARY_PATH} FOLLOW_SYMLINKS "*openclc")
	#file(GLOB_RECURSE OpenCL_COMPILER_PATH RELATIVE "*openclc")
	
	if(APPLE)
		file(GLOB OpenCL_COMPILER_PATH "${OpenCL_LIBRARY_PATH}/Libraries/openclc")
    endif()
    
	#MESSAGE(STATUS "OpenCL_COMPILER_PATH: '${OpenCL_COMPILER_PATH}'")
	#foreach(infileName ${OpenCL_COMPILER_PATH})
	#	MESSAGE(STATUS "Process file: ${infileName}")
	#endforeach()
	#MESSAGE(STATUS "OpenCL_LIBRARY_PATH: '${OpenCL_LIBRARY_PATH}'")
	#MESSAGE(SEND_ERROR "test")
	
	MESSAGE(STATUS "OpenCL found")
	set(OpenCL_FOUND On)
	set(OpenCL_INCLUDE_DIRS ${OpenCL_HEADER_INCLUDE_PATH})
	set(OpenCL_LIBRARIES ${OpenCL_LIBRARY_PATH})
else()
	MESSAGE(STATUS "OpenCL_HEADER_INCLUDE_PATH: ${OpenCL_HEADER_INCLUDE_PATH}")
	MESSAGE(STATUS "OpenCL_LIBRARY_PATH: ${OpenCL_LIBRARY_PATH}")
	set(OpenCL_FOUND Off)
	if(${OpenCL_FIND_REQUIRED})
		message(SEND_ERROR "OpenCL not found")
	endif()
endif()
