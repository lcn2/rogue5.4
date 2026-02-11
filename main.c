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
#include <ncurses.h>
#include "rogue.h"
#include "score.h"

char whoami[MAX_USERNAME] = {'\0'};	/* Name of player */

/*
 * main:
 *	The main program, of course
 */
int
main(int argc, char **argv)
{
    char *env;
    time_t lowtime;

    md_init();

#ifdef MASTER
    /*
     * Check to see if he is a wizard
     */
    if (argc >= 2 && argv[1][0] == '\0')
	if (strcmp(PASSWD, md_crypt(md_getpass("wizard's password: "), "mT")) == 0)
	{
	    wizard = TRUE;
	    player.t_flags |= SEEMONST;
	    argv++;
	    argc--;
	}

#endif

    /*
     * form critical paths
     */
    form_home();
    form_lock_path();
    form_save_path();
    form_score_path();

    /*
     * get options from the environment
     */
    memset(whoami, 0, sizeof(whoami)); /* paranoia */
    if ((env = getenv("ROGUEOPTS")) != NULL)
	parse_opts(env);
    if (env == NULL || whoami[0] == '\0')
        strucpy(whoami, md_getusername(), strlen(md_getusername()));
#ifdef MASTER
    if (wizard) {
	noscore = TRUE;
    }
#endif
    lowtime = time(NULL);
#if defined(ORIGINAL_MASTER) /* original MASTER behavior */
    dnum = lowtime + md_getpid();
#ifdef MASTER
    if (wizard) {
	if (getenv("SEED") != NULL) {
	    dnum = atoi(getenv("SEED"));
	}
    }
#endif
#else /* modified behavior */
    if (getenv("SEED") != NULL) {
	/* allow $SEED to set the initial state for debugging purposes */
	dnum = atoi(getenv("SEED"));
    } else {
	/* use time + pid to set the initial state */
	dnum = lowtime + md_getpid();
    }
#endif
    seed = dnum;

    /*
     * we use the random(3) facility, seeded, to generate random data for init_score()
     */
    srandom((unsigned)seed);

    open_score();

    /*
     * Drop setuid/setgid after opening the scoreboard file.
     */

    md_normaluser();

    /*
     * check for print-score option
     */

	md_normaluser(); /* we drop any setgid/setuid privileges here */

    if (argc == 2)
    {
	if (strcmp(argv[1], "-s") == 0)
	{
	    noscore = TRUE;
	    score(0, -1, 0);
	    exit(0);
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
	    dnum = rnd(100);	/* throw away some rnd()s to break patterns */
	    while (--dnum)
		rnd(100);
	    purse = rnd(100) + 1;
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
	else if (strcmp(argv[1], "-V") == 0)
	{
	    printf("version %s (chongo was here) %s\n", release, version);
	    exit(0);
	}
    }

    init_check();			/* check for legal startup */
    if (argc == 2)
	if (!restore(argv[1]))	/* Note: restore will never return */
	    my_exit(1);
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
    char pidfilename[BUFSIZ];
    FILE *pidfp;

    if (getenv("GETROGUEPID") != NULL) {
      pid = md_getpid ();
      memset (pidfilename, '\0', sizeof(pidfilename));
      snprintf (pidfilename, BUFSIZ-1, "roguepid.%d", pid);
      if ((pidfp = fopen (pidfilename, "w")) == NULL) {
        fprintf (stderr, "Can't open '%s'.\n", pidfilename);
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
	printf("\nSorry, the screen must be at least %dx%d\n", NUMLINES, NUMCOLS);
	endwin();
	my_exit(1);
    }

    /*
     * Set up windows
     */
    hw = newwin(LINES, COLS, 0, 0);
    idlok(stdscr, TRUE);
    idlok(hw, TRUE);
#ifdef MASTER
    if (wizard) {
	noscore = TRUE;
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
