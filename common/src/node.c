#include <stdlib.h>

#include "types.h"
#include "exception.h"

#include "node.h"

struct tNode {
    void *data;
    Node *next;
};

Node *node_init(void *data, Node *next) {
    Node *node = malloc(sizeof *node);
    if (node == NULL)
        exception_throw_OutOfMemory("Node malloc failed");

    node->data = data;
    node->next = next;

    return node;
}

void node_i_dispose(Node *node) { free(node); }

void *node_get_data(Node *node) { return node->data; }

Node *node_get_next(Node *node) { return node->next; }
