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

CAT= cat
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

# ROGUEDIR  - where to place/use the rogue lock file, rogue score file, and rogue save file
#
ROGUEDIR= ${HOME}

# Basenames of the rogue lock file, rogue score file, and rogue save file
#
# These files will be placed/used under the ${ROGUEDIR} directory.
#
LOCKFILE_BASENAME= .rogue.lck
SAVEFILE_BASENAME= .rogue.save
SCOREFILE_BASENAME= .rogue.scr

# PREFIX - Tree under which the rogue binary, findpw, scedit binaries,
# 	   rogue documentation, and rogue man page.
#
PREFIX= /usr/local
#
DESTDIR= ${PREFIX}/bin
SHAREDIR= ${PREFIX}/share
DESTDOC= ${SHAREDIR}/rogue5.4
MANDIR= ${PREFIX}/man
MAN6DIR= ${MANDIR}/man6


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
CPPFLAGS= -DLOCKPATH='"${ROGUEDIR}/${LOCKFILE_BASENAME}"' \
	  -DSAVEPATH='"${ROGUEDIR}/${SAVEFILE_BASENAME}"' \
	  -DSCOREPATH='"${ROGUEDIR}/${SCOREFILE_BASENAME}"'
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

# We recommend that you leave ${GROUPOWNER} empty.
#
# The program now defaults to using lock, save, and score files under the home directory,
# one does NOT need to set the user and group of the program, nor of the lock, save, and score files.
#
# If you leave ${GROUPOWNER} rempty, then rogue will install as run as the current user.
#
# WARNING: Setting ${GROUPOWNER} to a non-empty value is NOT recommended !!!

#GROUPOWNER= games	# NOT recommended - leave GROUPOWNER as an empty value !!!
GROUPOWNER=


################################
# source and destination files #
################################

HFILES= config.h rogue.h extern.h score.h strl.h

OBJS1= vers.o extern.o armor.o chase.o command.o common.o \
           daemon.o daemons.o fight.o init.o io.o list.o \
           mach_dep.o mdport.o misc.o monsters.o \
           move.o new_level.o

OBJS2= options.o pack.o passages.o potions.o rings.o \
           rip.o rooms.o save.o scrolls.o state.o sticks.o \
           things.o weapons.o wizard.o xcrypt.o strl.o

OBJS= ${OBJS1} ${OBJS2}

CFILES= vers.c extern.c armor.c chase.c command.c daemon.c \
           daemons.c fight.c init.c io.c list.c mach_dep.c \
           main.c mdport.c misc.c monsters.c move.c new_level.c \
           options.c pack.c passages.c potions.c rings.c rip.c \
           rooms.c save.c scrolls.c state.c sticks.c things.c \
           weapons.c wizard.c xcrypt.c strl.c

MISC_C= findpw.c scedit.c scmisc.c common.c have_strlcat.c have_strlcpy.c

MISC_H=

H_SRC= ${HFILES} ${MISC_H}

C_SRC= ${CFILES} ${MISC_C}

BUILD_HSRC= modern_curses.h have_strlcpy.h have_strlcat.h

BUILD_OBJS= have_strlcat.o have_strlcpy.o

BUILD_TOOLS= have_strlcat have_strlcpy

SRC= ${C_SRC} ${H_SRC} ${BUILD_HSRC}

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
# DEBUG:= -ggdb3
#
# CFLAGS+= ${FSANITIZE} -fstack-protector-all
# LDFLAGS+= ${FSANITIZE}
####


######################################
# all - default rule - must be first #
######################################

all: hsrc ${TARGETS} stddocs


##################
# compile C code #
##################

.c.o:
	${CC} ${CFLAGS} -c $*.c

rogue: ${H_SRC} ${BUILD_HSRC} ${OBJS} main.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} main.o ${LIBS} -o $@

findpw: ${OBJS} findpw.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} findpw.o ${LIBS} -o $@

scedit: ${OBJS} scmisc.o scedit.o
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} scmisc.o scedit.o ${LIBS} -o $@


