/*
 * config.h - rogue configuration
 *
 * These defines help control how rogue is compiled.
 * If in doubt, follow the "Suggestion:" line.
 */


#if !defined(HAVE_CONFIG_H)
#define HAVE_CONFIG_H


/*
 * ALLSCORES - use top scores or top player scores
 *
 * define - record top scores regardless of who played the game
 * undef  - record a score from each of the top players
 *
 * Suggestion: define
 */
#define ALLSCORES
/* #undef ALLSCORES */

/*
 * number of scores to store in scoreboard
 *
 * IMPORTANT NOTE: If you change NUMSCORES, then a existing rogue score file
 *                 will appear to be corrupted.  If that happens, simply remove
 *                 your existing rogue score file, and run "rogue -s" to
 *                 re-initialize it.
 *
 *                 If you really want to restore your rogue scores after the
 *                 removal and re-initialization, the use the `scedit` tool
 *                 to restore your score scores.
 *
 * Suggestion: define as 10
 */
#define NUMSCORES 10


/*
 * MASTER - to include wizard mode
 *
 * define - allow wizard mode
 * undef - disallow wizard mode
 *
 * Suggestion: define
 */
#define MASTER
/* #undef MASTER */


/*
 * CHECKTIME - number of minutes between load average checks
 *
 * When CHECKTIME defined as a number, the game checks your system load
 * average, and will warn, then kick you out when the system load is too high.
 *
 * define - minutes between load average checks
 * undef - do not check load average
 *
 * Suggestion: undef
 */
/* #define CHECKTIME 5 */
#undef CHECKTIME


/*
 * HAVE_ERASECHAR - if ncurses has the erasechar() function
 *
 * define - ncurses has the erasechar() function
 * undef - try _tty.c_cc[VERASE], or try _tty.sg_erase
 *
 * Suggestion: define
 */
#define HAVE_ERASECHAR
/* #undef HAVE_ERASECHAR */


/*
 * HAVE_ESCDELAY - if ncurses has the ESCDELAY variable
 *
 * define - ncurses has the ESCDELAY variable
 * undef - no ESCDELAY variable in ncurses
 *
 * Suggestion: define
 */
#define HAVE_ESCDELAY
/* #undef HAVE_ESCDELAY */


/*
 * HAVE_GETPASS - if system has a getpass() function
 *
 * define - system has a getpass() function
 * undef - simulate the getpass() function
 *
 * Suggestion: define
 */
#define HAVE_GETPASS
/* #undef HAVE_GETPASS */


/*
 * HAVE_KILLCHAR - if system as a killchar() function
 *
 * define - ncurses has the killchar() function
 * undef - try _tty.c_cc[VKILL], or try _tty.sg_kill
 *
 * Suggestion: define
 */
#define HAVE_KILLCHAR
/* #undef HAVE_KILLCHAR */


/*
 * MAXLOAD - if a maximum 5 minute load average game play limit should be used
 *
 * define - limit game play based on 5 minute load average, value is in 0.1 load average units
 * undef - do not limit game play based on load average
 *
 * Suggestion: undef
 */
/* #define MAXLOAD 40 */
#undef MAXLOAD


#endif
