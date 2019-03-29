/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   kernel.h: "Micro Industrial TRON" service calls.
   Provides partial compatibility with micro-itron 4.0 specification.
 */

#ifndef INCLUDE_GUARD_KERNEL_DEFINED
#define INCLUDE_GUARD_KERNEL_DEFINED 1

#include "itron.h"
#include "task.h"
#include "mailbox.h"

#define TA_HLNG 0x00 /* Start processing via high-level language interface. */
#define TA_ASM 0x01 /* Start procesisng via assembly language interface. */
#define TA_TFIFO 0x00 /* Task wait queue is in FIFO order. */
#define TA_TPRI 0x01 /* Task wait queue is in task priority order. */
#define TA_ACT 0x02 /* Task activate. */
#define TA_MFIFO 0x00 /* Message queue is in FIFO order. */
#define TA_MPRI 0x02 /* Message queue is in message priority order. */

#define TSK_SELF 0 /* Specify invoking task. */
#define TSK_NONE 0 /* No applicable task. */

#define TMIN_TPRI 1 /* Minimum task priority. */
#define TMAX_TPRI 127 /* Maximum task priority. */
#define TMIN_MPRI 1 /* Minimum message priority. */
#define TMAX_MPRI 127 /* Maximum message priority. */

#define TKERNEL_MAKER 0x0000 /* Kernel maker code: experimental system. */
#define TKERNEL_PRID /* Identification number of kernel. */
#define TKERNEL_SPVER 0x5400 /* Version number of ITRON specification. */
#define TKERNEL_PRVER /* Version number of kernel. */

/* Task. */
typedef struct c_tsk
{
  ATR tskatr;
  VP_INT exinf;
  FP task;
  PRI itskpri;
  SIZE stksz;
  VP stk;
  /* Other implementation specific field may be added. */
} T_CTSK;

typedef struct t_dinh
{
  ATR inhatr; /*  Interrupt handler attribute. */
  FP inthdr; /* Interrupt handler start address. */
} T_DINH;

typedef struct t_cmbx
{
  ATR mbxatr; /* Mailbox attribute. */
  PRI maxmpri; /* Maximum message priority. */
  VP mprihd; /* Start address of the area for message queue headers. */
} T_CMBX;

typedef struct mailmsg T_MSG;

/* Definitions for interrupt handlers.
   Single interrupt handler can be installed.

   Note: On some of the targets, this interrupt can be simulated via timer
   processed by scheduler, rather than being a true hardware interrupt.
*/
#define INHNO_TIMER 1

/* API: _m functions. These are the same than ITRON API, but functions 
   take task.h's */
struct task *get_task_m(ID taskid); /* Helper function (id->address). */
struct mailbox *get_mailbox_m(ID mboxid); /* Helper function (id->address). */

/* Create task.
   Note: Can also activate task with TA_ACT flag. */
static inline
ER cre_tsk(ID tskid, T_CTSK *tc);

/* Product specific API using struct task. */
ER cre_tsk_m(struct task *task, T_CTSK *tc);
  
/* Activate task. */
ER act_tsk_m(struct task *tsk);

/* Create mailbox. */
static inline
ER cre_mbx(ID mbxid, T_CMBX *pk_cmbx);

/* Send message to mailbox. */
static inline
ER snd_mbx(ID mbxid, T_MSG *pk_msg);

/* Receive message from mailbox (wait until message received). */
static inline
ER rcv_mbx(ID mbxid, T_MSG **ppk_msg);

/* Receive message from mailbox (poll). */
static inline
ER prcv_mbx(ID mbxid, T_MSG **ppk_msg);

/* Product specific API using struct mailbox. */
ER cre_mbx_m(struct mailbox *mailbox, T_CMBX *pk_cmbx);
ER snd_mbx_m(struct mailbox *mailbox, T_MSG *pk_msg);
ER rcv_mbx_m(struct mailbox *mailbox, T_MSG **pk_msg);
ER prcv_mbx_m(struct mailbox *mailbox, T_MSG **pk_msg);

/* Install interrupt handler. */
ER def_inh(INHNO inhno, T_DINH * pk_dinh);

/* Exit current task.
   Known bugs: Task cannot be reactivated with act_tsk() after exit with
   ext_tsk(). Exit task by existing the task main function if you need to
   reactivate the task. */
void ext_tsk(void);

/* Keep calling task_wait_all(); which keeps scheduler running the tasks. */
#define MULTITASK() do { task_wait_all(); } while(1) 

/* Run tasks until all tasks have exited.

   Note: Sample programs generally use this, because it is desirable that samples do exit. */
#define MULTITASK_UNTIL_ALL_EXIT() do { task_wait_all(); } while(0) 

/* Static inline functions:
   itron functions have been implemented as static inline functions.
   This implementation does not provide service call interface or static functions or configurator.
   Just these static inline function wrappers matching the mitron-400e.pdf interfaces. */
static inline
ER cre_tsk(ID tskid, T_CTSK *tc)
{
  struct task *tsk = get_task_m(tskid);
  ER ercd;

  if (tsk)
    {
      ercd = cre_tsk_m(tsk, tc);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;
}

static inline
ER act_tsk(ID tskid)
{
  struct task *tsk = get_task_m(tskid);
  ER ercd;

  if (tsk)
    {
      ercd = act_tsk_m(tsk);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;
}

static inline
ER cre_mbx(ID mbxid, T_CMBX *pk_cmbx)
{
  struct mailbox *mbx = get_mailbox_m(mbxid);
  ER ercd;

  if (mbx)
    {
      ercd = cre_mbx_m(mbx, pk_cmbx);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;  
}

static inline
ER snd_mbx(ID mbxid, T_MSG *pk_msg)
{
  struct mailbox *mbx = get_mailbox_m(mbxid);
  ER ercd;

  if (mbx)
    {
      ercd = snd_mbx_m(mbx, pk_msg);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;  
}

static inline
ER rcv_mbx(ID mbxid, T_MSG **ppk_msg)
{
  struct mailbox *mbx = get_mailbox_m(mbxid);
  ER ercd;

  if (mbx)
    {
      ercd = rcv_mbx_m(mbx, ppk_msg);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;  
}

static inline
ER prcv_mbx(ID mbxid, T_MSG **ppk_msg)
{
  struct mailbox *mbx = get_mailbox_m(mbxid);
  ER ercd;

  if (mbx)
    {
      ercd = prcv_mbx_m(mbx, ppk_msg);
    }
  else
    {
      ercd = E_ID;
    }
  return ercd;  
}

#endif /* INCLUDE_GUARD_KERNEL_DEFINED */
