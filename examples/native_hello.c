/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Multithreaded hello world program.
   One task produces the message and sends it to another task that will
   print it. Example application uses markos.h native API.
 */

#include <stdio.h> /* printf(), etc. */
#include <stdlib.h> /* malloc(), free(), exit(), EXIT_SUCCESS etc. */

#include "markos.h"

/* Task and synchronization resources used by this program. */
struct task message_producer_task;
struct task printer_task;
struct mailbox box;

/* System initialization function. Typically empty.
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
  while(*s)
    {
      struct mailmsg *node = malloc(sizeof(struct mailmsg) + 1);
      if (node)
	{
	  *(char *)mailmsg_data(node) = *s;
	  mailbox_send(&box, node);
	}
      s++;
      /* TODO: Add error handling. */
    }
}

void message_producer_task_func(void *arg_ignored)
{
  char x;

  task_yield(); /* Co-operatively multitask: surrender control for other tasks
                   of same priority. After yield, printer_task will print what
                   it finds in mailbox. */

  send_str("Hello");
  task_yield();
  send_str(", ");
  task_yield();
  send_str("World!\n");

  /* You may uncomment the following line to get notified when task exits. */
  /* printf("Exiting message_producer_task task\n"); */
}

void printer_task_func(void *arg_ignored)
{
  char ch = 0;

  while(ch != '\n')
    {
      struct mailmsg *node = mailbox_receive(&box);
      if (node)
	{
	  ch = *(const char *) mailmsg_data(node);
	  printf("%c", ch);
	  free(node);
	}
      /* TODO: Error handling. */
    }

  /* You may uncomment the following line to get notified when task exits. */
  /* printf("Exiting message_printer_task task\n"); */
}

int main(void)
{
  /* Initialize mailbox for intertask communication. */
  mailbox_init(&box);
 
  /* Setup and start first task. */
  message_producer_task.name="message";
  message_producer_task.start_address = &message_producer_task_func;
  message_producer_task.stacksz = 3072;
  if (task_start(&message_producer_task, 1) != 0)
    {
      goto exit_error;
    }

  /* Setup and start second task. */
  printer_task.name="printer";
  printer_task.start_address = &printer_task_func;
  printer_task.stacksz = 3072;
  if (task_start(&printer_task, 1) != 0)
    {
      goto exit_error;
    }

  /* Run multitasking dispatcher and wait for tasks to finish. */
  task_wait_all();
  printf("Message has been printed: All tasks are finished.\n");
  exit(EXIT_SUCCESS);
 exit_error:
  printf("Initialization failure.\n");
  printf("Maybe malloc API is not implemented or there is not enough ram.\n");
  exit(EXIT_FAILURE);
}
