
set(util_lib_src util_file.c util_net.c util_math.c)

ADD_LIBRARY(util STATIC ${util_lib_src})

add_executable(testUtil AllTests.c util_test.c)
target_link_libraries(testUtil cutest util)

