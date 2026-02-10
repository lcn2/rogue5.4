/*
 * Special wizard commands (some of which are also non-wizard commands
 * under strange circumstances)
 *
 * @(#)wizard.c	4.30 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "rogue.h"

/*
 * whatis:
 *	What a certain object is
 */

void
whatis(int insist, int type)
{
    THING *obj;

    if (pack == NULL)
    {
	msg("you don't have anything in your pack to identify");
	return;
    }

    for (;;)
    {
	obj = get_item("identify", type);
	if (insist)
	{
	    if (n_objs == 0)
		return;
	    else if (obj == NULL)
		msg("you must identify something");
	    else if (type && obj->o_type != type &&
	       !(type == R_OR_S && (obj->o_type == RING || obj->o_type == STICK)) )
		    msg("you must identify a %s", type_name(type));
	    else
		break;
	}
	else
	    break;
    }

    if (obj == NULL)
	return;

    switch (obj->o_type)
    {
        case SCROLL:
	    set_know(obj, scr_info);
        when POTION:
	    set_know(obj, pot_info);
	when STICK:
	    set_know(obj, ws_info);
        when WEAPON:
        case ARMOR:
	    obj->o_flags |= ISKNOW;
        when RING:
	    set_know(obj, ring_info);
    }
    msg("%s", inv_name(obj, FALSE));
}

/*
 * set_know:
 *	Set things up when we really know what a thing is
 */

void
set_know(THING *obj, struct obj_info *info)
{
    char **guess;

    info[obj->o_which].oi_know = TRUE;
    obj->o_flags |= ISKNOW;
    guess = &info[obj->o_which].oi_guess;
    if (*guess)
    {
	free(*guess);
	*guess = NULL;
    }
}

/*
 * type_name:
 *	Return a pointer to the name of the type
 */
const char *
type_name(int type)
{
    struct h_list *hp;
    struct h_list tlist[] = {
	{POTION, "potion",		FALSE},
	{SCROLL, "scroll",		FALSE},
	{FOOD,	 "food",		FALSE},
	{R_OR_S, "ring, wand or staff",	FALSE},
	{RING,	 "ring",		FALSE},
	{STICK,	 "wand or staff",	FALSE},
	{WEAPON, "weapon",		FALSE},
	{ARMOR,	 "suit of armor",	FALSE},
    };

    for (hp = tlist; hp->h_ch; hp++)
	if (type == hp->h_ch)
	    return hp->h_desc;
    /* NOTREACHED */
    return(0);
}

#ifdef MASTER

/*
 * highest_which_letter - determine the letter of the highest which
 *
 * NOTE: A bogus highest will return '0'.
 */
static char
highest_which_letter(int highest)
{
    /* firewall -- negative max maps onto 0 */
    if (highest <= 0) {
	return (char) '0';

    /* first 10 map into 0 thru 9 */
    } else if (highest < 10) {
	return (char) ('0' + highest);

    /* next 26 max into a to z */
    } else if (highest < 10+26) {
	return (char) ('a' + (highest-10));
    }

    /* firewall - max too large maps onto 0 */
    return (char) '0';
}

/*
 * which_char_to_int - convert a which char into an integer
 *
 * NOTE: A bogus char will return 0.
 */
static int
which_char_to_int(int ch)
{
    /* firewall - non-ASCII returns 0 */
    if (!isascii(ch)) {
	return 0;

    /* case: '0' to '9' */
    } else if (isdigit(ch)) {
	return (int) (ch - '0');

    /* case: 'a' to 'z' */
    } else if (islower(ch)) {
	return (int) ((ch - 'a')+10);
    }

    /* firewall - bogus char return 0 */
    return 0;
}

/*
 * create_obj:
 *	wizard command for getting anything he wants
 */

