# rogue 5.4

Rogue: Exploring the Dungeons of Doom version 5.4

This rogue plays like the original Vax/pdp binary that was distributed on the 4.3BSD tapes.

GitHub repo: [rogue5.4 repo](https://github.com/lcn2/rogue5.4)


## TL;DR

To configure:

If needed, edit `Makefile` and/or `config.h`.  Most likely you won't need to do so.

To make and install:

```sh
make clobber all
sudo make install
```

To run rogue after installing:

```sh
/usr/local/bin/rogue
```


## To play rogue

After installing, run:

```
/usr/local/bin/rogue
```

To quit, interrupt the program (type control-C), answer "y" to the question and press return.

Your score file is store in the by default, is maintained under `${ROGUEDIR}` which
defaults to your home directory (i.e., `~/rogue.scr`).

To save your rogue game, press "S" and save.

To restore your make, run:

```sh
/usr/local/bin/rogue ~/.rogue.save
```

**NOTE**: Restoring your rogue game will cause the rogue save file to be removed.

For help in the game type "**?**" and then "\*".

For your convenience, the full game help output is formatted below:

```
    ?       prints help
    /       identify object
    h       left
    j       down
    k       up
    l       right
    y       up & left
    u       up & right
    b       down & left
    n       down & right
            <SHIFT><dir>: run that way
            <CTRL><dir>: run till adjacent
    f<dir>  fight till death or near death
    t<dir>  throw something
    m<dir>  move onto without picking up
    z<dir>  zap a wand in a direction
    ^<dir>  identify trap type
    s       search for trap/secret door
    >       go down a staircase
    <       go up a staircase
    .       rest for a turn
    ,       pick something up
    i       inventory
    I       inventory single item
    q       quaff potion

    r       read scroll
    e       eat food
    w       wield a weapon
    W       wear armor
    T       take armor off
    P       put on ring
    R       remove ring
    d       drop object
    c       call object
    a       repeat last command
    )       print current weapon
    ]       print current armor
    =       print current rings
    @       print current stats
    D       recall what's been discovered
    o       examine/set options
    ^R      redraw screen
    ^P      repeat last message
    Esc     cancel command
    S       save game
    Q       quit
    !       shell escape
    F<dir>  fight till either of you dies
    v       print version number
```

The following table may help explain the symbols you see when playing rogue:

| symbol | object  |
|--------|---------|
|    #   | passage |
|    +   | door    |
|    .   | floor   |
|    @   | player  |
|    ^   | trap    |
|    *   | gold    |
|    !   | potion  |
|    ?   | scroll  |
|    $   | food    |
|    )   | weapon  |
|    ]   | armor   |
|    ,   | amulet  |
|    =   | ring    |
|    /   | staff   |

For more information about the make, see the `rogue(6)` man page after installing, or run the command
from the top level directory:

```sh
make rogue.6
man ./rogue.6
```

To read "A Guide to the Dungeons of Doom", try:

```sh
make rogue.doc
less rogue.doc
```

**GOAL**: As the man page states:

