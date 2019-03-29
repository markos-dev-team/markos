/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Wrapper for context API using setjmp.h.
   setjmp.h (setjmp/longjmp) provides store and retrieve of
   registers, for performing transfer of execution.
   This is almost all that is needed for context.h, except
   context.h also requires allocation and setting of stack.
 */
#include "context.h"
#include "context_impl.h"
#include "stdheader.h"
#include "debugf.h"

/* This code is only for setjmp.h */
#ifdef USE_SETJMP
#include <setjmp.h>
#include <stdlib.h>

/* Provide equivalent of ucontext via setjmp/longjmp. */

struct context *context_allocate(void)
{
  return calloc(sizeof(struct context), 1);
}

void context_free(struct context *ctx)
{
  free(ctx);
}

void task_begin(void);

int context_make(struct context *ctx,
		 struct context *ctx_main,
		 void (*func)(void), size_t stacksz)
{
  void *stack = malloc(stacksz);

  if (!stack)
    {
      return -1; /* Return with error. */
    }

  if (setjmp(ctx->jmp_buf))
    {
      /* after longjmp, in new task. */
      /* (note: stack is the new stack, so only global memory or values 
         coming in via registers are safe to use.) */
      task_begin(); /* Assume the function is always task_begin(). */
    }
  else
    {
      /* after setjmp */

      /* Alter registers stored with setjmp, so that the new stack is stored
	 in stack pointer register. On the currently supported platforms,
         stack grows downwards, so the register needs to be loaded with
         the address at end of stack area, rather than the beginning of stack
         area. */
#if defined __linux__ && defined __x86_64__
      /* Linux on x86-64 */
      unsigned long *regs = (unsigned long *)&ctx->jmp_buf;
      regs[3] = stacksz + (unsigned long) stack; /* 3 matches x86-64 sp reg in setjmp. */
#else
      /* Embedded ARM (with newlib). */
      ctx->jmp_buf[8] = stacksz + (long) stack; /* 8 matches ARM sp reg in setjmp. */
#endif
    }
  return 0;
}

void context_swap(struct context *ctx_now, struct context *ctx_next)
{
  if (setjmp(ctx_now->jmp_buf))
    {
      /* longjmp. task switch has been successful.
         Now tsk_next is executing. */
    }
  else
    {
      /* setjmp. task state has been stored, but
         tsk_now is still running. Transition to next task. */
      longjmp(ctx_next->jmp_buf, 1);
    }
  debugf("CONTEXT SWITCH FINISHED\n");
}

#endif
