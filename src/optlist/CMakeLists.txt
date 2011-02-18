
# add optlist library
ADD_LIBRARY(optlist STATIC optlist.c)

# test optlist library
add_executable(testOptlist EXCLUDE_FROM_ALL sample.c)
target_link_libraries(testOptlist optlist)

add_test(NAME testOptlist_run COMMAND $<TARGET_FILE:testOptlist>)
add_dependencies(check testOptlist)

