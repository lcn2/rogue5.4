/*
 * Read and execute the user commands
 *
 * @(#)command.c	4.73 (Berkeley) 08/06/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "modern_curses.h"
#include "have_strlcat.h"
#include "have_strlcpy.h"
#include "strl.h"
#include "extern.h"
#include "config.h"
#include "rogue.h"

/*
 * command:
 *	Process the user commands
 */
void
command(void)
{
    int ch;
    int ntimes = 1;			/* Number of player moves */
    int *fp;
    THING *mp;
    static int countch, direction, newcount = false;

    if (on(player, ISHASTE))
	ntimes++;
    /*
     * Let the daemons start up
     */
    do_daemons(BEFORE);
    do_fuses(BEFORE);
    while (ntimes--)
    {
	again = false;
	if (has_hit)
	{
	    endmsg();
	    has_hit = false;
	}
	/*
	 * these are illegal things for the player to be, so if any are
	 * set, someone's been poking in memeory
	 */
	if (on(player, ISSLOW|ISGREED|ISINVIS|ISREGEN|ISTARGET))
	    exit(1);

	look(true);
	if (!running)
	    door_stop = false;
	status();
	lastscore = purse;
	move(hero.y, hero.x);
	if (!((running || count) && jump))
	    refresh();			/* Draw screen */
	take = 0;
	after = true;
	/*
	 * Read command or continue run
	 */
#ifdef MASTER
	if (wizard)
	    noscore = true;
#endif
	if (!no_command)
	{
	    if (running || to_death)
		ch = runch;
	    else if (count)
		ch = countch;
	    else
	    {
		ch = readchar();
		move_on = false;
		if (mpos != 0)		/* Erase message if its there */
		{
		    if (ch != '.')
			msg("");
		}
	    }
	}
	else
	    ch = '.';
	if (no_command)
	{
	    if (--no_command == 0)
	    {
		player.t_flags |= ISRUN;
		msg("you can move again");
	    }
	}
	else
	{
	    /*
	     * check for prefixes
	     */
	    newcount = false;
	    if (isdigit(ch))
	    {
		count = 0;
		newcount = true;
		while (isdigit(ch))
		{
		    count = count * 10 + (ch - '0');
		    if (count > 255)
			count = 255;
		    ch = readchar();
		}
		countch = ch;
		/*
		 * turn off count for commands which don't make sense
		 * to repeat
		 */
		switch (ch)
		{
		    case CTRL('B'): case CTRL('H'): case CTRL('J'):
		    case CTRL('K'): case CTRL('L'): case CTRL('N'):
		    case CTRL('U'): case CTRL('Y'): case CTRL('Z'):
		    case '.': case 'a': case 'b': case 'h': case 'j':
		    case 'k': case 'l': case 'm': case 'n': case 'q':
		    case 'r': case 's': case 't': case 'u': case 'y':
		    case 'z': case 'B': case 'C': case 'H': case 'I':
		    case 'J': case 'K': case 'L': case 'N': case 'U':
		    case 'Y':
#ifdef MASTER
		    case CTRL('D'): case CTRL('A'):
#endif
			break;
		    default:
			count = 0;
		}
	    }
	    /*
	     * execute a command
	     */
	    if (count && !running)
		count--;
	    if (ch != 'a' && ch != ESCAPE && !(running || count || to_death))
	    {
		l_last_comm = last_comm;
		l_last_dir = last_dir;
		l_last_pick = last_pick;
		last_comm = ch;
		last_dir = '\0';
		last_pick = NULL;
	    }
over:
	    switch (ch)
	    {
		case ',': {
		    THING *obj = NULL;
		    int found = 0;
		    for (obj = lvl_obj; obj != NULL; obj = next(obj))
			{
			    if (obj->o_pos.y == hero.y && obj->o_pos.x == hero.x)
			    {
				found=1;
				break;
			    }
			}

		    if (found) {
			if (levit_check())
			    ;
			else
			    pick_up(obj->o_type);
		    }
		    else {
			if (!terse)
			    addmsg("there is ");
			addmsg("nothing here");
                        if (!terse)
                            addmsg(" to pick up");
                        endmsg();
		    }
		}
		when '!': shell();
		when 'h': do_move(0, -1);
		when 'j': do_move(1, 0);
		when 'k': do_move(-1, 0);
		when 'l': do_move(0, 1);
		when 'y': do_move(-1, -1);
		when 'u': do_move(-1, 1);
		when 'b': do_move(1, -1);
		when 'n': do_move(1, 1);
		when 'H': do_run('h');
		when 'J': do_run('j');
		when 'K': do_run('k');
		when 'L': do_run('l');
		when 'Y': do_run('y');
		when 'U': do_run('u');
		when 'B': do_run('b');
		when 'N': do_run('n');
		when CTRL('H'): case CTRL('J'): case CTRL('K'): case CTRL('L'):
		case CTRL('Y'): case CTRL('U'): case CTRL('B'): case CTRL('N'):
		{
		    if (!on(player, ISBLIND))
		    {
			door_stop = true;
			firstmove = true;
		    }
		    if (count && !newcount)
			ch = direction;
		    else
		    {
			ch += ('A' - CTRL('A'));
			direction = ch;
		    }
		    goto over;
		}
		when 'F':
		    kamikaze = true;
		    /* FALLTHROUGH */
		case 'f':
		    if (!get_dir())
		    {
			after = false;
			break;
		    }
		    delta.y += hero.y;
		    delta.x += hero.x;
		    if ( ((mp = moat(delta.y, delta.x)) == NULL)
			|| ((!see_monst(mp)) && !on(player, SEEMONST)))
		    {
			if (!terse)
			    addmsg("I see ");
			msg("no monster there");
			after = false;
		    }
		    else if (diag_ok(&hero, &delta))
		    {
			to_death = true;
			max_hit = 0;
			mp->t_flags |= ISTARGET;
			runch = ch = dir_ch;
			goto over;
		    }
		when 't':
		    if (!get_dir())
			after = false;
		    else
			missile(delta.y, delta.x);
		when 'a':
		    if (last_comm == '\0')
		    {
			msg("you haven't typed a command yet");
			after = false;
		    }
		    else
		    {
			ch = last_comm;
			again = true;
			goto over;
		    }
		when 'q': quaff();
		when 'Q':
		    after = false;
		    q_comm = true;
		    quit(0);
		    q_comm = false;
		when 'i': after = false; inventory(pack, 0);
		when 'I': after = false; picky_inven();
		when 'd': drop();
		when 'r': read_scroll();
		when 'e': eat();
		when 'w': wield();
		when 'W': wear();
		when 'T': take_off();
		when 'P': ring_on();
		when 'R': ring_off();
		when 'o': option(); after = false;
		when 'c': call(); after = false;
		when '>': after = false; d_level();
		when '<': after = false; u_level();
		when '?': after = false; help();
		when '/': after = false; identify();
		when 's': search();
		when 'z':
		    if (get_dir())
			do_zap();
		    else
			after = false;
		when 'D': after = false; discovered();
		when CTRL('P'): after = false; msg("%s", huh);
		when CTRL('R'):
		    after = false;
		    clearok(curscr,true);
		    wrefresh(curscr);
		when 'v':
		    after = false;
		    msg("rogue version %s release %s dungeon %u (chongo was here)", version, release, dnum);
		when 'S':
		    after = false;
		    save_game();
		when '.': ;			/* Rest command */
		when ' ': after = false;	/* "Legal" illegal command */
		when '^':
		    after = false;
		    if (get_dir()) {
			delta.y += hero.y;
			delta.x += hero.x;
			fp = &flat(delta.y, delta.x);
                        if (!terse)
                            addmsg("You have found ");
			if (chat(delta.y, delta.x) != TRAP)
			    msg("no trap there");
			else if (on(player, ISHALU))
			    msg(tr_name[rnd(NTRAPS)]);
			else {
			    msg(tr_name[*fp & F_TMASK]);
			    *fp |= F_SEEN;
			}
		    }
#ifdef MASTER
		when '+':
		    after = false;
		    if (wizard)
		    {
			wizard = false;
			turn_see(true);
			msg("not wizard any more");
		    }
		    else
		    {
			wizard = passwd();
			if (wizard)
			{
			    noscore = true;
			    turn_see(false);
			    msg("you are suddenly as smart as Ken Arnold in dungeon #%d", dnum);
			}
			else
			    msg("sorry");
		    }
#endif
		when ESCAPE:	/* Escape */
		    door_stop = false;
		    count = 0;
		    after = false;
		    again = false;
		when 'm':
		    move_on = true;
		    if (!get_dir())
			after = false;
		    else
		    {
			ch = dir_ch;
			countch = dir_ch;
			goto over;
		    }
		when ')': current(cur_weapon, "wielding", NULL);
		when ']': current(cur_armor, "wearing", NULL);
		when '=':
		    current(cur_ring[LEFT], "wearing",
					    terse ? "(L)" : "on left hand");
		    current(cur_ring[RIGHT], "wearing",
					    terse ? "(R)" : "on right hand");
		when '@':
		    stat_msg = true;
		    status();
		    stat_msg = false;
		    after = false;
		when CTRL('Z'):
		    tstp(0);
		when ';':
		    /* The ; command is ignored, no time passes - often used by rogomatic */
		    after = false;
		otherwise:
		    after = false;
#ifdef MASTER
		    if (wizard) switch (ch)
		    {
			case '|': msg("@ %d,%d", hero.y, hero.x);
			when 'C': create_obj();
			when '$': msg("inpack = %d", inpack);
			when CTRL('G'): inventory(lvl_obj, 0);
			when CTRL('W'): whatis(false, 0);
			when CTRL('D'): level++; new_level();
			when CTRL('A'): level--; new_level();
			when CTRL('F'): show_map();
			when CTRL('T'): teleport();
			when CTRL('E'): msg("food left: %d", food_left);
			when CTRL('Q'): add_pass();
			when CTRL('X'): turn_see(on(player, SEEMONST));
			when '~':
			{
			    THING *item;

			    if ((item = get_item("charge", STICK)) != NULL)
				item->o_charges = 10000;
			}
			when CTRL('I'):
			{
			    int i;
			    THING *obj;

			    for (i = 0; i < 9; i++)
				raise_level();
			    /*
			     * Give him a sword (+1,+1)
			     */
			    obj = new_thing_ptr();
			    init_weapon(obj, TWOSWORD);
			    obj->o_hplus = 1;
			    obj->o_dplus = 1;
			    add_pack(obj, true);
			    cur_weapon = obj;
			    /*
			     * And his suit of armor
			     */
			    obj = new_thing_ptr();
			    obj->o_type = ARMOR;
			    obj->o_which = PLATE_MAIL;
			    obj->o_arm = -5;
			    obj->o_flags |= ISKNOW;
			    obj->o_count = 1;
			    obj->o_group = 0;
			    cur_armor = obj;
			    add_pack(obj, true);
			}
			when '*' :
			    pr_list();
			otherwise:
			    illcom(ch);
		    }
		    else
#endif
			illcom(ch);
	    }
	    /*
	     * turn off flags if no longer needed
	     */
	    if (!running)
		door_stop = false;
	}
	/*
	 * If he ran into something to take, let him pick it up.
	 */
	if (take != 0)
	    pick_up(take);
	if (!running)
	    door_stop = false;
	if (!after)
	    ntimes++;
    }
    do_daemons(AFTER);
    do_fuses(AFTER);
    if (ISRING(LEFT, R_SEARCH))
	search();
    else if (ISRING(LEFT, R_TELEPORT) && rnd(50) == 0)
	teleport();
    if (ISRING(RIGHT, R_SEARCH))
	search();
    else if (ISRING(RIGHT, R_TELEPORT) && rnd(50) == 0)
	teleport();
}

