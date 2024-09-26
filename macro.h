#include <stdbool.h>
#include <windows.h>

#ifndef MACRO_H
#define MACRO_H

#define SECOND 1000
#define MAX_KB_HITS 12

#define MIN_REFRESH_RATE 30
#define MAX_REFRESH_RATE 1000

#define INIT_CAPACITY 100000
#define INCREMENT_FACTOR 2

typedef struct node_t node_t;
typedef struct frame_t frame_t;

struct frame_t {
    bool lclick; // 1/8
    bool rclick; // 1/8
    short num_hits; // 2
    char kb_hits[MAX_KB_HITS]; // 12
    POINT point; // 8
};

struct node_t {
    frame_t *frames;
    int size;
    int capacity;
}; 

node_t *init_node();
void insert_frame(node_t *node, bool l, bool r, char *keys, short num_keys, 
                  POINT pos);

#endif