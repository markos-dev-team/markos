/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Include C99 standard headers.
 */

#ifndef INCLUDE_GUARD_STDHEADER_DEFINED
#define INCLUDE_GUARD_STDHEADER_DEFINED 1

/* ISO C99 headers that the rest of the software depends on. 

   In case the target system does not provide these headers,
   you will need to build replacements, that provide
   prototypes of basic functions like malloc() and
   basic types like size_t.

   In case system does not offer these, functions, use small
   libc like newlib or build one of your own.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
 
#endif /* INCLUDE_GUARD_STDHEADER_DEFINED */
