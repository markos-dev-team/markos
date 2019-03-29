/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   itron.c: "Micro Industrial TRON" basic definitions file.
   Provides itron API support. (Translates mitron service calls to
   task.* and others.).
   Also allocates data/bss storage needed to store kernel memory
   structures.
 */

#include "itron.h"
#include "kernel.h"
#include "kernel_id.h"
#include "irq.h"

/* Static data allocations. */
static struct task tasks[ITRON_TASKS];
static struct mailbox mailboxes[ITRON_MAILBOXES];

/* Needs to find id of current task.

   TODO: Consider adding a function, to avoid reference to this structure or
   changing task_end API. */
struct task_list ready;

struct task *get_task_m(ID taskid)
{
  if (taskid >= 1 && taskid <= ITRON_TASKS)
    {
      return &tasks[taskid - 1];
    }
  return NULL;
}

struct mailbox *get_mailbox_m(ID mailboxid)
{
  if (mailboxid >= 1 && mailboxid <= ITRON_MAILBOXES)
    {
      return &mailboxes[mailboxid - 1];
    }
  return NULL;
}

/* Task creation. */
ER cre_tsk_m(struct task *task, T_CTSK *tc)
{
  task->name="itron"; /* All itron tasks have the same name.  */
  task->arg = (void *) tc->exinf;
  task->start_address = (void(*)(void *)) tc->task;
  task->priority = tc->itskpri;
  task->stacksz = tc->stksz;
  task->stack = tc->stk;
  task->ctx = NULL;
  task->next = NULL;
  if (task_start(task, (tc->tskatr & TA_ACT) == TA_ACT) != 0)
    {
      return E_NOMEM; /* Failures are mainly due to memory allocation. */
    }

  return E_OK;
}

/* Activate task */
ER act_tsk_m(struct task *tsk)
{
  task_activate(tsk);
  return E_OK;
}

/* Install interrupt handler. */
ER def_inh(INHNO inhno, T_DINH * pk_dinh)
{
  if (inhno == INHNO_TIMER)
    {
      irq_set_handler((irq_handler_func) pk_dinh->inthdr);
    }
  return E_OK;
}

/* ext_tsk can be replaced by a return from the main routine. */
void ext_tsk(void)
{
  struct task *current_tsk = ready.first;

  task_end(current_tsk);
}

ER cre_mbx_m(struct mailbox *mailbox, T_CMBX *pk_cmbx)
{
  ER ercd = E_PAR;

  if (pk_cmbx->mbxatr == (TA_TPRI | TA_MFIFO)) /* Supports only these flags. */
    {
      mailbox_init(mailbox);
      ercd = E_OK;
    }
  return ercd;
}

ER snd_mbx_m(struct mailbox *mailbox, T_MSG *pk_msg)
{
  ER ercd = E_PAR;

  if (pk_msg)
    {
      mailbox_send(mailbox, pk_msg);
      ercd = E_OK;
    }
  return ercd;
}

ER rcv_mbx_m(struct mailbox *mailbox, T_MSG **ppk_msg)
{
  ER ercd;
  struct mailmsg *msg;

  msg = mailbox_receive(mailbox);

  if (msg)
    {
      *ppk_msg = msg;
      ercd = E_OK;
    }
  else
    {
      ercd = E_NOEXS; /* Not known what error occurred. */
    }

  return ercd;
}


ER prcv_mbx_m(struct mailbox *mailbox, T_MSG **ppk_msg)
{
  ER ercd;

  if (mailbox_is_empty(mailbox))
    {
      ercd = E_TMOUT;
    }
  else
    {
      ercd = rcv_mbx_m(mailbox, ppk_msg);
    }
  return ercd;
}
