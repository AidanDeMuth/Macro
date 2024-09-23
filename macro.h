#include <windows.h>

#ifndef MACRO_H
#define MACRO_H

#define SECOND 1000

#define MIN_REFRESH_RATE 30
#define MAX_REFRESH_RATE 1000

#define INIT_CAPACITY 1000
#define INCREMENT_CAPACITY 1000

typedef struct node_t node_t;

struct node_t {
    POINT *points;
    int size;
    int capacity;
};

node_t *init_node();
int add_element(node_t *node, POINT element);
void increase_size(node_t *node);

#endif