
add_executable (prj prj_main.c func1.c func2.c)  
target_link_libraries (prj util)

add_test(NAME testPrj COMMAND $<TARGET_FILE:prj>)
add_dependencies(check prj)

