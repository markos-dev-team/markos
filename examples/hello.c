/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Multithreaded hello world program.
   One task produces the message and sends it to another task that will print it.
 */

#include <stdio.h> /* printf(), etc. */
#include <stdlib.h> /* malloc(), free(), exit(), EXIT_SUCCESS etc. */

#include "markos.h"
#include "mailbox.h"

#include "itron.h"
#include "kernel.h"

/* Task and synchronization resources used by this program. */
#define PRODUCER_TASK_ID 1
#define PRINTER_TASK_ID 2
#define BOX_ID 1

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

/* Send a string to another task via mailbox.
   The string is sent one character at a time.
   (Mainly to demonstrate behavior of mailbox.) */
void send_str(const char *s)
{
  ER ercd;
  int f = 0;

  while(*s)
    {
      char *node = malloc(sizeof(T_MSG) + 1);
      if (node)
	{
	  node[sizeof(T_MSG)] = *s;
	  ercd = snd_mbx(BOX_ID, (T_MSG *) node);
	  if (ercd != E_OK)
	    {
	      f++;
	      /* Transmit error: delete message. */
	      free(node);
	    }
	}
      else
	{
	  f++;
	}
      s++;
    }
  if (f > 0)
    {
      /* Debugging aid. */
      printf("%d messages lost due to memory allocation errors.\n", f);
    }
}

void message_producer_task_func(VP_INT str)
{
  char x;

  task_yield(); /* Co-operatively multitask: surrender control for other tasks
                   of same priority. After yield, printer_task will print what
                   it finds in mailbox. */
  if (str)
    {
      send_str((const char *)str); /* Begin with first argument. */
    }
  printf("First set has been sent.\n");
  task_yield();
  send_str(", ");
  task_yield();
  send_str("World!\n");

  /* You may uncomment the following line to get notified when task exits. */
  /* printf("Exiting message_producer_task task\n"); */
}

void printer_task_func(VP_INT ignored)
{
  char ch = 0;
  ER ercd;

  while(ch != '\n')
    {
      T_MSG *msg = NULL;

      ercd = rcv_mbx(BOX_ID, &msg);
      if (ercd == E_OK && msg != NULL)
	{
	  ch = *(const char *) (msg + 1); /* Access struct past the header. */
	  printf("%c", ch);
	  free(msg);
	}
    }

  /* You may uncomment the following line to get notified when task exits. */
  /* printf("Exiting message_printer_task task\n"); */
  ext_tsk();
  printf("ERROR: Task failed to terminate\n");
}

int main(void)
{
  ER ercd;
  char x;

  T_CMBX m_box = {
    TA_TPRI | TA_MFIFO,
    1,
    NULL,
  };
  
  T_CTSK t_producer = {
    TA_HLNG | TA_ACT,
    (VP_INT) "Hello",
    &message_producer_task_func,
    0,
    3072,
    NULL,
  };

  T_CTSK t_printer = {
    TA_HLNG | TA_ACT,
    (VP_INT) NULL,
    &printer_task_func,
    0,
    3072,
    NULL,
  };

  /* Initialize mailbox for intertask communication. */
  ercd = cre_mbx(BOX_ID, &m_box);
  if (ercd == E_OK)
    {
      ercd = cre_tsk(PRODUCER_TASK_ID, &t_producer);
    }
  if (ercd == E_OK)
    {
      ercd = cre_tsk(PRINTER_TASK_ID, &t_printer);
    }

  if (ercd != E_OK)
    {
      goto exit_error;
    }

  /* Run multitasking dispatcher and wait for tasks to finish. */
  MULTITASK_UNTIL_ALL_EXIT();
  printf("Message has been printed: All tasks are finished.\n");
  exit(EXIT_SUCCESS);
 exit_error:
  printf("Initialization failure.\n");
  printf("Maybe malloc API is not implemented or there is not enough ram.\n");
  exit(EXIT_FAILURE);
}
