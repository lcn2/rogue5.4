/*
 * Score file structure
 *
 * @(#)score.h	4.6 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <time.h>
#include <stdint.h>

/*
 * default basename of special files
 */
#if !defined(SCORENAME)
#define SCORENAME ".rogue.scr"
#endif

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

/* maximum length the score line apart from the username */
#define	MAX_OTHER_SCORE 80

/* must be an just integer = MAX_USERNAME+MAX_OTHER_SCORE */
#define	MAXSCORELINE   (MAX_USERNAME+MAX_OTHER_SCORE)

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

void	init_score_value(SCORE *scp);
void	rd_score(SCORE *top_score);
void	wr_score(SCORE *top_score);
