/*
 * File for the fun ends
 * Death or a total win
 *
 * @(#)rip.c	4.57 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>

#include "modern_curses.h"
#include "have_strlcat.h"
#include "have_strlcpy.h"
#include "strl.h"
#include "extern.h"
#include "config.h"
#include "score.h"
#include "rogue.h"

static char *rip[] = {
"                       __________\n",
"                      /          \\\n",
"                     /    REST    \\\n",
"                    /      IN      \\\n",
"                   /     PEACE      \\\n",
"                  /                  \\\n",
"                  |                  |\n",
"                  |                  |\n",
"                  |   killed by a    |\n",
"                  |                  |\n",
"                  |       1980       |\n",
"                 *|     *  *  *      | *\n",
"         ________)/\\\\_//(\\/(/\\)/\\//\\/|_)_______\n",
    0
};

/*
 * max length of a randomly generated username
 *
 * NOTE: Must be 1 <= RND_NAME_LEN <= MAX_USERNAME
 */
#define RND_NAME_LEN 8
#if RND_NAME_LEN > MAX_USERNAME
#error "invalid RND_NAME_LEN: Must be 1 <= RND_NAME_LEN <= MAX_USERNAME"
#endif

/*
 * init_score_value:
 *	Initialize a SCORE with random data.
 */

void
init_score_value(SCORE *scp)
{
    long rndname_len = 0;    /* length of a random name */
    int i;

    /*
     * initialize leading values in a SCORE
     */
    memset(scp, 0, sizeof(SCORE));
    scp->sc_uid = (uid_t) ((random()%59000)+1000); /* random UID from 1000 to 59999 */
    scp->sc_score = 0; /* 0 score will be at the bottom of the score set, and will NOT be printed */
    scp->sc_flags = 0; /* set death flag */
    scp->sc_monster = 'B'; /* just use the lowly bat as the monster */

    /*
     * form a random name using ASCII bytes from 33 (!) thru 126 (~)
     */
    memset(scp->sc_name, 0, sizeof(scp->sc_name)); /* paranoia */
    rndname_len = (random()%RND_NAME_LEN)+1; /* random name length from 1 to RND_NAME_LEN */
    for (i = 0; i < rndname_len; i++) {
	scp->sc_name[i] = (char) ((random()%94)+33); /* form ASCII bytes from 33 (!) thru 126 (~) */
    }
    scp->sc_name[MAX_USERNAME] = '\0'; /* paranoia */

    /*
     * random level that does NOT include the amulet level
     */
    scp->sc_level = (int) random()%25; /* random level 0 to 25 */

    /*
     * Random timestamp from 0 to 2^62-1
     *
     * NOTE: random(3) returns a value from 0 to 2^30-1
     */
    scp->sc_time = (((uintmax_t) random()) ^ (((uintmax_t) random()) << 31));
    return;
}


/*
 * score:
 *	Figure score and post it.
 *
 * We will ignore SIGHUP, SIGINT, SIGQUIT, and SIGTERM signal events.
 * then read the rogue score file, then insert the current game stats into
 * the score table if the current game ranks into the top scores,
 * print the "[Press return to continue]" and wait for the user to press return.
 *
 *	Wizard mode action: if wizard mode is enabled:
 *
 *	    Any text entered before the user presses return will be
 *	    parsed for a wizard keyword.  Currently there are 2 possible
 *	    wizard keywords:
 *
 *		names - set prflags to 1
 *		edit - set prflags to 2
 *
 * As this point we flush output, shutdown ncurses if setup, and restore both echo + canonical mode,
 * (because we are in the process ending game play, and will exit soon).
 *
 * We will then print the updated score table.
 *
 *	Wizard mode action: if wizard mode is enabled:
 *
 *	    prflags == 1: print usernames while printing slot scores
 *
 *	    prflags == 2: allow a score to be deleted
 *
 *		After each score line is printed, stdin will be read
 *		If the line read starts with the letter "d", the score
 *		just printed will be deleted from the score table.
 *
 * At this point the rogue score file will be replaced using the updated score table.
 *
 * Last free the score table, restore the SIGHUP, SIGINT, SIGQUIT, and SIGTERM signal handlers and return.
 *
 * NOTE: We only call this score function as we are ending game play.
 */

