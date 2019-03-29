/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   fifo.c: Implementation of first-in first-out queue.
   The implementation is based on doubly-linked list.
   Most of the operations are free of branches.
   All list operations are O(1) implementations.
 */

#include "fifo.h"
#include "stdheader.h"

void fifo_init(struct fifo_header *list)
{
  list->first = (struct fifo_node *)&list->null;
  list->null = NULL;
  list->last = (struct fifo_node *)list;
}

void fifo_in(struct fifo_header *list,
	     struct fifo_node *new)
{
  new->prev = list->last;
  new->next = (struct fifo_node *)&list->null;
  list->last->next = new;
  list->last = new;
}

int fifo_is_empty(struct fifo_header *list)
{
  return list->last == (struct fifo_node *)list;
}

struct fifo_node *fifo_out(struct fifo_header *list)
{
  struct fifo_node *node;

  node = list->first;
  if (node != (struct fifo_node *)&list->null)
    {
      node->prev->next = node->next;
      node->next->prev = node->prev;
      return node;
    }
  else
    {
      return NULL;
    }
}

