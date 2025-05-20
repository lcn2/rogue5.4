/*
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 *
 * @(#)main.c	4.22 (Berkeley) 02/05/99
 */

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <curses.h>
#include "rogue.h"

/*
 * NOTE: A number of non-main functions were in main.c. However, tools such as findpw and scedit needed them too,
 *	 and those tools have their own main() function.  So we moved them into a new common.c file.
 */

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
     * get home and options from environment
     */

    strcpy(home, md_gethomedir());

	if (strlen(home) > MAXSTR - strlen(".rogue.save") - 1)
		*home = 0;

    strcpy(file_name, home);
    strcat(file_name, ".rogue.save");

    if ((env = getenv("ROGUEOPTS")) != NULL)
	parse_opts(env);
    if (env == NULL || whoami[0] == '\0')
        strucpy(whoami, md_getusername(), strlen(md_getusername()));
    lowtime = time(NULL);
    if (getenv("SEED") != NULL)
    {
	dnum = atoi(getenv("SEED"));
	noscore = 1;
    }
    else
	dnum = (unsigned int) lowtime + md_getpid();
    seed = dnum;

    open_score();

    /*
     * Drop setuid/setgid after opening the scoreboard file.
     */

    md_normaluser();

    /*
     * check for print-score option
     */

	md_normaluser(); /* we drop any setgid/setuid priveldges here */

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
	    level = rnd(100) + 1;
	    initscr();
	    getltchars();
	    death(death_monst());
	    exit(0);
	}
    }

    init_check();			/* check for legal startup */
    if (argc == 2)
	if (!restore(argv[1]))	/* Note: restore will never return */
	    my_exit(1);
#ifdef MASTER
    if (wizard)
	printf("Hello %s, welcome to dungeon #%d", whoami, dnum);
    else
#endif
	printf("Hello %s, just a moment while I dig the dungeon...", whoami);
    fflush(stdout);

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
    noscore = wizard;
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
