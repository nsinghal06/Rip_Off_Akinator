#include "answer_input.h"

#define KEY_BASE 0xFF200050

int key_pressed(void) {
    volatile int *key_ptr = (int *) KEY_BASE;
    return (*key_ptr & 0x3) != 0;
}

int wait_for_answer(void) {
    volatile int *key_ptr = (int *) KEY_BASE;
    int keys;
    do {
        keys = *key_ptr & 0x3;
    } while (keys == 0);
    while (*key_ptr & 0x3);
    if (keys & 0x1) return 0;
    return 1;
}