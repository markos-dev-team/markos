/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   task.c: Core of the kernel.
   Task functions for adding/removing tasks (threads), and scheduler.
 */

#include "task.h"
#include "task_list.h"
#include "irq.h"
#include "stdheader.h"
#include "debugf.h"

struct task_list ready;
struct task dispatcher_task;
static struct task *new_task;
static int task_count;

static void task_switch(struct task *tsk_now, struct task *tsk_next);

static inline int need_scheduler_task(void);

void task_begin(void)
{
  volatile struct task *tsk = new_task;

  debugf("task start %p\n", tsk);
  tsk->state = RUNNING;
  while(1)
    {
      (*tsk->start_address)(tsk->arg);
      task_end((struct task *)tsk); /* task execution ended. */
      /* The task may start again due to reactivation */
      debugf("task reactivate %p\n", tsk);
    }
}

int task_start(struct task *tsk, int activate)
{
  int res;
  
  if (need_scheduler_task() == -1)
    {
      return -1;
    }
  if (tsk->ctx == NULL)
    {
      tsk->ctx = context_allocate();
      if (tsk->ctx == NULL)
	{
	  debugf("Memory allocation failure: task context.\n");
	  return -1;
	}
    }
  /* Setup context for task. */
  res = context_make(tsk->ctx, dispatcher_task.ctx, task_begin, tsk->stacksz);

  if (res == 0)
    {
      /* Task creation successful. Add task to the system. */
      if (activate)
	{
	  task_list_enqueue(&ready, tsk);
	  task_count++;
	}
      tsk->state = activate ? READY : DORMANT;
      tsk->activate = 0;
      debugf("Task added: %s (%s)\n", tsk->name, task_state_str(tsk));
    }
  else
    {
      debugf("Task creation failed: cannot make context.\n");
    }
  return res;
}

void task_end(struct task *tsk)
{
  debugf("End of task execution.\n");
  task_list_delete(&ready, tsk);
  debugf("Task Ending: %s\n", tsk->name);
 /* Handle queued activation requests */
  if (tsk->activate)
    {
      debugf("Reactivating task: %s\n", tsk->name);
      tsk->activate--;
      task_list_enqueue(&ready, tsk);
      tsk->state = READY;
    }
  else
    {
      tsk->state = DORMANT;
      task_count--;
    }
  task_switch(tsk, &dispatcher_task);
  /* Execution returns here if task is reactivated. */
}

void task_wait_all(void)
{
  /* This loop is scheduler.
     Scheduler will always let the most important task to run.
     If there are multiple tasks at the same priority, those will run in round-robin.
     Scheduling is co-operative, i.e. task will need to transfer control back to scheduler
     for other tasks to have their turn. */
  irq_check_time();

  while(task_count)
    {
      while(ready.first)
	{
	  irq_check_time();
	  debugf("DISPATCHING: current first task: %s:%p (QUEUE LEN: %d)\n",
		 ready.first ? ready.first->name : "", ready.first, task_list_len(&ready));
	  new_task = ready.first;
	  task_switch(&dispatcher_task, ready.first);
	}
      /* ready.first is NULL.
         The system should wait until next event, e.g. interrupt that may wake up processes. */
      /* For now, just do nothing. */
      assert(task_count >= 0);
    }
  /* Now quit may take place if all tasks are sleeping. */
  debugf("No ready to run tasks. Exiting\n");
}

void task_yield(void)
{
  if (ready.first)
    {
      struct task *tsk = ready.first;

      task_list_delete(&ready, tsk);
      task_list_enqueue(&ready, tsk);
      tsk->state = READY;
      debugf("Task yielding: %s -> %s.\n", tsk ? tsk->name: "(null)",
	     ready.first? ready.first->name : "(null)");
      task_switch(tsk, &dispatcher_task);
    }
}

/* Common function for moving task to ready list and
   then invoking scheduler to decide what task to run. */
static void task_enter_ready_and_schedule(struct task *tsk)
{
  struct task *current_tsk;

  current_tsk = ready.first;
  task_list_enqueue(&ready, tsk);
  tsk->state = READY;
  if (current_tsk)
    {
      current_tsk->state = READY;
      task_switch(current_tsk, &dispatcher_task); /* Make task run if it is high priority. */
    }
}

void task_activate(struct task *tsk)
{
  if (tsk->state != DORMANT)
    {
      /* Task is already in some list (waiting or ready).
         Increase activation requests. */
      tsk->activate++;
      return;
    }
  task_count++;
  task_enter_ready_and_schedule(tsk);
}

void task_exit_waiting(struct task *tsk)
{
  assert(tsk->state == WAITING); /* Task was in waiting state. */
  task_enter_ready_and_schedule(tsk);
}  

struct task *task_enter_waiting(void)
{
  struct task *tsk = ready.first;

  task_list_delete(&ready, tsk);
  tsk->state = WAITING;
  return tsk;
}

void task_schedule(struct task *current_tsk)
{
  task_switch(current_tsk, &dispatcher_task); /* Select what task to run. */
  /* Scheduler returned, continue this task. */
}

/* Other helper functions: Task switching. */

static void task_switch(struct task *tsk_now, struct task *tsk_next)
{
  debugf("CONTEXT SWITCH LEAVE %p[%p]:%s:%s (to: %p[%p]:%s:%s)\n",
	 tsk_now, tsk_now->ctx, task_name_str(tsk_now), task_state_str(tsk_now),
	 tsk_next, tsk_next->ctx, task_name_str(tsk_next), task_state_str(tsk_next));
  assert(tsk_next->state == READY || tsk_next == &dispatcher_task); /* Ready tasks have ready as their state. */
  tsk_next->state = RUNNING;
  context_swap(tsk_now->ctx, tsk_next->ctx);
}

/* Allocate scheduler task. Scheduler task is needed to schedule. */
static inline int need_scheduler_task(void)
{
  if (dispatcher_task.ctx == NULL)
    {
      dispatcher_task.name = "D";
      dispatcher_task.ctx = context_allocate();
      if (dispatcher_task.ctx == NULL)
	{
	  debugf("Memory allocation failure: dispatcher context.\n");
	  return -1;
	}
    }
  return 0;
}

