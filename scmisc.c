/*
 * copies of several routines needed for score
 *
 * @(#)smisc.c	4.7 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

# include	<stdio.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<ctype.h>
# include	<string.h>
# include	<unistd.h>
# include	<fcntl.h>
# include	<time.h>
# include	<stdbool.h>

# undef		TRUE
# define	TRUE true
# undef		FALSE
# define	FALSE false

# include	"score.h"

# define	MAXLINE		80
# define	when		break;case
# define	otherwise	break;default

typedef struct {
	char	*m_name;
} MONST;

char	*s_vowelstr(char *str);

char *lockfile = "/tmp/.fredlock";

static char	prbuf[MAXLINE+1];		/* buffer for sprintfs */

static MONST	monsters[] = {
	{ "aquator" }, { "bat" }, { "centaur" }, { "dragon" }, { "emu" },
	{ "venus flytrap" }, { "griffin" }, { "hobgoblin" }, { "ice monster" },
	{ "jabberwock" }, { "kobold" }, { "leprechaun" }, { "medusa" },
	{ "nymph" }, { "orc" }, { "phantom" }, { "quasit" }, { "rattlesnake" },
	{ "snake" }, { "troll" }, { "ur-vile" }, { "vampire" }, { "wraith" },
	{ "xeroc" }, { "yeti" }, { "zombie" }
};


/* duplicated external declarations - because including rogue.h brings in too much other stuff */

extern const char encstr[];
extern const char statlist[];

/* external functions from scedit.c */

extern void	md_sleep(int s);
extern int	md_unlink(char *file);
extern void	add_score(void);
extern void	del_score(void);
extern void	insert_score(SCORE *new);


/*
 * s_lock_sc:
 *	lock the score file.  If it takes too long, ask the user if
 *	they care to wait.  Return TRUE if the lock is successful.
 */
int
s_lock_sc(void)
{
    int cnt;
    struct stat sbuf;

over:
    close(8);	/* just in case there are no files left */
    if (creat(lockfile, 0000) >= 0)
	return TRUE;
    for (cnt = 0; cnt < 5; cnt++)
    {
	md_sleep(1);
	if (creat(lockfile, 0000) >= 0)
	    return TRUE;
    }
    if (stat(lockfile, &sbuf) < 0)
    {
	creat(lockfile, 0000);
	return TRUE;
    }
    if (time(NULL) - sbuf.st_mtime > 10)
    {
	if (md_unlink(lockfile) < 0)
	    return FALSE;
	goto over;
    }
    else
    {
	printf("The score file is very busy.  Do you want to wait longer\n");
	printf("for it to become free so your score can get posted?\n");
	printf("If so, type \"y\"\n");
	memset(prbuf, 0, sizeof(prbuf)); /* paranoia */
	(void) fgets(prbuf, MAXLINE, stdin);
	if (prbuf[0] == 'y')
	    for (;;)
	    {
		if (creat(lockfile, 0000) >= 0)
		    return TRUE;
		if (stat(lockfile, &sbuf) < 0)
		{
		    creat(lockfile, 0000);
		    return TRUE;
		}
		if (time(NULL) - sbuf.st_mtime > 10)
		{
		    if (md_unlink(lockfile) < 0)
			return FALSE;
		}
		md_sleep(1);
	    }
	else
	    return FALSE;
    }
}

/*
 * s_unlock_sc:
 *	Unlock the score file
 */
void
s_unlock_sc(void)
{
    md_unlink(lockfile);
}

/*
 * s_encwrite:
 *	Perform an encrypted write
 */
void
s_encwrite(char *start, size_t size, FILE *outf)
{
    const char *e1, *e2;
    char fb;
    int temp;

    e1 = encstr;
    e2 = statlist;
    fb = 0;

    while (size--)
    {
	putc(*start++ ^ *e1 ^ *e2 ^ fb, outf);
	temp = *e1++;
	fb += temp * *e2++;
	if (*e1 == '\0')
	    e1 = encstr;
	if (*e2 == '\0')
	    e2 = statlist;
    }
}

/*
 * s_encread:
 *	Perform an encrypted read
 */

void
s_encread(char *start, size_t size, int inf)
{
    const char *e1, *e2;
    char fb;
    int temp;
    int read_size;

    fb = 0;

    if ((read_size = read(inf, start, size)) == 0 || read_size == -1)
	return;

    e1 = encstr;
    e2 = statlist;

    while (size--)
    {
	*start++ ^= *e1 ^ *e2 ^ fb;
	temp = *e1++;
	fb += temp * *e2++;
	if (*e1 == '\0')
	    e1 = encstr;
	if (*e2 == '\0')
	    e2 = statlist;
    }
}

/*
 * s_killname:
 *	Convert a code to a monster name
 */
char *
s_killname(int monst, int doart)
{
    char *sp;
    int article;

    article = TRUE;
    switch (monst)
    {
	case 'a':
	    sp = "arrow";
	when 'b':
	    sp = "bolt";
	when 'd':
	    sp = "dart";
	when 's':
	    sp = "starvation";
	    article = FALSE;
	when 'h':
	    sp = "hypothermia";
	    article = FALSE;
	otherwise:
	    if (isupper(monst))
		sp = monsters[monst-'A'].m_name;
	    else
	    {
		sp = "God";
		article = FALSE;
	    }
    }
    memset(prbuf, 0, sizeof(prbuf));
    if (doart && article)
	snprintf(prbuf, sizeof(prbuf), "a%s ", s_vowelstr(sp));
    else
	prbuf[0] = '\0';
    strcat(prbuf, sp);
    return prbuf;
}

/*
 * s_vowelstr:
 *      For printfs: if string starts with a vowel, return "n" for an
 *	"an".
 */
char *
s_vowelstr(char *str)
{
    switch (*str)
    {
	case 'a': case 'A':
	case 'e': case 'E':
	case 'i': case 'I':
	case 'o': case 'O':
	case 'u': case 'U':
	    return "n";
	default:
	    return "";
    }
}
