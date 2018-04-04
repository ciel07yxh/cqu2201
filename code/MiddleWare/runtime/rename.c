/*******************
 *
 * Copyright 1998-2010 IAR Systems AB. 
 *
 * This is a template implementation of the "rename" function of the
 * standard library.  Replace it with a system-specific
 * implementation.
 *
 * The "rename" function should rename file from "old" to "new".  It
 * should return 0 on success and nonzero on failure.
 *
 ********************/

#include <stdio.h>

_STD_BEGIN

#pragma module_name = "?rename"

#pragma diag_suppress = Pe826

int rename(const char *old, const char *new)
{
  return (-1);
}

_STD_END