> To win the game (as opposed to merely playing to beat other people's high scores) you must
> locate the Amulet of Yendor which is somewhere below the 20th level of the dungeon and get
> it out.

**BTW**: The `rogue(6)` man page states:

> Nobody has achieved this yet and if somebody does, they will probably go down in
> history as a hero among heroes.

The above statement is hyperbole and not intended to imply that nobody
has achieved the goal.  The maintainer of the [rogue5.4 repo](https://github.com/lcn2/rogue5.4)
has completed the goal (won) at least 3 times (without using **wizard mode**) and reached a
maximum dungeon depth (and successful return to level 0) of 31.


## rogue options

The `rogue(6)` game options may be displayed and changed by using the "**o**" command.

The option names are:

* terse / noterse

Useful for those who are tired of the sometimes lengthy messages of rogue.

This is a useful option to enable when playing on an historically slow terminal.

Default: noterse

Recommendation: noterse

* flush / noflush

If this option is set, typeahead is thrown away after each round of battle.

This is useful for those who type far ahead, and then watch in dismay as a Bat kills them.

Default: noflush

Recommendation: flush

* jump / nojump

If this option is set, running moves will not be displayed until you reach the end of the move.

This is a useful option to enable when playing on an historically slow terminal.

Default: nojump

Recommendation: jump

* seefloor / noseefloor

Display the floor around you on the screen as you move through dark rooms.

This is a useful option to disable when playing on an historically slow terminal.

Default: seefloor

Recommendation: seefloor

* passgo / nopassgo

Follow turnings in passageways.
If you run in a passage and you run into stone or a wall, rogue will see if it can turn to the right or left.
If it can only turn one way, it will turn that way. If it can turn either or neither, it will stop.

This algorithm can sometimes lead to slightly confusing occurrences to some people, which is why it defaults to false.

Default: nopassgo

Recommendation: passgo

* tombstone / notombstone

Print out the tombstone at the end if you get killed.

This is a useful option to disable when playing on an historically slow terminal.

Default: tombstone

Recommendation: tombstone

* inven=style

Set the inventory style to one of "overwrite", "slow", or "clear".

With "overwrite" the top lines of the map are overwritten with the list when inventory is requested,
or when "Which item do you wish to ...?" questions are answered with a "\*".
However, if the list is longer than a screenful, the screen is cleared.

With "slow", lists are displayed one item at a time on the top of the screen.

With "clear", the screen is cleared, the list is displayed, and then the dungeon level is re-displayed.

Default: clear

Recommendation: overwrite

* name=rogue-name

This is the name of your character, as displayed in the rogue score file.

Default: <<Your account name>>

Recommendation: <<leave as default>>

* fruit=fruit-name

This should hold the name of a fruit that you enjoy eating.
It is basically a whimsy that rogue uses in a couple of places.

Default: slime-mold

Recommendation: apple

* file=rogue-save-path

The default file name for saving the game.

Default: ~/.rogue.save

Recommendation: <<leave as default>>

* score=rogue-score-path

The path of the rogue score file.

Default: ~/.rogue.scr

Recommendation: <<leave as default>>

* lock=rogue-lock-path

The rogue lock file used to protect the rogue score file.

Default: ~/.rogue.lck

Recommendation: <<leave as default>>


## ROGUEOPTS environment variable

The `$ROGUEOPTS` environment variable may be used to change the `rogue(6)` at startup,
rather than having to use the "**o**" command each time you play.

The `$ROGUEOPTS` environment variable would be a string that is a series
of comma separated values, with booleans being stated as "name" (true)
or "noname" (false), and strings being "name=....", with the string
being defined up to a comma or the end of the entire option string.

As an example, here is a recommended `$ROGUEOPTS` environment value to set in your environment:

```sh
export ROGUEOPTS="noterse,flush,jump,seefloor,passgo,tombstone,inven=overwrite,fruit=apple"
```

If the `$ROGUEOPTS` environment variable is missing or empty, the default
options will be used.  Unknown options are silently ignored.


## Rogue files

By default, the rogue stores files in your home (i.e., '${HOME}`) directory:

* ~/.rogue.lck

The rogue lock file is a zero-length file that must be writable by the user.
It is used to help prevent multiple processes on the local machine from
writing to the rogue score file at the same time.

If the rogue lock file does not exist, the lock file will created when
updating the rogue score file.

* ~/.rogue.save

The rogue save file is created whenever you use the "**S**" command to
save the game, or if certain signals are successfully caught and
the "auto save" action is successfully completed.

A rogue save file is not guaranteed to restore in a different system.

If the rogue save file does not exist, `rogue(6)` will create it a game is saved.

* ~/.rogue.scr

The rogue score file contains the top rogue games, in terms of gold collected.
The number of rogue score slots is defined by `NUMSCORES` in `config.h`
and defaults to 10 top rogue scores.

The rogue score file must be writable by the user.

The length of the rogue score file is usually 112 bytes per score slot,
so the default rogue score file size is 1120 bytes.

Unlike the BSD rogue game on the Vax/pdp, rogue keeps track of top rogue scores,
no matter how they were played, and no matter if they won the game or not.

If the rogue score file does not exist, `rogue(6)` will create it.

If the rogue score file is corrupted, or the format is too old, then
`rogue(6)` will display an error message and recommend removing it.

**IMPORTANT NOTE**: Changing the `NUMSCORES` value will cause rogue score files
with a different number of slots to become invalid.  A `rogue(6)` compiled
with a different value will treat the previous rogue score file as if
it were corrupt.

You may use the `scedit(1)` tool to modify the rogue score file for
some important reason, such as restoring a lost score.  Use of the `scedit(1)`
tool to create fake scores is not a nice thing to do.


# Changing the rogue directory

The make variable `${ROGUEDIR}` is the directory where the rogue
lock file, rogue score file, and the rogue save file are used and stored.
The make variable `${ROGUEDIR}` defaults to your home directory (i.e.,
`$HOME`).  If you wish to use a different directory, then compile and
install with a modified `${ROGUEDIR}`.

For example:

```sh
make clobber all ROGUEDIR=/var/tmp
sudo make install ROGUEDIR=/var/tmp
```


### Dependencies

* Modern C compiler (c17/gnu17 or better)
* Modern `make(1)` (recommend [GNU make](https://www.gnu.org/software/make/))
* [Ncurses](https://invisible-island.net/ncurses/announce.html) (<ncurses.h> and libncurses), or for NetBSD, NetBSD curses is likely to work
* [Single UNIX Specification](https://pubs.opengroup.org/onlinepubs/9799919799/) confirming (or reasonably conforming) operating system such as Linux, macOS, BSD, etc.


## Why yet another rouge repo?

This code is based on the "Rogue like restoration project"'s [rogue5.4](https://github.com/RoguelikeRestorationProject/rogue5.4).

The [rogue5.4 repo](https://github.com/lcn2/rogue5.4) improves on the above mentioned repo in several important aspects:

* Improved the C source to be able to compile under recent C compilers
* Fixed many bugs in the rogue code
* Fixed the code to compile both the `findpw(6)` and `scedit(6)` rouge tools
* Install the `findpw(6)` and `scedit(6)` rouge tools
* Removed GNU autoconf complexities replacing it with a simple `Makefile`
* To configure, simply edit `Makefile` and/or the `config.h` file
* Fixed `make stddocs` so that the proper configuration values are configured into the documentation
* Changed the rogue lock filename to `${ROGUEDIR}/.rogue.lck` (defaults to `.rogue.lck` under your home directory)
* Changed the rogue save filename to `${ROGUEDIR}/.rogue.save` (defaults to `.rogue.save` under your home directory)
* Changed the rogue score filename to `${ROGUEDIR}/.rogue.scr` (`defaults to .rogue.scr` under your home directory)
* Does not require use of `chown(1)`, nor `chgrp(1)` by default
* Fixed cases where creating invalid type of an item in **wizard mode** crashed the game
* Creating an item in **wizard mode** that does not have a sub-type no longer asks which type of item
* Added extensive SPOILER section notes in the lower part of this `README.md` file
* Fixed many buffer overflow bugs, fixed use of un-initialized variables, and fixed memory leak conditions
* Fixed bugs related to the reading and writing of the score file
* Detects if the rogue score file is an old incompatible format and/or corrupt
* If the rogue score file is empty or missing, the code will automatically re-initialize it
* The top scores are recorded in the rogue score file, regardless of if the game was won or not
* You may change the `NUMSCORES` value in `config.h` to a value other than 10
* By default, `rogue(6)` is **NOT** installed setguid (`${GROUPOWNER}`, by default, is empty)
* etc.

**IMPORTANT NOTE**: While a number of significant bugs have been fixed,
we are sure that there remain other bugs.  As such, we do **NOT**
recommend, nor support running rogue setuid/setgid.  We recommend that
you maintain the `Makefile` default of **NOT** setting the `GROUPOWNER`
make variable (leave them empty).  While you may need to `sudo(1)` in
order to install rogue under `/usr/local/bin/`, `/usr/local/share/`,
and `/usr/local/man/man6/`, you do **NOT** have to run `rogue(6)` with
any privileges!  Just run `rogue(6)` as yourself and let the game use
"dot-files" under your home directory.


## Rogue5.4 repo backstory

This [rogue5.4 GitHub repo](https://github.com/lcn2/rogue5.4)
was cloned from [RoguelikeRestorationProject's rogue5.4 repo](https://github.com/RoguelikeRestorationProject/rogue5.4).
[Landon Curt Noll](https://github.com/lcn2), who fondly remembers playing `rogue(1)` on the Vax running BSD, wanted to
port the original game to modern compilers and UNIX-like operating systems
(such as those that resonably conform to the [Single UNIX Specification](https://pubs.opengroup.org/onlinepubs/9799919799/).

As the [RoguelikeRestorationProject's rogue5.4 repo](https://github.com/RoguelikeRestorationProject/rogue5.4)
is efficetly a "read-only" archive of the Dump of the original
[svn r1490](https://github.com/RoguelikeRestorationProject/rogue5.4/commit/9d0dcccc8ec82454bd4d4310f4638985a4726d83),
[Landon Curt Noll](https://github.com/lcn2) elected to break the form and clone
[rogue5.4 GitHub repo](https://github.com/lcn2/rogue5.4) as a standalone repo
to make it easier for people to contribute [rogue5.4 pull requests](https://github.com/lcn2/rogue5.4/pulls)
directly to this repo.  Even so, we are greatful to the
[RoguelikeRestorationProject](https://github.com/RoguelikeRestorationProject)
for making original code base available.

**IMPORTANT NOTE**: This code is reasonably close to the original Rogue
source that used to generate the Vax/pdp binary. It was distributed on the
4.3BSD tapes: the so-called "Toy/Wichman/Arnold Vax binary" Rogue game.
The gameplay provided by this code is essentially the same.

We call this version 5.4.5 in order to distinguish it from older, and likely more buggy, version 5.4.4 code.


## Compatibility

**IMPORTANT NOTE**: This code is **NOT** so-called "Public domain rogue"
rouge game that distributions such as NetBSD imported from 386BSD.
The "Public domain rogue" was a reverse engineering attempt of the
Vax/pdp binary game found on 4.3BSD tapes.  As such, the "Public domain rogue"
game play differs from the "Toy/Wichman/Arnold Vax binary" rogue game in
a fair number of ways.  Moreover, the rogue save file, rogue lock file,
and rogue score file used by NetBSD rogue are **NOT** compatible.

**IMPORTANT NOTE**: Old rogue save files saved prior to **2026 Feb 25**,
are **NOT** compatible, as we had to fix some bugs with the rogue
save/restore code that required the rogue save file format to be changed.

**IMPORTANT NOTE**: A rogue save file is not guaranteed to restore in a different system.
Among the many reasons are such diverse reasons such as differences in CPU
architectures, a diffent byte order, C integer size differences, and the whim
of a C compiler.

**IMPORTANT NOTE**: Damaged or old rogue score files, modified prior to
**2026 Feb 11**, might not be compatible.  The game will exit with an ERROR
if your rogue score file format is too old and/or has been corrupted.
Remove the damaged or old rogue score file and run the game to rebuild.
You can use the `scedit` to restore old scores if you wish.


### Bug reports and Pull Requests welcome

We very much welcome fork [rogue5.4 pull requests](https://github.com/lcn2/rogue5.4/pulls) to fix any:

* failure to compile
* compiler warning messages
* program crashes

You may also file a [rogue5.4 bug report](https://github.com/lcn2/rogue5.4/issues/new?template=bug_report.md).


# Skip over the SPOILERS to the very bottom for Reporting Security Issues


# WARNING - SPOILERS BELOW !!!


## Spoilers

The **Amulet of Yendor** is on level 26.  Look for a **,** (comma) in a room on that level.
Once you have the **Amulet of Yendor**, you are allowed to go up a staircase (use the **<** command).

You may try your hand at going below level 26; however, the deeper you go,
the harder it becomes to survive.  For example, see notes under "**monster spoilers**".
Nevertheless, going deeper than level 26 is seen as a daring bonus, especially if
you survive and return to the surface (level 0).

If you do not pick up the **Amulet of Yendor** on level 26 and you go deeper than level 26,
the game will place the **Amulet of Yendor** on the deeper levels until you manage to pick it up.


## monster spoilers ==> A-Z

The following table lists the known monsters in the dungeon and their properties:

| symbol |    monster    |  carry | flags |  Exp |min lvl| armour| damage           |
|--------|---------------|--------|-------|------|-------|-------|------------------|
|    A   | aquator       |    0 % | m     |   20 |   5   |   2   | 0x0 / 0x0        |
|    B   | bat           |    0 % | f     |    1 |   1   |   3   | 1x2              |
|    C   | centaur       |   15 % |       |   17 |   4   |   4   | 1x2 / 1x5 / 1x5  |
|    D   | dragon        |  100 % | m     | 5000 |  10   |  -1   | 1x8 / 1x8 / 3x10 |
|    E   | emu           |    0 % | m     |    2 |   1   |   7   | 1x2              |
|    F   | venus flytrap |    0 % | m     |   80 |   8   |   3   | ++               |
|    G   | griffin       |   20 % | m f r | 2000 |  13   |   2   | 4x3 / 3x5        |
|    H   | hobgoblin     |    0 % | m     |    3 |   3   |   1   | 1x8              |
|    I   | ice monster   |    0 % |       |    5 |   5   |   1   | 0x0              |
|    J   | jabberwock    |   70 % |       | 3000 |  15   |   6   | 2x12 / 2x4       |
|    K   | kestrel       |    0 % | m f   |    1 |   1   |   1   | 1x4              |
|    L   | leprechaun    |    0 % |       |   10 |   3   |   8   | 1x1              |
|    M   | medusa        |   40 % | m     |  200 |   8   |   2   | 3x4 / 3x4 / 2x5  |
|    N   | nymph         |  100 % | g     |   37 |   3   |   9   | 0x0              |
|    O   | orc           |   15 % | i     |    5 |   1   |   6   | 1x8              |
|    P   | phantom       |    0 % | m     |  120 |   8   |   3   | 4x4              |
|    Q   | quagga        |    0 % | m     |   15 |   3   |   3   | 1x5 / 1x5        |
|    R   | rattlesnake   |    0 % | m     |    9 |   2   |   3   | 1x6              |
|    S   | snake         |    0 % | m     |    2 |   1   |   5   | 1x3              |
|    T   | troll         |   50 % | m   r |  120 |   6   |   4   | 1x8 / 1x8 / 2x6  |
|    U   | black unicorn |    0 % | m     |    7 |   7   |  -2   | 1x9 / 1x9 / 2x9  |
|    V   | vampire       |   20 % | m   r |  350 |   8   |   1   | 1x10             |
|    W   | wraith        |    0 % |       |   55 |   5   |   4   | 1x6              |
|    X   | xeroc         |   30 % |       |  100 |   7   |   7   | 4x4              |
|    Y   | yeti          |   30 % |       |   50 |   4   |   6   | 1x6 / 1x6        |
|    Z   | zombie        |    0 % | m     |    6 |   2   |   8   | 1x8              |

Some monsters carry treasure that they drop when they are killed.  The "carry chance"
is the chance that the monster will drop treasure when it dies.

The "flags" are:

* m ==> is mean
* f ==> can fly
* r ==> able to regenerate
* g ==> is greedy (can take things)
* i ==> may start out as invisible

The "Exp" is the experience gained when you kill such a monster.

The smaller the monster’s armor class, the harder they are to hit.  Thus, a dragon (D) armor
class of -1 is the toughest to hit, whereas a nymph (N) armor class of 9 is the easiest.

The "HxD" in the above table indicates hit bonus (H), which improves the monster’s
ability to hit you, and damage bonus (D), which increases
the damage they cause when they hit you.  Monsters with multiple "HxD"
values indicate that the given monster has more than one method of attack.

The "venus flytrap" hit bonus (H) of "*++**" starts about 1 and increases each turn as you are being held.

Below level 29, all monsters are hasted (fast).

The monster levels go up by 1 for each level below level 26.


## potion spoilers ==> !

When you quaff a potion:

| index | chance | potion            | message                                                 |
|-------|--------|-------------------|---------------------------------------------------------|
|   0   |    7 % | confusion         | Wait, what's going on here. Huh? What? Who?             |
|   1   |    8 % | hallucination     | Oh, wow!  Everything seems so cosmic!                   |
|   2   |    8 % | poison            | You feel very sick now                                  |
|   3   |   13 % | gain strength     | You feel stronger, now.  What bulging muscles!          |
|   4   |    3 % | see invisible     | This potion tastes like ... juice                       |
|   5   |   13 % | healing           | You begin to feel better                                |
|   6   |    6 % | monster detection | ((you see the monitors on the current level))           |
|   7   |    6 % | magic detection   | You sense the presence of magic on this level           |
|   8   |    2 % | raise level       | You suddenly feel much more skillful                    |
|   9   |    5 % | extra healing     | You begin to feel much better                           |
|   a   |    5 % | haste self        | You feel yourself moving much faster                    |
|   b   |   13 % | restore strength  | Hey, this tastes great.  It make you feel warm all over |
|   c   |    5 % | blindness         | A cloak of darkness falls around you                    |
|   d   |    6 % | levitation        | You start to float in the air                           |

**HINT**: The rogue v5.4.5 code, as of 2026-02-23, has a **special
feature**.  When, after drinking the potion, you are prompted with
the question:

> What do you want to call it?

If you answer with a single question mark (i.e., "**?**"), the game will
name provide the correct name of the potion.  Thus, when you find another
potion of the same type, you will see the correct potion name.


## scroll spoilers ==> ?

When you read a scroll:

| index | freq. | scroll                       | message                                               |
|-------|-------|------------------------------|-------------------------------------------------------|
|   0   |   7 % | monster confusion            | Your hands begin to glow ...                          |
|   1   |   4 % | magic mapping                | Oh, now this scroll has a map on it                   |
|   2   |   2 % | hold monster                 | The monster(s around you) freeze(s)                   |
|       |       |                              |   -or-                                                |
|       |       |                              | You feel a strange sense of loss                      |
|   3   |   3 % | sleep                        | You fall asleep                                       |
|   4   |   7 % | enchant armor                | Your armor glows ... for a moment                     |
|       |       |                              |   -or-                                                |
|       |       |                              | ((nothing))                                           |
|   5   |  10 % | identify potion              | This scroll is an identify potion scroll              |
|   6   |  10 % | identify scroll              | This scroll is an identify scroll scroll              |
|   7   |   6 % | identify weapon              | This scroll is an identify weapon scroll              |
|   8   |   7 % | identify armor               | This scroll is an identify armor scroll               |
|   9   |  10 % | identify ring, wand or staff | This scroll is an identify ring, wand or staff scroll |
|   a   |   3 % | scare monster                | You hear maniacal laughter in the distance            |
|   b   |   2 % | food detection               | Your nose tingles and you smell food.                 |
|       |       |                              |   -or-                                                |
|       |       |                              | Your nose tingles                                     |
|   c   |   5 % | teleportation                | ((you jump to a randomly spot on the current level))  |
|   d   |   8 % | enchant weapon               | Your ((name of armor)) glows ... for a moment         |
|       |       |                              |   -or-                                                |
|       |       |                              | You feel a strange sense of loss                      |
|   e   |   4 % | create monster               | ((a monster appears next to you))                     |
|       |       |                              |   -or-                                                |
|       |       |                              | You hear a faint cry of anguish in the distance       |
|   f   |   7 % | remove curse                 | You feel as if somebody is watching over you          |
|       |       |                              |   -or-                                                |
|       |       |                              | You feel in touch with the Universal Onenes           |
|       |       |                              |   -or-                                                |
|       |       |                              | There is nothing on it to read                        |
|   g   |   3 % | aggravate monsters           | You hear a high pitched humming noise                 |
|   h   |   2 % | protect armor                | Your armor is covered by a shimmering ... shield      |
|       |       |                              |   -or-                                                |
|       |       |                              | You feel a strange sense of loss                      |

**HINT**: The rogue v5.4.5 code, as of 2026-02-23, has a **special
feature**.  When, after reading the scroll, you are prompted with
the question:

> What do you want to call it?

If you answer with a single question mark (i.e., "**?**"), the game will
name provide the correct name of the scroll.  Thus, when you find another
scroll of the same type, you will see the correct scroll name.


## weapon spoilers ==> )

When you have a weapon:

| index | freq. | wielded | thrown | weapon           |
|-------|-------|---------|--------|------------------|
|  n/a  |   n/a |   0x0   |   0x0  | ((no weapon))    |
|   0   |  11 % |   2x4   |   1x3  | mace             |
|   1   |  11 % |   3x4   |   1x2  | long sword       |
|   2   |  12 % |   1x1   |   1x1  | short bow        |
|   3   |  12 % |   1x1   |   2x3  | arrow (11)       |
|   4   |   8 % |   1x6   |   1x4  | dagger (4)       |
|   5   |  10 % |   4x4   |   1x2  | two handed sword |
|   6   |  12 % |   1x1   |   1x3  | dart (10)        |
|   7   |  12 % |   1x2   |   2x4  | shuriken (11)    |
|   8   |  12 % |   2x3   |   1x6  | spear            |

A weapon can be (+) enhanced, (n) normal, or (-) reduced in its effectiveness.

The "HxD" in the above table indicates hit bonus (H) and damage bonus (D).
For example, a wielded "long sword" has a "3x4" meaning it has a base
hit bonus (H) of 3 and a base damage bonus (D) of 4.  Other factors such
as strength (Str), weapon bonus or negative bonus, and chances (internal
dice roll) factor into odds to hit and damage inflicted.

Unlike other objects, (-) weapons are not cursed: they simply have
reduced effectiveness.

When you wield a different weapon, any previously wielded weapon
is automatically un-wielded and remains in your pack.

If you throw your current weapon, it will leave your pack, become
un-wielded if you were previously wielding it, possibly hit and do damage
to a monster, and often falls to the ground nearby unless there is no
free space nearby in which case the weapon may simply vanish.

If you are not wielding a weapon, you default to using your fists as a weapon.

To use a bow and arrow:

0) Wield, the w command, the bow

1) Throw an arrow, the t<dir> command, where <dir> is:

    h       left
    j       down
    k       up
    l       right
    y       up & left
    u       up & right
    b       down & left
    n       down & right

To use a dart, or shuriken:

0) Throw the item, the t<dir> command, where <dir> is noted above.


## armor spoilers ==> ]

When you have armor:

| index | freq. | protection | armor                  |
|-------|-------|------------|-------------------------
|   0   |  20 % |      2     | leather armor          |
|   1   |  15 % |      3     | ring mail              |
|   2   |  15 % |      3     | studded leather armor  |
|   3   |  13 % |      4     | scale mail             |
|   4   |  12 % |      5     | chain mail             |
|   5   |  10 % |      6     | splint mail            |
|   6   |  10 % |      6     | banded mail            |
|   7   |   5 % |      7     | plate mail             |

Armor can be (+) enhanced, (n) normal, or (-) cursed.

Cursed armor cannot be removed except by a scroll of "**remove curse**".
Once a scroll of "**remove curse**" has been used to remove the curse,
cursed armor may be put on or removed as needed; however, the armor will
still have a negative effect.

If you are already wearing armor, you have to take it off before you
can put on other armor.


## ring spoilers ==> =

What rings can do:

| index | freq. | food used | name              | description                                 |
|-------|-------|-----------|-------------------|---------------------------------------------|
|   0   |   9 % |     1     | protection        | Adds to defense and magical saving throws.  |
|   1   |   9 % |     1     | add strength      | Adds to strength.                           |
|   2   |   5 % |     1     | sustain strength  | Prevents poison and Rattlesnakes from       |
|       |       |           |                   | reducing strength.                          |
|   3   |  10 % |    -3     | searching         | Helps detect secret doors and traps.        |
|   4   |  10 % |    -5     | see invisible     | Reveals Phantoms.                           |
|   5   |   1 % |     0     | adornment         | Worth 10 gold.                              |
|   6   |  10 % |     0     | aggravate monster | Cursed. Causes monsters to attack more      |
|       |       |           |                   | aggressively.                               |
|   7   |   8 % |    -3     | dexterity         | Improves weapon accuracy.                   |
|   8   |   8 % |    -3     | increase damage   | Increases weapon damage.                    |
|   9   |   4 % |     2     | regeneration      | Heals 1 hp per turn.                        |
|   a   |   9 % |    -2     | slow digestion    | Reduces food consumption by about 50%. Two  |
|       |       |           |                   | rings can cancel out food consumption.      |
|   b   |   5 % |     0     | teleportation     | Cursed. Randomly teleports the player       |
|       |       |           |                   | around the map.                             |
|   c   |   7 % |     1     | stealth           | Allows player to move without rousing       |
|       |       |           |                   | sleeping monsters.                          |
|   d   |   5 % |     1     | maintain armor    | Prevents rust damage from Aquators.         |

A ring can be worn in the left or right hand.

A ring can be (+) enhanced, (n) normal, or (-) cursed.

A cursed ring cannot be removed except by a scroll of "**remove curse**".
Once a scroll of "**remove curse**" has been used to remove the curse,
a cursed ring may be put on or removed as needed; however, the ring will
still have a negative effect.

When **food used** is negative, the ring effectively adds food per turn when worn.


## staff spoilers ==> /

When you read use a staff:

| index | freq. | staff         | Description                                 |
|-------|-------|---------------|---------------------------------------------|
|   0   |  12 % | light         | Illuminates the room.                       |
|   1   |   6 % | invisibility  | Makes a monster invisible.                  |
|   2   |   3 % | lightning     | Inflicts 6d6 damage. Bounces off walls.     |
|   3   |   3 % | fire          | Inflicts 6d6 damage. Bounces off walls.     |
|   4   |   3 % | cold          | Inflicts 6d6 damage. Bounces off walls.     |
|   5   |  15 % | polymorph     | Transforms a monster into another.          |
|   6   |  10 % | magic missile | Inflicts 1d4 damage on a target.            |
|   7   |  10 % | haste monster | Hastens a monster.                          |
|   8   |  11 % | slow monster  | Slows a monster.                            |
|   9   |   9 % | drain life    | Drains half hero's hp.  Take same hp from   |
|       |       |               | all monsters in room or next to in passage. |
|   a   |   1 % | nothing       | A "/dev/null" staff.                        |
|   b   |   6 % | teleport away | Teleport a monster somewhere else.          |
|   c   |   6 % | teleport to   | Teleport a monster to near you.             |
|   d   |   5 % | cancellation  | Suppresses monster's special abilities.     |

## wizard mode spoilers

**IMPORTANT NOTE**: If you enter **wizard mode** at any time during the game, your score will not be posted to the scoreboard.

If you do not know the **wizard password**, you will need to do a little reading of the source code.
You need to, as the expression goes, [RTFS](https://en.wikipedia.org/wiki/RTFS).

**HINT**: **The historic wizard password**" may be found in a comment about a defined symbol.

You may toggle **wizard mode** on and off by pressing the "+" key.
When you successfully toggle **wizard mode** on, you will see the message:

> You are suddenly as smart as Ken Arnold in dungeon #...

You may also start rogue in **wizard mode** by giving an empty 2nd argument:

```sh
rogue ""
```

If you start start rogue in **wizard mode**, then the `$SEED` environment variable
may be used to set a specific dungeon number and pseudo-random number seed.
This will allow you to run specific dungeon, say for debugging purposes.
For example:

```sh
SEED=1227897957 rogue ""
```

**NOTE**: A seed is very likely portable across different systems that
implement the BSD `random(3)` facility, for rogue release **2026-02-24**
or later.

When you are in **wizard mode**, you have these special commands available to you:

| command | action                                                 |
|---------|--------------------------------------------------------|
|    +    | Toggle wizard mode                                     |
|    |    | Show location                                          |
|    C    | Create                                                 |
|    $    | Count items in pack                                    |
| ctrl G  | Inventory current level                                |
| ctrl W  | Identify item                                          |
| ctrl D  | Go down to next level                                  |
| ctrl A  | Go up to previous level                                |
| ctrl F  | Show map of current level                              |
| ctrl T  | Teleport to random spot on level                       |
| ctrl E  | Show amount of food left                               |
| ctrl Q  | Show passage ways                                      |
| ctrl X  | Toggle see monsters on level                           |
|    ~    | Super charge staff                                     |
| ctrl I  | +10 Exp, add +8 plate mail, add +1,+1 two handed sword |
|    *    | List types of an object                                |

For "\*" ("_List types of an object_"), when it asks:

> For what type of object do you want a list?

enter the symbol you want to create.  See the symbol table under the "To play rogue" above.

The "\*" ("_list types of an object_") command is only useful for:

* ! (potion)
* ? (scroll)
* ) (weapon)
* ] (armor)
* = (ring)
* / (staff)

In all other cases, the "\*" ("_list types of an object_") command prints nothing.

When the types of an object are listed, the percentages indicate how common or rare the given type of object is.
The numbers (and letters) at the beginning of the line are needed when you create such a type of object.

For "C" ("_create object_", when it asks:

> Type of item:

enter the symbol you want to create from the table of symbols listed above under "A few game hints".

For symbols that have sub-types (potions, scrolls, weapons, armor, rings, staffs), it will ask

> Which ... do you want? (0-...)

Where the range is depending on the number of sub-types.

Finally, when creating a weapon, armor or ring, it will ask:

> Blessing? (+,-,n)

Enter "+" for a enhanced item, "-" for a reduced item, or "n" for normal item.


# Reporting Security Issues

**IMPORTANT NOTE**: We do **NOT** support running `rogue(6)` with setuid and/or setgid privileges.

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/rogue5.4/security/policy)".
