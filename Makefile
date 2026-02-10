#!/usr/bin/env make
#
# rogue5.4 - rogue version 5.4
#
# The following Copyright ONLY applies to this Makefile and README.md files.
#
# Copyright (c) 2025-2026 by Landon Curt Noll.  All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo (Landon Curt Noll) /\oo/\
#
# http://www.isthe.com/chongo/index.html
# https://github.com/lcn2
#
# Share and enjoy!  :-)


#############
# utilities #
#############

CC= cc
CHGRP= chgrp
CHMOD= chmod
CMP= cmp
COLCRT= colcrt
CP= cp
CTAGS= ctags
GREP= grep
GROFF= groff
ID= id
INSTALL= install
MAKEDEPEND= makedepend
MKDIR= mkdir
MV= mv
NROFF= nroff
RM= rm
RMDIR= rmdir
SED= sed
SHELL= bash
SORT= sort
TBL= tbl
TOUCH= touch
TRUE= true


#################
# configuration #
#################

DISTNAME= rogue5.4.5
PACKAGE_TARNAME= rogue-5.4.5
PROGRAM= rogue

# These files are stored under the home directory
#
LOCKFILE_BASENAME= .rogue.lck
SAVEFILE_BASENAME= .rogue.save
SCOREFILE_BASENAME= .rogue.scr

PREFIX= /usr/local
DESTDIR= ${PREFIX}/bin
DESTDOC= ${PREFIX}/share/rogue5.4
MAN6DIR= ${PREFIX}/man/man6


#####################
# debug information #
#####################

# V=@:  do not echo debug statements (quiet mode)
# V=@   echo debug statements (debug / verbose mode)
#
V=@:
#V=@

# Q=@	do not echo internal Makefile actions (quiet mode)
# Q=	echo internal Makefile actions (debug / verbose mode)
#
#Q=
Q=@


############################
# how to build and install #
############################

CSTD= -std=gnu17
CCWARN=
CPPFLAGS= -DHAVE_CONFIG_H -DALLSCORE -DLOCKNAME='"${LOCKFILE_BASENAME}"' \
	  -DSAVENAME='"${SAVEFILE_BASENAME}"' -DSCORENAME='"${SCOREFILE_BASENAME}"'
COPT= -O3
DEBUG= -ggdb3

CFLAGS= ${CSTD} ${CCWARN} ${CPPFLAGS} ${COPT} ${DEBUG}

LDFLAGS=

LC_CTYPE= C

AT= @

#LIBS= -lcurses
LIBS=  -lncurses

TARGETS= rogue findpw scedit


#############
# ownership #
#############

# We recommend that you leave ${GROUPOWNER}, $(INSTGROUP}, and ${INSTOWNER} empty.
#
# The program now defaults to using lock, save, and score files under the home directory,
# one does NOT need to set the user and group of the program, nor of the lock, save, and score files.

#GROUPOWNER= games
GROUPOWNER=

#INSTGROUP= -g rogue
INSTGROUP=

#INSTOWNER= -u rogue
INSTOWNER=


################################
# source and destination files #
################################

H_SRC= config.h rogue.h extern.h score.h

OBJS1= vers.o extern.o armor.o chase.o command.o common.o \
           daemon.o daemons.o fight.o init.o io.o list.o \
           mach_dep.o mdport.o misc.o monsters.o \
           move.o new_level.o

OBJS2= options.o pack.o passages.o potions.o rings.o \
           rip.o rooms.o save.o scrolls.o state.o sticks.o \
           things.o weapons.o wizard.o xcrypt.o

OBJS= ${OBJS1} ${OBJS2}

CFILES= vers.c extern.c armor.c chase.c command.c daemon.c \
           daemons.c fight.c init.c io.c list.c mach_dep.c \
           main.c  mdport.c misc.c monsters.c move.c new_level.c \
           options.c pack.c passages.c potions.c rings.c rip.c \
           rooms.c save.c scrolls.c state.c sticks.c things.c \
           weapons.c wizard.c xcrypt.c

MISC_C= findpw.c scedit.c scmisc.c

