
# add getlib library
set(getlib_src 
	Cinteger.c 
	Gcommand.c 
	Gsignal.c 
	Preproc.c 
	get_Util.c 
	Clogical.c 
	Gerr.c 
	Icommand.c 
	Putpromp.c 
	globals.c 
	Creal.c	
	Getnbl.c 
	Index.c 
	Real.c 
	linteger.c 
	Filename.c
	Ginput.c
	Integer.c
	Scommand.c 
	xcommand.c 
	Filename_ext.c 
	Gitem.c 
	Logical.c 
	Strtrm.c
	)
ADD_LIBRARY(getlib STATIC ${getlib_src})

# test getlib library
add_executable(testGetlib EXCLUDE_FROM_ALL test.c)
target_link_libraries(testGetlib getlib)

add_test(NAME testGetlib_run COMMAND $<TARGET_FILE:testGetlib>)
add_dependencies(check testGetlib)

