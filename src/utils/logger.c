static char *sccs_id = "%Z%%P%	$Revision: 3.8 $	- $Date: 2006-01-20 23:45:23 $";

#include	<fcntl.h>
#include	<stddef.h>
#include	<ctype.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdlib.h>
/*PUBLIC*/
#include	<stdio.h>
/*PUBLIC_END*/
#include	<stdarg.h>
#include	<sys/stat.h>
#include	<sys/stat.h>
#include	<sys/time.h>

/*#include	"message.h"*/
#include	"logger.h"
	
#ifdef SIMPLE_LOGICALS
#define NO_LOGICALS		1
#endif

#ifndef	NO_LOGICALS		/* Use this standalone W/O Logicals */
						/* Must call open_logger w/ valid filename */
/*#include "acsysutils.h"*/
#define		ACFS_DEFLOGDIR	"ACFS_LOG"
#define		DEF_LOG_FILE	"acfs.log"
#else
#define		DEF_LOG_FILE	"logger.log"
#endif

#define		MAX_MESSAGE_LENGTH 	(MAX_MESSAGE * 2)


#define		MSG_STORE_SIZE	(1024*1024)	/* 1 meg for now */

#define INFO_LOGLVL	  INFO
#define ATTN_LOGLVL	  ATTN
#define WARN_LOGLVL	  WARN
#define ERROR_LOGLVL  ERROR
#define SEVERE_LOGLVL SEVERE
#define CRIT_LOGLVL	  CRIT
#define FATAL_LOGLVL  FATAL

#define	DEF_LOGLVL	INFO_LOGLVL	/* Aything written to the screen */

#define DEBUG

/*********************************************************************/
/**************** UTILITIES ******************************************/
/*********************************************************************/

#define	ITOA_BUF_SIZ 256
/* Convert an integer to ascii */
/* will start from the back of the buf and fill out to the buf size */

/* send it an integer and the number of decimal places to fill to */
/*   (0 will not fill) 							*/
/* Returns a NULL terminated ascii string */

char *ItoA(int i, int zero_pad_num)
{
	static char itoabuf[ITOA_BUF_SIZ];
	char	*ptr;
	int	is_neg;
	long	wval;

	/* Check for negative value */
	if (i >= 0) {
	    wval = i;
	    is_neg = 0;
	} else {
	    wval = i * -1;
	    is_neg = 1;
	}

	itoabuf[ITOA_BUF_SIZ - 1] = '\0';	/* null term */

	for (ptr = &itoabuf[ITOA_BUF_SIZ - 2]; ptr >= itoabuf; ptr--) {
	    *ptr = (wval % 10) + '0';
	    wval = wval / 10;
	    zero_pad_num--;
	    if ((wval <= 0) && (zero_pad_num <= 0)) break;
	}

	if (is_neg) {
	    ptr--;
	    *ptr = '-';
	}
	return ptr;
}

int tstamp_len;
#ifndef vms	 /* NOT vms */
#include <time.h>
/*PUBLIC*/
/* Routine to return a string with a timestamp value */
/* should be the same for VMS and UNIX, but...		*/

char *tstamp()
{
	struct timeval	tval;
	time_t tim;
	char	*atim, *ptr, *uptr;
	static char tbuf[64];

	gettimeofday(&tval, 0);
	tim = tval.tv_sec;
	atim = ctime(&tim);
	ptr = &atim[11];
	ptr[8] = '.';
	ptr[9] = '\0';
#define	DO_USEC
#ifndef DO_USEC
	strcpy(tbuf, ptr);
#else
	uptr = ItoA((tval.tv_usec / 10000), 2);
	strcpy(tbuf, ptr); 
	strcat(tbuf, uptr);
#ifdef TEST_GETTIME
	sprintf(tbuf, "%s%s (%.2d) (%d)", ptr, uptr,
	    (tval.tv_usec / 10000), tval.tv_usec);
#endif	/* TEST_GETTIME */
#endif	/* DO_USEC */
	tstamp_len = strlen(tbuf);
	return(tbuf);
}

#else	/* def VMS */
#include	<file.h>
#include	<descrip.h>
char *tstamp()
{
	static	long	time[2];
	static	short	timlen;
	static	$DESCRIPTOR	(time_descrip,"hh:ss:mm.cc");
	sys$gettim (&time);
	sys$asctim (&timlen, &time_descrip, &time, 1);
	tstamp_len = 11;
	return(time_descrip.dsc$a_pointer);
}
#endif	/* vms */