C_SRC= ${CFILES} ${MISC_C} 

SRC= ${C_SRC} ${H_SRC}

DOCSRC= rogue.me.in rogue.6.in rogue.doc.in rogue.html.in rogue.cat.in rogue.md.in

DOCS= rogue.doc rogue.html rogue.cat rogue.me rogue.6 rogue.md

AFILES= rogue.6.in rogue.cat.in rogue.doc.in rogue.html.in rogue.md.in rogue.me.in

MISC= LICENSE rogue.spec rogue.png

MAKE_FILE= Makefile


###################################
# Common Address Sanitizer (ASAN) #
###################################

# Common Address Sanitizer (ASAN)
#
# For more info see: https://github.com/google/sanitizers/wiki/AddressSanitizer
# See also: https://developer.apple.com/documentation/xcode/diagnosing-memory-thread-and-crash-issues-early
# And also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
#
# The following Address Sanitizer (ASAN) are common to both RHEL9.7 (Linux) and macOS 26.2.
#
# By default, the Address Sanitizer is NOT enabled, not compiled into calc.
# To enable the Address Sanitizer, uncomment the appropriate lines in Makefile.local !!!
#
FSANITIZE:= -Wno-invalid-command-line-argument
FSANITIZE+= -fno-common
FSANITIZE+= -fno-omit-frame-pointer
FSANITIZE+= -fsanitize=address
FSANITIZE+= -fsanitize=alignment
FSANITIZE+= -fsanitize=bool
FSANITIZE+= -fsanitize=bounds
FSANITIZE+= -fsanitize=enum
FSANITIZE+= -fsanitize=float-cast-overflow
FSANITIZE+= -fsanitize=float-divide-by-zero
FSANITIZE+= -fsanitize=integer-divide-by-zero
FSANITIZE+= -fsanitize=nonnull-attribute
FSANITIZE+= -fsanitize=null
FSANITIZE+= -fsanitize=object-size
FSANITIZE+= -fsanitize=returns-nonnull-attribute
FSANITIZE+= -fsanitize=shift
FSANITIZE+= -fsanitize=signed-integer-overflow
FSANITIZE+= -fsanitize=undefined
FSANITIZE+= -fsanitize=unreachable
FSANITIZE+= -fsanitize=vla-bound
FSANITIZE+= -fsanitize=vptr

####
# macOS Address Sanitizer (ASAN)
#
# This comment block was tested under:
#
#       macOS 26.2 with Apple clang version 17.0.0 (clang-1700.6.3.2)
#
#       See: https://releases.llvm.org/17.0.1/tools/clang/docs/AddressSanitizer.html
#
# See the FSANITIZE comment block in Makefile.config for common FSANITIZE values and more info.
#
# To use the Address Sanitizer, uncomment this set set of lines and recompile (make clobber all):
#
# For more info see: https://github.com/google/sanitizers/wiki/AddressSanitizer
# See also: https://developer.apple.com/documentation/xcode/diagnosing-memory-thread-and-crash-issues-early
# And also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
#
# We recommend, for ease of debugging, building a static calc ("BLD_TYPE= calc-static-only").
#
# FSANITIZE+= -fsanitize=nullability-arg
# FSANITIZE+= -fsanitize=nullability-assign
# FSANITIZE+= -fsanitize=nullability-return
# CCWARN+= -Wall
# CCWARN+= -pedantic
# CCWARN+= -Werror
# CFLAGS+= ${FSANITIZE} -fstack-protector-all
# LDFLAGS+= ${FSANITIZE}
# COPT:= -O0
# DEBUG:= -ggdb3
####