void
create_obj(void)
{
    THING *obj;
    int ch, bless;
    char highest_char;
    int which;

    obj = new_item();
    msg("type of item: ");
    obj->o_type = readchar();
    mpos = 0;

    /*
     * determine a valid o_which given an o_type
     */
    switch (obj->o_type) {

    case POTION:    /* ! */
	highest_char = highest_which_letter(MAXPOTIONS-1);
	do {
	    msg("which %c (potion) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXPOTIONS) {
		endmsg();
		msg("invalid potion, try again");
	    }
	} while (which < 0 || which >= MAXPOTIONS);
	break;

    case SCROLL:    /* ? */
	highest_char = highest_which_letter(MAXSCROLLS-1);
	do {
	    msg("which %c (scroll) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXSCROLLS) {
		endmsg();
		msg("invalid scroll, try again");
	    }
	} while (which < 0 || which >= MAXSCROLLS);
	break;

    case WEAPON:    /* ) */
	highest_char = highest_which_letter(MAXWEAPONS-1);
	do {
	    msg("which %c (weapon) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXWEAPONS) {
		endmsg();
		msg("invalid weapon, try again");
	    }
	} while (which < 0 || which >= MAXWEAPONS);
	break;

    case ARMOR:	    /* ] */
	highest_char = highest_which_letter(MAXARMORS-1);
	do {
	    msg("which %c (armor) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXARMORS) {
		endmsg();
		msg("invalid armor, try again");
	    }
	} while (which < 0 || which >= MAXARMORS);
	break;

    case RING:	    /* = */
	highest_char = highest_which_letter(MAXRINGS-1);
	do {
	    msg("which %c (ring) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXRINGS) {
		endmsg();
		msg("invalid ring, try again");
	    }
	} while (which < 0 || which >= MAXRINGS);
	break;

    case STICK:	    /* / */
	highest_char = highest_which_letter(MAXSTICKS-1);
	do {
	    msg("which %c (staff) do you want? (0-%c)", obj->o_type, highest_char);
	    ch = readchar();
	    which = which_char_to_int(ch);
	    if (which < 0 || which >= MAXSTICKS) {
		endmsg();
		msg("invalid staff, try again");
	    }
	} while (which < 0 || which >= MAXSTICKS);
	break;

    default:	    /* anything that doesn't have an o_which */
	highest_char = '0';
	ch = '0';
	which = 0;
	break;
    }

    /*
     * form an object
     */
    obj->o_which = which;
    obj->o_group = 0;
    obj->o_count = 1;
    mpos = 0;
    if (obj->o_type == WEAPON || obj->o_type == ARMOR)
    {
	msg("blessing? (+,-,n)");
	bless = readchar();
	mpos = 0;
	if (bless == '-')
	    obj->o_flags |= ISCURSED;
	if (obj->o_type == WEAPON)
	{
	    init_weapon(obj, obj->o_which);
	    if (bless == '-')
		obj->o_hplus -= rnd(3)+1;
	    if (bless == '+')
		obj->o_hplus += rnd(3)+1;
	}
	else
	{
	    obj->o_arm = a_class[obj->o_which];
	    if (bless == '-')
		obj->o_arm += rnd(3)+1;
	    if (bless == '+')
		obj->o_arm -= rnd(3)+1;
	}
    }
    else if (obj->o_type == RING)
	switch (obj->o_which)
	{
	    case R_PROTECT:
	    case R_ADDSTR:
	    case R_ADDHIT:
	    case R_ADDDAM:
		msg("blessing? (+,-,n)");
		bless = readchar();
		mpos = 0;
		if (bless == '-')
		    obj->o_flags |= ISCURSED;
		obj->o_arm = (bless == '-' ? -1 : rnd(2) + 1);
	    when R_AGGR:
	    case R_TELEPORT:
		obj->o_flags |= ISCURSED;
	}
    else if (obj->o_type == STICK)
	fix_stick(obj);
    else if (obj->o_type == GOLD)
    {
	msg("how much?");
	get_num(&obj->o_goldval, stdscr);
    }
    add_pack(obj, FALSE);
}
#endif

/*
 * telport:
 *	Bamf the hero someplace else
 */

void
teleport(void)
{
    coord c;

    mvaddch(hero.y, hero.x, floor_at());
    find_floor(NULL, &c, FALSE, TRUE);
    if (roomin(&c) != proom)
    {
	leave_room(&hero);
	hero = c;
	enter_room(&hero);
    }
    else
    {
	hero = c;
	look(TRUE);
    }
    mvaddch(hero.y, hero.x, PLAYER);
    /*
     * turn off ISHELD in case teleportation was done while fighting
     * a Flytrap
     */
    if (on(player, ISHELD)) {
	player.t_flags &= ~ISHELD;
	vf_hit = 0;
	strcpy(monsters['F'-'A'].m_stats.s_dmg, "000x0");
    }
    no_move = 0;
    count = 0;
    running = FALSE;
    flush_type();
}

#ifdef MASTER
/*
 * passwd:
 *	See if user knows password
 */
int
passwd(void)
{
    char *sp;
    int c;
    static char buf[MAXSTR];

    msg("wizard's Password:");
    mpos = 0;
    sp = buf;
    while ((c = readchar()) != '\n' && c != '\r' && c != ESCAPE)
	if (c == md_killchar())
	    sp = buf;
	else if (c == md_erasechar() && sp > buf)
	    sp--;
	else
	    *sp++ = (char) c;
    if (sp == buf)
	return FALSE;
    *sp = '\0';
    return (strcmp(PASSWD, md_crypt(buf, "mT")) == 0);
}

/*
 * show_map:
 *	Print out the map for the wizard
 */

void
show_map(void)
{
    int y, x, real;

    wclear(hw);
    for (y = 1; y < NUMLINES - 1; y++)
	for (x = 0; x < NUMCOLS; x++)
	{
	    real = flat(y, x) & F_REAL;
	    if (!real)
		wstandout(hw);
	    wmove(hw, y, x);
	    waddch(hw, chat(y, x));
	    if (!(real & F_REAL))
		wstandend(hw);
	}
    show_win("---More (level map)---");
}
#endif