#####################
# constricted files #
#####################

hsrc: ${BUILD_HSRC}

# NOTE: We check for an empty ${ROGUEDIR}, ${LOCKFILE_BASENAME}, ${SAVEFILE_BASENAME}, or ${SCOREFILE_BASENAME}
# 	as a paranoia check here because the modern_curses.h MUST be constructed while compiling rogue.
# 	The modern_curses.h file doesn't need these make variables, we simply test them here as a convenience.
#
modern_curses.h: ${MAKE_FILE}
	${Q} if [[ -z "${ROGUEDIR}" ]]; then \
	    echo 'ERROR: The ROGUEDIR make variable CANNOT be empty!!!' ; \
	    exit 1 ; \
	fi
	${Q} if [[ -z "${LOCKFILE_BASENAME}" ]]; then \
	    echo 'ERROR: The LOCKFILE_BASENAME make variable CANNOT be empty!!!' ; \
	    exit 2 ; \
	fi
	${Q} if [[ -z "${SAVEFILE_BASENAME}" ]]; then \
	    echo 'ERROR: The SAVEFILE_BASENAME make variable CANNOT be empty!!!' ; \
	    exit 3 ; \
	fi
	${Q} if [[ -z "${SCOREFILE_BASENAME}" ]]; then \
	    echo 'ERROR: The SCOREFILE_BASENAME make variable CANNOT be empty!!!' ; \
	    exit 4 ; \
	fi
	${Q} ${RM} -f modern_curses.o modern_curses $@
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

have_strlcpy.h: have_strlcpy.c ${MAKE_FILE}
	${Q} ${RM} -f unused_tmp $@
	${H} echo 'forming $@'
	${Q} echo '/*' > $@
	${Q} echo ' * DO NOT EDIT -- generated by the Makefile rule $@' >> $@
	${Q} echo ' */' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#if !defined(INCLUDE_STRLCPY_H)' >> $@
	${Q} echo '#define INCLUDE_STRLCPY_H' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '/* do we have the strlcpy() function? */' >> $@
	${Q} ${RM} -f have_strlcpy.o have_strlcpy
	-${Q} ${CC} have_strlcpy.c -c ${S} \
		|| ${TRUE}
	-${Q} ${CC} have_strlcpy.o -o have_strlcpy ${S} \
		|| ${TRUE}
	-${Q} ./have_strlcpy > unused_tmp ${E} \
		|| ${TRUE}
	-${Q} if [ -s unused_tmp ]; then \
	    ${CAT} unused_tmp >> $@; \
	else \
	    echo '#undef INCLUDE_STRLCPY /* no - use code from strl.c */' \
	         >> $@; \
	fi
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#endif' >> $@
	${Q} ${RM} -f have_strlcpy have_strlcpy.o unused_tmp
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

have_strlcat.h: have_strlcat.c ${MAKE_FILE}
	${Q} ${RM} -f unused_tmp $@
	${H} echo 'forming $@'
	${Q} echo '/*' > $@
	${Q} echo ' * DO NOT EDIT -- generated by the Makefile rule $@' >> $@
	${Q} echo ' */' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#if !defined(INCLUDE_STRLCAT_H)' >> $@
	${Q} echo '#define INCLUDE_STRLCAT_H' >> $@
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '/* do we have the strlcat() function? */' >> $@
	${Q} ${RM} -f have_strlcat.o have_strlcat
	-${Q} ${CC} have_strlcat.c -c ${S} \
		|| ${TRUE}
	-${Q} ${CC} have_strlcat.o -o have_strlcat ${S} \
		|| ${TRUE}
	-${Q} ./have_strlcat > unused_tmp ${E} \
		|| ${TRUE}
	-${Q} if [ -s unused_tmp ]; then \
	    ${CAT} unused_tmp >> $@; \
	else \
	    echo '#undef INCLUDE_STRLCAT /* no - use code from strl.c */' \
	         >> $@; \
	fi
	${Q} echo '' >> $@
	${Q} echo '' >> $@
	${Q} echo '#endif' >> $@
	${Q} ${RM} -f have_strlcat have_strlcat.o unused_tmp
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
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			  -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			  rogue.6.in > rogue.6

