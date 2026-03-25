/* ----------------------------------------------------------
 * bayes_engine.h
 * Exposes bayes functions to game_controller.c
 * Include with: #include "bayes_engine.h"
 * ----------------------------------------------------------*/

#ifndef BAYES_ENGINE_H
#define BAYES_ENGINE_H

/* call once on entering PLAYING state
 * sets all probabilities equal, resets asked[] */
void bayes_init(void);

/* call after every answer
 * q:      which question was asked (index 0 to NUM_QUESTIONS-1)
 * answer: 0=YES  1=NO  2=SOMETIMES */
void bayes_update(int q, int answer);

/* call to get the next best question to ask
 * returns index into questions[] array in dataset.c
 * game_controller passes this to vga_draw_question(q) */
int bayes_next_question(void);

/* call after every bayes_update
 * returns highest probability float (0.0 to 1.0)
 * game_controller checks: if > 0.90 → state = GUESS */
float bayes_top_probability(void);

/* call in GUESS state
 * returns index of most likely food
 * game_controller passes to vga_draw_guess(index)
 * vga_display looks up food_names[index] */
int bayes_top_candidate(void);

#endif