/*
 * score editor
 *
 * @(#)score.c	4.6 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

# include   <ncurses.h>
# include	<stdio.h>
# include	<signal.h>
# include	<ctype.h>
# include	<unistd.h>
# include	<stdlib.h>
# include	<string.h>
# include	<stdbool.h>

# undef		TRUE
# define	TRUE true
# undef		FALSE
# define	FALSE false

# define	RN	(((seed = seed*11109+13849) >> 16) & 0xffff)

# define	MAXSTR	1024

# include	"score.h"
# include	"config.h"

static SCORE	top_scores[NUMSCORES+1];	/* scores from the score file, +1 for paranoia */

static char home[MAXSTR+1] = { '\0' };	/* User's home directory */

static char	buf[BUFSIZ+1+1],	/* +1 for when we read BUFSIZ starting with &buf[1], +1 for paranoia */
		*reason[] = {
			"killed",
			"quit",
			"A total winner",
			"killed with amulet",
		};

static int	seed;
static FILE	*finf;
static char	*scorefile;

int do_comm(void);
int pr_score(SCORE *, int);


/* forward declarations */

void	add_score(void);
void	del_score(void);
void	insert_score(SCORE *new);
int	pr_score(SCORE *scp, int num);

/* duplicated external declarations - because including extern.h and rogue.h brings in too much other stuff */

extern pid_t	md_getpid(void);
extern void	s_encread(char *start, size_t size, int inf);
extern int	s_lock_sc(void);
extern void	s_encwrite(char *start, size_t size, FILE *outf);
extern void	s_unlock_sc(void);
extern char	*md_getrealname(uid_t uid);
extern char	*md_gethomedir(void);

/* external functions from scmisc.c */

extern char *	s_killname(int monst, int doart);


int
main(int ac, char *av[])
{
	int	inf;
	size_t  scorename_len = 0;
	size_t  home_len = 0;

	memset(top_scores, 0, sizeof(top_scores)); /* paranoia */
	if (ac == 1) {

	    /*
	     * get home and options from environment
	     */

	    strncpy(home, md_gethomedir(), MAXSTR);
	    home[MAXSTR] = '\0'; /* paranoia */
	    scorename_len = strlen(SCORENAME);
	    home_len = strlen(home);
	    if (home_len > MAXSTR - scorename_len - 1)
		home[0] = '\0';
	    scorefile = calloc(MAXSTR+scorename_len+1, 1);
	    if (scorefile == NULL) {
		perror("calloc for scorefile failed");
		exit(1);
	    }
	    snprintf(scorefile, MAXSTR+scorename_len+1, "%s%s", home, SCORENAME);
	} else {
		scorefile = av[1];
	}
	seed = md_getpid();

	if ((finf = fopen(scorefile, "r+")) == NULL) {
		perror(scorefile);
		exit(1);
	}
	inf = fileno(finf);
	if (inf < 0) {
		perror(scorefile);
		exit(1);
	}
	s_encread((char *) top_scores, sizeof top_scores, inf);

	while (do_comm())
		continue;

	exit(0);
}

/*
 * do_comm:
 *	Get and execute a command
 */
int
do_comm(void)
{
	SCORE		*scp;
	static FILE	*outf = NULL;
	static int written = TRUE;

	printf("\nCommand: ");
	while (isspace(buf[0] = getchar()) || buf[0] == '\n')
		continue;
	memset(&buf[1], 0, sizeof(buf)-1); /* paranoia */
	(void) fgets(&buf[1], BUFSIZ, stdin);
	buf[strlen(buf) - 1] = '\0';
	switch (buf[0]) {
	  case 'w':
		if (strncmp(buf, "write", strlen(buf)))
			goto def;
		fseek(finf, 0L, 0);
		if (outf == NULL && (outf = fopen(scorefile, "w")) == NULL) {
			perror(scorefile);
			exit(1);
		}
		fseek(outf, 0L, 0);
		if (s_lock_sc())
		{
			void (*fp)(int);

			fp = signal(SIGINT, SIG_IGN);
			s_encwrite((char *) top_scores, sizeof top_scores, outf);
			s_unlock_sc();
			signal(SIGINT, fp);
			written = TRUE;
		}
		break;

	  case 'a':
		if (strncmp(buf, "add", strlen(buf)))
			goto def;
		add_score();
		written = FALSE;
		break;

	  case 'd':
		if (strncmp(buf, "delete", strlen(buf)))
			goto def;
		del_score();
		written = FALSE;
		break;

	  case 'p':
		if (strncmp(buf, "print", strlen(buf)))
			goto def;
		printf("\nTop Ten Rogueists:\nRank\tScore\tName\n");
		for (scp = top_scores; scp < &top_scores[10]; scp++)
			if (!pr_score(scp, TRUE))
				break;
		break;

	  case 'q':
		if (strncmp(buf, "quit", strlen(buf)))
			goto def;
		if (!written) {
			printf("No write\n");
			written = TRUE;
		}
		else
			return FALSE;
		break;

	  default:
def:
		printf("Unknown command: \"%s\"\n", buf);
	}
	return TRUE;
}