rogue.me: rogue.me.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			  -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			  rogue.me.in > rogue.me

rogue.html: rogue.html.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			  -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
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
	    echo "LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			      -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			      rogue.doc.in > $@" ; \
	    LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			      -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
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
	    echo "LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			      -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			      rogue.cat.in > $@" ; \
	    LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			      -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			      -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			      -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			      rogue.cat.in > $@ ; \
	fi

rogue.md: rogue.md.in
	${RM} -f $@
	LC_CTYPE=C ${SED} -e 's;${AT}PROGRAM${AT};rogue;' \
			  -e 's;${AT}LOCKFILE${AT};${ROGUEDIR}/${LOCKFILE_BASENAME};' \
			  -e 's;${AT}SAVEFILE${AT};${ROGUEDIR}/${SAVEFILE_BASENAME};' \
			  -e 's;${AT}SCOREFILE${AT};${ROGUEDIR}/${SCOREFILE_BASENAME};' \
			  rogue.md.in > rogue.md

# NOTE: This rule is NOT part of the build of rogue documentation!
#       We use this rule to form the rogue.cat.in file from the rogue.6.in file.
#
form_rogue_cat_in: rogue.6.in
	${RM} -f rogue.cat.in
	${GROFF} -Tascii -man rogue.6.in | LC_CTYPE=C ${SED} -e 's/.\x08//g' > rogue.cat.in


#################################################
# .PHONY list of rules that do not create files #
#################################################

.PHONY: all stddocs tags clean clobber distclean install uninstall reinstall depend \
	easy-all configure easy-configure post-configure easy-clean easy-clobber easy-install \
	easy-uninstall easy-reinstall dist dist.src


###################################
# standard Makefile utility rules #
###################################

tags: ${SRC}
	${CTAGS} -w ${SRC}

clean:
	${RM} -f ${OBJS1}
	${RM} -f ${OBJS2}
	${RM} -f main.o findpw.o scedit.o scmisc.o

clobber: clean
	${RM} -f empty tags unused_tmp
	${RM} -f rogue.6 rogue.me rogue.html rogue.doc rogue.cat rogue.md
	${RM} -f ${BUILD_HSRC} ${BUILD_OBJS} ${BUILD_TOOLS}
	${RM} -f ${TARGETS}
	${RM} -rf skel

distclean maintainer-clean: clobber

