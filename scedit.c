/*
 * score editor
 *
 * @(#)score.c	4.6 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

# include	<stdio.h>
# include	<signal.h>
# include	<ctype.h>
# include	<unistd.h>
# include	<stdlib.h>
# include	<string.h>
# include	<stdbool.h>
# include	<sys/types.h>
# include	<sys/time.h>
# include	<sys/stat.h>

# include	"modern_curses.h"
# include	"extern.h"
# include	"config.h"
# include	"score.h"

char whoami[MAX_USERNAME + 1] = {'\0'};	/* Name of player, +1 for paranoia */

static SCORE	top_scores[NUMSCORES + 1];	/* scores from the score file, +1 for paranoia */

static char	*reason[] = {
			"killed",
			"quit",
			"A total winner",
			"killed with amulet",
		};

#if defined(NON_BSD_RN_GENERATOR)
static unsigned int seed;
#endif
static unsigned int dnum;
static char     *scorefile;

int do_comm(void);
int pr_score(SCORE *, int);


/* forward declarations */

int	add_score(void);
void	del_score(void);
int	insert_score(SCORE *new);
int	pr_score(SCORE *scp, int num);

/* duplicated external declarations - because including extern.h and rogue.h brings in too much other stuff */

extern pid_t	md_getpid(void);
extern void	s_encwrite(char *start, size_t size, FILE *outf);
extern char	*md_getrealname(uid_t uid);
extern int	lock_sc(void);
extern void	unlock_sc(void);

/* external functions from scmisc.c */

extern char *	s_killname(int monst, int doart);


int
main(int ac, char *av[])
{
	struct timeval tp;
	SCORE *scp;

	 /*
	  * set mode for write access for the owner only
	  *
	  * NOTE: This is needed prior to calling md_init().
	  */
	(void) umask(S_IWGRP | S_IWOTH);

	/*
	 * parse args
	 */
	if (ac == 1) {

		/*
		 * form critical paths
		 *
		 * FYI: rogue save file path not needed.
		 */
		form_home();
		form_lock_path();
		form_score_path();
		scorefile = score_path;

	} else {
		scorefile = av[1];
	}

	/*
	 * set dnum as if it were a dungeon number
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
	char *env;

	if (wizard) {
	    env = getenv("SEED");
	    if (env != NULL) {
		dnum = (unsigned int)strtol(env, NULL, 0);
	    }
	}
#endif

	/*
	 * seed with the dungeon number
	 */
#if defined(NON_BSD_RN_GENERATOR)
	seed = (dnum ^ (dnum >> 16)) & 0xffff;  /* the RN macro uses a 16-bit seed */
#else
	srandom((unsigned)dnum);
#endif

	/*
	 * initialize top_scores array
	 */
	for (scp = top_scores; scp <  &top_scores[NUMSCORES]; scp++)
	{
	    init_score_value(scp);
	}

	/*
	 * open score file
	 */
	if ((scoreboard = fopen(scorefile, "r+")) == NULL) {
		perror(scorefile);
		exit(1);
	}

	/*
	 * read the score file
	 *
	 * NOTE: The rd_score() function will print an ERROR, WARNING, and exit non-zero
	 *       if the rogue score file is corrupted (i.e., does not correctly parse).
	 */
	rd_score(top_scores);

	/*
	 * process scedit commands until quit
	 */
	while (do_comm())
		continue;
	fclose(scoreboard);
	exit(0);
}

/*
 * do_comm:
 *	Get and execute a command
 */
int
do_comm(void)
{
	SCORE *scp;
	static int modified = false; /* true ==> top score table modified but not yet written to disk */
	int added = false;	     /* true ==> new score was inserted into top score table */
	char buf[SHORTSTR + 1];	     /* command buffer, +1 for paranoia */
	char *ret = NULL;	     /* fgets() return */

	/*
	 * show the current rogue scores
	 */
	printf("\nTop Ten Rogueists:\nRank\tScore\tName\n");
	fflush(stdout);
	for (scp = top_scores; scp < &top_scores[NUMSCORES]; scp++)
		if (!pr_score(scp, true))
			break;
	putchar('\n');

	/*
	 * print command table
	 */
	printf("Valid commands:\n");
	putchar('\n');
	printf("    a - add score to rogue score file\n");
	printf("    d - delete score from rogue score file\n");
	printf("    f - print rogue score file path\n");
	printf("    p - print rogue score file\n");
	printf("    w - write rogue score file\n");
	printf("    q - quit\n");
	printf("    Q - Force QUIT without saving changes\n");

	/*
	 * read command from stdin
	 */
	do {
		fflush(stdin);
		printf("\nCommand: ");
		fflush(stdout);
		memset(buf, 0, sizeof(buf));
		ret = fgets(buf, SHORTSTR, stdin);
	} while (ret != NULL && isspace(buf[0]));

	/*
	 * process command line
	 */
	if (ret == NULL) {
	    return false;
	}
	switch (buf[0]) {
        case 'w':
		if (lock_sc())
		{
			wr_score(top_scores);
			unlock_sc();
		}
		printf("\nScore file successfully updated: %s\n", scorefile);
		modified = false;
		break;

	case 'a':
		added = add_score();
		if (added) {
			modified = true;
		}
		break;

	case 'd':
		del_score();
		modified = true;
		break;

	case 'f':
		printf("\nRogue score file path: %s\n", scorefile);
		break;

	case 'p':
		/* rogue scores will be printed by the next command prompt cycle */
		break;

	case 'q':
		if (modified) {
			printf("\nChanges to rogue score file not yet saved!\n");
			fflush(stdout);
		} else {
			return false; /* quit */
		}
		break;

	case 'Q':
		if (!modified) {
			printf("\nQuitting without saving rogue score file changes!\n");
			fflush(stdout);
		}
		return false; /* quit */
		break;

	case EOF:
		printf("EOF\n");
		fflush(stdout);
		return false;
		break;

	default:
		printf("Unknown command character: \"%c\"\n", buf[0]);
		fflush(stdout);
	}
	return true;
}

