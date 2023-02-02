#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "types.h"

typedef struct tLinkedList LinkedList;

LinkedList *ll_init(cpy_fn cpyValues, free_fn disposeValues);

void ll_dispose(LinkedList *list);

size_t ll_get_length(LinkedList *list);

void ll_append(LinkedList *list, void *data);

void *ll_iter(LinkedList *list, void **saveptr);

#endif
