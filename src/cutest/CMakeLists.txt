
# add cutest library
ADD_LIBRARY(cutest STATIC CuTest.c)

# test cutest library
add_executable(testCuTest EXCLUDE_FROM_ALL AllTests.c CuTestTest.c)
target_link_libraries(testCuTest cutest)

add_test(NAME testCu COMMAND $<TARGET_FILE:testCuTest>)
add_dependencies(check testCuTest)

