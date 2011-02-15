
set(util_lib_src util_file.c util_net.c util_math.c)

ADD_LIBRARY(util STATIC ${util_lib_src})

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

