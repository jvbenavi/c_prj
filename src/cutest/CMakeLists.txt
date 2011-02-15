
ADD_LIBRARY(cutest STATIC CuTest.c)

add_executable(testCuTest AllTests.c CuTestTest.c)
target_link_libraries(testCuTest cutest)

