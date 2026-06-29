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
#include <unistd.h>

#include "modern_curses.h"
#include "have_strlcat.h"
#include "have_strlcpy.h"
#include "strl.h"
#include "extern.h"
#include "config.h"
#include "score.h"
#include "rogue.h"

static char *program = "rogue"; /* our name */

static char *usage =
    "usage: %s [-S] [-s [ score_file ] | -d | -V | -h | -r] [ save_file ]\n"
    "\n"
    "\t-S\t\tquit game catching a terminating signal (def: save game)\n"
    "\n"
    "\t-s\t\tprint out the list of scores from: %s\n"
    "\t-s score_file\tprint out the list of scores from score_file\n"
    "\n"
    "\t-d\t\tkill the rogue and try to add the rogue to the score file\n"
    "\n"
    "\t-h\t\toutput this message and exit\n"
    "\n"
    "\t-V\t\tprint version string and exit\n"
    "\n"
    "\t-r\t\tsave game file will be the default save game file\n"
    "\n"
    "\t[ save_file ]\tset the save file path (def save file path: %s)\n"
    "\n"
    "Exit codes:\n"
    "     0   rogue exited normally, all OK\n"
    "     2   -h and help string printed or -V and version string printed\n"
    "     3   invalid command line, invalid option or option missing an argument\n"
    "     4   score file path too long\n"
    "     5   failed to restore game\n"
    "     6   failed to open rogue pid file for writing\n"
    "     7   screen to small to restore game\n"
    " >= 10   internal rogue error\n"
    "\n"
    "rogue version: %s %s (chongo was here)";

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
    int len;
    extern char *optarg;	/* option argument */
    extern int optind;		/* argv index of the next arg */
    int i;

    /*
     * save our name
     */
    program = argv[0];

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
     * If the 1st argument (after argv[0] program name), is an empty string,
     * then ask then to prove they are a wizard.
     *
     * Skip over 1st argument empty string, regardless of if they prove
     * they are a wizard, or not.
     */
    if (argc >= 2 && argv[1] != NULL && argv[1][0] == '\0')
    {

	/* ask them to prove they are a wizard */
	if (strcmp(PASSWD, md_crypt(md_getpass("wizard's password: "), "mT")) == 0)
	{
	    wizard = true;
	    player.t_flags |= SEEMONST;
	}

	/* skip the empty string, regardless */
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
    /* whoami rogue name starting with rogo- may use $ROGOSEED to set dungeon number for rogomatic debugging */
    if (strncmp(whoami, "rogo-", strlen("rogo-")) == 0) {
	env = getenv("ROGOSEED");
	if (env != NULL) {
	    dnum = (unsigned int)strtol(env, NULL, 0);
	}
    }
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
     * parse args
     */
    while ((i = getopt(argc, argv, ":Ss::dVhr")) != -1) {
	switch (i) {
	case 'S':   /* -S ==> terminating signal will quit the game */
	    signal_quit = true;
	    break;

	case 's':   /* -s ==> list of scores from default score file, -s score_file ==> list of scores from score_file */
	    if (optarg != NULL) {
		/*
		 * By overriding score_path below, the subsequent call to open_score() will read that score file path.
		 * Then when we "-s ==> check for print-score option", the call to score(0, -1, 0) will cause us
		 * to print the scores from that particular score file path.
		 */
		len = strlen(optarg);
		if (len > MAXSTR) {
		    fprintf(stderr, "ERROR: score path length: %d > MAXSTR: %d\n", len, MAXSTR);
		    exit(4); /*ooo*/
		}
		strncpy(score_path, optarg, len+1);
		score_path[len] = '\0'; /* paranoia */
	    }

	    /*
	     * print score file and exit
	     */
	    noscore = true;
	    open_score();
	    score(0, -1, 0);
	    exit(0); /*ooo*/
	    break;

	case 'd':   /* -d ==> die immediately by bat (or random monster if wizard mode) */
	    /*
	     * throw away some rnd()s to break patterns
	     */
	    {
		unsigned int rnum = rnd(100)+1;
		while (--rnum > 0) {
		    rnd(100);
		}
	    }

	    /*
	     * set the level
	     */
	    if (wizard) {
		level = rnd(30) + 1;
	    } else {
		level = 1;
	    }

	    /*
	     * set the purse size
	     */
	    purse = GOLDCALC;

	    /*
	     * setup the screen
	     */
	    initscr();
	    getltchars();

	    /*
	     * die due to a monster
	     */
	    if (wizard) {
		death(death_monst());
	    } else {
		death('B');
	    }
	    exit(0); /*ooo*/
	    break;

	case 'V':   /* -V ==> print the rogue version, release date, and then exit */
	    /* Due to atexit(), a newline will be printed, so we don't print a newline now */
	    printf("%s %s", version, release);
	    fflush(stdout);
	    exit(2); /*ooo*/

	case 'h':   /* -h ==> print usage message and exit */
	    /* Due to atexit(), a newline will be printed, so we don't print a newline now */
	    fflush(stdout);
	    fprintf(stderr, usage, program, score_path, file_name, version, release);
	    fflush(stderr);
	    exit(2); /*ooo*/

	case 'r':   /* -r ==>  that the save game file will be the default save game file */
	    break;

	case ':':
	    fflush(stdout);
            (void) fprintf(stderr, "%s: ERROR: requires an argument -- %c\n", program, optopt);
	    (void) fprintf(stderr, usage, program, score_path, file_name, version, release);
	    fflush(stderr);
	    exit(3); /* ooo */
	    /*NOTREACHED*/
	    break;

	case '?':
	    fflush(stdout);
            (void) fprintf(stderr, "%s: ERROR: illegal option -- %c\n", program, optopt);
	    (void) fprintf(stderr, usage, program, score_path, file_name, version, release);
	    fflush(stderr);
	    exit(3); /* ooo */
	    /*NOTREACHED*/
	    break;

	default:
	    fflush(stdout);
	    (void) fprintf(stderr, "%s: ERROR: invalid -flag\n", program);
	    (void) fprintf(stderr, usage, program, score_path, file_name, version, release);
	    fflush(stderr);
	    exit(3); /* ooo */
	    /*NOTREACHED*/
	    break;
	}
    }
    /* skip over command line options */
    argv += optind;
    argc -= optind;

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
     * check for legal startup
     */
    init_check();

    /*
     * use any remaining command line option as the save_file to restore from
     */
    if (argc > 0) {
	/* if restore works, it will never return */
	if (!restore(argv[0])) {
	    my_exit(5); /*ooo*/
	}
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
	fflush(stdout);
        fprintf(stderr, "Can't open '%s'.\n", pidfilename);
	fflush(stderr);
        exit(6); /*ooo*/
      }
    }

    /*
     * Start up cursor package
     */
    initscr();
    keypad(stdscr, TRUE);

    /*
     * The screen must be at least NUMLINES x NUMCOLS
     */
    if (LINES < NUMLINES || COLS < NUMCOLS) {
	endwin_and_ncurses_cleanup();
	fflush(stdout);
	fprintf(stderr, "\nSorry, current screen only has %d lines and %d columns.\n", LINES, COLS);
	fprintf(stderr, "The screen have at least %d lines and %d columns.\n", NUMLINES, NUMCOLS);
	fflush(stderr);
	my_exit(7); /*ooo*/
    }

    /*
     * initialize rogue same state
     */
    init_probs();			/* Set up prob tables for objects */
    init_player();			/* Set up initial player stats */
    init_names();			/* Set up names of scrolls */
    init_colors();			/* Set up colors of potions */
    init_stones();			/* Set up stone settings of rings */
    init_materials();			/* Set up materials of wands */

    /*
     * Setup signal handlers to auto_save(), and get the terminal setup
     */
    setup();

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

    /*
     * next call to wrefresh with this window will clear the screen
     */
    mpos = 0;
    clearok(stdscr, true);

    /*
     * Draw current level
     */
    new_level();

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
