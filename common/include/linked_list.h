#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include "types.h"

typedef struct tLinkedList LinkedList;

LinkedList *ll_init(cpy_fn cpyValue, free_fn disposeValue);

void ll_dispose(LinkedList *list);

size_t ll_get_length(LinkedList *list);

void ll_append(LinkedList *list, void *data);

void *ll_iter(LinkedList *list, void **saveptr);

/**
 * @brief Efetua uma copia da @ref LinkedList* @p list
 * 
 * @warning A lista retornada está na inversa da original
 * 
 * @param list A @ref LinkedList*
 * @return void* Uma nova instância de @ref LinkedList* (veja o warning)
 */
LinkedList *ll_cpy(const LinkedList *list);

#endif