/*
 * add_score:
 *	Add a score to the score file
 */

void
add_score(void)
{
	uid_t id = 0;
	int		i;
	SCORE	new;

	printf("Name: ");
	(void) fgets(new.sc_name, MAX_USERNAME, stdin);
	new.sc_name[strlen(new.sc_name) - 1] = '\0';
	do {
		printf("reason: ");
		if ((new.sc_flags = getchar()) < '0' || new.sc_flags > '2') {
			for (i = 0; i < 3; i++)
				printf("%d: %s\n", i, reason[i]);
			ungetc(new.sc_flags, stdin);
		}
		while (getchar() != '\n')
			continue;
	} while (new.sc_flags < '0' || new.sc_flags > '2');
	new.sc_flags -= '0';
	do {
		printf("User Id: ");
		memset(buf, 0, sizeof(buf)); /* paranoia */
		(void) fgets(buf, BUFSIZ, stdin);
		buf[strlen(buf) - 1] = '\0';
        id = atoi(buf);
	} while (id == -1);
	new.sc_uid = id;
	do {
		printf("Monster: ");
		if (!isalpha(new.sc_monster = getchar())) {
			printf("type A-Za-z [%s]\n", unctrl(new.sc_monster));
			ungetc(new.sc_monster, stdin);
		}
		while (getchar() != '\n')
			continue;
	} while (!isalpha(new.sc_monster));
	printf("Score: ");
	scanf("%d", &new.sc_score);
	printf("level: ");
	scanf("%d", &new.sc_level);
	while (getchar() != '\n')
		continue;
	pr_score(&new, FALSE);
	printf("Really add it? ");
	if (getchar() != 'y')
		return;
	while (getchar() != '\n')
		continue;
	insert_score(&new);
}

/*
 * pr_score:
 *	Print out a score entry.  Return FALSE if last entry.
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
		    printf(" by %s", s_killname(scp->sc_monster, TRUE));

        printf(" (%s)", md_getrealname(scp->sc_uid));
		putchar('\n');
	}
	return scp->sc_score;
}

/*
 * insert_score:
 *	Insert a score into the top ten list
 */
void
insert_score(SCORE *new)
{
	SCORE	*scp, *sc2;
	int	flags, amount;
	uid_t uid;

	flags = new->sc_flags;
	uid = new->sc_uid;
	amount = new->sc_score;

	for (scp = top_scores; scp < &top_scores[10]; scp++)
		if (amount > scp->sc_score)
			break;
		else if (flags != 2 && scp->sc_uid == uid && scp->sc_flags != 2)
			scp = &top_scores[10];
	if (scp < &top_scores[10]) {
		if (flags != 2)
			for (sc2 = scp; sc2 < &top_scores[10]; sc2++) {
			    if (sc2->sc_uid == uid && sc2->sc_flags != 2)
				break;
			}
		else
			sc2 = &top_scores[9];
		while (sc2 > scp) {
			*sc2 = sc2[-1];
			sc2--;
		}
		*scp = *new;
		sc2 = scp;
	}
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

	for (;;) {
		printf("Which score? ");
		memset(buf, 0, sizeof(buf)); /* paranoia */
		(void) fgets(buf, BUFSIZ, stdin);
		if (buf[0] == '\n')
			return;
		sscanf(buf, "%d", &num);
		if (num < 1 || num > 10)
			printf("range is 1-10\n");
		else
			break;
	}
	num--;
	for (scp = &top_scores[num]; scp < &top_scores[9]; scp++)
		*scp = scp[1];
	init_score_value(&top_scores[NUMSCORES-1]);
}
