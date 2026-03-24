/*
 * global variable initializaton
 *
 * @(#)extern.c	4.82 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

/* All global variables are defined here, or in vers.c or init.c */

#include "modern_curses.h"
#include "extern.h"
#include "config.h"
#include "rogue.h"

int  after;				/* True if we want after daemons */
int  again;				/* Repeating the last command */
int  noscore = false;			/* Was a wizard sometime */
int  seenstairs;			/* Have seen the stairs (for lsd) */
int  amulet = false;			/* He found the amulet */
int  door_stop = false;			/* Stop running when we pass a door */
int  fight_flush = false;		/* True if toilet input */
int  firstmove = false;			/* First move after setting door_stop */
int  got_ltc = false;			/* We have gotten the local tty chars */
int  has_hit = false;			/* Has a "hit" message pending in msg */
int  in_shell = false;			/* True if executing a shell */
int  inv_describe = true;		/* Say which way items are being used */
int  jump = false;			/* Show running as series of jumps */
int  kamikaze = false;			/* to_death really to DEATH */
int  lower_msg = false;			/* Messages should start w/lower case */
int  move_on = false;			/* Next move shouldn't pick up items */
int  msg_esc = false;			/* Check for ESC from msg's --More-- */
int  passgo = false;			/* Follow passages */
int  playing = true;			/* True until he quits */
int  q_comm = false;			/* Are we executing a 'Q' command? */
int  running = false;			/* True if player is running */
int  save_msg = true;			/* Remember last msg */
int  see_floor = true;			/* Show the lamp illuminated floor */
int  stat_msg = false;			/* Should status() print as a msg() */
int  terse = false;				/* True if we should be short */
int  to_death = false;			/* Fighting is to the death! */
int  tombstone = true;			/* Print out tombstone at end */
#ifdef MASTER
int  wizard = false;			/* True if allows wizard commands */
#endif
int  pack_used[MAXPACK + 1] = {		/* Is the character used in the pack? +1 for paranoia */
    false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false,
    false, /* paranoia */
};

int  dir_ch;				/* Direction from last get_dir() call */
char home[MAXSTR + 1] = {'\0'};		/* User's home directory plus a trailing /, +1 for paranoia */
char file_name[MAXSTR + 1] = {'\0'};	/* rogue save file path, +1 for paranoia */
char lock_path[MAXSTR + 1] = {'\0'};	/* rogue lock file path, +1 for paranoia */
char score_path[MAXSTR + 1] = {'\0'};	/* rogue score file path, +1 for paranoia */
char huh[MAXSTR+1];			/* The last message printed, +1 for paranoia */
const char *p_colors[MAXPOTIONS + 1];	/* Colors of the potions, +1 for paranoia */
char prbuf[PFBUF_LEN + 1] = {'\0'};	/* buffer for snprintfs, +1 for paranoia */
const char *r_stones[MAXRINGS + 1];	/* Stone settings of the rings, +1 for paranoia */
int  runch;				/* Direction player is running */
char *s_names[MAXSCROLLS + 1];		/* Names of the scrolls, +1 for paranoia */
int  take;				/* Thing she is taking */
const char *ws_made[MAXSTICKS];		/* What sticks are made of */
char *ws_type[MAXSTICKS];		/* Is it a wand or a staff */
int  orig_dsusp;			/* Original dsusp char */
char fruit[MAXSTR+1] =			/* Favorite fruit, +1 for paranoia */
		{ 's', 'l', 'i', 'm', 'e', '-', 'm', 'o', 'l', 'd', '\0' };
const char *inv_t_name[] = {
	"Overwrite",
	"Slow",
	"Clear"
};
int  l_last_comm = '\0';		/* Last last_comm */
int  l_last_dir = '\0';			/* Last last_dir */
int  last_comm = '\0';			/* Last command typed */
int  last_dir = '\0';			/* Last direction given */
const char *tr_name[] = {			/* Names of the traps */
	"a trapdoor",
	"an arrow trap",
	"a sleeping gas trap",
	"a bear trap",
	"a teleport trap",
	"a poison dart trap",
	"a rust trap",
        "a mysterious trap"
};


