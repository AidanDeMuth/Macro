#include <assert.h>
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

#include "macro.h"

void 
main() {
    // Get monitor refresh rate
    float hz = 0;
    DEVMODE scr_settings;
    if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &scr_settings)) {
        hz = scr_settings.dmDisplayFrequency;
    }
    else {
        hz = MIN_REFRESH_RATE; // Just default to 60, should be safe
    }

    node_t *my_node = init_node(hz);
    record_macro(my_node);

    sleep(5);
    play_macro(my_node);
} /* main() */

void
record_macro(node_t *node) {
    float rate = SECOND / node->hz;

    // Start of execution time
    clock_t start_time = clock();
    
    int count = 0;

    // Var list
    int frame_count = 0;

    bool lclick = 0;
    bool rclick = 0;
    unsigned char keys[MAX_KB_HITS];
    short num_keys = 0;

    while (1) {

        // Handle Clicks

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { // most significant
            lclick = 1;
        }
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            rclick = 1;
        }

        // printf("%d %d", lclick, rclick);

        // Handle kb-hits
        while (kbhit()) {
            if (num_keys < MAX_KB_HITS) {
                keys[num_keys] = getch();
                printf("%c", keys[num_keys]);
                
                ++num_keys;
            }
            else { 
                getch(); // Flush
            }
        }


        POINT pos = {0};
        GetCursorPos(&pos);

        if (count == 40000) {
            break;
        }

        // END OLD FRAME
        // Add frame struct
        // Reset fields for next loop

        int curr_frame = get_frame(start_time, rate);
        if (curr_frame > frame_count) {
            insert_frame(node, lclick, rclick, keys, num_keys, pos, frame_count);

            memset(keys, 0, sizeof(keys));
            num_keys = 0;

            lclick = 0;
            rclick = 0;

            frame_count = curr_frame;
        }
        count++;

        /* 
        // Test printing frames
        for (int i = 0; i < num_keys; i++) {
            printf("%c ", keys[i]);
        }
        printf("\n");
        */
    }

    printf("Its / sec = %f\n", ((float) count / ((clock() - start_time))) * 1000);
    printf("Points: %d\n", count);
} /* record_macro() */

void
play_macro(node_t *node) {
    // PLAYBACK SECTION
    double delay = SECOND / node->hz;
    clock_t frame_loop = clock();

    for (int i = 0; i < node->size; i++) {
        SetCursorPos(node->frames[i].point.x, node->frames[i].point.y);

        double wait_time = ((double) clock() - (double) frame_loop + delay) * 1000000; // in nanoseconds 
        printf("Wait time: %lf\n", wait_time);
        nanosleep((const struct timespec[]) {{0, wait_time}}, NULL);
        frame_loop = clock();
    }
}

node_t 
*init_node(float hz) {
    node_t *new_node = malloc(sizeof(node_t));
    assert(new_node);

    new_node->frames = malloc(sizeof(node_t) * INIT_CAPACITY);
    assert(new_node->frames);

    new_node->size = 0;
    new_node->capacity = INIT_CAPACITY;
    new_node->hz = hz;
    
    return new_node;
} /* init_node() */

void 
insert_frame(node_t *node, bool l, bool r, char *keys, short num_keys, 
             POINT pos, int frame_num) {
    frame_t new_frame = {0};
    new_frame.lclick = l;
    new_frame.rclick = r;
    new_frame.num_keys = num_keys;
    
    for (int i = 0; i < num_keys; i++) {
        new_frame.kb_hits[i] = keys[i];
    }

    new_frame.point = pos;
    new_frame.frame_num = frame_num;

    node->frames[node->size] = new_frame;
    ++(node->size);
} /* insert_frame() */

// Returns the FLOOR of the current frame
inline int 
get_frame(clock_t start_time, float rate) {
    return (clock() - start_time) / rate;
} /* get_frame() */

void
print_frame(node_t *node, int index) {
    printf("%d - %d %d - %d %d - %hu - ", 
            node->frames[index].frame_num,
            node->frames[index].lclick,
            node->frames[index].rclick,
            node->frames[index].point.x,
            node->frames[index].point.y,
            node->frames[index].num_keys);
    for (int i = 0; i < node->frames[index].num_keys; i++) {
        printf("%c ", node->frames[index].kb_hits[i]);
    }
    printf("\n");
} /* print_frame() */