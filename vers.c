/*
 * Release and Version number.
 *
 * release - just a rogue version number that is written to the rogue save file
 * version - source location and date
 *
 * Whenever a new version or release string is desired, modify the string below.
 *
 * The original authors claim:
 *
 *	encstr is declared here to force it to be loaded before the version number,
 *	and therefore not to be written in saved games.
 *
 * IMPORTANT NOTE: As of rogue 5.4.5, the release, NOT the version is written to the rogue save file.
 *		   The release is written to the rogue save file instead of the version.
 *		   So when you change the release string, you invalidate all old rogue save files.
 *		   You can now change the version without impacting rogue save files.
 *
 * NOTE: The encstr[] and statlist[] are used by encwrite() and encread().
 *	 You probably don't want to change either of them as doing so would almost
 *	 certainly invalidate existing rogue score files, and existing rogue save files.
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE for full copyright and licensing information.
 */

const char *release = "5.4.5";
const char encstr[] = "\300k||`\251Y.'\305\321\201+\277~r\"]\240_\223=1\341)\222\212\241t;\t$\270\314/<#\201\254";
const char statlist[] = "\355kl{+\204\255\313idJ\361\214=4:\311\271\341wK<\312\321\213,,7\271/Rk%\b\312\f\246";
const char version[] = "(github.com/lcn2/rogue5.4) 2026-02-15";
