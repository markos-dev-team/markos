/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   mailbox.c: Combines fifo with waiting tasks list for convenient
   message passing between tasks.
 */
#include "mailbox.h"
#include "task_list.h"
#include "task.h"

void mailbox_init(struct mailbox *mbox)
{
  fifo_init(&mbox->box);
  task_list_init(&mbox->waiting);
}

void mailbox_send(struct mailbox *mbox, struct mailmsg *msg)
{
  struct task *waiting;

  fifo_in(&mbox->box, &msg->node);
  waiting = task_list_first(&mbox->waiting);
  if (waiting)
    {
      /* Move first waiting task to active state. */
      task_list_delete(&mbox->waiting, waiting);
      task_exit_waiting(waiting);
    }
}

struct mailmsg *mailbox_receive(struct mailbox *mbox)
{
  struct task *task;
  struct fifo_node *out;

  do
    {
      /* Check if there is a message already available. */
      out = fifo_out(&mbox->box);
      if (out != NULL)
	{
	  return (struct mailmsg *) out;
	}

      /* Empty mailbox:
	 Put task sleeping and wait for it to get reactivated. */
      task = task_enter_waiting();
      if (task)
	{
	  task_list_enqueue(&mbox->waiting, task);
	  task_schedule(task);
	}
      /* Now task has been activated. maybe there is a message now. */
    }
  while(1); /* Wait as long as it takes before a message arrives. */
}
