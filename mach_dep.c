/*
 * Various installation dependent routines
 *
 * @(#)mach_dep.c	4.37 (Berkeley) 05/23/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

/*
 * The various tuneable defines are:
 *
 *	SCORENAME	Basename of the score file
 *	LOCKNAME	Basename of the lock file
 *	ALLSCORES	Score file is top ten scores, not top ten
 *			players.  This is only useful when only a few
 *			people will be playing; otherwise the score file
 *			gets hogged by just a few people.
 *	NUMSCORES	Number of scores in the score file (default 10).
 *	MAXLOAD		What (if any) the maximum load average should be
 *			when people are playing.  Since it is divided
 *			by 10, to specify a load limit of 4.0, MAXLOAD
 *			should be "40".	 If defined, then
 *      LOADAV		Should it use it's own routine to get
 *		        the load average?
 *      NAMELIST	If so, where does the system namelist
 *		        hide?
 *	MAXUSERS	What (if any) the maximum user count should be
 *	                when people are playing.  If defined, then
 *      UCOUNT		Should it use it's own routine to count
 *		        users?
 *      UTMP		If so, where does the user list hide?
 *	CHECKTIME	How often/if it should check during the game
 *			for high load average.
 */

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ncurses.h>
#include <sys/file.h>
#include "rogue.h"
#include "score.h"
#include "extern.h"

#define NOOP(x) (x += 0)

#ifdef CHECKTIME
static int num_checks = 0;		/* times we've gone over in checkout() */
#endif /* CHECKTIME */

static int lock_fd = -1;		/* rogue lock file descriptor, <0 ==> rogue lock file not open */
static bool locked = FALSE;		/* TRUE ==> locked, FALSE ==> not locked */

/*
 * init_check:
 *	Check out too see if it is proper to play the game now
 */

void
init_check(void)
{
#if defined(MAXLOAD) || defined(MAXUSERS)
    if (too_much())
    {
	printf("Sorry, %s, but the system is too loaded now.\n", whoami);
	printf("Try again later.  Meanwhile, why not enjoy a%s %s?\n",
	    vowelstr(fruit), fruit);
	fflush(stdout);
	if (author()) {
	    printf("However, since you're a good guy, it's up to you\n");
	    fflush(stdout);
	} else {
	    exit(1);
	}
    }
#endif
}

/*
 * open_score:
 *	Open up the score file for future use
 */

void
open_score(void)
{
    struct stat buf;	    /* score file status */
    numscores = NUMSCORES;

#ifdef ALLSCORES
    allscore = TRUE;
#else  /* ALLSCORES */
    allscore = FALSE;
#endif /* ALLSCORES */

     /*
      * We drop setgid privileges after opening the score file, so subsequent
      * open()'s will fail.  Just reuse the earlier file handle.
      */

    if (scoreboard != NULL) {
        rewind(scoreboard);
        return;
    }

    scoreboard = fopen(score_path, "r+");

    /*
     * If we opened a score file, verify that the score file size is correct
     *
     * If the score file as empty, close the score file and let the next section
     * initialize the score file.
     */
    if (scoreboard != NULL) {

	/*
	 * determine size of the opened score file
	 */
	if (fstat(fileno(scoreboard), &buf) < 0) {
	    /* stat filed, close down and treat as if the previous open failed */
	    fclose(scoreboard);
	    scoreboard = NULL;

	/*
	 * if score file is empty
	 */
	} else if (buf.st_size == 0) {
	    /* close score file, and let the next section initialize the score file */
	    fclose(scoreboard);
	    scoreboard = NULL;
	}
    }

    /*
     * initialize the score file if the score file is not open
     */
    if (scoreboard == NULL) {
	SCORE top_scores[NUMSCORES+1];        /* scores from the score file, +1 for paranoia */
	int i;

	/*
	 * create and truncate a writable score file
	 */
	scoreboard = fopen(score_path, "w+");
	if (scoreboard == NULL) {
	     printf("Could not open %s for writing: %s\n", score_path, strerror(errno));
	     fflush(stdout);
	     exit(1);
	}
        md_chmod(score_path, 0664);

	/*
	 * initialize all scores
	 */
	memset(top_scores, 0, sizeof(top_scores)); /* paranoia */
	for (i=0; i < NUMSCORES; ++i) {
	    init_score_value(&top_scores[i]);
	}

	/*
	 * write the score file with initialized scores, and then rewind it
	 */
	wr_score(top_scores);
    }

}

