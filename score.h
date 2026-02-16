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
 */
#define MAX_USERNAME	32

struct sc_ent {
    uid_t sc_uid;
    int sc_score;
    unsigned int sc_flags;
    int sc_monster;
    char sc_name[MAX_USERNAME+1];
    int sc_level;
    uintmax_t sc_time;
};

typedef struct sc_ent SCORE;

extern char whoami[MAX_USERNAME+1];	/* Name of player, +1 for paranoia */

void	init_score_value(SCORE *scp);
void	rd_score(SCORE *top_score);
void	wr_score(SCORE *top_score);


#endif
