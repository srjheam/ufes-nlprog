#ifndef _NODE_
#define _NODE_

typedef struct tNode Node;

Node *node_init(void *value, Node *next);

void node_i_dispose(Node *node);

void *node_get_data(Node *node);

Node *node_get_next(Node *node);

#endif
