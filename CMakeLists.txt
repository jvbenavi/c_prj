cmake_minimum_required (VERSION 2.6)
project (prj)

# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

if (CMAKE_COMPILER_IS_GNUCC)
    #set_source_files_properties(${EXAMPLE_SRC} PROPERTIES COMPILE_FLAGS "-std=c99 -Werror -Wall -pedantic")
	ADD_DEFINITIONS(-g -fsigned-char -Wall -Wextra -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Winline -Werror)
	# -pedantic -std=c99 
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

include_directories (inc)

add_subdirectory(src)

# add custom target distclean
# cleans and removes cmake generated files etc.
# Jan Woetzel 04/2003
#
IF (UNIX)
  ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
  SET(DISTCLEANED
    cmake.depends
    cmake.check_depends
    CMakeCache.txt
    cmake.check_cache
    *.cmake
    Makefile
	CMakeFiles
    )
								   
  ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS    -Rf ${DISTCLEANED}
    TARGET  distclean
    )
ENDIF(UNIX)
