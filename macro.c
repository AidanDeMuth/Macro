#include <assert.h>
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
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

    node_t *my_node = init_node();

    // Create a clock 

    clock_t before = clock();
    int count = 0;

    char a = 0;
    while (1) {
        bool l = 0;
        bool r = 0;

        // Handle Clicks
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { // most significant
            l = 1;
        }
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { // least
            r = 1;
        }

        // printf("%d %d", lclick, rclick);

        // Handle kb-hits
        
        char keys[MAX_KB_HITS];
        short num_keys = 0;
        while (kbhit()) {
            if (num_keys < MAX_KB_HITS) { // read them all anyways
                keys[num_keys] = getch();
                printf("%c", keys[num_keys]);
                
                ++num_keys;
            }
            else {
                getch();
            }
            printf("%d", count);
        }

        POINT pos = {0};
        GetCursorPos(&pos);

        insert_frame(my_node, l, r, keys, num_keys, pos);

        if (count == 10000) {
            break;
        }
        count++;
        printf("\n");
    }

    // Why not printing

    frame_t iterator = my_node->frames[0];
    printf("EFJALSDFJAKLSDJLASKDJFASJDF");
    for (int i = 0; i < 10000; i++) {
        printf("%d %d %d ", iterator.lclick, iterator.rclick, iterator.num_hits);
        for (int j = 0; j < iterator.num_hits; j++) {
            printf("%c ", iterator.kb_hits[j]);
        }
        printf("%d %d \n", iterator.point.x, iterator.point.y);
        iterator = my_node->frames[i];
    }

    clock_t after = clock();
    clock_t diff = after - before;
    clock_t total = diff * 1000 / CLOCKS_PER_SEC;

    printf("Points: %d\n", count);
    printf("%ld\n", total);
} /* main() */

node_t *init_node() {
    node_t *new_node = malloc(sizeof(node_t));
    assert(new_node);

    new_node->frames = malloc(sizeof(node_t) * INIT_CAPACITY);
    assert(new_node->frames);

    new_node->size = 0;
    new_node->capacity = INIT_CAPACITY;
    
    return new_node;
} /* init_node() */

void insert_frame(node_t *node, bool l, bool r, char *keys, short num_keys, 
                  POINT pos) {
    frame_t new_frame = {0};
    new_frame.lclick = l;
    new_frame.rclick = r;
    new_frame.num_hits = num_keys;
    
    for (int i = 0; i < num_keys; i++) {
        new_frame.kb_hits[i] = keys[i];
    }

    new_frame.point = pos;

    node->frames[node->size] = new_frame;
    ++(node->size);
}