/*
 * illcom:
 *	What to do with an illegal command
 */
void
illcom(int ch)
{
    save_msg = false;
    count = 0;
    msg("illegal command '%s'", unctrl(ch));
    save_msg = true;
}

/*
 * search:
 *	player gropes about him to find hidden things.
 */
void
search(void)
{
    int y, x;
    int *fp;
    int ey, ex;
    int probinc;
    int found;

    ey = hero.y + 1;
    ex = hero.x + 1;
    probinc = (on(player, ISHALU) ? 3 : 0);
    probinc += (on(player, ISBLIND) ? 2 : 0);
    found = false;
    for (y = hero.y - 1; y <= ey; y++)
	for (x = hero.x - 1; x <= ex; x++)
	{
	    if (y == hero.y && x == hero.x)
		continue;
	    fp = &flat(y, x);
	    if (!(*fp & F_REAL))
		switch (chat(y, x))
		{
		    case '|':
		    case '-':
			if (rnd(5 + probinc) != 0)
			    break;
			chat(y, x) = DOOR;
                        msg("a secret door");
foundone:
			found = true;
			*fp |= F_REAL;
			count = false;
			running = false;
			break;
		    case FLOOR:
			if (rnd(2 + probinc) != 0)
			    break;
			chat(y, x) = TRAP;
			if (!terse)
			    addmsg("you found ");
			if (on(player, ISHALU))
			    msg(tr_name[rnd(NTRAPS)]);
			else {
			    msg(tr_name[*fp & F_TMASK]);
			    *fp |= F_SEEN;
			}
			goto foundone;
			break;
		    case ' ':
			if (rnd(3 + probinc) != 0)
			    break;
			chat(y, x) = PASSAGE;
			goto foundone;
		}
	}
    if (found)
	look(false);
}

