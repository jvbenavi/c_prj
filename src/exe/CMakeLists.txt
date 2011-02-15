
# add the executable
add_executable (prj prj_main.c func1.c func2.c)  

target_link_libraries (prj util)
include_directories("${PROJECT_SOURCE_DIR}/src/utl/util")

