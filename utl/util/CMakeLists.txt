

# add the MathFunctions library
#set (EXTRA_LIBS ${EXTRA_LIBS} util)

set(util_lib_src util_file.c util_net.c util_math.c)

ADD_LIBRARY(util STATIC ${util_lib_src})

include_directories(.)
