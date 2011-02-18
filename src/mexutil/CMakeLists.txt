
# add mex util library
include_directories(${MATLAB_INCLUDE_DIR})
ADD_LIBRARY(mexutil STATIC mexutil.c)

