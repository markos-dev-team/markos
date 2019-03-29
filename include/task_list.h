#include "stdheader.h"

#ifndef INCLUDE_GUARD_TASK_LIST_DEFINED
#define INCLUDE_GUARD_TASK_LIST_DEFINED 1

struct task;
struct task_list
{
  struct task *first;
};

static inline void task_list_init(struct task_list *list);
static inline int task_list_is_empty(struct task_list *list);
static inline struct task *task_list_first(struct task_list *list);
int task_list_len(const struct task_list *list);
void task_list_enqueue(struct task_list *list, struct task *task);
void task_list_delete(struct task_list *list, struct task *task);

/* Implementations of inlined functions.
   This function is typically inlined, because the operation is typically simpler than function call. */
static inline void task_list_init(struct task_list *list)
{
  list->first = NULL;
}

static inline int task_list_is_empty(struct task_list *list)
{
  return list->first == NULL;
}
 
static inline struct task *task_list_first(struct task_list *list)
{
  return list->first;
}
 
#endif /* INCLUDE_GUARD_TASK_LIST_DEFINED */