int n_objs;				/* # items listed in inventory() call */
int ntraps;				/* Number of traps on this level */
int hungry_state = 0;			/* How hungry is he */
int inpack = 0;				/* Number of things in pack */
int inv_type = 0;			/* Type of inventory to use */
int level = 1;				/* What level she is on */
int max_hit;				/* Max damage done to her in to_death */
int max_level;				/* Deepest player has gone */
int mpos = 0;				/* Where cursor is on top line */
int no_food = 0;			/* Number of levels without food */
const int a_class[MAXARMORS] = {		/* Armor class for each armor type */
	8,	/* LEATHER */
	7,	/* RING_MAIL */
	7,	/* STUDDED_LEATHER */
	6,	/* SCALE_MAIL */
	5,	/* CHAIN_MAIL */
	4,	/* SPLINT_MAIL */
	4,	/* BANDED_MAIL */
	3,	/* PLATE_MAIL */
};

int count = 0;				/* Number of times to repeat command */
FILE *scoreboard = NULL;		/* File descriptor for score file */
int food_left;				/* Amount of food in hero's stomach */
int lastscore = -1;			/* Score before this turn */
int no_command = 0;			/* Number of turns asleep */
int no_move = 0;			/* Number of turns held in place */
int purse = 0;				/* How much gold he has */
int quiet = 0;				/* Number of quiet turns */
int vf_hit = 0;				/* Number of time flytrap has hit */

unsigned int dnum;				/* Dungeon number */
#if defined(NON_BSD_RN_GENERATOR)
unsigned int seed;				/* Random number seed */
#endif
const int e_levels[] = {
        10L,
	20L,
	40L,
	80L,
       160L,
       320L,
       640L,
      1300L,
      2600L,
      5200L,
     13000L,
     26000L,
     50000L,
    100000L,
    200000L,
    400000L,
    800000L,
   2000000L,
   4000000L,
   8000000L,
	 0L
};

coord delta;				/* Change indicated to get_dir() */
coord oldpos;				/* Position before last look() call */
coord stairs;				/* Location of staircase */

PLACE places[(MAXLINES*MAXCOLS) + 1];		/* level map, +1 for paranoia */

THING *cur_armor;			/* What he is wearing */
THING *cur_ring[2];			/* Which rings are being worn */
THING *cur_weapon;			/* Which weapon he is weilding */
THING *l_last_pick = NULL;		/* Last last_pick */
THING *last_pick = NULL;		/* Last object picked in get_item() */
THING *lvl_obj = NULL;			/* List of objects on this level */
THING *mlist = NULL;			/* List of monsters on the level */
THING player;				/* His stats */
					/* restart of game */

WINDOW *hw = NULL;			/* used as a scratch window */

#define INIT_STATS { 16, 0, 1, 10, 12, "1x4", 12 }

struct stats max_stats = INIT_STATS;	/* The maximum for the player */

struct room *oldrp;			/* Roomin(&oldpos) */
struct room rooms[MAXROOMS + 1];	/* One for each room -- A level, +1 for paranoia */
struct room passages[MAXPASS + 1] =	/* One for each passage, +1 for paranoia */
{
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} },
    { {0, 0}, {0, 0}, {0, 0}, 0, ISGONE|ISDARK, 0, {{0,0}} } /* paranoia */
};

