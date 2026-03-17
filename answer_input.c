#include "answer_input.h"

#define KEY_BASE 0xFF200050

//Any key pressed, used in welcome, called by game_controller.c
int key_pressed(void) {
    volatile int *key_ptr = (int *) KEY_BASE;
    int edge = *(key_ptr + 3); // read edgeCap
    edge = edge & 0xF; //check if any edge is 1111
    if (edge != 0) {
        *(key_ptr + 3) = edge; //clear
        return 1;
    }
    return 0;
}

//KEY0: YES, KEY1: NO, used in selecting answer, called by game_controller.c
int wait_for_answer(void) {
    volatile int *key_ptr = (int *) KEY_BASE;
    int edge = 0;

    //poll till key0 or key1
    while(edge == 0){
        edge = *(key_ptr + 3);
        edge = edge & 0x3; //key 0 and 1
    } 
    *(key_ptr + 3) = edge; //clear

    if (edge & 0x1) return 0;   // KEY0 = YES                       
    if (edge & 0x2) return 1;   // KEY1 = NO                        
    return -1;                  // both pressed together      
}