/*
 * getltchars:
 *	Get the local tty chars for later use
 */

void
getltchars(void)
{
    got_ltc = TRUE;
    orig_dsusp = md_dsuspchar();
    md_setdsuspchar( md_suspchar() );
}

/*
 * setup:
 *	Get starting setup for all games
 */

void
setup(void)
{
#ifdef DUMP
    md_onsignal_autosave();
#else
    md_onsignal_default();
#endif

#ifdef CHECKTIME
    md_start_checkout_timer(CHECKTIME*60);
    num_checks = 0;
#endif

    raw();				/* Raw mode */
    noecho();				/* Echo off */
    keypad(stdscr,1);
    getltchars();			/* get the local tty chars */
}

/*
 * resetltchars:
 *      Reset the local tty chars to original values.
 */
void
resetltchars(void)
{
    if (got_ltc) {
        md_setdsuspchar(orig_dsusp);
    }
}

/*
 * playltchars:
 *      Set local tty chars to the values we use when playing.
 */
void
playltchars(void)
{
    if (got_ltc) {
        md_setdsuspchar( md_suspchar() );
    }
}

/*
 * start_score:
 *	Start the scoring sequence
 */

void
start_score(void)
{
#ifdef CHECKTIME
    md_stop_checkout_timer();
#endif
}

/*
 * is_symlink:
 *      See if the file has a symbolic link
  */
int
is_symlink(const char *sp)
{
#ifdef S_IFLNK
    struct stat sbuf2;

    if (lstat(sp, &sbuf2) < 0)
        return FALSE;
    else
        return ((sbuf2.st_mode & S_IFMT) != S_IFREG);
#else
	NOOP(sp);
    return FALSE;
#endif
}

#if defined(MAXLOAD) || defined(MAXUSERS)
/*
 * too_much:
 *	See if the system is being used too much for this game
 */
int
too_much(void)
{
#ifdef MAXLOAD
    double avec[3];
#else
    int cnt;
#endif

#ifdef MAXLOAD
    md_loadav(avec);
    if (avec[1] > (MAXLOAD / 10.0))
	return TRUE;
#endif
#ifdef MAXUSERS
    if (ucount() > MAXUSERS)
	return TRUE;
#endif
    return FALSE;
}

/*
 * author:
 *	See if a user is an author of the program
 */
int
author(void)
{
#ifdef MASTER
    if (wizard)
	return TRUE;
#endif
    switch (md_getuid())
    {
	case -1:
	    return TRUE;
	default:
	    return FALSE;
    }
}
#endif

#ifdef CHECKTIME
/*
 * checkout:
 *	Check each CHECKTIME seconds to see if the load is too high
 */

checkout(int sig)
{
    char *msgs[] = {
	"The load is too high to be playing.  Please leave in %0.1f minutes",
	"Please save your game.  You have %0.1f minutes",
	"Last warning.  You have %0.1f minutes to leave",
    };
    int checktime;

    if (too_much())
    {
	if (author())
	{
	    num_checks = 1;
	    chmsg("The load is rather high, O exalted one");
	}
	else if (num_checks++ == 3)
	    fatal("Sorry.  You took too long.  You are dead\n");
	checktime = (CHECKTIME * 60) / num_checks;
	chmsg(msgs[num_checks - 1], ((double) checktime / 60.0));
    }
    else
    {
	if (num_checks)
	{
	    num_checks = 0;
	    chmsg("The load has dropped back down.  You have a reprieve");
	}
	checktime = (CHECKTIME * 60);
    }

	md_start_checkout_timer(checktime);
}

/*
 * chmsg:
 *	checkout()'s version of msg.  If we are in the middle of a
 *	shell, do a printf instead of a msg to avoid the refresh.
 */
/* VARARGS1 */

chmsg(char *fmt, int arg)
{
    if (!in_shell)
	msg(fmt, arg);
    else
    {
	printf(fmt, arg);
	putchar('\n');
	fflush(stdout);
    }
}
#endif

#ifdef UCOUNT
/*
 * ucount:
 *	count number of users on the system
 */
#include <utmp.h>

struct utmp buf;

int
ucount(void)
{
    struct utmp *up;
    FILE *utmp;
    int count;

    if ((utmp = fopen(UTMP, "r")) == NULL)
	return 0;

    up = &buf;
    count = 0;

    while (fread(up, 1, sizeof (*up), utmp) > 0)
	if (buf.ut_name[0] != '\0')
	    count++;
    fclose(utmp);
    return count;
}
#endif

