#include <assert.h>
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "macro.h"

void main() {
    // Get monitor refresh rate
    float hz = 0;
    DEVMODE scr_settings;
    if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &scr_settings)) {
        hz = scr_settings.dmDisplayFrequency;
    }
    else {
        hz = MIN_REFRESH_RATE; // Just default to 60, should be safe
    }

    float rate = SECOND / hz;

    // List to store all points

    node_t *list = init_node();
    printf("%d", sizeof(*list));

    // Create a clock 

    clock_t before = clock();
    int count = 0;

    char a = 0;
    while (1) {
        if (kbhit()) {
            char key = getch();

            if (key == 'a') {
                break;
            }
        }
        POINT pos = {0};
        GetCursorPos(&pos);
        add_element(list, pos);

        printf("Points: %d %d", pos.x, pos.y);
        count++;
    }

    clock_t after = clock();
    clock_t diff = after - before;
    clock_t total = diff * 1000 / CLOCKS_PER_SEC;

    printf("Points: %d\n", count);
    printf("%ld\n", total);

    POINT *iterator = list->points;
    for (int x = 0; x < list->size; x++) {
        printf("Point %d   :   %d %d  \n", x, iterator->x, iterator->y);
        SetCursorPos(iterator->x, iterator->y);
        iterator++;
    }
}

node_t *init_node() {
    POINT *new_points = malloc(sizeof(POINT) * INIT_CAPACITY);
    node_t *new_node = malloc(sizeof(node_t));

    assert(new_points);
    assert(new_node);    

    new_node->size = 0;
    new_node->capacity = INIT_CAPACITY;
    new_node->points = new_points;

    return new_node;
} /* init_node() */

int add_element(node_t *node, POINT element) {
    assert(node);

    if (node->size == node->capacity) {
        increase_size(node);
    }

    node->points[node->size] = element;
    node->size += 1;

} /* add_element() */

void increase_size(node_t *node) {
    assert(node);
        
    assert((__INT_MAX__ - node->capacity) > INCREMENT_CAPACITY); // Prevent overflow

    node->points = realloc(node->points, (INCREMENT_CAPACITY + node->capacity) * sizeof(POINT));
    node->capacity = INCREMENT_CAPACITY + node->capacity; 
    printf("\n\nSIZE: %d %d", node->size, node->capacity);
    assert(node->points);

} /* increase_size() */