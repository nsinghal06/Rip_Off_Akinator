// game_controller.c 

#include "vga_display.h"
#include "answer_input.h"
#include "category_select.h"
#include "bayes_engine.h"

#define WELCOME 0
#define CATEGORY_SELECT 1
#define PLAYING 2
#define GUESS 3
#define RESULT 4

int state;
int prev_state; 
int category;
int questions_asked;
int last_answer;

void run_game(void) {
    state = WELCOME;
    prev_state = -1; // to keep track of when we enter a new state to avoid repeating functions

    while (1) {
        // ---------- WELCOME ----------
        //poll welcome till user presses a key, then moves to CATEGORY_SELECT state
        if (state == WELCOME) {
            if (prev_state != WELCOME) { // just entered this state
                vga_draw_welcome(); // draw screen ONCE
                prev_state = WELCOME;
            }
            if (key_pressed())
                state = CATEGORY_SELECT;
        }

        // ---------- CATEGORY SELECT ----------
        //poll until user selects a valid category, then moves to PLAYING state
        else if (state == CATEGORY_SELECT) {
            if (prev_state != CATEGORY_SELECT) {  
                vga_draw_category();
                prev_state = CATEGORY_SELECT;
            }
            category = read_category(); //returns 0, 1, 2, 3 for food animal country movie -1 invalid
            if (category != -1) { //invalid switch input, keep polling
                hex_start_marquee(category); //TO DO - scrolls category name on HEX0-3
                bayes_init();               //initialise equal probabilities, reset asked[]
                questions_asked = 0;        //reset counter for new game
                prev_state = -1;            //force PLAYING to redraw on entry
                state = PLAYING;
            }
        }

        // ---------- PLAYING ----------
        else if (state == PLAYING) {
            int q;                          //declare here so whole block can see it

            if (prev_state != PLAYING) {    
                vga_draw_background(category); 
                hex_set_questions(20); //TO DO
                prev_state = PLAYING;
            
                //swap question text each round
                q = bayes_next_question();  //get first best question
                vga_draw_question(q);       // only redraws text
                //audio_thinking(); //TO DO
            }

            last_answer = wait_for_answer();
            if (last_answer == 0 || last_answer == 1) {
                bayes_update(q, last_answer); //update algorithm with new information
                questions_asked++;
                hex_set_questions(20 - questions_asked); //TO DO

            //check game conditions
            if (bayes_top_probability() > 0.90) { //confident enough to guess
                prev_state = -1;            //force GUESS to redraw on entry
                state = GUESS;
            }
            else if (questions_asked >= 20) { //ran out of questions, guess anyway
                prev_state = -1;
                state = GUESS;
            }
            else { //still playing
                q = bayes_next_question();  //get next best question
                vga_draw_question(q);
            }
        }
    }

        // ---------- GUESS ----------
        else if (state == GUESS) {
            if (prev_state != GUESS) {      // just entered GUESS
                int guess = bayes_top_candidate(); //index of most likely food
                vga_draw_guess(guess);      // draw screen ONCE
                audio_play_guess();         //TO DO
                prev_state = GUESS;
            }
            // just wait here until user acknowledges
            if (key_pressed()) {
                prev_state = -1;            //force RESULT to redraw on entry
                state = RESULT;
            }
        }

        // ── RESULT ───────────────────────────────────────────
        else if (state == RESULT) {
            if (prev_state != RESULT) {     // just entered RESULT
                vga_draw_result(           
                    bayes_top_probability() > 0.90  // did akinator win?
                );
                prev_state = RESULT;
            }
            if (key_pressed()) {
                prev_state = -1;            //reset for fresh WELCOME on restart
                state = WELCOME;
            }
        }
    }
}