#!/usr/bin/env make
#
# rogue5.4 - rogue version 5.4.5
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

# S= >/dev/null 2>&1	silence ${CC} output during hsrc file formation
# S=			full ${CC} output during hsrc file formation
#
S= >/dev/null 2>&1
#S=


############################
# how to build and install #
############################

CSTD= -std=gnu17
CCWARN=
CPPFLAGS= -DLOCKNAME='"${LOCKFILE_BASENAME}"' \
	  -DSAVENAME='"${SAVEFILE_BASENAME}"' \
	  -DSCORENAME='"${SCOREFILE_BASENAME}"'
COPT= -O3
DEBUG= -ggdb3

CFLAGS= ${CSTD} ${CCWARN} ${CPPFLAGS} ${COPT} ${DEBUG}

LDFLAGS=

LC_CTYPE= C

AT= @

# The NetBSD maintainers insist in using curses instead of ncurses.
# NetBSD uses their own curses: a hybrid of BSD curses and X/Open curses.
# There are known incompatibles between NetBSD curses and what the next of the
# world calls ncurses. See https://wiki.netbsd.org/curses_in_netbsd/
# where not only does the NetBSD maintainers admit to being one of the
# last operating systems (or even the last one?) not using ncurses, and
# where they indicate what is different with ncurses.  One should also
# read: https://www.invisible-island.net/ncurses/ncurses-netbsd.html
# for more info.  Well if you use NetBSD, we hope the NetBSD curses
# works for you.
#
# Alternatively NetBSD users can install the ncurses package for
# NetBSD: https://cdn.netbsd.org/pub/pkgsrc/current/pkgsrc/devel/ncurses/README.html
#
target=$(shell uname -s 2>/dev/null)
ifeq ($(target),NetBSD)
LIBS= -lcurses
else
LIBS= -lncurses
endif

TARGETS= rogue findpw scedit


#############
# ownership #
#############

# We recommend that you leave ${GROUPOWNER}, $(INSTGROUP}, and ${INSTOWNER} empty.
#
# The program now defaults to using lock, save, and score files under the home directory,
# one does NOT need to set the user and group of the program, nor of the lock, save, and score files.
#
# If you leave ${GROUPOWNER}, $(INSTGROUP}, and ${INSTOWNER} empty, then rogue will
# install as run as the current user.

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

MISC= LICENSE rogue.spec rogue.png README.md

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
# By default, the Address Sanitizer is NOT enabled.
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
# To use the Address Sanitizer, uncomment this set set of lines and recompile (make clobber all):
#
# For more info see: https://github.com/google/sanitizers/wiki/AddressSanitizer
# See also: https://developer.apple.com/documentation/xcode/diagnosing-memory-thread-and-crash-issues-early
# And also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
#
# For Homebrew gcc version 15 only:
#
# CC:= gcc-15
# DEBUG:= -g2
#
# For Apple clang and Homebrew gcc:
#
# CCWARN+= -Wall
# CCWARN+= -pedantic
# CCWARN+= -Werror
# COPT:= -O0
#
# For Apple clang only:
#
# FSANITIZE+= -fsanitize=nullability-arg
# FSANITIZE+= -fsanitize=nullability-assign
# FSANITIZE+= -fsanitize=nullability-return
#
# CFLAGS+= ${FSANITIZE} -fstack-protector-all
# LDFLAGS+= ${FSANITIZE}
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
# To use the Address Sanitizer, uncomment this set set of lines and recompile (make clobber all):
#
# For more info see: https://github.com/google/sanitizers/wiki/AddressSanitizer
# And also: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
#
# Be sure you have the following dnf packages installed:
#
#   dnf install readline-devel ncurses-libs ncurses-devel libasan libubsan
#
# CCWARN+= -Wall
# CCWARN+= -pedantic
# CCWARN+= -Werror
# COPT:= -O0
#
# CFLAGS+= ${FSANITIZE} -fstack-protector-all
# LDFLAGS+= ${FSANITIZE}
# DEBUG:= -ggdb3
####


######################################
# all - default rule - must be first #
######################################

all: modern_curses.h ${TARGETS} stddocs


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


#####################
# constricted files #
#####################

modern_curses.h: ${MAKE_FILE}
	${Q} ${RM} -f modern_curses.o modern_curses${EXT} $@
	${H} echo 'forming $@'
	${Q} echo '/*' > $@
	${Q} echo ' * DO NOT EDIT -- generated by the Makefile rule $@' >> $@
	${Q} echo ' */' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#if !defined(INCLUDE_MODERN_CURSES_H)' >> $@
	${Q} echo '#define INCLUDE_MODERN_CURSES_H' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '/* do have modern new curses ? */' >> $@
	-${Q} if echo '#include <ncurses.h>' | ${CC} -E - ${S}; then \
	    echo '#include <ncurses.h> /* yes, using new curses */' >> $@; \
	    echo >> $@; \
	    echo '#define MODERN_CUESES NEW_CURSES' >> $@; \
	elif echo '#include <curses.h>' | ${CC} -E - ${S}; then \
	    echo '#include <curses.h> /* no, you have old curses, or what NetBSD calls curses */' >> $@; \
	    echo >> $@; \
	    echo '#define MODERN_CUESES OLD_CURSES' >> $@; \
	else \
	    echo 'Neither <ncurses.h> nor <curses.h> found' 1>&2; \
	    echo 'Perhaps you need you install both ncurses and ncurses-devel ?' 1>&2; \
	    exit 1; \
	fi
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#endif' >> $@
	${H} echo '$@ formed'
	-${Q}if [ -z "${Q}" ]; then \
	    echo ''; \
	    echo '=-=-= start of $@ =-=-='; \
	    ${CAT} $@; \
	    echo '=-=-= end of $@ =-=-='; \
	    echo ''; \
	else \
	    ${TRUE}; \
	fi


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
	${RM} -f empty tags modern_curses.h
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
	-${INSTALL} -m 0644 LICENSE ${DESTDOC}/LICENSE
	-${INSTALL} -m 0644 LICENSE ${DESTDOC}/LICENSE.TXT
	-${INSTALL} -m 0644 rogue.me ${DESTDOC}/${PROGRAM}.me
	-${INSTALL} -m 0644 rogue.md ${DESTDOC}/${PROGRAM}.md
	-${INSTALL} -m 0644 README.md ${DESTDOC}/README.md
	-@if test ! -f ~/${LOCKFILE_BASENAME}; then \
	    echo "${INSTALL} -m 0666 empty ~/${LOCKFILE_BASENAME}" ; \
	    ${INSTALL} -m 0666 empty ~/${LOCKFILE_BASENAME} ; \
	fi
	${RM} -f empty

