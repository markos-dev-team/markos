/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Hello world program, that activates itself multiple times.
 */

#include <stdio.h> /* printf(), etc. */
#include <stdlib.h> /* exit(), EXIT_SUCCESS etc. */

#include "markos.h"
#include "mailbox.h"

#include "itron.h"
#include "kernel.h"

/* Use macros to provide names for the tasks. */
#define PRINTER_TASK_ID 1

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

void printer_task_func(VP_INT string)
{
  static int wake_up_count = 1;

  printf("%s", (const char *) string);
  if (wake_up_count > 0)
    {
      act_tsk(PRINTER_TASK_ID);
      wake_up_count--; /* Reactivate. */
    }
  }

int main(void)
{
  ER ercd;
  char x;
  T_CTSK t_printer = {
    TA_HLNG | TA_ACT,
    (VP_INT) "hello, world!\n",
    &printer_task_func,
    0,
    3072,
    NULL,
  };
  ercd = cre_tsk(PRINTER_TASK_ID, &t_printer);
  
  /* Run multitasking dispatcher and wait for tasks to finish. */
  MULTITASK_UNTIL_ALL_EXIT();
  /* manual reactivate: */
  act_tsk(PRINTER_TASK_ID);
  MULTITASK_UNTIL_ALL_EXIT();
  printf("Messages has been printed: All tasks are finished.\n");
  exit(EXIT_SUCCESS);
}