#define ___ 1
#define XX 10
struct monster monsters[26 + 1] =
    {
/* Name		 CARRY	FLAG    str, exp, lvl, amr, hpt, dmg */
{ "aquator",	   0,	ISMEAN,	{ XX, 20,   5,   2, ___, "0x0/0x0" } },
{ "bat",	   0,	ISFLY,	{ XX,  1,   1,   3, ___, "1x2" } },
{ "centaur",	  15,	0,	{ XX, 17,   4,   4, ___, "1x2/1x5/1x5" } },
{ "dragon",	 100,	ISMEAN,	{ XX,5000, 10,  -1, ___, "1x8/1x8/3x10" } },
{ "emu",	   0,	ISMEAN,	{ XX,  2,   1,   7, ___, "1x2" } },
{ "venus flytrap", 0,	ISMEAN,	{ XX, 80,   8,   3, ___, "%%%x0" } },
	/* NOTE: the damage is %%% so that xstr won't merge this */
	/* string with others, since it is written on in the program */
{ "griffin",	  20,	ISMEAN|ISFLY|ISREGEN, { XX,2000, 13,   2, ___, "4x3/3x5" } },
{ "hobgoblin",	   0,	ISMEAN,	{ XX,  3,   1,   5, ___, "1x8" } },
{ "ice monster",   0,	0,	{ XX,  5,   1,   9, ___, "0x0" } },
{ "jabberwock",   70,	0,	{ XX,3000, 15,   6, ___, "2x12/2x4" } },
{ "kestrel",	   0,	ISMEAN|ISFLY,	{ XX,  1,   1,   7, ___, "1x4" } },
{ "leprechaun",	   0,	0,	{ XX, 10,   3,   8, ___, "1x1" } },
{ "medusa",	  40,	ISMEAN,	{ XX,200,   8,   2, ___, "3x4/3x4/2x5" } },
{ "nymph",	 100,	0,	{ XX, 37,   3,   9, ___, "0x0" } },
{ "orc",	  15,	ISGREED,{ XX,  5,   1,   6, ___, "1x8" } },
{ "phantom",	   0,	ISINVIS,{ XX,120,   8,   3, ___, "4x4" } },
{ "quagga",	   0,	ISMEAN,	{ XX, 15,   3,   3, ___, "1x5/1x5" } },
{ "rattlesnake",   0,	ISMEAN,	{ XX,  9,   2,   3, ___, "1x6" } },
{ "snake",	   0,	ISMEAN,	{ XX,  2,   1,   5, ___, "1x3" } },
{ "troll",	  50,	ISREGEN|ISMEAN,{ XX, 120, 6, 4, ___, "1x8/1x8/2x6" } },
{ "black unicorn", 0,	ISMEAN,	{ XX,190,   7,  -2, ___, "1x9/1x9/2x9" } },
{ "vampire",	  20,	ISREGEN|ISMEAN,{ XX,350,   8,   1, ___, "1x10" } },
{ "wraith",	   0,	0,	{ XX, 55,   5,   4, ___, "1x6" } },
{ "xeroc",	  30,	0,	{ XX,100,   7,   7, ___, "4x4" } },
{ "yeti",	  30,	0,	{ XX, 50,   4,   6, ___, "1x6/1x6" } },
{ "zombie",	   0,	ISMEAN,	{ XX,  6,   2,   8, ___, "1x8" } },
{ "",		   0,	0,	{ 0,   0,   0,   0,   0, "" } }, /* paranoia */
    };
#undef ___
#undef XX

struct obj_info things[NUMTHINGS + 1] = {
    { 0,			26 },	/* potion */
    { 0,			36 },	/* scroll */
    { 0,			16 },	/* food */
    { 0,			 7 },	/* weapon */
    { 0,			 7 },	/* armor */
    { 0,			 4 },	/* ring */
    { 0,			 4 },	/* stick */
    { 0,			 0 },	/* paranoia */
};

