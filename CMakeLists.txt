cmake_minimum_required (VERSION 2.6)
project (prj)

# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

ADD_DEFINITIONS(-g -O2 -fsigned-char -freg-struct-return -Wall -W -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Winline -Werror)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories (inc lib)

add_subdirectory(utl)
add_subdirectory(exe)

