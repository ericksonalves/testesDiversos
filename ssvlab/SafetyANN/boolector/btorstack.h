/*  Boolector: Satisfiablity Modulo Theories (SMT) solver.
 *
 *  Copyright (C) 2007-2009 Robert Daniel Brummayer.
 *  Copyright (C) 2007-2013 Armin Biere.
 *  Copyright (C) 2013-2014 Aina Niemetz.
 *
 *  All rights reserved.
 *
 *  This file is part of Boolector.
 *  See COPYING for more information on using this software.
 */

#ifndef BTOR_STACK_H_INCLUDED
#define BTOR_STACK_H_INCLUDED

#include "btormem.h"

#include <assert.h>
#include <stdlib.h>

#define BTOR_DECLARE_STACK(name,type) \
typedef struct name ## Stack name ## Stack; \
struct name ## Stack { type * start; type * top; type * end; }

#define BTOR_INIT_STACK(stack) \
do { \
  (stack).start = 0; \
  (stack).top = 0; \
  (stack).end = 0; \
} while (0)

#define BTOR_COUNT_STACK(stack) ((stack).top - (stack).start)

#define BTOR_EMPTY_STACK(stack) ((stack).top == (stack).start)

#define BTOR_RESET_STACK(stack) do { (stack).top = (stack).start; } while (0)

#define BTOR_SIZE_STACK(stack) ((stack).end - (stack).start)

#define BTOR_FULL_STACK(stack) ((stack).top == (stack).end)

#define BTOR_RELEASE_STACK(mm,stack) \
do { \
  BTOR_DELETEN ((mm), (stack).start, BTOR_SIZE_STACK ((stack))); \
  BTOR_INIT_STACK ((stack)); \
} while (0)

#define BTOR_ENLARGE_STACK(mm,stack) \
do { \
  size_t old_size = BTOR_SIZE_STACK (stack), new_size; \
  size_t old_count = BTOR_COUNT_STACK (stack); \
  BTOR_ENLARGE ((mm), (stack).start, old_size, new_size); \
  (stack).top = (stack).start + old_count; \
  (stack).end = (stack).start + new_size; \
} while (0)

#define BTOR_ENLARGE_STACK_TO_SIZE(mm,stack,new_size) \
  do { \
      size_t old_size = BTOR_SIZE_STACK (stack); \
      size_t old_count = BTOR_COUNT_STACK (stack); \
      BTOR_REALLOC ((mm), (stack).start, old_size, new_size); \
      (stack).top = (stack).start + old_count; \
      (stack).end = (stack).start + new_size; \
  } while (0)

/* adjust count and size of stack2 to count and size of stack1, new
 * stack elements in stack2 are cleared */
#define BTOR_ADJUST_STACK(mm, stack1, stack2) \
  do { \
    size_t stack1_size = BTOR_SIZE_STACK (stack1); \
    size_t stack2_size = BTOR_SIZE_STACK (stack2); \
    size_t stack1_count = BTOR_COUNT_STACK (stack1); \
    size_t stack2_count = BTOR_COUNT_STACK (stack2); \
    assert (stack1_size >= stack2_size); \
    assert (stack1_count >= stack2_count); \
    if (stack1_size > stack2_size) \
      BTOR_ENLARGE_STACK_TO_SIZE (mm, (stack2), (stack1_size)); \
    if (stack1_count > stack2_count) \
      { \
        memset ((stack2).top, 0, stack1_count - stack2_count); \
        (stack2).top += stack1_count - stack2_count; \
      } \
  } while (0)

#define BTOR_FIT_STACK(mm,stack,idx) \
do { \
  size_t old_size = BTOR_SIZE_STACK (stack), old_count, new_size; \
  if (old_size > (size_t)(idx)) break; \
  old_count = BTOR_COUNT_STACK (stack); \
  new_size = old_size ? 2*old_size : 1; \
  while (new_size <= (size_t)(idx)) \
    new_size *= 2; \
  assert ((new_size) > (size_t)(idx)); \
  BTOR_REALLOC ((mm), (stack).start, old_size, new_size); \
  (stack).top = (stack).start + old_count; \
  (stack).end = (stack).start + new_size; \
  BTOR_CLRN ((stack).top + old_size, new_size - old_size); \
} while (0)

#define BTOR_PUSH_STACK(mm,stack,elem) \
do { \
  if (BTOR_FULL_STACK ((stack))) \
    BTOR_ENLARGE_STACK ((mm),(stack)); \
  *((stack).top++) = (elem); \
} while (0)

#define BTOR_PUSH_STACK_IF(cond,mm,stack,elem) \
do { \
  if (cond) BTOR_PUSH_STACK (mm, stack, elem); \
} while (0)

#define BTOR_POP_STACK(stack) \
  (assert (!BTOR_EMPTY_STACK (stack)), (*--(stack).top))

#define BTOR_DEQUEUE_STACK(stack,dequeued) \
do { \
  typeof((stack).start[0]) * BTOR_DEQUEUE_P; \
  assert (!BTOR_EMPTY_STACK (stack)); \
  (dequeued) = (stack).start[0]; \
  BTOR_DEQUEUE_P = (stack).start; \
  while (++BTOR_DEQUEUE_P < (stack).top) \
    BTOR_DEQUEUE_P[-1] = BTOR_DEQUEUE_P[0]; \
  (stack).top--; \
} while (0)

#define BTOR_TOP_STACK(stack) \
  (assert (!BTOR_EMPTY_STACK (stack)), (stack).top[-1])

#define BTOR_PEEK_STACK(stack,idx) \
  (assert ((idx) < BTOR_COUNT_STACK (stack)), (stack).start[idx])

#define BTOR_POKE_STACK(stack,idx,elem) \
do { \
  assert ((idx) < BTOR_COUNT_STACK (stack)); \
  (stack).start[idx] = (elem); \
} while (0)

BTOR_DECLARE_STACK (BtorInt, int);

BTOR_DECLARE_STACK (BtorChar, char);

BTOR_DECLARE_STACK (BtorCharPtr, char *);

BTOR_DECLARE_STACK (BtorVoidPtr, void *);

#endif
