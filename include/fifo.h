/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   FIFO: First In - First Out.
   Doubly linked list-based implementation for FIFO.
   Most list operations are O(1) and branch-free.
 */

#ifndef INCLUDE_GUARD_FIFO_DEFINED
#define INCLUDE_GUARD_FIFO_DEFINED 1

#include "stdheader.h"

/* Node: This is linkage part of node, the data follows after these fields. */
struct fifo_node
{
  struct fifo_node *next;
  struct fifo_node *prev;
};

/* This is list header.
   The list is constructed of two nodes that have been overlapped.
   This old trick allows efficient implementation for node insert/remove
   (avoids special cases). */
struct fifo_header
{
  struct fifo_node *first;
  struct fifo_node *null;
  struct fifo_node *last;
};

void fifo_init(struct fifo_header *list);

int fifo_is_empty(struct fifo_header *list);

void fifo_in(struct fifo_header *list,
	     struct fifo_node *new);

struct fifo_node *fifo_out(struct fifo_header *list);

#endif /* INCLUDE_GUARD_FIFO_DEFINED */
