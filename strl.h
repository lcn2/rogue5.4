/*
 * strl - size-bounded string copying and concatenation
 *
 * Copyright (C) 2021,2026  Landon Curt Noll
 *
 * Calc is open software; you can redistribute it and/or modify it under
 * the terms of the version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * Calc is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * A copy of version 2.1 of the GNU Lesser General Public License is
 * distributed with calc under the filename COPYING-LGPL.  You should have
 * received a copy with calc; if not, write to Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Under source code control:   2021/03/08 21;58:10
 * File existed as early as:    2021
 *
 * Share and enjoy!  :-)        http://www.isthe.com/chongo/tech/comp/calc/
 *
 * NOTE: This code was contributed from the calc application by permission.
 */

#if !defined(INCLUDE_STRL_H)
#  define INCLUDE_STRL_H


#  if !defined(HAVE_STRLCPY)
extern size_t strlcpy(char *dst, const char *src, size_t dstsize);
#  endif

#  if !defined(HAVE_STRLCAT)
extern size_t strlcat(char *dst, const char *src, size_t dstsize);
#  endif


#endif
