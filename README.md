# rogue 5.4


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


## To use / to play

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

NOTE: restoring your make will remove the `~/.rogue.save` file.

For help in the game type "**?**" and then "**\***".


## Why yet another rouge repo?

This code is based on the "Rogue like restoration project"'s [rogue5.4](https://github.com/RoguelikeRestorationProject/rogue5.4).

This repo improves on the above mentioned repo in several important aspects:

* improved the C source to be able to compile under recent C compilers
* fixed several bugs in the rogue code
* fixed the code to compile both the `findpw(6)` and `scedit(6)` rouge tools
* moved the rogue source down into `src/` sub-directory
* created a top level `Makefile` wrapper to manage the `src/configure` options
* install using the common `install(1)` utility (instead of using src/install-sh`)
* install the `findpw(6)` and `scedit(6)` rouge tools
* provided a easy set of make files in case you wish to avoid the **Autoconf** complexity
* fixed `make stddocs` so that the proper configuration values are configured into the documentation
* changed the rogue save filename to `~/.rogue.save` (`.rogue.save` under your home directory)
* etc.


## Avoiding configure and autoconf(1)

The upper level `Makefile` will, by default, attempt to use `src/configure`, you have `autoconf(1)` installed.
The upper level `Makefile` will attempt to run `src/configure` with a reasonable set of options that
have a change to work, assuming that [Gnu Autoconf](https://www.gnu.org/software/autoconf/) in installed.

Some people prefer to not bother with the **Autoconf** faciluty and running `configure`.

>  For more information on  **Autoconf** see [Gnu Autoconf](https://www.gnu.org/software/autoconf/)

Should the use of `src/configure` fail, you may wish to try our easy make rules:

* make easy-configure

This rule simply moves into place, some reasonable estimations of what `src/configure` tried to produce.

You may have to edit `src/config.h` and/or `src/Makefile` to suit your needs afterwards

* make easy-all

This rule invokes the all rule from the `src/Makefile.easy` makefile.

Now that you have both `src/config.h` and `src/Makefile`, use the
`src/Makefile.easy` makefile to compile the code.

Chances are, the above should work.  However should something go wrong,
you have have to edit `src/config.h` and/or `src/Makefile` or perhaps
even modify the code.  If do have to modify stuff, please consider sharing by opening up a
[GitHub Pull Request](https://github.com/lcn2/rogue5.4/pulls).

* sudo make easy-install easy-clobber

Use the install rule in the `src/Makefile.easy` makefile to install `rogue(6)`,
the `findpw(6)` and `scedit(6)` rouge tools, rogue docuentation, and `rogue(6)` man page.

The addition of easy-clobber on the sudo line will "clean up" the source tree after installing.



# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/rogue5.4/security/policy)".