/*
 * help:
 *	Give single character help, or the whole mess if he wants it
 */
void
help(void)
{
    const struct h_list *strp;
    int helpch;
    int numprint, cnt;
    msg("character you want help for (* for all): ");
    helpch = readchar();
    mpos = 0;
    /*
     * If its not a *, print the right help string
     * or an error if he typed a funny character.
     */
    if (helpch != '*')
    {
	move(0, 0);
	for (strp = helpstr; strp->h_desc != NULL; strp++)
	    if (strp->h_ch == helpch)
	    {
		lower_msg = true;
		msg("%s%s", unctrl(strp->h_ch), strp->h_desc);
		lower_msg = false;
		return;
	    }
	msg("unknown character '%s'", unctrl(helpch));
	return;
    }
    /*
     * Here we print help for everything.
     * Then wait before we return to command mode
     */
    numprint = 0;
    for (strp = helpstr; strp->h_desc != NULL; strp++)
	if (strp->h_print)
	    numprint++;
    if (numprint & 01)		/* round odd numbers up */
	numprint++;
    numprint /= 2;
    if (numprint > LINES - 1)
	numprint = LINES - 1;

    wclear(hw);
    cnt = 0;
    for (strp = helpstr; strp->h_desc != NULL; strp++)
	if (strp->h_print)
	{
	    wmove(hw, cnt % numprint, cnt >= numprint ? COLS / 2 : 0);
	    if (strp->h_ch)
		waddstr(hw, unctrl(strp->h_ch));
	    waddstr(hw, strp->h_desc);
	    if (++cnt >= numprint * 2)
		break;
	}
    wmove(hw, LINES - 1, 0);
    waddstr(hw, "--Press space to continue--");
    wrefresh(hw);
    wait_for(hw, ' ');
    clearok(stdscr, true);
/*
    refresh();
*/
    msg("");
    touchwin(stdscr);
    wrefresh(stdscr);
}