install: all
	@if [[ -z "${DESTDIR}" ]]; then \
	    echo 'ERROR: The DESTDIR make variable CANNOT be empty!!!' ; \
	    exit 1 ; \
	fi
	-${INSTALL} -d -m 0755 ${DESTDIR}
	@if [[ ! -d "${DESTDIR}" ]]; then \
	    echo 'ERROR: failed to create DESTDIR directory: ${DESTDIR}' 1>&2 ; \
	    exit 2 ; \
	fi
	@if [[ -z "${ROGUEDIR}" ]]; then \
	    echo 'ERROR: The ROGUEDIR make variable CANNOT be empty!!!' ; \
	    exit 3 ; \
	fi
	-${INSTALL} -d -m 0755 ${ROGUEDIR}
	@if [[ ! -d "${ROGUEDIR}" ]]; then \
	    echo 'ERROR: failed to create ROGUEDIR directory: ${ROGUEDIR}' 1>&2 ; \
	    exit 4 ; \
	fi
	@if [[ -z "${MANDIR}" ]]; then \
	    echo 'ERROR: The MANDIR make variable CANNOT be empty!!!' ; \
	    exit 5 ; \
	fi
	-${INSTALL} -d -m 0755 ${MANDIR}
	@if [[ ! -d "${MANDIR}" ]]; then \
	    echo 'ERROR: failed to create MANDIR directory: ${MANDIR}' 1>&2 ; \
	    exit 6 ; \
	fi
	@if [[ -z "${MAN6DIR}" ]]; then \
	    echo 'ERROR: The MAN6DIR make variable CANNOT be empty!!!' ; \
	    exit 7 ; \
	fi
	-${INSTALL} -d -m 0755 ${MAN6DIR}
	@if [[ ! -d "${MAN6DIR}" ]]; then \
	    echo 'ERROR: failed to create MAN6DIR directory: ${MAN6DIR}' 1>&2 ; \
	    exit 8 ; \
	fi
	@if [[ -z "${SHAREDIR}" ]]; then \
	    echo 'ERROR: The SHAREDIR make variable CANNOT be empty!!!' ; \
	    exit 9 ; \
	fi
	-${INSTALL} -d -m 0755 ${SHAREDIR}
	@if [[ ! -d "${SHAREDIR}" ]]; then \
	    echo 'ERROR: failed to create SHAREDIR directory: ${SHAREDIR}' 1>&2 ; \
	    exit 10 ; \
	fi
	@if [[ -z "${DESTDOC}" ]]; then \
	    echo 'ERROR: The DESTDOC make variable CANNOT be empty!!!' ; \
	    exit 11 ; \
	fi
	-${INSTALL} -d -m 0755 ${DESTDOC}
	@if [[ ! -d "${DESTDOC}" ]]; then \
	    echo 'ERROR: failed to create DESTDOC directory: ${DESTDOC}' 1>&2 ; \
	    exit 12 ; \
	fi
	-@if [[ -n "${GROUPOWNER}" ]]; then \
	    echo -e "WARNING: Use of non-empty ${GROUPOWNER} is NOT recommended !!!\a" ; \
	    sleep 1; \
	    echo -e "WARNING: Use of non-empty ${GROUPOWNER} is NOT recommended !!!\a" ; \
	    sleep 1 ; \
	    echo -e "WARNING: Use of non-empty ${GROUPOWNER} is NOT recommended !!!\a" ; \
	    sleep 1 ; \
	    echo "${INSTALL} -m 2755 rogue -g ${GROUPOWNER} ${DESTDIR}/rogue" ; \
	    ${INSTALL} -m 2755 rogue -g ${GROUPOWNER} ${DESTDIR}/rogue ; \
	    echo "${TOUCH} -- ${ROGUEDIR}/${SCOREFILE_BASENAME}"; \
	    ${TOUCH} -- ${ROGUEDIR}/${SCOREFILE_BASENAME}; \
	    echo "${CHGRP} ${GROUPOWNER} ${ROGUEDIR}/${SCOREFILE_BASENAME}" ; \
	    ${CHGRP} ${GROUPOWNER} ${ROGUEDIR}/${SCOREFILE_BASENAME} ; \
	    echo "${CHMOD} 0664 ${ROGUEDIR}/${SCOREFILE_BASENAME}" ; \
	    ${CHMOD} 0664 ${ROGUEDIR}/${SCOREFILE_BASENAME} ; \
	    echo "${TOUCH} -- ${ROGUEDIR}/${LOCKFILE_BASENAME}" ; \
	    ${TOUCH} -- ${ROGUEDIR}/${LOCKFILE_BASENAME} ; \
	    echo "${CHGRP} ${GROUPOWNER} ${ROGUEDIR}/${LOCKFILE_BASENAME}" ; \
	    ${CHGRP} ${GROUPOWNER} ${ROGUEDIR}/${LOCKFILE_BASENAME} ; \
	    echo "${CHMOD} 0664 ${ROGUEDIR}/${LOCKFILE_BASENAME}" ; \
	    ${CHMOD} 0664 ${ROGUEDIR}/${LOCKFILE_BASENAME} ; \
	else \
	    echo "${INSTALL} -m 0755 rogue ${DESTDIR}/rogue" ; \
	    ${INSTALL} -m 0755 rogue ${DESTDIR}/rogue ; \
        fi
	-${INSTALL} -m 0755 findpw ${DESTDIR}/findpw
	-${INSTALL} -m 0755 scedit ${DESTDIR}/scedit
	-${INSTALL} -m 0644 rogue.6 ${MAN6DIR}/rogue.6
	-${INSTALL} -m 0444 rogue.doc ${DESTDOC}/rogue.doc
	-${INSTALL} -m 0444 rogue.html ${DESTDOC}/rogue.html
	-${INSTALL} -m 0444 rogue.cat ${DESTDOC}/rogue.cat
	-${INSTALL} -m 0444 rogue.me ${DESTDOC}/rogue.me
	-${INSTALL} -m 0444 LICENSE ${DESTDOC}/LICENSE
	-${INSTALL} -m 0444 LICENSE ${DESTDOC}/LICENSE.TXT
	-${INSTALL} -m 0444 README.md ${DESTDOC}/README.md

