/*
 * print out an encrypted password on the standard output
 *
 * @(#)findpw.c	1.4 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */
#include <stdio.h>
#include <string.h>

#define MAX_PW_LEN (80)	/* longest password input line we will allow */

extern char *xcrypt(const char *, const char *);

int
main(int argc, char *argv[])
{
    char buf[MAX_PW_LEN+1+1];

    memset(buf, 0, sizeof(buf)); /* paranoia */
    fprintf(stderr, "Password: ");
    (void) fgets(buf, MAX_PW_LEN+1, stdin);
    buf[strlen(buf) - 1] = '\0';
    printf("%s\n", xcrypt(buf, "mT"));
    return 0;
}
