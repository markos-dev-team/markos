/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Interrupt handling: Emulates interrupt using clock().
 */

#include "irq.h"
#include "stdheader.h"

/* The irq can be turned off with NO_IRQ_TICK preprocessor define.
   This can be used e.g. on platforms where clock() function is
   not available. */

#ifndef NO_IRQ_TICK
#include <time.h>
#endif /* NO_IRQ_TICK */

static void (*irq_handler)(void *) = IRQ_DEFAULT_HANDLER;
void irq_set_handler(irq_handler_func handler)
{
  irq_handler = handler;
}

irq_handler_func irq_get_handler(void)
{
  irq_handler_func handler = irq_handler;

  return handler;
}

void irq_check_time(void)
{
#ifndef NO_IRQ_TICK
  static int first = 1;
  static clock_t kernel_clock;
  clock_t kernel_clock_new;

  kernel_clock_new = clock();

  if (first == 1)
    {
      kernel_clock = kernel_clock_new;
      first = 0;
    }
  else
    {
      while ((unsigned long) (kernel_clock_new - kernel_clock) >=
	     (unsigned long) (CLOCKS_PER_SEC / IRQ_FREQUENCY_HZ))
	{
	  kernel_clock += CLOCKS_PER_SEC / IRQ_FREQUENCY_HZ;

	  if (irq_handler != IRQ_DEFAULT_HANDLER)
	    {
	      (*irq_handler)((void *) 0);
	    }
	}
    }
#else
  /* irq_check_time() function does nothing. */
#endif /* NO_IRQ_TICK */
}