####
# RHEL (Linux) Address Sanitizer (ASAN)
#
# This comment block was tested under:
#
#       RHEL9.7 with clang version 20.1.8 (Red Hat, Inc. 20.1.8-3.el9)
#
# with these RPMs installed:
#
#       libasan-11.5.0-11.el9.x86_64
#       libubsan-11.5.0-11.el9.x86_64
#
# See the FSANITIZE comment block in Makefile.config for common FSANITIZE values and more info.
#
# To use the Address Sanitizer, uncomment this set set of lines and recompile (make clobber all):
#
# For more info see: https://github.com/google/sanitizers/wiki/AddressSanitizer
# And also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
#
# We recommend, for ease of debugging, building a static calc ("BLD_TYPE= calc-static-only").
#
# Be sure you have the following dnf packages installed:
#
#   dnf install readline-devel ncurses-libs ncurses-devel libasan libubsan
#
# CCWARN+= -Wall
# CCWARN+= -pedantic
# CCWARN+= -Werror
# CFLAGS+= ${FSANITIZE} -fstack-protector-all
# LDFLAGS+= ${FSANITIZE}
# COPT:= -O0
# DEBUG:= -ggdb3
####


######################################
# all - default rule - must be first #
######################################

all: ${TARGETS} stddocs


##################
# compile C code #
##################

.c.o:
	${CC} ${CFLAGS} -c $*.c

rogue: ${H_SRC} ${OBJS} main.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} main.o ${LIBS} -o $@

findpw: ${OBJS} findpw.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} findpw.o ${LIBS} -o $@

scedit: ${OBJS} scmisc.o scedit.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} scmisc.o scedit.o ${LIBS} -o $@


############################
# form rogue documentation #
############################

stddocs: rogue.6 rogue.me rogue.html rogue.doc rogue.cat rogue.md

rogue.6: rogue.6.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			  -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			  rogue.6.in > rogue.6

rogue.me: rogue.me.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			  -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			  rogue.me.in > rogue.me

rogue.html: rogue.html.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			  -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			  rogue.html.in > rogue.html

rogue.doc: rogue.me rogue.doc.in
	${RM} -f $@
	@-if test "x${GROFF}" != "x" -a "x${SED}" != "x" ; then \
	    echo "${GROFF} -P-c -t -me -Tascii rogue.me | LC_CTYPE=C ${SED} -e 's/.\x08//g' > $@" ; \
	    ${GROFF} -P-c -t -me -Tascii rogue.me | LC_CTYPE=C ${SED} -e 's/.\x08//g' > $@ ; \
	elif test "x${NROFF}" != "x" -a "x${TBL}" != "x" -a "x${COLCRT}" != "x" ; then \
	    echo "${TBL} rogue.me | ${NROFF} -me | colcrt - > $@" ; \
	    ${TBL} rogue.me | ${NROFF} -me | colcrt - > $@ ; \
        else \
	    echo "LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			      -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			      rogue.doc.in > $@" ; \
	    LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			      -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			      rogue.doc.in > $@ ; \
	fi

rogue.cat: rogue.6 rogue.cat.in
	${RM} -f $@
	@-if test "x${GROFF}" != "x" -a "x${SED}" != "x" ; then \
	    echo "${GROFF} -Tascii -man rogue.6 | LC_CTYPE=C ${SED} -e 's/.\x08//g' > $@" ; \
	    ${GROFF} -Tascii -man rogue.6 | LC_CTYPE=C ${SED} -e 's/.\x08//g' > $@ ; \
	elif test "x${NROFF}" != "x" -a "x${TBL}" != "x" -a "x${COLCRT}" != "x" ; then \
	    echo "${NROFF} -man rogue.6 | ${COLCRT} - > $@" ; \
	    ${NROFF} -man rogue.6 | ${COLCRT} - > $@ ; \
        else \
	    echo "LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			      -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			      rogue.cat.in > $@" ; \
	    LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			      -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			      rogue.cat.in > $@ ; \
	fi

rogue.md: rogue.md.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};${PROGRAM};' \
			  -e 's;${AT}LOCKFILE${AT};~/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};~/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};~/${SCOREFILE_BASENAME};' \
			  rogue.md.in > rogue.md


#################################################
# .PHONY list of rules that do not create files #
#################################################

.PHONY: all stddocs tags clean clobber distclean install uninstall reinstall depend \
	easy-all configure easy-configure post-configure easy-clean easy-clobber easy-install \
	easy-uninstall easy-reinstall dist dist.src


###################################
# standard Makefile utility rules #
###################################