/*
 * identify:
 *	Tell the player what a certain thing is.
 */
void
identify(void)
{
    int ch;
    const struct h_list *hp;
    const char *str;
    const struct h_list ident_list[] = {
	{'|',		"wall of a room",		false},
	{'-',		"wall of a room",		false},
	{GOLD,		"gold",				false},
	{STAIRS,	"a staircase",			false},
	{DOOR,		"door",				false},
	{FLOOR,		"room floor",			false},
	{PLAYER,	"you",				false},
	{PASSAGE,	"passage",			false},
	{TRAP,		"trap",				false},
	{POTION,	"potion",			false},
	{SCROLL,	"scroll",			false},
	{FOOD,		"food",				false},
	{WEAPON,	"weapon",			false},
	{' ',		"solid rock",			false},
	{ARMOR,		"armor",			false},
	{AMULET,	"the Amulet of Yendor",		false},
	{RING,		"ring",				false},
	{STICK,		"wand or staff",		false},
	{'\0'}
    };

    msg("what do you want identified? ");
    ch = readchar();
    mpos = 0;
    if (ch == ESCAPE)
    {
	msg("");
	return;
    }
    if (isupper(ch))
	str = monsters[ch-'A'].m_name;
    else
    {
	str = "unknown character";
	for (hp = ident_list; hp->h_ch != '\0'; hp++)
	    if (hp->h_ch == ch)
	    {
		str = hp->h_desc;
		break;
	    }
    }
    msg("'%s': %s", unctrl(ch), str);
}

