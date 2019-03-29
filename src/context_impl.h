/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Select context implementation suitable for target system.
 */

#ifndef INCLUDE_GUARD_CONTEXT_IMPL_H
#define INCLUDE_GUARD_CONTEXT_IMPL_H 1

#include "context.h"

/* #define USE_UCONTEXT to use ucontext.h */
/* #define USE_SETJMP to use setjmp.h */

#if !defined USE_UCONTEXT && !defined USE_SETJMP
#if defined __linux__
/* On linux use ucontext.
   The API allows straight forward context swithing and easier set-up of alternative stack. */
#define USE_UCONTEXT
#include <ucontext.h>

struct context
{
  ucontext_t uc;
};

#else
/* Otherwise use setjmp/longjmp. */
#define USE_SETJMP
#include <setjmp.h>

struct context
{
  jmp_buf jmp_buf;
};
     
#endif
#endif /* USE_CONTEXT or USE_SETJMP */

#endif /* INCLUDE_GUARD_CONTEXT_IMPL_H */