tags: ${CFILES} ${MISC_C}
	${CTAGS} -w ${CFILES} ${MISC_C}

clean:
	${RM} -f ${OBJS1}
	${RM} -f ${OBJS2}
	${RM} -f main.o findpw.o scedit.o scmisc.o

clobber: clean
	${RM} -f empty tags
	${RM} -f rogue.6 rogue.me rogue.html rogue.doc rogue.cat rogue.md
	${RM} -f ${TARGETS}
	${RM} -rf skel

distclean maintainer-clean: clobber

install: all
	${RM} -f empty
	${TOUCH} empty
	-@if test ! -f ~/${SCOREFILE_BASENAME} ; then \
	    echo "${INSTALL} -m 0664 empty ~/${SCOREFILE_BASENAME}" ; \
	    ${INSTALL} -m 0664 empty ~/${SCOREFILE_BASENAME} ; \
	fi
	-${INSTALL} -d -m 0755 ${DESTDIR}
	-${INSTALL} -m 0755 ${PROGRAM} ${DESTDIR}/${PROGRAM}
	-@if [[ -n "${GROUPOWNER}" ]]; then \
	    echo "${CHGRP} ${GROUPOWNER} ~/${SCOREFILE_BASENAME}" ; \
	    ${CHGRP} ${GROUPOWNER} ~/${SCOREFILE_BASENAME} ; \
	    echo "${CHGRP} ${GROUPOWNER} ~/${PROGRAM}" ; \
	    ${CHGRP} ${GROUPOWNER} ~/${PROGRAM} ; \
	    echo "${CHMOD} 02755 ~/${PROGRAM}" ; \
	    ${CHMOD} 02755 ~/${PROGRAM} ; \
	    echo "${CHMOD} 0464 $/${SCOREFILE_BASENAME}" ; \
	    ${CHMOD} 0464 ~/${SCOREFILE_BASENAME} ; \
         fi
	-${INSTALL} -m 0755 findpw ${DESTDIR}/findpw
	-${INSTALL} -m 0755 scedit ${DESTDIR}/scedit
	-${INSTALL} -d -m 0755 ${MAN6DIR}
	-${INSTALL} -m 0644 rogue.6 ${MAN6DIR}/${PROGRAM}.6
	-${INSTALL} -d -m 0755 ${DESTDOC}
	-${INSTALL} -m 0644 rogue.doc ${DESTDOC}/${PROGRAM}.doc
	-${INSTALL} -m 0644 rogue.html ${DESTDOC}/${PROGRAM}.html
	-${INSTALL} -m 0644 rogue.cat ${DESTDOC}/${PROGRAM}.cat
	-${INSTALL} -m 0644 LICENSE ${DESTDOC}/LICENSE.TXT
	-${INSTALL} -m 0644 rogue.me ${DESTDOC}/${PROGRAM}.me
	-${INSTALL} -m 0644 rogue.md ${DESTDOC}/${PROGRAM}.md
	-@if test ! -f ~/${LOCKFILE_BASENAME}; then \
	    echo "${INSTALL} -m 0666 empty ~/${LOCKFILE_BASENAME}" ; \
	    ${INSTALL} -m 0666 empty ~/${LOCKFILE_BASENAME} ; \
	    echo "${RM} -f ~/${LOCKFILE_BASENAME}" ; \
	    ${RM} -f ~/${LOCKFILE_BASENAME} ; \
	fi
	${RM} -f empty

uninstall:
	-${RM} -f ${DESTDIR}/${PROGRAM}
	-${RM} -f ${MAN6DIR}/${PROGRAM}.6
	-${RM} -f ${DESTDOC}/${PROGRAM}.doc
	-${RM} -f ${DESTDOC}/${PROGRAM}.html
	-${RM} -f ${DESTDOC}/${PROGRAM}.cat
	-${RM} -f ${DESTDOC}/LICENSE.TXT
	-${RM} -f ${DESTDOC}/${PROGRAM}.me
	-${RM} -f ~/${LOCKFILE_BASENAME}
	-${RMDIR} ${DESTDOC}

reinstall: uninstall install


