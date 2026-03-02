/*
 * Score file structure
 *
 * @(#)score.h	4.6 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */


#if !defined(INCLUDE_SCORE_H)
#define INCLUDE_SCORE_H


#include <time.h>
#include <stdint.h>

/*
 * maximum username length
 *
 * NOTE: A number of C compilers do not correctly process a sscanf(3) line such as:
 *
 *	 sscanf(string, "%*s", MAX_USERNAME, str);
 *
 * So we must HARD code the maximum width in the sscanf(3) call found in rd_score()
 * in the save.c file.  The value MAX_USERNAME MUST match the %32s format string width.
 *
 * NOTE: Regarding "cause of death" and sc_monster:
 *
 *	If sc_monster is an UPPER CASE letter, then it the monster that killed,
 *	else if
 *	    a ==> killed by arrow
 *	    b ==> killed by lightning bolt
 *	    d ==> killed by dart
 *	    h ==> death by hypothermia
 *	    s ==> death by starvation
 *	else
 *	    killed by "Wally the Wonder Badger"
 */
#define MAX_USERNAME	32

struct sc_ent {
    uid_t sc_uid;		    /* uid of player */
    int sc_score;		    /* 0 ==> score slot is unused. >0 ==> score death, quit, or win */
    unsigned int sc_flags;	    /* 0 ==> killed by monster, 1 ==> quit, 2 ==> total winner, 3 ==> killed with Amulet */
    int sc_monster;		    /* sc_flags == 0 or 3 cause of death, sc_flags == 1 or 2 ==> unused, set to ' ' (space) */
    char sc_name[MAX_USERNAME +1 ]; /* Name of player in this score slot, +1 for paranoia */
    int sc_level;		    /* sc_flags == 1 ==> quit level, sc_flags == 2 ==> deepest level, else level killed on */
    uintmax_t sc_time;		    /* timestamp of when the score slot was updated */
};

typedef struct sc_ent SCORE;

extern char whoami[MAX_USERNAME + 1];	/* Name of player, +1 for paranoia */

void	init_score_value(SCORE *scp);
void	rd_score(SCORE *top_score);
void	wr_score(SCORE *top_score);


#endif