/*********************************************************************/
/**************** End UTILITIES **************************************/
/*********************************************************************/

/* Some global definitions for  outside access */
int	msg_logging_enabled = 1;
int	flush_logger_buf = 0;
/* Some local definitions */
static int	logfd	= -1;		/* the real log file des.	*/
static FILE	*logfp	= NULL;		/* only defined at request 	*/
static char	*lmsgbuf = NULL;
static long	cmsgloc = 0;
static long	msgbsiz = 0;
static long	msgbmaxsiz = MSG_STORE_SIZE;
static int	outsetfd_flg = 0;	/* we didnt open logfd so dont close */

static int	loglvl	= DEF_LOGLVL;	 /* Current lvl to write/buff */

/* To set for function names, and such*/
/* if Set, added after tstamp, before message */
static char logpfx[MAX_MESSAGE_LENGTH + 1] = "";

/*PUBLIC*/
int logger_setlvl(int severity)
{
	int oldlvl;
	oldlvl = loglvl;

	switch (severity) {
	    case INFO :
		loglvl = INFO_LOGLVL;
		break;
	    case ATTN :
		loglvl = ATTN_LOGLVL;
		break;
	    case WARN :
		loglvl = WARN_LOGLVL;
		break;
	    case ERROR :
		loglvl = ERROR_LOGLVL;
		break;
	    case FATAL :
		loglvl = FATAL_LOGLVL;
		break;
	    default :
		loglvl = severity;
	} 
	return oldlvl;
}

/* This must be called before any logger or open_logger calls */
void logger_setbsiz(long newsiz)
{
       if (logfd < 0) return;
       if (newsiz >= 0) msgbmaxsiz = newsiz;
}

int start_logger(void)
{
	char msg_buf[128];
	time_t tm;
	tm = time((time_t) (NULL));
	sprintf(msg_buf, "******* MESSAGE LOGGER STARTING at %s", ctime(&tm));
	logger (ATTN, msg_buf);

	if (! lmsgbuf) {	/* if not already init */
	    if ((lmsgbuf = (char *)malloc(msgbmaxsiz))) {
	        char tmsgbuf[512];
	        msgbsiz = msgbmaxsiz;
	        cmsgloc = 0;
	        memset(lmsgbuf, 0, msgbsiz);
	        sprintf(tmsgbuf, "******* Message Storage Size is %ld", msgbsiz);
	        logger(INFO, tmsgbuf);
	    } else {
	        logger(ERROR, "Failed to malloc Message Buffer\n");
	        return 0;
	    }
    } else { flush_logger(); };

	return 1;
}


/*PUBLIC*/
int open_logger (char *log_file_name)
{
	char *lognm;

	if (logfd >= 0) return logfd;	/* Already Open */
	lognm = 0;
	if (log_file_name) lognm = log_file_name;
	else {
#ifndef	NO_LOGICALS		/* Use this standalone W/O Logicals */
	    lognm = LT_tran_file(ACFS_DEFLOGDIR, DEF_LOG_FILE);
#else
	    lognm = DEF_LOG_FILE;
#endif
	}

	if (!lognm) return -3;	/* Need to open the log file and no name */

	if (logfd < 0) {
	    if ((logfd = open(lognm, (O_CREAT|O_RDWR|O_APPEND), 0664)) < 0) {
	        fprintf(stderr, "STDERR: Failed to open LOG_FILE %s\n", lognm);
	        fprintf(stdout, "ERROR:  Failed to open LOG_FILE %s\n", lognm);
	        msg_logging_enabled = 0;
	        return -1;
	    } else {
	        msg_logging_enabled++;
	        start_logger();
	    }
	}

	return logfd;
}

/*PUBLIC*/
void flush_logger()
{
	write(logfd, lmsgbuf, cmsgloc);
	cmsgloc = 0;
}

/*PUBLIC*/
void close_logger ()
{
	char msg_buf[128];
	time_t tm;

	tm = time(0);
	sprintf(msg_buf, "******* MESSAGE LOGGER STOPPING at %s", ctime(&tm));
	logger (ATTN, msg_buf);
	flush_logger();
	if (!outsetfd_flg) {
	    close(logfd);
		logfd = -2;
    }
}
/*TODO add global flush flag */

