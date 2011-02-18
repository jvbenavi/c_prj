
# add util library
set(lib_util_src util_file.c util_net.c util_math.c)
ADD_LIBRARY(util STATIC ${lib_util_src})

# test util library
add_executable(testUtil EXCLUDE_FROM_ALL AllTests.c util_test.c)
target_link_libraries(testUtil cutest util)

add_test(NAME testUtl COMMAND $<TARGET_FILE:testUtil>)
add_dependencies(check testUtil)

