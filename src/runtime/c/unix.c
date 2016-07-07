/*------------------------------------------------------------------------------
 * CLiCC: The Common Lisp to C Compiler
 * Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
 * Christian-Albrechts-Universitaet zu Kiel, Germany
 *------------------------------------------------------------------------------
 * CLiCC has been developed as part of the APPLY research project,
 * funded by the German Ministry of Research and Technology.
 * 
 * CLiCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CLiCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in file COPYING for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *------------------------------------------------------------------------------
 * Function : - unix-current-directory
 *            - unix-lstat
 *            - unix-stat
 *            - unix-readlink
 *            - unix-get-unix-error-msg
 *
 * $Revision: 1.12 $
 * $Id: unix.c,v 1.12 1994/11/22 14:54:01 hk Exp $
 *--------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef __STDC__
extern char *getwd(char pathname[]);
#else
extern char *getwd();
#endif

/*------------------------------------------------------------------------------
 * unix-current-directory
 * Returns 2 values
 *----------------------------------------------------------------------------*/
LISP_FUN(unix_current_directory)
{
   char pathname[MAXPATHLEN];
   char *result;
   
   result = getwd(pathname);
   make_string(ARG(0), pathname);
   COPY(ARG(0), OFFSET(mv_buf, 0));
   mv_count = 2;
   RET_BOOL(result != NULL);
}

/*------------------------------------------------------------------------------
 * unix-file-mode name
 *----------------------------------------------------------------------------*/
LISP_FUN(unix_file_mode)
{
   struct stat buf;
   int success = stat(get_c_string(ARG(0)), &buf);

   if(success == 0)
   {
      LOAD_FIXNUM(ARG(0), buf.st_mode, ARG(0));
   }
   else
   {
      LOAD_NIL(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * UNIX-LINK-MODE name
 *----------------------------------------------------------------------------*/
LISP_FUN(unix_link_mode)
{
   struct stat buf;
#ifdef MSDOS   
   int success = stat(get_c_string(ARG(0)), &buf);  /* No links under DOS */
#else
   int success = lstat(get_c_string(ARG(0)), &buf); 
#endif
   
   if (success == 0)
   {
      LOAD_FIXNUM(ARG(0), buf.st_mode, ARG(0));
   }
   else
   {
      LOAD_NIL(ARG(0));
   }
}

/*------------------------------------------------------------------------------
 * UNIX-READLINK name
 * Returns 1 or 2 values
 *----------------------------------------------------------------------------*/
LISP_FUN(unix_readlink)
{
   char buf[MAXPATHLEN];
#ifdef MSDOS
   int success = -1;            /* reading a symbolic link is always an error,
                                   because there are none */
   errno = EINVAL;              /* The named file is not a symbolic link. */
#else
   int success = readlink(get_c_string(ARG(0)), buf, sizeof(buf)-1);
#endif

   if(success >= 0)
   {
      buf[success] = '\0';
      make_string(ARG(0), buf);
   }
   else
   {
      LOAD_NIL(ARG(0));
      LOAD_SMALLFIXNUM(errno, OFFSET(mv_buf, 0));
      mv_count = 2;
   }
}
