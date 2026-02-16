/*
 * Defines for things used in mach_dep.c
 *
 * @(#)extern.h	4.35 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */


#include "config.h"

/*
 * Don't change the constants, since they are used for sizes in many
 * places in the program.
 */

#include <stdlib.h>

#undef SIGTSTP

#define MAXSTR		1024	/* maximum length of strings */
#define MAXLINES	32	/* maximum number of screen lines used */
#define MAXCOLS		80	/* maximum number of screen columns used */

#define RN		(((seed = seed*11109+13849) >> 16) & 0xffff)
#ifdef CTRL
#undef CTRL
#endif
#define CTRL(c)		(c & 037)

/* maximum length the score line apart from the username */
#define	MAX_OTHER_SCORE 80

/* must be an just integer = MAX_USERNAME+MAX_OTHER_SCORE */
#define	MAXSCORELINE   (MAX_USERNAME+MAX_OTHER_SCORE)

/*
 * Now all the global variables
 */

#define PFBUF_LEN (2*MAXSTR)
extern int got_ltc, in_shell;
extern int	wizard;
extern char	fruit[MAXSTR+1]; /* +1 for paranoia */
extern char	prbuf[PFBUF_LEN+1]; /* +1 for paranoia */
extern int orig_dsusp;
extern FILE	*scoreboard;
extern int numscores;
extern char *Numname;
extern int allscore;
extern char home[MAXSTR+1];			/* home path plus trailing /, +1 for paranoia */
extern char file_name[2*MAXSTR+1];		/* home plus / plus rogue save file path, +1 for paranoia */
extern char lock_path[2*MAXSTR+1];		/* home plus / plus rogue lock file path, +1 for paranoia */
extern char score_path[2*MAXSTR+1];		/* home plus / plus rogue score file path, +1 for paranoia */

/*
 * Function types
 */

int	md_chmod(const char *filename, int mode);
char	*md_crypt(const char *key, const char *salt);
int	md_dsuspchar(void);
int	md_erasechar(void);
char	*md_gethomedir(void);
char	*md_getusername(void);
uid_t	md_getuid(void);
char	*md_getpass(char *prompt);
pid_t	md_getpid(void);
char	*md_getrealname(uid_t uid);
void	md_init(void);
int	md_killchar(void);
void	md_normaluser(void);
void	md_raw_standout(void);
void	md_raw_standend(void);
int	md_readchar(WINDOW *win);
int	md_setdsuspchar(int c);
int	md_shellescape(void);
void	md_sleep(int s);
int	md_suspchar(void);
int	md_hasclreol(void);
int	md_unlink(char *file);
int	md_unlink_open_file(const char *file, FILE *inf);
void md_tstpsignal(void);
void md_tstphold(void);
void md_tstpresume(void);
void md_ignoreallsignals(void);
void md_onsignal_autosave(void);
void md_onsignal_exit(void);
void md_onsignal_default(void);
int md_issymlink(char *sp);
extern char *xcrypt(const char *key, const char *setting);

extern void form_home(void);
extern void form_lock_path(void);
extern void form_save_path(void);
extern void form_score_path(void);