void
score(int amount, int flags, int monst)
{
    SCORE *scp;
    SCORE *sc2;
    SCORE *top_scores, *endp;
# ifdef MASTER
    int prflags = 0;
# endif
    uid_t uid;
    char *reason[] = {
	"killed",
	"quit",
	"A total winner",
	"killed with Amulet"
    };
#if defined(SIGHUP)
    void (*sig_hup)(int);
#endif
#if defined(SIGINT)
    void (*sig_int)(int);
#endif
#if defined(SIGQUIT)
    void (*sig_quit)(int);
#endif
#if defined(SIGTERM)
    void (*sig_term)(int);
#endif

    /*
     * temporarily disable SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGHUP)
    sig_hup = signal(SIGHUP, SIG_IGN);
#endif
#if defined(SIGINT)
    sig_int = signal(SIGINT, SIG_IGN);
#endif
#if defined(SIGQUIT)
    sig_quit = signal(SIGQUIT, SIG_IGN);
#endif
#if defined(SIGTERM)
    sig_term = signal(SIGTERM, SIG_IGN);
#endif
    md_tstphold();

    /*
     * Start the scoring sequence
     */
    start_score();

    /*
     * initialize top_scores array
     */
    top_scores = calloc(numscores+1, sizeof(SCORE)); /* +1 for paranoia */
    if (top_scores == NULL)
	return;
    endp = &top_scores[numscores];
    for (scp = top_scores; scp < endp; scp++)
    {
	init_score_value(scp);
    }

    /*
     * wait for the user to press return
     *
     * NOTE: If wizard mode, entering names or edit before the return
     *	     will enable later actions via prflags.
     */
 if (flags >= 0
#ifdef MASTER
            || wizard
#endif
        )
    {
	mvaddstr(LINES - 1, 0 , "[Press return to continue]");
        refresh();
        wgetnstr(stdscr, prbuf, NUMCOLS);   /* read up to NUMCOLS bytes and append a NUL byte */
    }
#ifdef MASTER
    if (wizard) {
	if (strcmp(prbuf, "names") == 0) {
	    prflags = 1;
	} else if (strcmp(prbuf, "edit") == 0) {
	    prflags = 2;
	}
    }
#endif

    /*
     * flush output, and shutdown ncurses if setup, restore echo and canonical mode
     */
    endwin_and_ncurses_cleanup();

    /*
     * read the score file
     *
     * NOTE: The rd_score() function will print an ERROR, WARNING, and exit non-zero
     *	     if the rogue score file is corrupted (i.e., does not correctly parse).
     */
    rd_score(top_scores);

    /*
     * Insert her in list if need be
     */
    sc2 = NULL;
    if (!noscore)
    {
	uid = md_getuid();
	for (scp = top_scores; scp < endp; scp++)
	    if (amount > scp->sc_score)
		break;
#if !defined(ALLSCORES)
	    else if (!allscore &&	/* only one score per nowin uid */
		flags != 2 && scp->sc_uid == uid && scp->sc_flags != 2)
		    scp = endp;
#endif
	if (scp < endp)
	{
#if !defined(ALLSCORES)
	    if (flags != 2 && !allscore)
	    {
		for (sc2 = scp; sc2 < endp; sc2++)
		{
		    if (sc2->sc_uid == uid && sc2->sc_flags != 2)
			break;
		}
		if (sc2 >= endp)
		    sc2 = endp - 1;
	    }
	    else
		sc2 = endp - 1;
#else
	    sc2 = endp - 1;
#endif
	    while (sc2 > scp)
	    {
		*sc2 = sc2[-1];
		sc2--;
	    }
	    memset(scp, 0, sizeof(*scp)); /* paranoia */
	    scp->sc_score = amount;
	    strlcpy(scp->sc_name, whoami, MAX_USERNAME);
	    scp->sc_name[MAX_USERNAME] = '\0'; /* paranoia */
	    scp->sc_flags = flags;
	    if (flags == 2)
		scp->sc_level = max_level;
	    else
		scp->sc_level = level;
	    scp->sc_monster = monst;
	    scp->sc_uid = uid;
	    scp->sc_time = (uintmax_t) time(NULL);
	    sc2 = scp;
	}
    }

    /*
     * Print the score table
     *
     * We will print scores until we find a score slot with a - score.
     */
    if (flags != -1)
	putchar('\n');
#if defined(ALLSCORES)
    printf("Top %d Scores:\n", NUMSCORES);
#else
    printf("Top %d Rogueists:\n", NUMSCORES);
#endif
    printf("   Score Name\n");
    fflush(stdout);
    for (scp = top_scores; scp < endp; scp++)
    {
	if (scp->sc_score > 0) {
	    if (sc2 == scp)
		md_raw_standout();
	    printf("%2d %5d %s: %s on level %d",
		(int) (scp - top_scores + 1), scp->sc_score, scp->sc_name,
		reason[scp->sc_flags], scp->sc_level);
	    if (scp->sc_flags == 0 || scp->sc_flags == 3)
		printf(" by %s", killname(scp->sc_monster, true));
#ifdef MASTER
	    if (prflags == 1)
	    {
		printf(" (%s)", md_getrealname(scp->sc_uid));
	    }
	    else if (prflags == 2)
	    {
		memset(prbuf, 0, sizeof(prbuf));
		fflush(stdout);
		(void) fgets(prbuf,10,stdin);
		if (prbuf[0] == 'd')
		{
		    for (sc2 = scp; sc2 < endp - 1; sc2++)
			*sc2 = *(sc2 + 1);
		    sc2 = endp - 1;
		    init_score_value(sc2);
		    scp--;
		}
	    }
	    else
#endif /* MASTER */
                putchar('.');
	    if (sc2 == scp)
		md_raw_standend();
            putchar('\n');
	    fflush(stdout);
	}
	else
	{
	    /* found a score slot with a 0 score, stop printing */
	    break;
	}
    }
    putchar('\n');
    fflush(stdout);

    /*
     * Update the list file
     */
    if (sc2 != NULL)
    {
	/*
	 * lock and update score
	 */
	if (lock_sc())
	{
	    /*
	     * write score and unlock
	     */
	    wr_score(top_scores);
	    unlock_sc();
	}
    }

    /*
     * free top score list
     */
    if (top_scores != NULL) {
	free(top_scores);
	top_scores = NULL;
    }

    /*
     * restore SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGINT)
    signal(SIGINT, sig_hup);
#endif
#if defined(SIGINT)
    signal(SIGINT, sig_int);
#endif
#if defined(SIGQUIT)
    signal(SIGQUIT, sig_quit);
#endif
#if defined(SIGTERM)
    signal(SIGTERM, sig_term);
#endif
    md_tstpresume();
}

/*
 * death:
 *	Do something really fun when he dies
 */

void
death(int monst)
{
    char **dp;
    const char *killer;
    struct tm *lt;
    time_t date;
#if defined(SIGHUP)
    void (*sig_hup)(int);
#endif
#if defined(SIGINT)
    void (*sig_int)(int);
#endif
#if defined(SIGQUIT)
    void (*sig_quit)(int);
#endif
#if defined(SIGTERM)
    void (*sig_term)(int);
#endif

    /*
     * temporarily disable SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGHUP)
    sig_hup = signal(SIGHUP, SIG_IGN);
#endif
#if defined(SIGINT)
    sig_int = signal(SIGINT, SIG_IGN);
#endif
#if defined(SIGQUIT)
    sig_quit = signal(SIGQUIT, SIG_IGN);
#endif
#if defined(SIGTERM)
    sig_term = signal(SIGTERM, SIG_IGN);
#endif

    /*
     * 10% death tax :-)
     */
    purse -= purse / 10;
    clear();
    killer = killname(monst, false);
    if (!tombstone)
    {
	mvprintw(LINES - 2, 0, "Killed by ");
	killer = killname(monst, false);
	if (monst != 's' && monst != 'h')
	    printw("a%s ", vowelstr(killer));
	printw("%s with %d gold", killer, purse);
    }
    else
    {
	time(&date);
	lt = localtime(&date);
	move(8, 0);
	dp = rip;
	while (*dp)
	    addstr(*dp++);
	mvaddstr(17, center(killer), killer);
	if (monst == 's' || monst == 'h')
	    mvaddch(16, 32, ' ');
	else
	    mvaddstr(16, 33, vowelstr(killer));
	mvaddstr(14, center(whoami), whoami);
	memset(prbuf, 0, sizeof(prbuf)); /* paranoia */
	snprintf(prbuf, sizeof(prbuf), "%d Au", purse);
	move(15, center(prbuf));
	addstr(prbuf);
	memset(prbuf, 0, sizeof(prbuf)); /* paranoia */
	snprintf(prbuf, sizeof(prbuf), "%4d", 1900+lt->tm_year);
	mvaddstr(18, 26, prbuf);
    }
    move(LINES - 1, 0);
    refresh();
    score(purse, amulet ? 3 : 0, monst);

    /*
     * restore SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGINT)
    signal(SIGINT, sig_hup);
#endif
#if defined(SIGINT)
    signal(SIGINT, sig_int);
#endif
#if defined(SIGQUIT)
    signal(SIGQUIT, sig_quit);
#endif
#if defined(SIGTERM)
    signal(SIGTERM, sig_term);
#endif
    my_exit(0);
}

/*
 * center:
 *	Return the index to center the given string
 */
int
center(const char *str)
{
    return 28 - (((int)strlen(str) + 1) / 2);
}

/*
 * total_winner:
 *	Code for a winner
 */

void
total_winner(void)
{
    THING *obj;
    struct obj_info *op;
    int worth = 0;
    int oldpurse;
#if defined(SIGHUP)
    void (*sig_hup)(int);
#endif
#if defined(SIGINT)
    void (*sig_int)(int);
#endif
#if defined(SIGQUIT)
    void (*sig_quit)(int);
#endif
#if defined(SIGTERM)
    void (*sig_term)(int);
#endif

    /*
     * temporarily disable SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGHUP)
    sig_hup = signal(SIGHUP, SIG_IGN);
#endif
#if defined(SIGINT)
    sig_int = signal(SIGINT, SIG_IGN);
#endif
#if defined(SIGQUIT)
    sig_quit = signal(SIGQUIT, SIG_IGN);
#endif
#if defined(SIGTERM)
    sig_term = signal(SIGTERM, SIG_IGN);
#endif

    clear();
    standout();
    addstr("                                                               \n");
    addstr("  @   @               @   @           @          @@@  @     @  \n");
    addstr("  @   @               @@ @@           @           @   @     @  \n");
    addstr("  @   @  @@@  @   @   @ @ @  @@@   @@@@  @@@      @  @@@    @  \n");
    addstr("   @@@@ @   @ @   @   @   @     @ @   @ @   @     @   @     @  \n");
    addstr("      @ @   @ @   @   @   @  @@@@ @   @ @@@@@     @   @     @  \n");
    addstr("  @   @ @   @ @  @@   @   @ @   @ @   @ @         @   @  @     \n");
    addstr("   @@@   @@@   @@ @   @   @  @@@@  @@@@  @@@     @@@   @@   @  \n");
    addstr("                                                               \n");
    addstr("     Congratulations, you have made it to the light of day!    \n");
    standend();
    addstr("\nYou have joined the elite ranks of those who have escaped the\n");
    addstr("Dungeons of Doom alive.  You journey home and sell all your loot at\n");
    addstr("a great profit and are admitted to the Fighters' Guild.\n");
    mvaddstr(LINES - 1, 0, "--Press space to continue--");
    refresh();
    wait_for(stdscr, ' ');
    clear();
    mvaddstr(0, 0, "   Worth  Item\n");
    oldpurse = purse;
    for (obj = pack; obj != NULL; obj = next(obj))
    {
	switch (obj->o_type)
	{
	    case FOOD:
		worth = 2 * obj->o_count;
	    when WEAPON:
		worth = weap_info[obj->o_which].oi_worth;
		worth *= 3 * (obj->o_hplus + obj->o_dplus) + obj->o_count;
		obj->o_flags |= ISKNOW;
	    when ARMOR:
		worth = arm_info[obj->o_which].oi_worth;
		worth += (9 - obj->o_arm) * 100;
		worth += (10 * (a_class[obj->o_which] - obj->o_arm));
		obj->o_flags |= ISKNOW;
	    when SCROLL:
		worth = scr_info[obj->o_which].oi_worth;
		worth *= obj->o_count;
		op = &scr_info[obj->o_which];
		if (!op->oi_know)
		    worth /= 2;
		op->oi_know = true;
	    when POTION:
		worth = pot_info[obj->o_which].oi_worth;
		worth *= obj->o_count;
		op = &pot_info[obj->o_which];
		if (!op->oi_know)
		    worth /= 2;
		op->oi_know = true;
	    when RING:
		op = &ring_info[obj->o_which];
		worth = op->oi_worth;
		if (obj->o_which == R_ADDSTR || obj->o_which == R_ADDDAM ||
		    obj->o_which == R_PROTECT || obj->o_which == R_ADDHIT)
		{
			if (obj->o_arm > 0)
			    worth += obj->o_arm * 100;
			else
			    worth = 10;
		}
		if (!(obj->o_flags & ISKNOW))
		    worth /= 2;
		obj->o_flags |= ISKNOW;
		op->oi_know = true;
	    when STICK:
		op = &ws_info[obj->o_which];
		worth = op->oi_worth;
		worth += 20 * obj->o_charges;
		if (!(obj->o_flags & ISKNOW))
		    worth /= 2;
		obj->o_flags |= ISKNOW;
		op->oi_know = true;
	    when AMULET:
		worth = 1000;
	}
	if (worth < 0)
	    worth = 0;
	printw("%c) %5d  %s\n", obj->o_packch, worth, inv_name(obj, false));
	purse += worth;
    }
    printw("   %5d  Gold Pieces          ", oldpurse);
    refresh();
    score(purse, 2, ' ');
    my_exit(0);

    /*
     * restore SIGHUP, SIGINT, SIGQUIT, and SIGTERM
     */
#if defined(SIGINT)
    signal(SIGINT, sig_hup);
#endif
#if defined(SIGINT)
    signal(SIGINT, sig_int);
#endif
#if defined(SIGQUIT)
    signal(SIGQUIT, sig_quit);
#endif
#if defined(SIGTERM)
    signal(SIGTERM, sig_term);
#endif
}

/*
 * killname:
 *	Convert a code to a monster name
 */
const char *
killname(int monst, int doart)
{
    struct h_list *hp;
    const char *sp;
    int article;
    struct h_list nlist[] = {
	{'a',	"arrow",		true},
	{'b',	"bolt",			true},
	{'d',	"dart",			true},
	{'h',	"hypothermia",		false},
	{'s',	"starvation",		false},
	{'\0'}
    };

    if (isupper(monst))
    {
	sp = monsters[monst-'A'].m_name;
	article = true;
    }
    else
    {
	sp = "Wally the Wonder Badger";
	article = false;
	for (hp = nlist; hp->h_ch; hp++)
	    if (hp->h_ch == monst)
	    {
		sp = hp->h_desc;
		article = hp->h_print;
		break;
	    }
    }
    if (doart && article) {
	memset(prbuf, 0, sizeof(prbuf)); /* paranoia */
	snprintf(prbuf, sizeof(prbuf), "a%s ", vowelstr(sp));
    } else {
	memset(prbuf, 0, sizeof(prbuf)); /* paranoia */
    }
    strcat(prbuf, sp);
    return prbuf;
}

/*
 * death_monst:
 *	Return a monster appropriate for a random death.
 */
int
death_monst(void)
{
    int poss[] =
    {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'h', 'd', 's',
	' '	/* This is provided to generate the "Wally the Wonder Badger"
		   message for killer */
    };

    return poss[rnd(sizeof poss / sizeof (int))];
}
