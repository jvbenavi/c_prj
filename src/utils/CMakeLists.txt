
# -DNOWARN_UNUSED_PARAM -DNO_LOGICALS

# add utils library
set(lib_utils_src 
	deadband.c
	filter2.c
	fix.c
	interp.c
	lag.c
	leadlag.c
	limit.c
	load.c
	logger.c
	lowercase.c
	matrix.c
	mt19937.c
	ra.c
	randu.c
	randn.c
	roundz.c
	row_vector_matrix_ops.c
	sign.c
	soddir.c
	sodinv.c
	strsub.c
	trimit.c
	vector.c
	vector_operators.c
	zlfi1u.c
	zlfi2u.c
	zlfi3u.c
	ztable_load.c
	)
ADD_LIBRARY(utils STATIC ${lib_utils_src})

# test utils library
#add_executable(testUtil EXCLUDE_FROM_ALL AllTests.c util_test.c)
#target_link_libraries(testUtil cutest util)
#
#add_test(NAME testUtl COMMAND $<TARGET_FILE:testUtil>)
#add_dependencies(check testUtil)

