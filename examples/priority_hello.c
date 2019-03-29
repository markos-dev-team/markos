/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Multithreaded hello world program.
   Threads are launched at different priorities.
 */

#include <stdio.h> /* printf(), etc. */
#include <stdlib.h> /* exit(), EXIT_SUCCESS etc. */

#include "markos.h"
#include "mailbox.h"

#include "itron.h"
#include "kernel.h"

/* Use macros to provide names for the tasks. */
#define P1_TASK_ID 1
#define P2_TASK_ID 3
#define P3_TASK_ID 2

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
  printf("%s: Starting.\n", (char *)string);
  task_yield();
  printf("%s: Running.\n", (char *)string);
  task_yield();
  printf("%s: Ending.\n", (char *)string);
}

int main(void)
{
  ER ercd;
  char x;
  T_CTSK t_printer1 = {
    TA_HLNG | TA_ACT,
    (VP_INT) "P1",
    &printer_task_func,
    1,
    3072,
    NULL,
  };

  T_CTSK t_printer2 = {
    TA_HLNG | TA_ACT,
    (VP_INT) "P2",
    &printer_task_func,
    2,
    3072,
    NULL,
  };

  T_CTSK t_printer3 = {
    TA_HLNG | TA_ACT,
    (VP_INT) "P3",
    &printer_task_func,
    3,
    3072,
    NULL,
  };

  ercd = cre_tsk(P1_TASK_ID, &t_printer1);
  ercd |= cre_tsk(P2_TASK_ID, &t_printer2);
  ercd |= cre_tsk(P3_TASK_ID, &t_printer3);

  if (ercd != E_OK) /* One of return codes was not E_OK. */
    {
      printf("Initialization failure.\n");
      exit(EXIT_SUCCESS);      
    }
  
  /* Run multitasking dispatcher and wait for tasks to finish. */
  MULTITASK_UNTIL_ALL_EXIT();
  printf("Messages has been printed: All tasks are finished.\n");
  exit(EXIT_SUCCESS);
}
