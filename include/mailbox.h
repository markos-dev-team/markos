/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   MAILBOX for communication between tasks.
 */

#ifndef INCLUDE_GUARD_MAILBOX_DEFINED
#define INCLUDE_GUARD_MAILBOX_DEFINED 1

#include "fifo.h"
#include "task_list.h"

struct mailmsg
{
  struct fifo_node node;
  /* Put actual message here. */
};

struct mailbox
{
  struct fifo_header box;
  struct task_list waiting;
};

void mailbox_init(struct mailbox *mbox);
void mailbox_send(struct mailbox *mbox, struct mailmsg *msg);
struct mailmsg *mailbox_receive(struct mailbox *mbox);
static inline int mailbox_is_empty(struct mailbox *mbox);
static inline void *mailmsg_data(struct mailmsg *msg);

/* Inline functions. */
static inline void *mailmsg_data(struct mailmsg *msg)
{
  return (void *)&msg[1];
}

static inline int mailbox_is_empty(struct mailbox *mbox)
{
  return fifo_is_empty(&mbox->box);
}

#endif /* INCLUDE_GUARD_MAILBOX_DEFINED */
