#!/usr/bin/env make
#
# rogue5.4 - rogue version 5.4
#
# The following Copyright ONLY applies to this Makefile and README.md files.
#
# Copyright (c) 2025 by Landon Curt Noll.  All Rights Reserved.
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
CHMOD= chmod
CP= cp
ID= id
INSTALL= install
RM= rm
SED= sed
SHELL= bash


######################
# target information #
######################

# V=@:  do not echo debug statements (quiet mode)
# V=@   echo debug statements (debug / verbose mode)
#
V=@:
#V=@

PREFIX= /usr/local
DESTDIR= ${PREFIX}/bin
DESTDOC= ${PREFIX}/share/rogue5.4
MAN6DIR= ${PREFIX}/man/man6

TARGETS= src/rogue


######################################
# all - default rule - must be first #
######################################

all: ${TARGETS}

src/rogue:
	${V} echo DEBUG =-= $@ start =-=
	@-if [[ ! -f src/Makefile || ! -f src/config.h ]]; then \
	    echo ${MAKE} configure; \
	    ${MAKE} configure; \
	fi
	cd src; ${MAKE} all
	${V} echo DEBUG =-= $@ end =-=

easy-all: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy all
	${V} echo DEBUG =-= $@ end =-=

src/config.h: src/config.h.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy config.h
	${V} echo DEBUG =-= $@ end =-=


#################################################
# .PHONY list of rules that do not create files #
#################################################

.PHONY: all configure clean clobber install uninstall reinstall \
	easy-all post-configure easy-configure easy-clean easy-clobber distclean easy-install easy-uninstall easy-reinstall


###################################
# standard Makefile utility rules #
###################################

configure:
	${V} echo DEBUG =-= $@ start =-=
	cd src; ./configure CFLAGS="" COPT="-O3" \
			--prefix=${PREFIX} --bindir=${DESTDIR} --docdir=${DESTDOC} \
			--with-ncurses --enable-scorefile=.rogue.scr --enable-lockfile=.rogue.lck \
			--enable-wizardmode
	${V} echo DEBUG =-= $@ end =-=


# post-configure
#
# NOTE: use only to build a new src/Makefile.easy and src/config.h.easy after successful configure
#
post-configure:
	${V} echo DEBUG =-= $@ start =-=
	@-if [[ -f src/Makefile && -f src/config.h ]]; then \
	    echo ${RM} -f src/Makefile.easy; \
	    ${RM} -f src/Makefile.easy; \
	    echo ${SED} -E -e 's/\s+$$//' < src/Makefile > src/Makefile.easy; \
	    ${SED} -E -e 's/\s+$$//' < src/Makefile > src/Makefile.easy; \
	    echo ${RM} -f src/config.h.easy; \
	    ${RM} -f src/config.h.easy; \
	    echo ${SED} -E -e 's/\s+$$//' < src/config.h > src/config.h.easy; \
	    ${SED} -E -e 's/\s+$$//' < src/config.h > src/config.h.easy; \
	else \
	    echo "$$0: ERROR: must have both src/Makefile and src/config.h to makr $@" 1>&2; \
	    echo "$$0: notice: try make configure $@" 1>&2; \
	    exit 1; \
	fi
	${V} echo DEBUG =-= $@ end =-=

easy-configure: src/Makefile.easy src/config.h.easy
	${V} echo DEBUG =-= $@ start =-=
	${RM} -f src/Makefile
	${CP} -f src/Makefile.easy src/Makefile
	${RM} -f src/config.h
	${CP} -f src/config.h.easy src/config.h
	${V} echo DEBUG =-= $@ end =-=

stddocs: configure
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

tags: ${CFILES} ${MISC_C}
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

clean:
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

easy-clean: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy clean
	${V} echo DEBUG =-= $@ end =-=

clobber: clean
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

easy-clobber: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy clobber
	${V} echo DEBUG =-= $@ end =-=

# distclean - store src tree to an initial state
#
# We use easy-clobber to clobber, then remove the src/Makefile and src/config.h files.
#
distclean:
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy clobber
	${RM} -f src/Makefile
	${RM} -f src/config.h
	${V} echo DEBUG =-= $@ end =-=

install: all
	${V} echo DEBUG =-= $@ start =-=
	@if [[ $$(${ID} -u) != 0 ]]; then echo "ERROR: must be root to make $@" 1>&2; exit 2; fi
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

easy-install: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy install
	${V} echo DEBUG =-= $@ end =-=

uninstall:
	${V} echo DEBUG =-= $@ start =-=
	@if [[ $$(${ID} -u) != 0 ]]; then echo "ERROR: must be root to make $@" 1>&2; exit 2; fi
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

easy-uninstall: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy uninstall
	${V} echo DEBUG =-= $@ end =-=

reinstall:
	${V} echo DEBUG =-= $@ start =-=
	@if [[ $$(${ID} -u) != 0 ]]; then echo "ERROR: must be root to make $@" 1>&2; exit 2; fi
	cd src; ${MAKE} $@
	${V} echo DEBUG =-= $@ end =-=

easy-reinstall: src/Makefile.easy
	${V} echo DEBUG =-= $@ start =-=
	cd src; ${MAKE} -f Makefile.easy reinstall
	${V} echo DEBUG =-= $@ end =-=