/* ques a message in the buffer (if space). 0 = no room, 1 = OK */
int que_logger_msg(char *newmsg, int len)
{
	int idx;
	if ((len + cmsgloc) > msgbsiz) return 0;

	for (idx = 0; (idx < len) && newmsg[idx] ; idx++) {
	    lmsgbuf[cmsgloc + idx] = newmsg[idx];
	}

	cmsgloc = cmsgloc + idx;
	lmsgbuf[cmsgloc] = '\0';
	return 1;
}

/*PUBLIC*/
/* only used for logging multi-line messages */
void write_logger(int s_lvl, char *msg, int len) 
{
	if (s_lvl > loglvl) {
	    if (que_logger_msg(msg, len)) return;
	}
	flush_logger();
	write(logfd, msg, len);
#ifdef DEBUG
        if (s_lvl <= loglvl) printf("%s",msg);
#endif
}

/*PUBLIC*/
/* these two functions return fd/fp for outside use.  No real checks are   */
/* done 								   */
int	get_logfd()
{
	return logfd;
}

/*PUBLIC*/
FILE *get_logfp()
{

	if (logfp)	return logfp;
	if (logfd < 0)	return NULL;
	logfp = fdopen(logfd, "a");
	return logfp;
}

/*PUBLIC*/
/* Functions for setting and getting current logpfx */
/* If set this is added between timestamp and log message */
/* mostly for tracking down where messages are coming from */
/* logpfx is currently set to MAX_MESSAGE_LENGTH */
void set_logpfx(char *ptr)
{
	if (!ptr) {
		*logpfx = 0;
		return;
	}
	strncpy(logpfx, ptr, MAX_MESSAGE_LENGTH);
	logpfx[MAX_MESSAGE_LENGTH] = 0;
}

/*PUBLIC*/
char *get_logpfx()
{
	return logpfx;
}

/*PUBLIC*/
/* these two functions set fd/fp from the outside. More checks S/B done    */
/* like fstating the files fully (write mode, ...) to see if they are valid */
int	set_logfd(int fd)
{
	int ologfd;
	struct stat stbuf;

	/* erroro check(s)	*/
	if (fd < 0) return fd;
	if (fstat(fd, &stbuf) < 0) return fd;

	outsetfd_flg ++;
	ologfd = logfd;
	logfd = fd;
	start_logger();

	return ologfd;
}

/*PUBLIC*/
FILE *set_logfp(FILE *fp)
{
	FILE *ofp;
	int ofd;
	struct stat stbuf;

	/* erroro check(s)	*/
	if (!fp)	return fp;
	if (fstat(fileno(fp), &stbuf) < 0) return fp;

	ofd = logfd;
	if (logfp) {
	    ofp = logfp;
	} else {
	    if (logfd < 0) {
		ofp = NULL;
	    } else {
	        ofp = fdopen(logfd, "a");
	    }
	}

	outsetfd_flg ++;
	logfp = fp;
	logfd = fileno(fp);
	start_logger();

	return ofp;
}