/*
 * add_score:
 *	Add a score to the score file
 *
 * Returns:
 *	true ==> score was added
 *	false ==> score was not added
 */

int
add_score(void)
{
	uid_t id = 0;
	SCORE new;
	char buf[BUFSIZ + 1]; /* +1 for paranoia */
	int tmp = 0;
	int answer = false;
	int i;

	printf("Rogue player name: ");
	fflush(stdout);
	(void) fgets(new.sc_name, MAX_USERNAME, stdin);
	new.sc_name[strlen(new.sc_name) - 1] = '\0';
	do {

		/*
		 * ask for a reason
		 */
		printf("reason [0: %s: 1: %s 2: %s: 3: %s]: ",
		       reason[0], reason[1], reason[2], reason[3]);
		new.sc_flags = getchar();

		/*
		 * validate reason
		 */
		if (new.sc_flags == EOF) {
			printf("\nRead error or EOF on stdin\n");
			fflush(stdout);
			return false;
		}
		if (new.sc_flags < '0' || new.sc_flags > '3') {
			printf("\nInvalid reason: must be a number: 0 thru 3\n\n");
			fflush(stdout);
			ungetc(new.sc_flags, stdin);
			continue;
		}

		/*
		 * toss input until newline or EOF
		 */
		do {
			tmp = getchar();
			if (tmp == EOF) {
				printf("\nRead error or EOF on stdin\n");
				fflush(stdout);
				return false;
			}
		} while (tmp != '\n');
	} while (new.sc_flags < '0' || new.sc_flags > '3');

	/*
	 * convert reason into a sc_flag
	 */
	new.sc_flags -= '0';

	/*
	 * ask for the uid
	 */
	do {
		printf("Numerical user id: ");
		fflush(stdout);
		memset(buf, 0, sizeof(buf)); /* paranoia */
		(void) fgets(buf, BUFSIZ, stdin);
		buf[strlen(buf) - 1] = '\0';
		id = atoi(buf);
	} while (id == -1);
	new.sc_uid = id;

	/*
	 * determine cause of death
	 */
	if (new.sc_flags == 0 || new.sc_flags == 3) {
		do {

			/*
			 * prompt for cause of death
			 */
			printf("Monster letter [A-Z], a(rrow) b(olt) d(art) h(ypothermia) s(tarvation): ");
			new.sc_monster = getchar();
			if (!isalpha(new.sc_monster)) {
				printf("type A-Za-z [%s]\n", unctrl(new.sc_monster));
				fflush(stdout);
				ungetc(new.sc_monster, stdin);
			}

			/*
			 * toss input until newline or EOF
			 */
			do {
				tmp = getchar();
				if (tmp == EOF) {
					printf("\nRead error or EOF on stdin\n");
					fflush(stdout);
					return false;
				}
			} while (tmp != '\n');
		} while (!isalpha(new.sc_monster));
	} else {
		new.sc_monster = ' ';
	}

	/*
	 * enter final score
	 */
	printf("Score: ");
	fflush(stdout);
	scanf("%d", &new.sc_score);

	/*
	 * enter level
	 */
	new.sc_level = -1; /* invalid level */
	do {
		/*
		 * prompt for level
		 */
		switch (new.sc_flags) {
		case 0:
			printf("Death level: ");
			break;
		case 1:
			printf("Quit level: ");
			break;
		case 2:
			printf("Deepest level reached: ");
			break;
		case 3:
			printf("Death level with amulet: ");
			break;
		default:
			printf("Bogus flag: %d level: ", new.sc_flags);
			break;
		}
		fflush(stdout);

		/*
		 * read integer
		 */
		tmp = -1;
		i = scanf("%d", &tmp);
		if (i != 1 && tmp < 0) {
			if (i == EOF) {
				printf("\nRead error or EOF on stdin\n");
				fflush(stdout);
				return false;
			}
			printf("\nLevel must be an integer >= 0\n\n");
			continue;
		}
		new.sc_level = tmp;

		/*
		 * toss input until newline or EOF
		 */
		do {
			tmp = getchar();
			if (tmp == EOF) {
				printf("\nRead error or EOF on stdin\n");
				fflush(stdout);
				return false;
			}
		} while (tmp != '\n');
	} while (new.sc_level < 0);

	/*
	 * ask to confirm new score entry
	 */
	printf("\nConfirm new score entry:\n\n");
	fflush(stdout);
	pr_score(&new, false);
	tmp = '\0';
	do {

		/*
		 * ask if we should add the new score
		 */
		printf("\nReally add the new score? ");
		fflush(stdout);
		answer = getchar();

		/*
		 * toss input until newline or EOF
		 */
		do {
			tmp = getchar();
			if (tmp == EOF) {
				printf("\nRead error or EOF on stdin\n");
				fflush(stdout);
				return false;
			}
		} while (tmp != '\n');

		/*
		 * insert if confirmed
		 */
		if (answer == 'y' || answer == 'Y') {
			int inserted;

			/*
			 * attempt to insert score
			 */
			inserted = insert_score(&new);
			printf("\n%s new score\n", (inserted ? "Inserted" : "Score too small, did not insert"));
			return inserted;

		} else if (answer == 'n' || answer == 'N') {
			printf("\nNew score not inserted\n");
			return false;
		} else {
			printf("\nPlease answer y or n\n");
		}
	} while (answer != 'n' && answer != 'N');
	return false;
}