uninstall:
	-${RM} -f ${DESTDIR}/${PROGRAM}
	-${RM} -f ${MAN6DIR}/${PROGRAM}.6
	-${RM} -f ${DESTDOC}/${PROGRAM}.doc
	-${RM} -f ${DESTDOC}/${PROGRAM}.html
	-${RM} -f ${DESTDOC}/${PROGRAM}.cat
	-${RM} -f ${DESTDOC}/LICENSE
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
	    ${SED} -n '/^#[	 ]*include[	 ]*"/p' "$$i" > "skel/$$i"; \
	done
	${Q} ${MKDIR} -p skel/custom
	-${Q} for i in ${H_SRC} modern_curses.h /dev/null; do \
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
	    ${SRC} modern_curses.h 2>/dev/null
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
armor.o: extern.h
armor.o: modern_curses.h
armor.o: rogue.h
chase.o: chase.c
chase.o: extern.h
chase.o: modern_curses.h
chase.o: rogue.h
command.o: command.c
command.o: extern.h
command.o: modern_curses.h
command.o: rogue.h
daemon.o: daemon.c
daemon.o: extern.h
daemon.o: modern_curses.h
daemon.o: rogue.h
daemons.o: daemons.c
daemons.o: extern.h
daemons.o: modern_curses.h
daemons.o: rogue.h
extern.o: config.h
extern.o: extern.c
extern.o: extern.h
extern.o: modern_curses.h
extern.o: rogue.h
fight.o: extern.h
fight.o: fight.c
fight.o: modern_curses.h
fight.o: rogue.h
findpw.o: findpw.c
findpw.o: score.h
init.o: extern.h
init.o: init.c
init.o: modern_curses.h
init.o: rogue.h
io.o: extern.h
io.o: io.c
io.o: modern_curses.h
io.o: rogue.h
list.o: extern.h
list.o: list.c
list.o: modern_curses.h
list.o: rogue.h
mach_dep.o: config.h
mach_dep.o: extern.h
mach_dep.o: mach_dep.c
mach_dep.o: modern_curses.h
mach_dep.o: rogue.h
mach_dep.o: score.h
main.o: extern.h
main.o: main.c
main.o: modern_curses.h
main.o: rogue.h
main.o: score.h
mdport.o: config.h
mdport.o: extern.h
mdport.o: mdport.c
mdport.o: modern_curses.h
mdport.o: score.h
misc.o: extern.h
misc.o: misc.c
misc.o: modern_curses.h
misc.o: rogue.h
monsters.o: extern.h
monsters.o: modern_curses.h
monsters.o: monsters.c
monsters.o: rogue.h
move.o: extern.h
move.o: modern_curses.h
move.o: move.c
move.o: rogue.h
new_level.o: extern.h
new_level.o: modern_curses.h
new_level.o: new_level.c
new_level.o: rogue.h
options.o: extern.h
options.o: modern_curses.h
options.o: options.c
options.o: rogue.h
options.o: score.h
pack.o: extern.h
pack.o: modern_curses.h
pack.o: pack.c
pack.o: rogue.h
passages.o: extern.h
passages.o: modern_curses.h
passages.o: passages.c
passages.o: rogue.h
potions.o: extern.h
potions.o: modern_curses.h
potions.o: potions.c
potions.o: rogue.h
rings.o: extern.h
rings.o: modern_curses.h
rings.o: rings.c
rings.o: rogue.h
rip.o: config.h
rip.o: extern.h
rip.o: modern_curses.h
rip.o: rip.c
rip.o: rogue.h
rip.o: score.h
rooms.o: extern.h
rooms.o: modern_curses.h
rooms.o: rogue.h
rooms.o: rooms.c
save.o: extern.h
save.o: modern_curses.h
save.o: rogue.h
save.o: save.c
save.o: score.h
scedit.o: config.h
scedit.o: extern.h
scedit.o: modern_curses.h
scedit.o: scedit.c
scedit.o: score.h
scmisc.o: scmisc.c
scmisc.o: score.h
scrolls.o: extern.h
scrolls.o: modern_curses.h
scrolls.o: rogue.h
scrolls.o: scrolls.c
state.o: extern.h
state.o: modern_curses.h
state.o: rogue.h
state.o: score.h
state.o: state.c
sticks.o: extern.h
sticks.o: modern_curses.h
sticks.o: rogue.h
sticks.o: sticks.c
things.o: extern.h
things.o: modern_curses.h
things.o: rogue.h
things.o: things.c
vers.o: vers.c
weapons.o: extern.h
weapons.o: modern_curses.h
weapons.o: rogue.h
weapons.o: weapons.c
wizard.o: extern.h
wizard.o: modern_curses.h
wizard.o: rogue.h
wizard.o: wizard.c
xcrypt.o: xcrypt.c