/*
 * d_level:
 *	He wants to go down a level
 */
void
d_level(void)
{
    if (levit_check())
	return;
    if (chat(hero.y, hero.x) != STAIRS)
	msg("I see no way down");
    else
    {
	level++;
	seenstairs = false;
	new_level();
    }
}

/*
 * u_level:
 *	He wants to go up a level
 */
void
u_level(void)
{
    if (levit_check())
	return;
    if (chat(hero.y, hero.x) == STAIRS)
	if (amulet)
	{
	    level--;
	    if (level == 0)
		total_winner();
	    new_level();
	    msg("you feel a wrenching sensation in your gut");
	}
	else
	    msg("your way is magically blocked");
    else
	msg("I see no way up");
}

/*
 * levit_check:
 *	Check to see if she's levitating, and if she is, print an
 *	appropriate message.
 */
int
levit_check(void)
{
    if (!on(player, ISLEVIT))
	return false;
    msg("You can't.  You're floating off the ground!");
    return true;
}

/*
 * call:
 *	Allow a user to call a potion, scroll, or ring something
 */
void
call(void)
{
    THING *obj;
    struct obj_info *op = NULL;
    char **guess;
    const char *elsewise = NULL;
    int *know;

    obj = get_item("call", CALLABLE);
    /*
     * Make certain that it is somethings that we want to wear
     */
    if (obj == NULL)
	return;
    switch (obj->o_type)
    {
	case RING:
	    op = &ring_info[obj->o_which];
	    elsewise = r_stones[obj->o_which];
	    goto norm;
	when POTION:
	    op = &pot_info[obj->o_which];
	    elsewise = p_colors[obj->o_which];
	    goto norm;
	when SCROLL:
	    op = &scr_info[obj->o_which];
	    elsewise = s_names[obj->o_which];
	    goto norm;
	when STICK:
	    op = &ws_info[obj->o_which];
	    elsewise = ws_made[obj->o_which];
norm:
	    know = &op->oi_know;
	    guess = &op->oi_guess;
	    if (*guess != NULL)
		elsewise = *guess;
	when FOOD:
	    msg("you can't call that anything");
	    return;
	otherwise:
	    guess = &obj->o_label;
	    know = NULL;
	    elsewise = obj->o_label;
    }
    if (know != NULL && *know)
    {
	msg("that has already been identified");
	return;
    }
    if (elsewise != NULL && elsewise == *guess)
    {
	if (!terse)
	    addmsg("Was ");
	msg("called \"%s\"", elsewise);
    }
    if (terse)
	msg("call it: ");
    else
	msg("what do you want to call it? ");

    if (elsewise == NULL)
	strcpy(prbuf, "");
    else
	strcpy(prbuf, elsewise);
    if (get_str(prbuf, stdscr) == NORM)
    {
	if (*guess != NULL)
	    free(*guess);
	*guess = malloc(strlen(prbuf) + 1);
	if (*guess != NULL)
		strcpy(*guess, prbuf);
    }

    msg("");
}

/*
 * current:
 *	Print the current weapon/armor
 */
void
current(const THING *cur, const char *how, const char *where)
{
    after = false;
    if (cur != NULL)
    {
	if (!terse)
	    addmsg("you are %s (", how);
	inv_describe = false;
	addmsg("%c) %s", cur->o_packch, inv_name(cur, true));
	inv_describe = true;
	if (where)
	    addmsg(" %s", where);
	endmsg();
    }
    else
    {
	if (!terse)
	    addmsg("you are ");
	addmsg("%s nothing", how);
	if (where)
	    addmsg(" %s", where);
	endmsg();
    }
}
