/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Basic debug logging capability (printf under alternative name).
 */

/* #define USE_DEBUGF to print debugf() output to standard output stream. */

#ifndef INCLUDE_GUARD_DEBUGF_H
#define INCLUDE_GUARD_DEBUGF_H 1

#ifndef USE_DEBUGF

/* Default setting: Omit debugging logs. */
#define debugf(...) do { } while(0) /* Replace with e.g. printf to debug. */

#else

/* Building with USE_DEBUGF: Print debugf() logs to track execution of the system.
   Note: When this capability is enabled, software becomes larger and slower.
   Use the capability only to debug software or inspect its operation. */

#include "stdheader.h" /* Take all standard headers, for printf(). */
#define debugf printf /* Use printf for all debugf statements. */

#endif

#endif /* INCLUDE_GUARD_DEBUGF_H */
