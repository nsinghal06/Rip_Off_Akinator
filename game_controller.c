// game_controller.c 

#include "vga_display.h"
#include "answer_input.h"
#include "category_select.h"

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
                hex_display(category); //TO DO
                bayes_thm(category); //TO DO, tells bayes which dataset/category to load
                state = PLAYING;
            }
        }

        // ---------- PLAYING ----------
        else if (state == PLAYING) {
            if (prev_state != PLAYING) {    
                vga_draw_background(category); 
                hex_set_questions(20); //TO DO
                prev_state = PLAYING;
            
                //swap question text each round
                int questions_asked = 0;
                int q = question(); //TO DO
                vga_draw_question(q); // only redraws text
                //audio_thinking(); //TO DO
            }

            last_answer = wait_for_answer();
            if (last_answer == 0 || last_answer == 1) {
                bayes_update(q, last_answer); //TO DO, update algorithm with new information
                questions_asked++;
                hex_set_questions(20 - questions_asked);

            //check game conditions
            if (bayes_probability() > 0.90) state = GUESS; //TO DO
            else if (questions_asked >= 20) state = GUESS;
            else { //still playing
                int q = question();  
                vga_draw_question(q);
            }
        }
    }

        // ---------- GUESS ----------
        else if (state == GUESS) {
            if (prev_state != GUESS) {      // just entered GUESS
                int guess = bayes_top_candidate();
                vga_draw_guess(guess);      // draw screen ONCE
                audio_play_guess();
                prev_state = GUESS;
            }
            // just wait here until user acknowledges
            if (key_pressed())
                state = RESULT;
        }

        // ── RESULT ───────────────────────────────────────────
        else if (state == RESULT) {
            if (prev_state != RESULT) {     // just entered RESULT
                vga_draw_result(           
                    bayes_top_probability() > 0.90  // did akinator win?
                );
                prev_state = RESULT;
            }
            if (key_pressed())
                state = WELCOME;
        }
    }
}