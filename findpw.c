/*
 * print out an encrypted password on the standard output
 *
 * @(#)findpw.c	1.4 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "score.h"

#define MAX_PW_LEN (80)	/* longest password input line we will allow */

extern char *xcrypt(const char *, const char *);

char whoami[MAX_USERNAME+1] = {'\0'};	/* Name of player, +1 for paranoia */

int
main(int argc, char *argv[])
{
    char buf[MAX_PW_LEN+1+1];

    memset(buf, 0, sizeof(buf)); /* paranoia */
    fprintf(stderr, "Password: ");
    (void) fgets(buf, MAX_PW_LEN+1, stdin);
    buf[strlen(buf) - 1] = '\0';
    printf("%s\n", xcrypt(buf, "mT"));
    fflush(stdout);
    return 0;
}
