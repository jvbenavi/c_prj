
cmake_minimum_required (VERSION 2.8)
project (prj)

# Ensures that we do an out of source build
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir)
IF(insource OR insourcesubdir)
    MESSAGE(FATAL_ERROR "${PROJECT_NAME} requires an out of source build. Please, see README.txt")
ENDIF(insource OR insourcesubdir)

#find_package(Matlab) #outdated, doesn't work
if(UNIX)
	#set(MATLAB_INCLUDE_DIR /Applications/MATLAB_R2011a.app/extern/include)
	set(MATLAB_INCLUDE_DIR /Applications/MATLAB_R2010a.app/extern/include)
else(UNIX) # WIN32
	#set(MATLAB_INCLUDE_DIR C:\Programs\MATLAB_R1010a\extern\include)
endif(UNIX)

# The version number. (currently unused)
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 2)

# gcc flags
if (CMAKE_COMPILER_IS_GNUCC)
	ADD_DEFINITIONS(-g -fsigned-char -Wall -Wextra -Wshadow -Wstrict-prototypes 
	                -Wpointer-arith -Wcast-qual -Winline -pedantic -ansi) # -Werror
endif(CMAKE_COMPILER_IS_GNUCC) 

# designate output directores for executibles and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# enable testing
enable_testing()

# setup check target to compile & run tests
set(CMAKE_CTEST_COMMAND ctest -V)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# add inc directory for all the header files
include_directories(inc)
# add lib directory for all the libraries
cmake_policy(SET CMP0015 NEW)
link_directories(lib)

# procede to find and compile all source files
add_subdirectory(src)

