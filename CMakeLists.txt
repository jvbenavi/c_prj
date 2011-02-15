cmake_minimum_required (VERSION 2.6)
project (prj)

# Ensures that we do an out of source build
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)

GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir)

IF(insource OR insourcesubdir)
    MESSAGE(FATAL_ERROR "${PROJECT_NAME} requires an out of source build. Please, see README.")
ENDIF(insource OR insourcesubdir)

# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

# gcc flags
if (CMAKE_COMPILER_IS_GNUCC)
    #set_source_files_properties(${EXAMPLE_SRC} PROPERTIES COMPILE_FLAGS "-std=c99 -Werror -Wall -pedantic")
	ADD_DEFINITIONS(-g -fsigned-char -Wall -Wextra -Wshadow -Wstrict-prototypes 
	                -Wpointer-arith -Wcast-qual -Winline -Werror)
					# -pedantic -std=c99 
endif(CMAKE_COMPILER_IS_GNUCC) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

include_directories(inc)

add_subdirectory(src)