/*PUBLIC*/
void logger (int severity, char *msg) 
{

	char	new_msg[MAX_MESSAGE_LENGTH*4];	/* twice for safety */
	int	length;
	int	i;
	int	sev_lvl;


	if (!msg_logging_enabled) return;

	if (logfd < 0) {
		if (open_logger(0) < 0) {
		    /* printf("Failed to open LOG_FILE %s\n", DEF_LOG_FILE); */
		    return;
	        }
	}

	length = 0;
	sev_lvl = 0;
	if (severity == INFO) {
		sev_lvl = INFO_LOGLVL;
		new_msg[length++] = 'I';
		new_msg[length++] = 'N';
		new_msg[length++] = 'F';
		new_msg[length++] = 'O';
		new_msg[length++] = ' ';
		new_msg[length++] = ' ';
	} else if (severity == ATTN) {
		sev_lvl = ATTN_LOGLVL;
		new_msg[length++] = 'A';
		new_msg[length++] = 'T';
		new_msg[length++] = 'T';
		new_msg[length++] = 'N';
		new_msg[length++] = ' ';
		new_msg[length++] = ' ';
	} else if (severity == WARN) {
		sev_lvl = WARN_LOGLVL;
		new_msg[length++] = 'W';
		new_msg[length++] = 'A';
		new_msg[length++] = 'R';
		new_msg[length++] = 'N';
		new_msg[length++] = ' ';
		new_msg[length++] = ' ';
	} else if (severity == ERROR) {
		sev_lvl = ERROR_LOGLVL;
		new_msg[length++] = 'E';
		new_msg[length++] = 'R';
		new_msg[length++] = 'R';
		new_msg[length++] = 'O';
		new_msg[length++] = 'R';
		new_msg[length++] = ' ';
	} else if (severity == SEVERE) {
		sev_lvl = FATAL_LOGLVL;
		new_msg[length++] = 'S';
		new_msg[length++] = 'E';
		new_msg[length++] = 'V';
		new_msg[length++] = 'E';
		new_msg[length++] = 'R';
		new_msg[length++] = 'E';
		new_msg[length++] = ' ';
	} else if (severity == CRIT) {
		sev_lvl = CRIT_LOGLVL;
		new_msg[length++] = 'C';
		new_msg[length++] = 'R';
		new_msg[length++] = 'I';
		new_msg[length++] = 'T';
		new_msg[length++] = 'I';
		new_msg[length++] = 'C';
		new_msg[length++] = 'A';
		new_msg[length++] = 'L';
		new_msg[length++] = ' ';
	} else if (severity == FATAL) {
		sev_lvl = FATAL_LOGLVL;
		new_msg[length++] = 'F';
		new_msg[length++] = 'A';
		new_msg[length++] = 'T';
		new_msg[length++] = 'A';
		new_msg[length++] = 'L';
		new_msg[length++] = ' ';
	} else {			/* Default */
		sev_lvl = 0;
		new_msg[length++] = 'U';
		new_msg[length++] = 'N';
		new_msg[length++] = 'K';
		new_msg[length++] = ' ';
		new_msg[length++] = ' ';
		new_msg[length++] = ' ';
	}
	new_msg[length] = (char)0;

	strcat (new_msg, tstamp());
	length += tstamp_len;

	new_msg[length++] = ':';
	new_msg[length++] = ' ';
	new_msg[length] = (char)0;

	if (logpfx) {
		int lplen;
		lplen = strlen(logpfx);
		strcat(new_msg, logpfx);
		length += lplen;
	    new_msg[length++] = ':';
	    new_msg[length++] = ' ';
	}

	for (i = 0; (msg[i] != 0) && (i < MAX_MESSAGE_LENGTH) && isprint(msg[i]); i++) {
		new_msg[length++] = msg[i];
	}
	if (msg[i] && (i >= MAX_MESSAGE_LENGTH)) new_msg[length++] = '+';

	if (new_msg[length-1] != '\n') 
	    new_msg[length++] = '\n';
	new_msg[length] = 0;

	write_logger(sev_lvl, new_msg, length);
	
}

/*PUBLIC*/
/* A varargs logger to avoid having msglog all over */
/* except for simple messages, this is probably the logger to use */
/* Will eventually call logger after formatting the message */
/* logger is a little faster, But ONLY if you are not calling sprintf */
void valogger(int severity, char *format, ...)
{
	char bufr[BUFSIZ];	/* Hopefully big enough ;) */
	va_list args;

	va_start(args, format);
	/* Transfer the error message for logger */
	vsprintf(bufr, format, args);

	bufr[MAX_MESSAGE_LENGTH] = 0;
	logger(severity, bufr);
	va_end(args);
}

/*PUBLIC*/
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
void flogger(int *severity, char *msg, int mlen)
{
	/* Just call below */
	filogger(*severity, msg, mlen);
}

/*PUBLIC*/
/* This one must pass Severity by dereferencing (pass by value)
        call flogger(%val(INFO), 'This Debug Message')
*/
void filogger(int severity, char *msg, int mlen)
{
	int nmlen, idx;
	char *ptr;
	char bufr[MAX_MESSAGE_LENGTH * 2];	/* Hopefully big enough ;) */

	if ((mlen < (sizeof(bufr) - 1)) && (msg[mlen - 1] == ' ')) {
	    /* Strip off ending spaces */
	    for (idx = mlen - 2; (idx && (msg[idx] == ' ')); idx++);
	    msg[idx+1] = 0;
	    nmlen = mlen - 1;	/* Leave space for NULL, not counted */
	    ptr = msg;
	} else {

	    /* Transfer the error message for logger */
	    nmlen = (mlen < (sizeof(bufr) - 1) ? mlen : (sizeof(bufr) -1));
	    strncpy(bufr, msg, nmlen);
	    bufr[nmlen] = 0;
	    ptr = bufr;
	}

	/* Strip off ending spaces */
	for (idx = nmlen - 1; (idx && (ptr[idx] == ' ')); idx--);
	ptr[idx+1] = 0;

	logger(severity, ptr);
}