###############################
# easy backward compatibility #
###############################

# These rules were used when rogue was under autoconf, and
# the source was under the src sub-directory.
#
# We provide these stub rules for backward compatibility now that autoconf is NOT used, and
# the source was moved back to the top level.

# backward compatibility
#
easy-all: all
	@:

configure:
	@echo GNU autoconf is not longer used
	@echo Just edit Makefile and/or config.h as needed

easy-configure: configure
	@:

post-configure:
	@:

easy-clean: clean
	@:

easy-clobber: clobber
	@:

easy-install: install
	@:

easy-uninstall: uninstall
	@:

easy-reinstall: reinstall
	@:

dist dist.src:
	@echo Simply clone the repo from https://github.com/lcn2/rogue5.4.git


####################
# dependency rules #
####################

###
#
# Home grown make dependency rules.  Your system may not support
# or have the needed tools.  You can ignore this section.
#
# We will form a skeleton tree of *.c files containing only #include "foo.h"
# lines and .h files containing the same lines surrounded by multiple include
# prevention lines.  This allows us to build a static depend list that will
# satisfy all possible cpp symbol definition combinations.
#
###

depend: ${SRC}
	${Q} if [ -f ${MAKE_FILE}.bak ]; then \
	    echo "${MAKE_FILE}.bak exists, remove or move it"; \
	    exit 1; \
	else \
	    ${TRUE}; \
	fi
	${Q} if type -f ${MAKEDEPEND}; then \
	    ${TRUE}; \
	else \
	    echo "make depend failed: cannot find makedepend command: ${MAKEDEPEND}" 1>&2; \
	    exit 1; \
	fi
	${Q} ${RM} -rf skel
	${Q} ${MKDIR} -p skel
	-${Q} for i in ${C_SRC}; do \
	    ${SED} -n '/^#[	 ]*include[	 ]*"/p' "$$i" | \
	    ${GREP} -E -v '\.\./getopt/getopt\.h' > "skel/$$i"; \
	done
	${Q} ${MKDIR} -p skel/custom
	-${Q} for i in ${H_SRC} /dev/null; do \
	    if [ X"$$i" != X"/dev/null" ]; then \
		tag="`echo $$i | ${SED} 's/[\.+,:]/_/g'`"; \
		echo "#if !defined($$tag)" > "skel/$$i"; \
		echo "#define $$tag" >> "skel/$$i"; \
		${SED} -n '/^#[	 ]*include[	 ]*"/p' "$$i" | \
		    LANG=C ${SORT} -u >> "skel/$$i"; \
		echo '#endif /* '"$$tag"' */' >> "skel/$$i"; \
	    fi; \
	done
	${Q} ${RM} -f skel/makedep.out
	${Q} echo top level skel formed
	${Q} echo forming dependency list
	${Q} :> skel/makedep.out
	${Q} cd skel; ${MAKEDEPEND} \
	    -w 1 -f makedep.out -- \
	    ${CFLAGS} -- \
	    ${SRC} 2>/dev/null
	-${Q} for i in ${C_SRC} /dev/null; do \
	    if [ X"$$i" != X"/dev/null" ]; then \
	      echo "$$i" | ${SED} 's/^\(.*\)\.c/\1.o: \1.c/'; \
	    fi; \
	done >> skel/makedep.out
	${Q} LANG=C ${SORT} -u skel/makedep.out -o skel/makedep.out
	${Q} echo dependency list formed
	${Q} echo forming new ${MAKE_FILE}
	${Q} ${RM} -f ${MAKE_FILE}.bak
	${Q} ${MV} ${MAKE_FILE} ${MAKE_FILE}.bak
	${Q} ${SED} -n '1,/^# DO NOT DELETE THIS LINE/p' \
		    ${MAKE_FILE}.bak > ${MAKE_FILE}
	${Q} ${GREP} -E -v '^#' skel/makedep.out >> ${MAKE_FILE}
	${Q} echo removing top level skel
	${Q} ${RM} -rf skel
	-${Q} if ${CMP} -s ${MAKE_FILE}.bak ${MAKE_FILE}; then \
	    echo 'top level ${MAKE_FILE} was already up to date'; \
	    echo 'restoring original ${MAKE_FILE}'; \
	    ${MV} -f ${MAKE_FILE}.bak ${MAKE_FILE}; \
	else \
	    echo 'old ${MAKE_FILE} is now ${MAKE_FILE}.bak'; \
	    echo 'new top level ${MAKE_FILE} formed'; \
	    echo 'try: diff -u ${MAKE_FILE}.bak ${MAKE_FILE}'; \
	fi

