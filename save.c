/*
 * save and restore routines
 *
 * @(#)save.c	4.33 (Berkeley) 06/01/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>

#include "modern_curses.h"
#include "have_strlcat.h"
#include "have_strlcpy.h"
#include "strl.h"
#include "extern.h"
#include "config.h"
#include "score.h"
#include "rogue.h"

/*
 * save_game:
 *	Implement the "save game" command
 */

void
save_game(void)
{
    FILE *savef;
    int c;
    char buf[MAXSTR];
    struct stat sbuf;
    /*
     * get file name
     */
    mpos = 0;
over:
    if (file_name[0] != '\0')
    {
	for (;;)
	{
	    msg("save file (%s)? ", file_name);
	    c = readchar();
	    mpos = 0;
	    if (c == ESCAPE)
	    {
		msg("");
		return;
	    }
	    else if (c == 'n' || c == 'N' || c == 'y' || c == 'Y')
		break;
	    else
		msg("please answer Y or N");
	}
	if (c == 'y' || c == 'Y')
	{
	    addstr("Yes\n");
	    refresh();
	    strcpy(buf, file_name);
	    goto gotfile;
	}
    }

    do
    {
	mpos = 0;
	msg("file name: ");
	buf[0] = '\0';
	if (get_str(buf, stdscr) == QUIT)
	{
quit_it:
	    msg("");
	    return;
	}
	mpos = 0;
gotfile:
	/*
	 * test to see if the file exists
	 */
	if (stat(buf, &sbuf) >= 0)
	{
	    for (;;)
	    {
		msg("File exists.  Do you wish to overwrite it?");
		mpos = 0;
		if ((c = readchar()) == ESCAPE)
		    goto quit_it;
		if (c == 'y' || c == 'Y')
		    break;
		else if (c == 'n' || c == 'N')
		    goto over;
		else
		    msg("Please answer Y or N");
	    }
	    msg("file name: %s", buf);
	    md_unlink(file_name);
	}
	strcpy(file_name, buf);
	if ((savef = fopen(file_name, "w")) == NULL)
	    msg(strerror(errno));
    } while (savef == NULL);
    msg("");
    save_file(savef);
    /* NOTREACHED */
}

/*
 * auto_save:
 *	Automatically save a file.  This is most commonly used with various signals
 *	are received.
 *
 *	The md_onsignal_autosave() in mdport.c, which is called via
 *	setup() from mach_dep.c, and md_init() from mdport.c.
 *
 *	The setup() function is called from main() (short after the
 *	"just a moment while I dig the dungeon" message a printed,
 *	and after ncurses setup and data structures are setup) in main.c,
 *	and from restore() (shortly after data structures are restored
 *	and ncurses setup) in save.c.
 */

void
auto_save(int sig)
{
    FILE *savef;
    NOOP(sig);

    md_ignoreallsignals();
    if (file_name[0] != '\0' && ((savef = fopen(file_name, "w")) != NULL ||
	(md_unlink_open_file(file_name, savef) >= 0 && (savef = fopen(file_name, "w")) != NULL)))
	    save_file(savef);
    exit(0);
}

/*
 * save_file:
 *	Write the saved game on the file
 */

void
save_file(FILE *savef)
{
    char buf[80+1]; /* +1 for paranoia */

    md_chmod(file_name, 0400);
    encwrite(version, strlen(version)+1, savef);
    snprintf(buf, 80, "%d x %d\n", LINES, COLS);
    buf[80] = '\0'; /* paranoia */
    encwrite(buf,80,savef);
    rs_save_file(savef);
    fflush(savef);
    fclose(savef);
    exit(0);
}

/*
 * restore:
 *	Restore a saved game from a file with elaborate checks for file
 *	integrity from cheaters
 */
