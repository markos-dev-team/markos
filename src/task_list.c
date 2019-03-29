/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   task_list.c: Implementation of list for tasks.
   Task list is single linked list, with operations taking
   O(n) time. Other data structures than single linked list
   offer better performance for large amount of tasks,
   but this kernel has been designed for only a small amounts of tasks.
 */

#include "task.h"
#include "task_list.h"
#include "stdheader.h"
#include "debugf.h"

/* Smallest priority first. */
void task_list_enqueue(struct task_list *list, struct task *task)
{
  struct task *prev_task = list->first;

  /* Put the task first. */
  if (prev_task == NULL || prev_task->priority > task->priority)
    {
      task->next = list->first;
      list->first = task;
      return;
    }

  while(prev_task->next && prev_task->next->priority <= task->priority)
    {
      prev_task = prev_task->next;
    }

  task->next = prev_task->next;
  prev_task->next = task;
}

void task_list_delete(struct task_list *list, struct task *task)
{
  struct task *prev_task = list->first;

  if (prev_task == task)
    {
      debugf("Removing first %p, will point %p\n", prev_task, task->next);
      list->first = task->next;
      task->next = NULL;
      return;
    }
  while(prev_task != NULL && prev_task->next != task)
    {
      prev_task = prev_task->next;
    }
  if (prev_task)
    {
      prev_task->next = task->next;
      task->next = NULL;
    }
  else
    {
      /* Task is not in the list. */
    }
}

int task_list_len(const struct task_list *list)
{
  struct task *tsk;
  int count = 0;

  tsk = list->first;
  while(tsk)
    {
      tsk = tsk->next;
      count ++;
    }

  return count;
} 

