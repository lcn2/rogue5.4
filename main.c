/*
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 *
 * @(#)main.c	4.22 (Berkeley) 02/05/99
 */

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include "modern_curses.h"
#include "have_strlcat.h"
#include "have_strlcpy.h"
#include "strl.h"
#include "extern.h"
#include "config.h"
#include "score.h"
#include "rogue.h"

char whoami[MAX_USERNAME+1] = {'\0'};	/* Name of player, +1 for paranoia */

/*
 * main:
 *	The main program, of course
 */
int
main(int argc, char **argv)
{
    char *env;
    struct timeval tp;

    /*
     * on exit: cleanup I/O, and shutdown ncurses (if needed)
     *
     * NOTE: This is needed prior to calling md_init().
     */
    (void) atexit(endwin_and_ncurses_cleanup);

    /*
     * set mode for write access for the owner only
     *
     * NOTE: This is needed prior to calling md_init().
     */
    (void) umask(S_IWGRP | S_IWOTH);

#ifdef MASTER
    /*
     * Check to see if he is a wizard
     */
    if (argc >= 2 && argv[1][0] == '\0')
	if (strcmp(PASSWD, md_crypt(md_getpass("wizard's password: "), "mT")) == 0)
	{
	    wizard = true;
	    player.t_flags |= SEEMONST;
	    argv++;
	    argc--;
	}

#endif

    /*
     * form default paths
     */
    form_home();
    form_lock_path();
    form_save_path();
    form_score_path();

    /*
     * get options from the environment
     */
    memset(whoami, 0, sizeof(whoami)); /* paranoia */
    env = getenv("ROGUEOPTS");
    if (env != NULL) {
	/*
	 * parse ROGUEOPTS
	 */
	parse_opts(env);
    }

    /*
     * correct any lack of a rogue name (whoami[])
     *
     * The whoami[] string starts out as an empty string.
     * However, the parse_opts() could fail to set the rogue name, or the parse_opts()
     * could set the rogue name (via the "name" option) to an empty string.
     *
     * In such a case, we will use the login name of the current user, or
     * in case that login name is empty, then rogue name will be "nobody".
     */
    if (whoami[0] == '\0') {
	char *username;

	username = md_getusername();
	if (username == NULL || username[0] == '\0') {
	    username = "nobody"; /* paranoia */
	}
        strlcpy(whoami, username, MAX_USERNAME);
    }

    /*
     * correct any lack of rogue lock path (lock_path[])
     *
     * The lock_path[] string is set by the form_lock_path() function.
     * However, the parse_opts() could fail to set the rogue lock path, or
     * the parse_opts() could set the rogue name (via the "lock" option) to an empty string.
     *
     * In such a case, we will use the LOCKPATH string constant.
     *
     * NOTE: The form_lock_path() function will NOT return if LOCKPATH is an empty string.
     */
    if (lock_path[0] == '\0') {
	strlcpy(lock_path, LOCKPATH, MAXSTR); /* paranoia */
    }

    /*
     * correct any lack of rogue save path (file_name[])
     *
     * The file_name[] string is set by the form_save_path() function.
     * However, the parse_opts() could fail to set the rogue save path, or
     * the parse_opts() could set the rogue name (via the "file" option) to an empty string.
     *
     * In such a case, we will use the SAVEPATH string constant.
     *
     * NOTE: The form_save_path() function will NOT return if SAVEPATH is an empty string.
     */
    if (file_name[0] == '\0') {
	strlcpy(file_name, SAVEPATH, MAXSTR); /* paranoia */
    }

    /*
     * correct any lack of rogue score path (score_path[])
     *
     * The file_name[] string is set by the form_score_path() function.
     * However, the parse_opts() could fail to set the rogue score path, or
     * the parse_opts() could set the rogue name (via the "score" option) to an empty string.
     *
     * In such a case, we will use the SCOREPATH string constant.
     *
     * NOTE: The form_score_path() function will NOT return if SCOREPATH is an empty string.
     */
    if (score_path[0] == '\0') {
	strlcpy(score_path, SCOREPATH, MAXSTR); /* paranoia */
    }

    /*
     * disable score file use if starting out in wizard mode
     */
#ifdef MASTER
    if (wizard) {
	noscore = true;
    }
#endif

    /*
     * determine dungeon number
     */
    memset(&tp, 0, sizeof(tp));
    if (gettimeofday(&tp, NULL) < 0) {
	dnum = time(NULL);
    } else {
	dnum = ((unsigned int)(tp.tv_sec) ^ (((unsigned int)tp.tv_usec) << 12));
    }
    dnum += (unsigned int)md_getpid();
    dnum += (unsigned int)md_getuid();
#ifdef MASTER
    if (wizard) {
	/*
	 * wizard's can use $SEED to force a given dungeon number
	 */
	env = getenv("SEED");
	if (env != NULL) {
	    dnum = (unsigned int)strtol(env, NULL, 0);
	}
    }
#endif

    /*
     * seed the game with the dungeon number
     */
#if defined(NON_BSD_RN_GENERATOR)
    seed = md_getpid();
#else
    srandom((unsigned)dnum);
#endif

    /*
     * open the rogue score file
     *
     * If there is no rogue score file, or if the rogue score file is empty,
     * we will initialize the rogue score file with slots containing 0 scores.
     */
    open_score();

    /*
     * Drop setuid/setgid after opening the scoreboard file.
     */
    md_normaluser();

    /*
     * parse 2nd arg
     */
    if (argc == 2)
    {

	/*
	 * -s ==> check for print-score option
	 */
	if (strcmp(argv[1], "-s") == 0)
	{
	    noscore = true;
	    score(0, -1, 0);
	    exit(0);
	}

	/*
	 * -d ==> die immediately by bat (or random monster if wizard mode)
	 */
	else if (strcmp(argv[1], "-d") == 0)
	{
	    unsigned int rnum;

	    rnum = rnd(100)+1;	/* throw away some rnd()s to break patterns */
	    while (--rnum)
		rnd(100);
	    purse = GOLDCALC;
	    if (wizard) {
		level = rnd(100) + 1;
	    } else {
		level = 1;
	    }
	    initscr();
	    getltchars();
	    if (wizard) {
		death(death_monst());
	    } else {
		death('B');
	    }
	    exit(0);
	}

	/*
	 * -V ==> print version and exit
	 */
	else if (strcmp(argv[1], "-V") == 0)
	{
	    /* Due to atexit(), a newline will be printed, so we don't print a newline now */
	    printf("rogue version %s release %s (chongo was here)", version, release);
	    fflush(stdout);
	    exit(0);
	}
    }

    init_check();			/* check for legal startup */
    if (argc == 2)
	/* if restore works, it will never return */
	if (!restore(argv[1])) {
	    exit(1);
	}

    /*
     * report dungeon (or wizard dungeon)
     */
#ifdef MASTER
    if (wizard)
	printf("Hello %s, welcome to dungeon #%u", whoami, dnum);
    else
	printf("Hello %s, just a moment while I dig the dungeon #%u...", whoami, dnum);
#else
    printf("Hello %s, just a moment while I dig the dungeon #%u...", whoami, dnum);
#endif
    fflush(stdout);

   /*
    * Get the process id of this rogue program if the
    * environment variable is set which requests this be
    * done.  Then create the file name with the PID so
    * that the debugging scripts can find it and use the
    * PID.
    *
    * This code can be removed if you don't need to use
    * the debugging scripts.
    *
    */

    /* Process ID */
    pid_t pid;
    char pidfilename[SHORTSTR + 1];
    FILE *pidfp;

    if (getenv("GETROGUEPID") != NULL) {
      pid = md_getpid ();
      memset (pidfilename, '\0', sizeof(pidfilename));
      snprintf (pidfilename, SHORTSTR, "roguepid.%d", pid);
      if ((pidfp = fopen (pidfilename, "w")) == NULL) {
        printf("Can't open '%s'.\n", pidfilename);
	fflush(stdout);
        exit(1);
      }
    }

    initscr();				/* Start up cursor package */
    init_probs();			/* Set up prob tables for objects */
    init_player();			/* Set up initial player stats */
    init_names();			/* Set up names of scrolls */
    init_colors();			/* Set up colors of potions */
    init_stones();			/* Set up stone settings of rings */
    init_materials();			/* Set up materials of wands */
    setup();

    /*
     * The screen must be at least NUMLINES x NUMCOLS
     */
    if (LINES < NUMLINES || COLS < NUMCOLS)
    {
	printf("Sorry, the screen must be at least %dx%d\n", NUMLINES, NUMCOLS);
	fflush(stdout);
	my_exit(1);
    }

    /*
     * Set up windows
     */
    hw = newwin(LINES, COLS, 0, 0);
    idlok(stdscr, true);
    idlok(hw, true);
#ifdef MASTER
    if (wizard) {
	noscore = true;
    }
#endif
    new_level();			/* Draw current level */
    /*
     * Start up daemons and fuses
     */
    start_daemon(runners, 0, AFTER);
    start_daemon(doctor, 0, AFTER);
    fuse(swander, 0, WANDERTIME, AFTER);
    start_daemon(stomach, 0, AFTER);
    playit();
    return(0);
}
