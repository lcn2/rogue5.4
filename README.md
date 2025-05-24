# rogue 5.4

Rogue: Exploring the Dungeons of Doom version 5.4 - bug fixed ported to modern C and Unix-like systems


## TL;DR - to install

```sh
make
sudo make install
```

If `configure` fails and/or if you do not have/want the **Autoconf** facility, try the easy way to install:

```sh
make easy-clobber easy-all
sudo make easy-install
```


### Dependencies

* Modern C compiler (perhaps c11 or better)
* Modern `make(1)` (recommend [GNU make](https://www.gnu.org/software/make/))
* [Ncurses](https://invisible-island.net/ncurses/announce.html) (<ncurses.h> and libncurses)
* [Single UNIX Specification](https://pubs.opengroup.org/onlinepubs/9799919799/) confirming (or reasonably conforming) operating system such as Linux, macOS, FreeBSD, etc.
* [GNU Autoconf](https://www.gnu.org/software/autoconf/) (this is optional: see "Configure and autoconf" section below)


### Bug reports and Pull Requests welcome

We very much welcome fork [rogue5.4 pull requests](https://github.com/lcn2/rogue5.4/pulls) to fix any:

* failure to compile
* compiler warning messages
* program crashes

You may also file a [rogue5.4 bug report](https://github.com/lcn2/rogue5.4/issues/new?template=bug_report.md).


## To play rogue

After installing:

```
/usr/local/bin/rogue
```

To quit, interrupt the program (type control-C), answer "y" to the question and press return.

Your score file is store in the `.rogue.scr` file in your current directory (you can change this via
modifying how the upper `Makefile` runs `src/configure`).

To save your rogue game, press "S" and save.

To restore your make, run:

```sh
/usr/local/bin/rogue ~/.rogue.save
```

**NOTE**: restoring your make will remove the `~/.rogue.save` file.

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

```
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
```

For more information about the make, see the `rogue(6)` man page after installing, or run the command
from the top level directory:

```sh
man src/rogue.6
```

**GOAL**: As the man page states:

> To win the game (as opposed to merely playing to beat other people's high scores) you must
> locate the Amulet of Yendor which is somewhere below the 20th level of the dungeon and get
> it out.

**BTW**: The `rogue(6)` man page states:

> Nobody has achieved this yet and if somebody does, they will probably go down in
> history as a hero among heroes.

The above statement is hyperbole and not intended to imply that nobody
has achieved the goal.  The maintainer of this repo has completed the goal
(won) at least 3 times (without using **wizard mode**) and reached a
maximum dungeon depth (and successful return to level 0) of 31.


## Why yet another rouge repo?

This code is based on the "Rogue like restoration project"'s [rogue5.4](https://github.com/RoguelikeRestorationProject/rogue5.4).

This repo improves on the above mentioned repo in several important aspects:

* Improved the C source to be able to compile under recent C compilers
* Fixed several bugs in the rogue code
* Fixed the code to compile both the `findpw(6)` and `scedit(6)` rouge tools
* Moved the rogue source down into `src/` sub-directory
* Created a top level `Makefile` wrapper to manage the `src/configure` options
* Install using the common `install(1)` utility (instead of using src/install-sh`)
* Install the `findpw(6)` and `scedit(6)` rouge tools
* Provided a easy set of make files in case you wish to avoid the **Autoconf** complexity
* Fixed `make stddocs` so that the proper configuration values are configured into the documentation
* Changed the rogue save filename to `~/.rogue.save` (`.rogue.save` under your home directory)
* Fixed cases where creating invalid type of an item in **wizard mode** crashed the game
* Creating an item in **wizard mode** that does not have a sub-type no longer asks which type of item
* Added extensive SPOILER section notes in the lower part of this `README.md` file
* Fixed several buffer overflow bugs and memory leak conditions.
* etc.


## Rogue5.4 repo backstory

This [rogue5.4 GitHub repo](https://github.com/lcn2/rogue5.4)
was cloned from [RoguelikeRestorationProject's rogue5.4 repo](https://github.com/RoguelikeRestorationProject/rogue5.4).
[Landon Curt Noll](https://github.com/lcn2), who fondly remembers playing `rogue(1)` onn the Vax running BSD, wanted to
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


## Configure and autoconf(1)

Some people prefer to not bother with the **Autoconf** facility and running `configure`.

>  For more information on  **Autoconf** see [Gnu Autoconf](https://www.gnu.org/software/autoconf/)

The upper level `Makefile` will, by default, attempt to use `src/configure`, you have `autoconf(1)` installed.
The upper level `Makefile` will attempt to run `src/configure` with a reasonable set of options that
have a change to work, assuming that [Gnu Autoconf](https://www.gnu.org/software/autoconf/) in installed.

Should the use of `src/configure` fail, you may wish to try our easy make rules:

* make easy-configure

This rule simply moves into place, some reasonable estimations of what `src/configure` tried to produce.

You may have to edit `src/config.h` and/or `src/Makefile` to suit your needs afterwards

* make easy-all

This rule invokes the all rule from the `src/Makefile.easy` makefile.

Now that you have both `src/config.h` and `src/Makefile`, use the
`src/Makefile.easy` makefile to compile the code.

**IMPORTANT NOTE**: When bypassing [Gnu Autoconf](https://www.gnu.org/software/autoconf/), you no longer
benefit from the **Autoconf** facility ability to configure the code for your system.
Chances are, the above should work.  However should something go wrong,
you have have to edit `src/config.h` and/or `src/Makefile` or perhaps
even modify the code.  If do have to modify stuff, please consider sharing by opening up a
[GitHub Pull Request](https://github.com/lcn2/rogue5.4/pulls).

* sudo make easy-install easy-clobber

Use the install rule in the `src/Makefile.easy` makefile to install `rogue(6)`,
the `findpw(6)` and `scedit(6)` rouge tools, rogue documentation, and `rogue(6)` man page.

The addition of easy-clobber on the sudo line will "clean up" the source tree after installing.


# Skip over the SPOILERS to the very bottom for Reporting Security Issues


# WARNING - SPOILERS BELOW !!!


## Spoilers


The **Amulet of Yendor** is on level 26.  Look for a "**,**"" (comma) in a room on that level.
Once you have the **Amulet of Yendor**, you are allowed to go up a staircase (use the "**<**" command).

You may try your hand at going below level 26, however the deeper you go,
the harder to becomes to survive.  For example, see notes under "**monster spoilers**".
Nevertheless, going deeper than level 26 is seen as a daring bonus, especially if
you survive and return to the surface (level 0).

If you do not pick up the **Amulet of Yendor** on level 26 and you go deeper than level 26,
the game will place the **Amulet of Yendor** the deeper levels until you manage to pick it up.


## monster spoilers ==> A-Z

The following table lists the known monsters in the dungeon and their properties:

```
    |        |    monster    | carry  |       |      |  min  | armor |
    | symbol |     name      | chance | flags |  Exp | level | class | damage           |
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
```

Some monsters carry treasure that they drop when they are killed.  The "carry chance"
is the chance that the monster will drop treasure when they die.

The "flags" are:

* m ==> is mean
* f ==> can fly
* r ==> able to regenerate
* g ==> is greedy (can take things)
* i ==> may start out as invisible

The "Exp" is the experience gained when you kill such a monster.

The smaller the monster armor class of harder they are to hit.  Thus a dragon (D) armor
class of -1 is the toughest to hit, whereas nymph (N) armor class of 9 is the easiest.

The "HxD" in the above table indicates hit bonus (H) which improves the
ability of the monster to hit you, and damage bonus (D) which increases
the damage they cause when they hit you.  Monsters with multiple "HxD"
values indicate that the given monster has more than one method of attack.

The "venus flytrap" hit bonus (H) of "*++**" starts about 1 and increases each turn as you are being held.

Below level 29, all monsters are hasted (fast).

The monster levels goes up by 1 for each level below level 26.


## potion spoilers ==> !

When you quaff a potion:

```
    | letter | frequency | potion            | message                                                 |
    |--------|-----------|-------------------|---------------------------------------------------------|
    |    0   |    7 %    | confusion         | Wait, what's going on here. Huh? What? Who?             |
    |    1   |    8 %    | hallucination     | Oh, wow!  Everything seems so cosmic!                   |
    |    2   |    8 %    | poison            | You feel very sick now                                  |
    |    3   |   13 %    | gain strength     | You feel stronger, now.  What bulging muscles!          |
    |    4   |    3 %    | see invisible     | This potion tastes like ... juice                       |
    |    5   |   13 %    | healing           | You begin to feel better                                |
    |    6   |    6 %    | monster detection | ((you see the monitors on the current level))           |
    |    7   |    6 %    | magic detection   | You sense the presence of magic on this level           |
    |    8   |    2 %    | raise level       | You suddenly feel much more skillful                    |
    |    9   |    5 %    | extra healing     | You begin to feel much better                           |
    |    a   |    5 %    | haste self        | You feel yourself moving much faster                    |
    |    b   |   13 %    | restore strength  | Hey, this tastes great.  It make you feel warm all over |
    |    c   |    5 %    | blindness         | A cloak of darkness falls around you                    |
    |    d   |    6 %    | levitation        | You start to float in the air                           |
```


## scroll spoilers ==> ?

When you read a scroll:

```
    | letter | frequency | scroll                       | message                                               |
    |--------|-----------|------------------------------|-------------------------------------------------------|
    |    0   |    7 %    | monster confusion            | Your hands begin to glow ...                          |
    |    1   |    4 %    | magic mapping                | Oh, now this scroll has a map on it                   |
    |    2   |    2 %    | hold monster                 | The monster(s around you) freeze(s)                   |
    |        |           |                              |   -or-                                                |
    |        |           |                              | You feel a strange sense of loss                      |
    |    3   |    3 %    | sleep                        | You fall asleep                                       |
    |    4   |    7 %    | enchant armor                | Your armor glows ... for a moment                     |
    |        |           |                              |   -or-                                                |
    |        |           |                              | ((nothing))                                           |
    |    5   |   10 %    | identify potion              | This scroll is an identify potion scroll              |
    |    6   |   10 %    | identify scroll              | This scroll is an identify scroll scroll              |
    |    7   |    6 %    | identify weapon              | This scroll is an identify weapon scroll              |
    |    8   |    7 %    | identify armor               | This scroll is an identify armor scroll               |
    |    9   |   10 %    | identify ring, wand or staff | This scroll is an identify ring, wand or staff scroll |
    |    a   |    3 %    | scare monster                | You hear maniacal laughter in the distance            |
    |    b   |    2 %    | food detection               | Your nose tingles and you smell food.                 |
    |        |           |                              |   -or-                                                |
    |        |           |                              | Your nose tingles                                     |
    |    c   |    5 %    | teleportation                | ((you jump to a randomly spot on the current level))  |
    |    d   |    8 %    | enchant weapon               | Your ((name of armor)) glows ... for a moment         |
    |        |           |                              |   -or-                                                |
    |        |           |                              | You feel a strange sense of loss                      |
    |    e   |    4 %    | create monster               | ((a monster appears next to you))                     |
    |        |           |                              |   -or-                                                |
    |        |           |                              | You hear a faint cry of anguish in the distance       |
    |    f   |    7 %    | remove curse                 | You feel as if somebody is watching over you          |
    |        |           |                              |   -or-                                                |
    |        |           |                              | You feel in touch with the Universal Onenes           |
    |        |           |                              |   -or-                                                |
    |        |           |                              | There is nothing on it to read                        |
    |    g   |    3 %    | aggravate monsters           | You hear a high pitched humming noise                 |
    |    h   |    2 %    | protect armor                | Your armor is covered by a shimmering ... shield      |
    |        |           |                              |   -or-                                                |
    |        |           |                              | You feel a strange sense of loss                      |
```


## weapon spoilers ==> )

When you have a weapon:

```
    |        |           | damage  | damage |                  |
    | letter | frequency | wielded | thrown | weapon           |
    |--------|-----------|---------|--------|------------------|
    |   n/a  |    n/a    |   0x0   |   0x0  | ((no weapon))    |
    |    0   |   11 %    |   2x4   |   1x3  | mace             |
    |    1   |   11 %    |   3x4   |   1x2  | long sword       |
    |    2   |   12 %    |   1x1   |   1x1  | short bow        |
    |    3   |   12 %    |   1x1   |   2x3  | arrow (11)       |
    |    4   |    8 %    |   1x6   |   1x4  | dagger (4)       |
    |    5   |   10 %    |   4x4   |   1x2  | two handed sword |
    |    6   |   12 %    |   1x1   |   1x3  | dart (10)        |
    |    7   |   12 %    |   1x2   |   2x4  | shuriken (11)    |
    |    8   |   12 %    |   2x3   |   1x6  | spear            |
```

A weapon can be (+) enhanced, (n) normal, or (-) reduced in its effectiveness.

The "HxD" in the above table indicates hit bonus (H) and damage bonus (D).
For example, a wielded "long sword" has a "3x4" meaning if has a base
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


## armor spoilers ==> ]

When you have armor:

```
    | letter | frequency | protection | armor                  |
    |--------|-----------|------------|-------------------------
    |    0   |    20 %   |      2     | leather armor          |
    |    1   |    15 %   |      3     | ring mail              |
    |    2   |    15 %   |      3     | studded leather armor  |
    |    3   |    13 %   |      4     | scale mail             |
    |    4   |    12 %   |      5     | chain mail             |
    |    5   |    10 %   |      6     | splint mail            |
    |    6   |    10 %   |      6     | banded mail            |
    |    7   |     5 %   |      7     | plate mail             |
```

Armor can be (+) enhanced, (n) normal, or (-) cursed.

Cursed armor cannot be removed except by a scroll of "**remove curse**".
Once a scroll of "**remove curse**" has been used to remove the curse,
cursed armor may be put on or removed as needed, however the armor will
still have a negative effect.

If you are already wearing armor, you have to take it off before you
can put on other armor.


## ring spoilers ==> =

```
    | letter | frequency | food used | ring              |
    |--------|-----------|-----------|--------------------
    |    0   |     9 %   |     1     | protection        |
    |    1   |     9 %   |     1     | add strength      |
    |    2   |     5 %   |     1     | sustain strength  |
    |    3   |    10 %   |    -3     | searching         |
    |    4   |    10 %   |    -5     | see invisible     |
    |    5   |     1 %   |     0     | adornment         |
    |    6   |    10 %   |     0     | aggravate monster |
    |    7   |     8 %   |    -3     | dexterity         |
    |    8   |     8 %   |    -3     | increase damage   |
    |    9   |     4 %   |     2     | regeneration      |
    |    a   |     9 %   |    -2     | slow digestion    |
    |    b   |     5 %   |     0     | teleportation     |
    |    c   |     7 %   |     1     | stealth           |
    |    d   |     5 %   |     1     | maintain armor    |
```

A ring can be worn in the Left or Right hand.

A ring can be (+) enhanced, (n) normal, or (-) cursed.

A cursed ring cannot be removed except by a scroll of "**remove curse**".
Once a scroll of "**remove curse**" has been used to remove the curse,
a cursed ring may be put on or removed as needed, however the ring will
still have a negative effect.

When **food used** is negative, the ring effectively adds food per turn when worn.


## staff spoilers ==> /

When you read use a staff:

```
    | letter | frequency | staff         |
    |--------|-----------|---------------|
    |    0   |   12 %    | light         |
    |    1   |    6 %    | invisibility  |
    |    2   |    3 %    | lightning     |
    |    3   |    3 %    | fire          |
    |    4   |    3 %    | cold          |
    |    5   |   15 %    | polymorph     |
    |    6   |   10 %    | magic missile |
    |    7   |   10 %    | haste monster |
    |    8   |   11 %    | slow monster  |
    |    9   |    9 %    | drain life    |
    |    a   |    1 %    | nothing       |
    |    b   |    6 %    | teleport away |
    |    c   |    6 %    | teleport to   |
    |    d   |    5 %    | cancellation  |
```


## wizard mode spoilers

If you do not know the **wizard password**, you will need to do a little reading in the source code.
You need to, as the expression goes, [RTFS](https://en.wikipedia.org/wiki/RTFS).

**HINT**: "**The historic wizard password is**" found in comment about a defined symbol.

You may toggle **wizard mode** on and off by pressing the "+" key.
When successfully toggle **wizard mode** on, you will see the message:

> You are suddenly as smart as Ken Arnold in dungeon #...

You may also start rogue in **wizard mode** by giving an empty 2nd argument:

```sh
rogue ""
```

When you are in **wizard mode**, you have these special commands available to you:

```
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
    | ctrl E  | Show about of food left                                |
    | ctrl Q  | Show passage ways                                      |
    | ctrl X  | Toggle see monsters on level                           |
    |    ~    | Super charge staff                                     |
    | ctrl I  | +10 Exp, add +8 plate mail, add +1,+1 two handed sword |
    |    *    | List types of an object                                |
```

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

In all other cases the "\*" ("_list types of an object_") command prints nothing.

When the types of an object are listed, the percentages indicate how common or rare is the given type of object.
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

**IMPORTANT NOTE**: If you enter **wizard mode** at any time during the game, your score will not be posted to the scoreboard.


# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/rogue5.4/security/policy)".
