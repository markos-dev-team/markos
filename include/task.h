/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   itron.h: "Micro Industrial TRON" basic definitions file.
   Provides partial compatibility with micro-itron 4.0 specification.
 */

#ifndef INCLUDE_GUARD_TASK_DEFINED
#define INCLUDE_GUARD_TASK_DEFINED 1

#include "context.h"

enum task_state
{
  RUNNING,
  READY,
  WAITING,
  DORMANT
};

struct task
{
  enum task_state state;
  const char *name;
  void *arg;
  void (*start_address)(void *);
  int priority;
  size_t stacksz;
  void *stack;
  int activate;
  struct task *next;
  struct context *ctx;
};

/* Task API for controlling tasks. */
int task_start(struct task *tsk, int activate);
void task_end(struct task *tsk);
void task_wait_all(void);
void task_yield(void);
void task_activate(struct task *tsk);
struct task *task_enter_waiting(void);
void task_exit_waiting(struct task *tsk);
void task_schedule(struct task *current_tsk);
static inline const char *task_state_str(const struct task *tsk);
static inline const char *task_name_str(const struct task *tsk);

/* Internal function that is first thing executed within a task.
   This function takes care of clean exit, in case task's function exits. */
void task_begin(void);


/* Implementations of inline functions: Helper functions for debugging
   have been implemented as inline.
*/
static inline const char *task_name_str(const struct task *tsk)
{
  if (tsk && tsk->name)
    {
      return tsk->name;
    }
  return "(no name)";
}
static inline const char *task_state_str(const struct task *tsk)
{
  extern struct task dispatcher_task;

  if (tsk == &dispatcher_task)
    {
      return "DISPATCHER";
    }
  
  switch(tsk->state)
    {
    case RUNNING:
      return "RUNNING";
    case READY:
      return "READY";
    case WAITING:
      return "WAITING";
    case DORMANT:
      return "DORMANT";
    }

  /* The state should have been one of the above. */
  return "???";
}

#endif /* INCLUDE_GUARD_TASK_DEFINED */
