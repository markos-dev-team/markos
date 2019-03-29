/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Definition of context and functions to interact with it.
   Context stores values of the registers, and allows switching between register sets.
   In other words, context is the core of implementation for lightweight threads.

   This is very low-level API, for more convenient abstraction layer,
   these functions should generally be called via e.g. task.h.

   The API is intentionally similar to ucontext.h, but ucontext.h is not available on
   all platforms, so a new abstraction is provided.
   To support new target platforms, it is required to make new implementation of this
   API or tweak one of implementations (context_ucontext.c, context_setjmp.c).
 */

#ifndef INCLUDE_GUARD_CONTEXT_H
#define INCLUDE_GUARD_CONTEXT_H 1

#include "stdheader.h"

struct context; /* Opaque structure, details provided in context_impl.h. */

struct context *context_allocate(void);
void context_free(struct context *ctx);
void context_swap(struct context *ctx_now, struct context *ctx_next);
int context_make(struct context *ctx,
		 struct context *ctx_main,
		 void (*func)(void), size_t stacksz);

#endif /* INCLUDE_GUARD_CONTEXT_H */