uninstall:
	-${RM} -f -v ${DESTDIR}/rogue
	-${RM} -f -v ${DESTDIR}/findpw
	-${RM} -f -v ${DESTDIR}/scedit
	-${RM} -f -v ${MAN6DIR}/rogue.6
	-${RM} -f -v ${DESTDOC}/rogue.doc
	-${RM} -f -v ${DESTDOC}/rogue.html
	-${RM} -f -v ${DESTDOC}/rogue.cat
	-${RM} -f -v ${DESTDOC}/rogue.me
	-${RM} -f -v ${DESTDOC}/LICENSE
	-${RM} -f -v ${DESTDOC}/LICENSE.TXT
	-${RM} -f -v ${DESTDOC}/README.md
	-${RMDIR} -v ${DESTDOC}

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
	-${Q} for i in ${H_SRC} ${BUILD_HSRC} /dev/null; do \
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
armor.o: extern.h
armor.o: modern_curses.h
armor.o: rogue.h
chase.o: chase.c
chase.o: config.h
chase.o: extern.h
chase.o: modern_curses.h
chase.o: rogue.h
command.o: command.c
command.o: config.h
command.o: extern.h
command.o: have_strlcat.h
command.o: have_strlcpy.h
command.o: modern_curses.h
command.o: rogue.h
command.o: strl.h
common.o: common.c
common.o: extern.h
common.o: modern_curses.h
common.o: rogue.h
daemon.o: config.h
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
fight.o: config.h
fight.o: extern.h
fight.o: fight.c
fight.o: have_strlcat.h
fight.o: have_strlcpy.h
fight.o: modern_curses.h
fight.o: rogue.h
fight.o: strl.h
findpw.o: findpw.c
findpw.o: score.h
have_strlcat.o: have_strlcat.c
have_strlcpy.o: have_strlcpy.c
init.o: config.h
init.o: extern.h
init.o: have_strlcat.h
init.o: have_strlcpy.h
init.o: init.c
init.o: modern_curses.h
init.o: rogue.h
init.o: strl.h
io.o: extern.h
io.o: have_strlcat.h
io.o: have_strlcpy.h
io.o: io.c
io.o: modern_curses.h
io.o: rogue.h
io.o: strl.h
list.o: config.h
list.o: extern.h
list.o: list.c
list.o: modern_curses.h
list.o: rogue.h
mach_dep.o: config.h
mach_dep.o: extern.h
mach_dep.o: have_strlcat.h
mach_dep.o: have_strlcpy.h
mach_dep.o: mach_dep.c
mach_dep.o: modern_curses.h
mach_dep.o: rogue.h
mach_dep.o: score.h
mach_dep.o: strl.h
main.o: config.h
main.o: extern.h
main.o: have_strlcat.h
main.o: have_strlcpy.h
main.o: main.c
main.o: modern_curses.h
main.o: rogue.h
main.o: score.h
main.o: strl.h
mdport.o: config.h
mdport.o: extern.h
mdport.o: have_strlcat.h
mdport.o: have_strlcpy.h
mdport.o: mdport.c
mdport.o: modern_curses.h
mdport.o: score.h
mdport.o: strl.h
misc.o: config.h
misc.o: extern.h
misc.o: have_strlcat.h
misc.o: have_strlcpy.h
misc.o: misc.c
misc.o: modern_curses.h
misc.o: rogue.h
misc.o: strl.h
monsters.o: config.h
monsters.o: extern.h
monsters.o: have_strlcat.h
monsters.o: have_strlcpy.h
monsters.o: modern_curses.h
monsters.o: monsters.c
monsters.o: rogue.h
monsters.o: strl.h
move.o: extern.h
move.o: modern_curses.h
move.o: move.c
move.o: rogue.h
new_level.o: extern.h
new_level.o: have_strlcat.h
new_level.o: have_strlcpy.h
new_level.o: modern_curses.h
new_level.o: new_level.c
new_level.o: rogue.h
new_level.o: strl.h
options.o: config.h
options.o: extern.h
options.o: have_strlcat.h
options.o: have_strlcpy.h
options.o: modern_curses.h
options.o: options.c
options.o: rogue.h
options.o: score.h
options.o: strl.h
pack.o: config.h
pack.o: extern.h
pack.o: have_strlcat.h
pack.o: have_strlcpy.h
pack.o: modern_curses.h
pack.o: pack.c
pack.o: rogue.h
pack.o: strl.h
passages.o: config.h
passages.o: extern.h
passages.o: modern_curses.h
passages.o: passages.c
passages.o: rogue.h
potions.o: config.h
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
rip.o: have_strlcat.h
rip.o: have_strlcpy.h
rip.o: modern_curses.h
rip.o: rip.c
rip.o: rogue.h
rip.o: score.h
rip.o: strl.h
rooms.o: extern.h
rooms.o: modern_curses.h
rooms.o: rogue.h
rooms.o: rooms.c
save.o: config.h
save.o: extern.h
save.o: have_strlcat.h
save.o: have_strlcpy.h
save.o: modern_curses.h
save.o: rogue.h
save.o: save.c
save.o: score.h
save.o: strl.h
scedit.o: config.h
scedit.o: extern.h
scedit.o: modern_curses.h
scedit.o: scedit.c
scedit.o: score.h
scmisc.o: have_strlcat.h
scmisc.o: have_strlcpy.h
scmisc.o: scmisc.c
scmisc.o: score.h
scmisc.o: strl.h
scrolls.o: config.h
scrolls.o: extern.h
scrolls.o: modern_curses.h
scrolls.o: rogue.h
scrolls.o: scrolls.c
state.o: extern.h
state.o: modern_curses.h
state.o: rogue.h
state.o: score.h
state.o: state.c
sticks.o: config.h
sticks.o: extern.h
sticks.o: have_strlcat.h
sticks.o: have_strlcpy.h
sticks.o: modern_curses.h
sticks.o: rogue.h
sticks.o: sticks.c
sticks.o: strl.h
strl.o: have_strlcat.h
strl.o: have_strlcpy.h
strl.o: strl.c
strl.o: strl.h
things.o: config.h
things.o: extern.h
things.o: have_strlcat.h
things.o: have_strlcpy.h
things.o: modern_curses.h
things.o: rogue.h
things.o: strl.h
things.o: things.c
vers.o: vers.c
weapons.o: extern.h
weapons.o: have_strlcat.h
weapons.o: have_strlcpy.h
weapons.o: modern_curses.h
weapons.o: rogue.h
weapons.o: strl.h
weapons.o: weapons.c
wizard.o: config.h
wizard.o: extern.h
wizard.o: have_strlcat.h
wizard.o: have_strlcpy.h
wizard.o: modern_curses.h
wizard.o: rogue.h
wizard.o: strl.h
wizard.o: wizard.c
xcrypt.o: have_strlcat.h
xcrypt.o: have_strlcpy.h
xcrypt.o: strl.h
xcrypt.o: xcrypt.c