/*
 * lock_sc:
 *	lock the rogue lock file.  Return TRUE if the lock is successful.
 */
int
lock_sc(void)
{
    int ret;	    /* flock return */

    /*
     * do nothing if already locked
     */
    if (locked) {
	return TRUE;
    }

    /*
     * form lock path if needed
     */
    if (lock_path[0] == '\0') {
	form_lock_path();
    }

    /*
     * open lock file if not already open
     */
    if (lock_fd < 0) {
	lock_fd = open(lock_path, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); /* more 0644 */
	if (lock_fd < 0) {
	    /* failed to open and/or create the lock file */
	    printf("ERROR: failed to open lock file: %s\n", lock_path);
	    fflush(stdout);
	    return FALSE;
	}
    }

    /*
     * lock the file
     */
    ret = flock(lock_fd, LOCK_EX);
    if (ret < 0) {
	/* failed to lock */
	printf("ERROR: failed to lock: %s\n", lock_path);
	fflush(stdout);
	return FALSE;
    }

    /*
     * lock successful
     */
    locked = TRUE;
    return TRUE;
}

/*
 * unlock_sc:
 *	Unlock the rogue lock file
 */

void
unlock_sc(void)
{
    int ret;	    /* flock return */

    /*
     * do nothing if not locked
     */
    if (! locked) {
	return;
    }

    /*
     * do nothing is lock file is not open
     */
    if (lock_fd < 0) {
	locked = FALSE;
	return;
    }

    /*
     * unlock
     */
    ret = flock(lock_fd, LOCK_UN);
    if (ret < 0) {
	/* failed to lock */
	printf("ERROR: failed to unlock: %s\n", lock_path);
	fflush(stdout);
	return;
    }

    /*
     * unlock successful
     */
    locked = FALSE;
    return;
}

/*
 * flush_type:
 *	Flush typeahead for traps, etc.
 */

void
flush_type(void)
{
    flushinp();
}

/*
 * form_home:
 *      Form home path, plus trailing /, if not already formed.
 */

void
form_home(void)
{
    /*
     * do nothing if home string is non-empty
     */
    if (home[0]) {
        return;
    }

    /*
     * get home plus trialing / from environment
     */
    memset(home, 0, sizeof(home)); /* paranoia */
    strncpy(home, md_gethomedir(), MAXSTR);
    home[MAXSTR] = '\0'; /* paranoia */
    return;
}

/*
 * form_lock_path:
 *	determine rouge score lock path
 */

void
form_lock_path(void)
{
    /*
     * do nothing if lock path string is non-empty
     */
    if (lock_path[0]) {
        return;
    }

    /*
     * form home plus trialing / if not already set
     */
    if (home[0] == '\0') {
	form_home();
    }

    /*
     * determine lock file path
     */
    memset(lock_path, 0, sizeof(lock_path)); /* paranoia */
    snprintf(lock_path, 2*MAXSTR, "%s%s", home, LOCKNAME);
    lock_path[2*MAXSTR] = '\0'; /* paranoia */
    return;
}

/*
 * form_save_path:
 *	determine rouge save file path
 */

void
form_save_path(void)
{
    /*
     * do nothing if save path string is non-empty
     */
    if (file_name[0]) {
        return;
    }

    /*
     * form home plus trialing / if not already set
     */
    if (home[0] == '\0') {
	form_home();
    }

    /*
     * determine save file path
     */
    memset(file_name, 0, sizeof(file_name)); /* paranoia */
    snprintf(file_name, 2*MAXSTR, "%s%s", home, SAVENAME);
    file_name[2*MAXSTR] = '\0'; /* paranoia */
    return;
}

/*
 * form_score_path:
 *	determine rouge score file path
 */

void
form_score_path(void)
{
    /*
     * do nothing if score path string is non-empty
     */
    if (score_path[0]) {
        return;
    }

    /*
     * form home plus trialing / if not already set
     */
    if (home[0] == '\0') {
	form_home();
    }

    /*
     * determine score file path
     */
    memset(score_path, 0, sizeof(score_path)); /* paranoia */
    snprintf(score_path, 2*MAXSTR, "%s%s", home, SCORENAME);
    score_path[2*MAXSTR] = '\0'; /* paranoia */
    return;
}