struct obj_info arm_info[MAXARMORS + 1] = {
    { "leather armor",		 20,	 20, NULL, false },
    { "ring mail",		 15,	 25, NULL, false },
    { "studded leather armor",	 15,	 20, NULL, false },
    { "scale mail",		 13,	 30, NULL, false },
    { "chain mail",		 12,	 75, NULL, false },
    { "splint mail",		 10,	 80, NULL, false },
    { "banded mail",		 10,	 90, NULL, false },
    { "plate mail",		  5,	150, NULL, false },
    { NULL,			  0,	  0, NULL, false }, /* paranoia */
};
struct obj_info pot_info[MAXPOTIONS + 1] = {
    { "confusion",		 7,   5, NULL, false },
    { "hallucination",		 8,   5, NULL, false },
    { "poison",			 8,   5, NULL, false },
    { "gain strength",		13, 150, NULL, false },
    { "see invisible",		 3, 100, NULL, false },
    { "healing",		13, 130, NULL, false },
    { "monster detection",	 6, 130, NULL, false },
    { "magic detection",	 6, 105, NULL, false },
    { "raise level",		 2, 250, NULL, false },
    { "extra healing",		 5, 200, NULL, false },
    { "haste self",		 5, 190, NULL, false },
    { "restore strength",	13, 130, NULL, false },
    { "blindness",		 5,   5, NULL, false },
    { "levitation",		 6,  75, NULL, false },
    { NULL,			 0,   0, NULL, false }, /* paranoia */
};
struct obj_info ring_info[MAXRINGS + 1] = {
    { "protection",		 9, 400, NULL, false },
    { "add strength",		 9, 400, NULL, false },
    { "sustain strength",	 5, 280, NULL, false },
    { "searching",		10, 420, NULL, false },
    { "see invisible",		10, 310, NULL, false },
    { "adornment",		 1,  10, NULL, false },
    { "aggravate monster",	10,  10, NULL, false },
    { "dexterity",		 8, 440, NULL, false },
    { "increase damage",	 8, 400, NULL, false },
    { "regeneration",		 4, 460, NULL, false },
    { "slow digestion",		 9, 240, NULL, false },
    { "teleportation",		 5,  30, NULL, false },
    { "stealth",		 7, 470, NULL, false },
    { "maintain armor",		 5, 380, NULL, false },
    { NULL,			 0,   0, NULL, false }, /* paranoia */
};
struct obj_info scr_info[MAXSCROLLS + 1] = {
    { "monster confusion",		 7, 140, NULL, false },
    { "magic mapping",			 4, 150, NULL, false },
    { "hold monster",			 2, 180, NULL, false },
    { "sleep",				 3,   5, NULL, false },
    { "enchant armor",			 7, 160, NULL, false },
    { "identify potion",		10,  80, NULL, false },
    { "identify scroll",		10,  80, NULL, false },
    { "identify weapon",		 6,  80, NULL, false },
    { "identify armor",			 7, 100, NULL, false },
    { "identify ring, wand or staff",	10, 115, NULL, false },
    { "scare monster",			 3, 200, NULL, false },
    { "food detection",			 2,  60, NULL, false },
    { "teleportation",			 5, 165, NULL, false },
    { "enchant weapon",			 8, 150, NULL, false },
    { "create monster",			 4,  75, NULL, false },
    { "remove curse",			 7, 105, NULL, false },
    { "aggravate monsters",		 3,  20, NULL, false },
    { "protect armor",			 2, 250, NULL, false },
    { NULL,				 0,   0, NULL, false }, /* paranoia */
};
struct obj_info weap_info[MAXWEAPONS + 1 + 1] = {
    { "mace",				11,   8, NULL, false },
    { "long sword",			11,  15, NULL, false },
    { "short bow",			12,  15, NULL, false },
    { "arrow",				12,   1, NULL, false },
    { "dagger",				 8,   3, NULL, false },
    { "two handed sword",		10,  75, NULL, false },
    { "dart",				12,   2, NULL, false },
    { "shuriken",			12,   5, NULL, false },
    { "spear",				12,   5, NULL, false },
    { NULL, 0 },	/* DO NOT REMOVE: fake entry for dragon's breath */
    { NULL,				 0,   0, NULL, false }, /* paranoia */
};
struct obj_info ws_info[MAXSTICKS + 1] = {
    { "light",			12, 250, NULL, false },
    { "invisibility",		 6,   5, NULL, false },
    { "lightning",		 3, 330, NULL, false },
    { "fire",			 3, 330, NULL, false },
    { "cold",			 3, 330, NULL, false },
    { "polymorph",		15, 310, NULL, false },
    { "magic missile",		10, 170, NULL, false },
    { "haste monster",		10,   5, NULL, false },
    { "slow monster",		11, 350, NULL, false },
    { "drain life",		 9, 300, NULL, false },
    { "nothing",		 1,   5, NULL, false },
    { "teleport away",		 6, 340, NULL, false },
    { "teleport to",		 6,  50, NULL, false },
    { "cancellation",		 5, 280, NULL, false },
    { NULL,			 0,   0, NULL, false }, /* paranoia */
};

