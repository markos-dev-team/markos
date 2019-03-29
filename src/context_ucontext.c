/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Wrapper for context API using similar ucontext.h.
   ucontext.h is available on System V -like environments, such as Linux.

   Because ucontext.h is used to change location of stack, this code may not
   interoperate with other implementations of threads, such as pthreads.
 */
#include "context.h"
#include "context_impl.h"
#include "stdheader.h"
#include "debugf.h"

/* This code is only for ucontext.h */
#ifdef USE_UCONTEXT

struct context *context_allocate(void)
{
  return calloc(sizeof(struct context), 1);
}

void context_free(struct context *ctx)
{
  free(ctx);
}

int context_make(struct context *ctx,
		 struct context *ctx_main,
		 void (*func)(void), size_t stacksz)
{
  void *stack = malloc(stacksz);

  if (!stack)
    {
      return -1; /* Return with error. */
    }

  getcontext(&ctx->uc);
  /* Note: Definition ss_sp member varies between systems.
     This statement assumes it wants base of memory region allocated for
     stack. Instead, some systems require ss_sp to be the same value as stack
     pointer. */
  ctx->uc.uc_stack.ss_sp = stack;
  ctx->uc.uc_stack.ss_size = stacksz;
  ctx->uc.uc_link = ctx_main ? &ctx_main->uc : NULL;
  makecontext(&ctx->uc, (void (*)()) func, 0);
  return 0;
}

void context_swap(struct context *ctx_store, struct context *ctx_use)
{
  debugf("Leaving ctx %p\n", ctx_store);
  swapcontext(&ctx_store->uc, &ctx_use->uc);
  debugf("Back at ctx %p\n", ctx_store);
  /* Control reaches this point if context_swap(*, ctx_store) is executed. */
}

#endif
