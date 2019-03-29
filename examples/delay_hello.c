/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Multithreaded hello world program.
   One task produces the message and sends it to another task that will print it.
 */

#include <stdio.h> /* printf(), etc. */
#include <stdlib.h> /* malloc(), free(), exit(), EXIT_SUCCESS etc. */

#include "markos.h"
#include "mailbox.h"
#include "irq.h"

#include "itron.h"
#include "kernel.h"

/* Task and synchronization resources used by this program. */
struct task printer_task;

/* System initialization function.
   This function is required for arm-none-eabi start up code. */
void SystemInit(void)
{
  char *mallocpool;

  /* This temporary large large allocation is to ensure OS will perform sbrk()
     before starting threads. This is needed, because otherwise malloc()
     may misoperate in threads (due to low stack pointer). */
  mallocpool = malloc(16384);
  free(mallocpool);
}

static volatile int finished;
void printer_task_func(VP_INT string)
{
  printf("%s", (const char *) string);
  finished = 1;
}

void timer_countdown_func(VP_INT stub)
{
  static int countdown = 200;

  if (countdown > 0)
    {
      countdown--;

      if (countdown == 0)
	{
	  act_tsk_m(&printer_task);
	}
    }
}

int main(void)
{
  ER ercd;
  char x;
  T_CTSK t_printer = {
    TA_HLNG,
    (VP_INT) "2 seconds delayed message: hello, world!\n",
    &printer_task_func,
    0,
    3072,
    NULL,
  };
  ercd = cre_tsk_m(&printer_task, &t_printer);

  T_DINH i_timer_countdown = {
    0,
    timer_countdown_func
  };

  ercd = def_inh(INHNO_TIMER, &i_timer_countdown);

  /* Run multitasking dispatcher and wait for tasks to finish. */
  while(finished == 0)
  {
    task_wait_all();
  }
  printf("Messages has been printed: All tasks are finished.\n");
  exit(EXIT_SUCCESS);
}