###
#
# make depend stuff
#
###

# DO NOT DELETE THIS LINE -- make depend depends on it.

armor.o: armor.c
armor.o: config.h
armor.o: extern.h
armor.o: rogue.h
chase.o: chase.c
chase.o: config.h
chase.o: extern.h
chase.o: rogue.h
command.o: command.c
command.o: config.h
command.o: extern.h
command.o: rogue.h
daemon.o: config.h
daemon.o: daemon.c
daemon.o: extern.h
daemon.o: rogue.h
daemons.o: config.h
daemons.o: daemons.c
daemons.o: extern.h
daemons.o: rogue.h
extern.o: config.h
extern.o: extern.c
extern.o: extern.h
extern.o: rogue.h
fight.o: config.h
fight.o: extern.h
fight.o: fight.c
fight.o: rogue.h
findpw.o: findpw.c
init.o: config.h
init.o: extern.h
init.o: init.c
init.o: rogue.h
io.o: config.h
io.o: extern.h
io.o: io.c
io.o: rogue.h
list.o: config.h
list.o: extern.h
list.o: list.c
list.o: rogue.h
mach_dep.o: config.h
mach_dep.o: extern.h
mach_dep.o: mach_dep.c
main.o: config.h
main.o: extern.h
main.o: main.c
main.o: rogue.h
mdport.o: config.h
mdport.o: extern.h
mdport.o: mdport.c
misc.o: config.h
misc.o: extern.h
misc.o: misc.c
misc.o: rogue.h
monsters.o: config.h
monsters.o: extern.h
monsters.o: monsters.c
monsters.o: rogue.h
move.o: config.h
move.o: extern.h
move.o: move.c
move.o: rogue.h
new_level.o: config.h
new_level.o: extern.h
new_level.o: new_level.c
new_level.o: rogue.h
options.o: config.h
options.o: extern.h
options.o: options.c
options.o: rogue.h
pack.o: config.h
pack.o: extern.h
pack.o: pack.c
pack.o: rogue.h
passages.o: config.h
passages.o: extern.h
passages.o: passages.c
passages.o: rogue.h
potions.o: config.h
potions.o: extern.h
potions.o: potions.c
potions.o: rogue.h
rings.o: config.h
rings.o: extern.h
rings.o: rings.c
rings.o: rogue.h
rip.o: config.h
rip.o: extern.h
rip.o: rip.c
rip.o: rogue.h
rip.o: score.h
rogue.o: config.h
rogue.o: extern.h
rooms.o: config.h
rooms.o: extern.h
rooms.o: rogue.h
rooms.o: rooms.c
save.o: config.h
save.o: extern.h
save.o: rogue.h
save.o: save.c
save.o: score.h
scedit.o: config.h
scedit.o: scedit.c
scedit.o: score.h
scmisc.o: scmisc.c
scmisc.o: score.h
scrolls.o: config.h
scrolls.o: extern.h
scrolls.o: rogue.h
scrolls.o: scrolls.c
state.o: config.h
state.o: extern.h
state.o: rogue.h
state.o: state.c
sticks.o: config.h
sticks.o: extern.h
sticks.o: rogue.h
sticks.o: sticks.c
things.o: config.h
things.o: extern.h
things.o: rogue.h
things.o: things.c
vers.o: vers.c
weapons.o: config.h
weapons.o: extern.h
weapons.o: rogue.h
weapons.o: weapons.c
wizard.o: config.h
wizard.o: extern.h
wizard.o: rogue.h
wizard.o: wizard.c
xcrypt.o: xcrypt.c
