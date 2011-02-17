#include <stdio.h>
#include "define.h"

/* getlib.h */
int get_errorNumber		= 0;
int get_externalError	= 0;
int get_flag			= 0;

/* getnb.h */
int		get_verbose			= 1;		/* This variable is set in preproc.c and init in preproc.h */
int		get_expandArguments	= 1;		/* This variable is set in preproc.c and init in preproc.h */
int		get_log				= 1;
int		get_preproc;
FILE	*get_logPtr			= NULL;		/* This variable is set in preproc.c and init in preproc.h */
char get_logFilename[64]= "getlib.log";	/* This variable is set in preproc.c and init in preproc.h */

/* Gsignal.c */
FILE	*indirect_file_ptr[p_max_indirect_files];
int		indirect_file_count	= 0;