const struct h_list helpstr[] = {
    {'?',	"	prints help",				true},
    {'/',	"	identify object",			true},
    {'h',	"	left",					true},
    {'j',	"	down",					true},
    {'k',	"	up",					true},
    {'l',	"	right",					true},
    {'y',	"	up & left",				true},
    {'u',	"	up & right",				true},
    {'b',	"	down & left",				true},
    {'n',	"	down & right",				true},
    {'H',	"	run left",				false},
    {'J',	"	run down",				false},
    {'K',	"	run up",				false},
    {'L',	"	run right",				false},
    {'Y',	"	run up & left",				false},
    {'U',	"	run up & right",			false},
    {'B',	"	run down & left",			false},
    {'N',	"	run down & right",			false},
    {CTRL('H'),	"	run left until adjacent",		false},
    {CTRL('J'),	"	run down until adjacent",		false},
    {CTRL('K'),	"	run up until adjacent",			false},
    {CTRL('L'),	"	run right until adjacent",		false},
    {CTRL('Y'),	"	run up & left until adjacent",		false},
    {CTRL('U'),	"	run up & right until adjacent",		false},
    {CTRL('B'),	"	run down & left until adjacent",	false},
    {CTRL('N'),	"	run down & right until adjacent",	false},
    {'\0',	"	<SHIFT><dir>: run that way",		true},
    {'\0',	"	<CTRL><dir>: run till adjacent",	true},
    {'f',	"<dir>	fight till death or near death",	true},
    {'t',	"<dir>	throw something",			true},
    {'m',	"<dir>	move onto without picking up",		true},
    {'z',	"<dir>	zap a wand in a direction",		true},
    {'^',	"<dir>	identify trap type",			true},
    {'s',	"	search for trap/secret door",		true},
    {'>',	"	go down a staircase",			true},
    {'<',	"	go up a staircase",			true},
    {'.',	"	rest for a turn",			true},
    {',',	"	pick something up",			true},
    {'i',	"	inventory",				true},
    {'I',	"	inventory single item",			true},
    {'q',	"	quaff potion",				true},
    {'r',	"	read scroll",				true},
    {'e',	"	eat food",				true},
    {'w',	"	wield a weapon",			true},
    {'W',	"	wear armor",				true},
    {'T',	"	take armor off",			true},
    {'P',	"	put on ring",				true},
    {'R',	"	remove ring",				true},
    {'d',	"	drop object",				true},
    {'c',	"	call object",				true},
    {'a',	"	repeat last command",			true},
    {')',	"	print current weapon",			true},
    {']',	"	print current armor",			true},
    {'=',	"	print current rings",			true},
    {'@',	"	print current stats",			true},
    {'D',	"	recall what's been discovered",		true},
    {'o',	"	examine/set options",			true},
    {CTRL('R'),	"	redraw screen",				true},
    {CTRL('P'),	"	repeat last message",			true},
    {ESCAPE,	"	cancel command, ^[ is the escape key",	true},
    {'S',	"	save game",				true},
    {'Q',	"	quit",					true},
    {'!',	"	shell escape",				true},
    {'F',	"<dir>	fight till either of you dies",		true},
    {'v',	"	print version, release, dungeon number",true},
    {0,		NULL }
};
int numscores = NUMSCORES;
char *Numname;
int allscore;
int between;

#define _X_ { EMPTY }

struct delayed_action d_list[MAXDAEMONS+1] = {
    _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_,
    _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_, _X_,
    _X_
};

int group = 2;
