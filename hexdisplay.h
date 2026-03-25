/* ----------------------------------------------------------
 * hex_display.h
 * Exposes HEX display functions to game_controller.c
 * Include with: #include "hex_display.h"
 * ----------------------------------------------------------*/

#ifndef HEX_DISPLAY_H
#define HEX_DISPLAY_H

/* call once when category is confirmed in CATEGORY_SELECT
 * starts the scrolling marquee on HEX0-3
 * category: 0=Food 1=Animal 2=Country 3=Movie */
void hex_start_marquee(int category);

/* call every loop iteration in PLAYING state
 * advances the marquee scroll by one position */
void hex_tick_marquee(void);

/* call once on entering PLAYING state to show 20
 * then call after every answer with (20 - questions_asked)
 * displays on HEX4-5 */
void hex_set_questions(int n);

/* call at start of WELCOME state to blank all displays */
void hex_clear(void);

#endif