int
restore(const char *file)
{
    FILE *inf;
    char buf[MAXSTR];
    struct stat sbuf2;
    int lines = 0;
    int cols = 0;
    int ret = 0;

    /*
     * paranoia
     */
    if (strlen(file) >= MAXSTR) {
	printf("Sorry, the rogue save file path is too long: %s\n", file);
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	return false;
    }

    /*
     * -r arg bead restore the default rogue save file
     */
    if (strcmp(file, "-r") == 0) {
	file = file_name;
    }

    /*
     * hold off on suspending for now
     */
    md_tstphold();

    /*
     * verify that the rogue save file is not a symlink
     */
    memset(&sbuf2, 0, sizeof(sbuf2)); /* paranoia */
    {
	int fd;

	fd = open(file, O_RDONLY | O_NOFOLLOW);
	if (fd < 0) {
	    printf("Sorry, failed to open for reading rogue save file: %s - %s\n", file, strerror(errno));
	    printf("Unable to restore: %s\n", file);
	    fflush(stdout);
	    md_tstpresume();
	    return false;
	}

	if (fstat(fd, &sbuf2) < 0) {
	    printf("Sorry, cannot stat rogue save file: %s - %s\n", file, strerror(errno));
	    printf("Unable to restore: %s\n", file);
	    fflush(stdout);
	    close(fd);
	    md_tstpresume();
	    return false;
	}
	if (!S_ISREG(sbuf2.st_mode)) {
	    printf("Sorry, rogue save file must be a regular file: %s\n", file);
	    printf("Unable to restore: %s\n", file);
	    fflush(stdout);
	    close(fd);
	    md_tstpresume();
	    return false;
	}

	inf = fdopen(fd, "r");
	if (inf == NULL)
	{
	    printf("Sorry, failed to open for reading rogue save file: %s - %s\n", file, strerror(errno));
	    printf("Unable to restore: %s\n", file);
	    fflush(stdout);
	    close(fd);
	    md_tstpresume();
	    return false;
	}
    }

    /*
     * verify that the file is not a symlink
     */
    memset(&sbuf2, 0, sizeof(sbuf2)); /* paranoia */
    if (fstat(fileno(inf), &sbuf2) < 0) {
	printf("Sorry, cannot stat open rogue save file: %s - %s\n", file, strerror(errno));
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }
    if ((sbuf2.st_mode & S_IFMT) != S_IFREG) {
	printf("Sorry, open rogue save file is not a regular file: %s\n", file);
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * read version from the rogue save file
     */
    memset(buf, 0, sizeof(buf)); /* paranoia */
    encread(buf, strlen(version) + 1, inf);
    if (strcmp(buf, version) != 0)
    {
	printf("Sorry, saved game is out of date.\n");
	printf("Expected version: %s found version: %s\n", version, buf);
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * read screen size from the rogue save file
     */
    memset(buf, 0, sizeof(buf)); /* paranoia */
    encread(buf, NUMCOLS, inf);
    ret = sscanf(buf, "%d x %d\n", &lines, &cols);
    if (ret != 2) {
	printf("Sorry, failed to parse the lines and columns from: %s\n", file);
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * Start up cursor package
     */
    initscr();
    keypad(stdscr, TRUE);
    hw = newwin(LINES, COLS, 0, 0);
    setup(); /* also setup signal handlers */

    /*
     * object if screen is too small
     */
    if (lines < NUMLINES || cols < NUMCOLS) {
	endwin_and_ncurses_cleanup();
	printf("\nSorry, current screen only has %d lines and %d columns.\n", LINES, COLS);
	printf("The screen have at least %d lines and %d columns.\n", NUMLINES, NUMCOLS);
	fflush(stdout);
	return false;
    }

    /*
     * stat the open rogue file again in case someone linked to it while we were restoring the game state
     *
     * NOTE: we do not close the file so that we will have a hold of the inode for as long as possible
     */
    memset(&sbuf2, 0, sizeof(sbuf2)); /* paranoia */
    errno = 0; /* paranoia */
    if (fstat(fileno(inf), &sbuf2) < 0) {
	endwin_and_ncurses_cleanup();
	printf("Sorry, cannot re-stat open rogue save file: %s\n", strerror(errno));
	printf("The screen have at least %d lines and %d columns.\n", NUMLINES, NUMCOLS);
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * unlink the rogue save file now that we have restored our game state
     */
    errno = 0; /* paranoia */
    if (
#ifdef MASTER
	!wizard &&
#endif
        md_unlink_open_file(file, inf) < 0)
    {
	endwin_and_ncurses_cleanup();
	printf("Sorry, cannot remove rogue save file after restoring: %s\n", strerror(errno));
        printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * defeat multiple restarting from the same place
     */
#ifdef MASTER
    if (!wizard)
#endif
	if (sbuf2.st_nlink != 1)
	{
	    endwin_and_ncurses_cleanup();
	    printf("Sorry, cannot restore from a rogue save file that linked\n");
	    printf("Link count: %ld != 1\n", (long int)sbuf2.st_nlink);
	    printf("Unable to restore: %s\n", file);
	    fflush(stdout);
	    fclose(inf);
	    md_tstpresume();
	    return false;
	}

    /*
     * complete the game state restoration process
     */
    rs_restore_file(inf);

    /*
     * catch the attempt to save a dead player
     */
    if (pstats.s_hpt <= 0)
    {
	endwin_and_ncurses_cleanup();
	printf("\"He's dead, Jim\"\n");
	printf("Attempt to restore a game of a dead rogue player, HP: %d\n", pstats.s_hpt);
	printf("Unable to restore: %s\n", file);
	fflush(stdout);
	fclose(inf);
	md_tstpresume();
	return false;
    }

    /*
     * next call to wrefresh with this window will clear the screen
     */
    mpos = 0;
    clearok(stdscr, true);

    /*
     * change rogue save filename if needed
     */
    if (strcmp(file_name, file) != 0) {
	memset(file_name, 0, sizeof(file_name)); /* paranoia */
	strlcpy(file_name, file, MAXSTR);
    }

    /*
     * setup game conditions
     */
    clearok(curscr, true);
    printw("file name: %s", file);
    refresh();
    fclose(inf);
    md_tstpresume();
    playit();
    /*NOTREACHED*/
    exit(0);
}

static int encerrno = 0;

int
encerror(void)
{
    return encerrno;
}

void
encseterr(int err)
{
    encerrno = err;
}

int
encclearerr(void)
{
    int n = encerrno;

    encerrno = 0;

    return(n);
}

/*
 * encwrite:
 *	Perform an encrypted write
 */

size_t
encwrite(const char *start, size_t size, FILE *outf)
{
    const char *e1, *e2;
    char fb;
    int temp;
    size_t o_size = size;
    e1 = encstr;
    e2 = statlist;
    fb = 0;

    if (encerrno) {
	errno = encerrno;
	return 0;
    }

    while(size)
    {
	if (putc(*start++ ^ *e1 ^ *e2 ^ fb, outf) == EOF)
	{
	    encerrno = errno;
            break;
	}

	temp = *e1++;
	fb = fb + ((char) (temp * *e2++));
	if (*e1 == '\0')
	    e1 = encstr;
	if (*e2 == '\0')
	    e2 = statlist;
	size--;
    }

    return(o_size - size);
}

/*
 * encread:
 *	Perform an encrypted read
 */
size_t
encread(char *start, size_t size, FILE *inf)
{
    const char *e1, *e2;
    char fb;
    int temp;
    size_t read_size;
    size_t items;
    fb = 0;

    if (encerrno) {
	errno = encerrno;
	return 0;
    }

    items = read_size = fread(start,1,size,inf);

    e1 = encstr;
    e2 = statlist;

    while (read_size--)
    {
	*start++ ^= *e1 ^ *e2 ^ fb;
	temp = *e1++;
	fb = fb + (char)(temp * *e2++);
	if (*e1 == '\0')
	    e1 = encstr;
	if (*e2 == '\0')
	    e2 = statlist;
    }

    if (items != size)
	encerrno = errno;

    return(items);
}

/*
 * rd_scrore:
 *	Read in the score file
 */
void
rd_score(SCORE *top_score)
{
    char scoreline[MAXSCORELINE+1]; /* +1 for paranoia */
    SCORE score;		    /* scanned score */
    bool failed = false;	    /* true ==> score file scan failed */
    int ret;			    /* scanf return value */
    int i;

    if (scoreboard == NULL)
	return;

    rewind(scoreboard);

    for(i = 0; i < numscores; i++)
    {
	memset(scoreline, 0, sizeof(scoreline)); /* paranoia */
	memset(&score, 0, sizeof(score)); /* paranoia */
        encread(scoreline, MAXSCORELINE, scoreboard);
	/*
	 * NOTE: A number of C compilers do not correctly process a sscanf(3) line such as:
	 *
	 *       sscanf(string, "%*s", MAX_USERNAME, str);
	 *
	 * So we must HARD code the maximum width in the sscanf(3) call below.
	 * The value MAX_USERNAME in score.h MUST match the %32s format string width.
	 */
        ret = sscanf(scoreline, "%32s %u %d %u %d %d %jx \n",
            score.sc_name,
	    &score.sc_uid, &score.sc_score,
            &score.sc_flags, &score.sc_monster,
            &score.sc_level, &score.sc_time);
	if (ret == 7) {
	    top_score[i] = score;
	} else {
	    failed = true;
	}
    }
    if (failed) {
	printf("ERROR: The score file format is too old and/or has been corrupted!\n");
	printf("WARNING: Before running rouge again, remove the score file: %s\n", score_path);
	fflush(stdout);
	exit(1);
    }

    rewind(scoreboard);
}

/*
 * wr_scrore:
 *	Write in the score file
 */
void
wr_score(SCORE *top_score)
{
    char scoreline[MAXSCORELINE+1]; /* +1 for paranoia */
    int i;

    if (scoreboard == NULL)
	return;
    if (top_score == NULL)
	return;

    rewind(scoreboard);

    for(i = 0; i < numscores; i++)
    {
	  memset(scoreline, 0, sizeof(scoreline)); /* paranoia */
          snprintf(scoreline, MAXSCORELINE, "%*s %u %d %u %d %d %jx \n",
	      MAX_USERNAME, top_score[i].sc_name,
              top_score[i].sc_uid, top_score[i].sc_score,
              top_score[i].sc_flags, top_score[i].sc_monster,
              top_score[i].sc_level, top_score[i].sc_time);
          encwrite(scoreline, MAXSCORELINE, scoreboard);
    }
    fflush(scoreboard);

    rewind(scoreboard);
}
