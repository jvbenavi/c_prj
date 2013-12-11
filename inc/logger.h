#ifndef LOGGER_H_INC
#define LOGGER_H_INC

/* AUTOGENERATED From File: ../sysutil/logger.c at Thu Jul 11 02:27:36 2002 */

#include	<stdio.h>

/* Routine to return a string with a timestamp value */
/* should be the same for VMS and UNIX, but...		*/

char *tstamp(void);

int logger_setlvl(int severity);

int open_logger (char *log_file_name);

void flush_logger(void);

void close_logger (void);

/* only used for logging multi-line messages */
void write_logger(int s_lvl, char *msg, int len);

/* these two functions return fd/fp for outside use.  No real checks are   */
/* done 								   */
int	get_logfd(void);

FILE *get_logfp(void);

/* Functions for setting and getting current logpfx */
/* If set this is added between timestamp and log message */
/* mostly for tracking down where messages are coming from */
/* logpfx is currently set to MAX_MESSAGE_LENGTH */
void set_logpfx(char *ptr);

char *get_logpfx(void);

/* these two functions set fd/fp from the outside. More checks S/B done    */
/* like fstating the files fully (write mode, ...) to see if they are valid */
int	set_logfd(int fd);

FILE *set_logfp(FILE *fp);

void logger (int severity, char *msg) ;

/* A varargs logger to avoid having msglog all over */
/* except for simple messages, this is probably the logger to use */
/* Will eventually call logger after formatting the message */
/* logger is a little faster, But ONLY if you are not calling sprintf */
void valogger(int severity, char *format, ...);

/* A Fortran Callable version of logger (or 2)				*/
/* The regular logger can be called but Fortran passes the length after	*/
/* the string and does not NULL terminate, so a special version is	*/
/* needed to get the length of the string to properly NULL terminate    */
/* I added trimming of spaces from end of Fortran String, since Fortran */
/* also tends to Space pad						*/
/* I checked that Fortran 77 DOES pass length with Absoft, SGI and g77, */
/* But it may not work everywhere					*/
/* Oh BTW, I guess that this does not need to be in a C header file since */
/* only Fortran will be using it, but WTH				*/

/* This one can be called from Fortran
        call flogger(INFO, 'This Debug Message')
*/
void flogger(int *severity, char *msg, int mlen);

/* This one must pass Severity by dereferencing (pass by value)
        call flogger(%val(INFO), 'This Debug Message')
*/
void filogger(int severity, char *msg, int mlen);

/********** message.h  */
#define MAX_NUM_MESSAGE		500	/* Max. # message records in global
					   common ( See above NOTE) */

#define ELEMENT_SIZE		128 	/* Max. length in bytes of complete 
                                           message record */

#define MAX_MESSAGE 		256	/* Max. length of users portion of
                                           message record */

					/* size of statistics struc (bytes)*/
#define MSG_STATS_SIZE 		(2 * ELEMENT_SIZE)

/* 	message severity levels	*/
#define DBUG	1200		/* Debug message */
#define INFO	1000		/* Informational message */
#define ATTN  	 800		/* Operator's attention */
#define WARN	 600		/* Warning message */
#define ERROR	 400		/* error message (non-fatal to sim) */
#define SEVERE   300		/* Severe message, */
#define CRIT     200		/* Critical message, */
#define FATAL    100		/* Fatal message, also bring down
					   the simuation */
#endif /* LOGGER_H_INC */
