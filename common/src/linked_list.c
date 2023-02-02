#include <stdlib.h>

#include "node.h"
#include "exception.h"

#include "linked_list.h"

struct tLinkedList {
    Node *head;
    size_t length;

    cpy_fn cpyValue;
    free_fn disposeValue;
};

LinkedList *ll_init(cpy_fn cpyValue, free_fn disposeValue) {
    LinkedList *list = malloc(sizeof *list);
    if (list == NULL)
        exception_throw_OutOfMemory("LinkedList malloc failed");

    list->head = NULL;
    list->length = 0;

    list->cpyValue = cpyValue;
    list->disposeValue = disposeValue;

    return list;
}

void ll_dispose(LinkedList *list) {
    Node *curr = list->head;
    while (curr != NULL) {
        Node *next = node_get_next(curr);

        list->disposeValue(node_get_data(curr));
        node_i_dispose(curr);

        curr = next;
    }

    free(list);
}

size_t ll_get_length(LinkedList *list) { return list->length; }

void ll_append(LinkedList *list, void *data) {
    Node *node = node_init(data, list->head);
    list->head = node;

    list->length++;
}

void *ll_iter(LinkedList *list, void **saveptr) {
    if (*saveptr == NULL) {
        *saveptr = list->head;
        return *saveptr;
    }

    Node *next = node_get_next(*saveptr);
    if (next == NULL)
        return NULL;

    *saveptr = next;

    return *saveptr;
}

LinkedList *ll_cpy(const LinkedList *list) {
    LinkedList *cpy = ll_init(list->cpyValue, list->disposeValue);
    if (cpy == NULL)
        exception_throw_OutOfMemory("LinkedList cpy failed");

    void *saveptr = NULL;
    Node *node = NULL;
    while ((node = ll_iter((LinkedList *)list, &saveptr)) != NULL)
        ll_append(cpy, list->cpyValue(node_get_data(node)));

    return cpy;
}