/*
 * pr_score:
 *	Print out a score entry.  Return false if last entry.
 */

int
pr_score(SCORE *scp, int num)
{
	if (scp->sc_score) {
		if (num)
			printf("%ld", scp - top_scores + 1);
		printf("\t%d\t%s: %s on level %d", scp->sc_score, scp->sc_name,
		    reason[scp->sc_flags], scp->sc_level);
		if (scp->sc_flags == 0)
		    printf(" by %s", s_killname(scp->sc_monster, true));

        printf(" (%s)", md_getrealname(scp->sc_uid));
		putchar('\n');
	}
	fflush(stdout);
	return scp->sc_score;
}

/*
 * insert_score:
 *	Insert a score into the top score list
 *
 * Returns:
 *	true ==> score was inserted into the top score list
 *	false ==> score is too small to be inserted into the top score list
 */
int
insert_score(SCORE *new)
{
	SCORE	*scp;
	SCORE	*sc2;
	SCORE	*endp;
#if !defined(ALLSCORES)
	int	flags;
	uid_t	uid;
#endif
	int	amount;

	/*
	 * setup
	 */
#if !defined(ALLSCORES)
	flags = new->sc_flags;
	uid = new->sc_uid;
#endif
	amount = new->sc_score;
	endp = &top_scores[NUMSCORES];

	/*
	 * skip over score slots with higher priority
	 */
	for (scp = top_scores; scp < endp; scp++) {
		if (amount > scp->sc_score)
			break;
#if !defined(ALLSCORES)
		else if (flags != 2 && scp->sc_uid == uid && scp->sc_flags != 2)
			scp = endp;
#endif
	}

	/*
	 * case: score cannot be inserted in the top score table: it is too small
	 */
	if (scp >= endp) {
		return false;
	}

	/*
	 * insert score
	 */
	if (scp < endp) {
#if !defined(ALLSCORES)
		if (flags != 2)
			for (sc2 = scp; sc2 < &top_scores[NUMSCORES]; sc2++) {
			    if (sc2->sc_uid == uid && sc2->sc_flags != 2)
				break;
			}
		else
			sc2 = endp - 1;
#else
		sc2 = endp - 1;
#endif

		/*
		 * move other scores down
		 */
		while (sc2 > scp) {
			*sc2 = sc2[-1];
			sc2--;
		}
		*scp = *new;
		sc2 = scp;
	}
	return true;
}

/*
 * del_score:
 *	Delete a score from the score list.
 */
void
del_score(void)
{
	SCORE	*scp;
	int	num;
	char buf[BUFSIZ + 1]; /* +1 for paranoia */

	for (;;) {
		printf("Which score? ");
		fflush(stdout);
		memset(buf, 0, sizeof(buf)); /* paranoia */
		(void) fgets(buf, BUFSIZ, stdin);
		if (buf[0] == '\n')
			return;
		sscanf(buf, "%d", &num);
		if (num < 1 || num > NUMSCORES) {
			printf("range is 1-%d\n", NUMSCORES);
			fflush(stdout);
		} else {
			break;
		}
	}
	num--;
	for (scp = &top_scores[num]; scp < &top_scores[9]; scp++)
		*scp = scp[1];
	init_score_value(&top_scores[NUMSCORES-1]);
}
