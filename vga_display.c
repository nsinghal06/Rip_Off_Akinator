/* vga_display.c
 * all VGA drawing is here, called by game_controller.c
 */

#include "vga_display.h"

#define PIXEL_BUF 0xC8000000
#define SCREEN_W  320
#define SCREEN_H  240

void draw_pixel(int x, int y, short colour) {}
void draw_rect(int x, int y, int w, int h, short colour) {}
void draw_char(int x, int y, char c, short colour) {}
void draw_string(int x, int y, const char *s, short colour) {}
void draw_image(int x, int y, int w, int h, const short *img) {}

/* ── HIGH LEVEL (exposed in vga_display.h, called by game_controller) ── */

void vga_draw_welcome(void) {
    draw_image(0, 0, 320, 240, welcome_bg);
    draw_string(100, 100, "AKINATOR", WHITE);
    draw_string(80, 160,  "Press any key", WHITE);
}

void vga_draw_category(void) {
    draw_image(0, 0, 320, 240, category_select_bg);
    draw_string(60, 50,  "Pick a category", WHITE);
    draw_string(60, 100, "SW0 = Food",    WHITE);
    draw_string(60, 120, "SW1 = Animal",  WHITE);
    draw_string(60, 140, "SW2 = Country", WHITE);
    draw_string(60, 160, "SW3 = Movie",   WHITE);
}

void vga_draw_background(int category) {
    // called ONCE when entering PLAYING state
    draw_image(0, 0, 320, 240, category_backgrounds[category]);
    draw_rect(20, 80, 280, 80, DARK_BLUE);   // question box
    draw_string(60, 200, "KEY0=YES", GREEN);
    draw_string(180, 200, "KEY1=NO", RED);
}

void vga_draw_question(int q) {
    // called every question, just swaps the text
    draw_rect(25, 85, 270, 50, DARK_BLUE);          // erase old text
    draw_string(30, 100, question_strings[q], WHITE); // draw new text
}

void vga_draw_guess(int candidate) {
    draw_image(0, 0, 320, 240, guess_bg);
    draw_string(60, 80,  "I think it is...", WHITE);
    draw_string(60, 130, candidate_names[candidate], YELLOW);
}

void vga_draw_result(int akinator_won) {
    if (akinator_won)
        draw_image(0, 0, 320, 240, win_bg);
    else
        draw_image(0, 0, 320, 240, lose_bg);
    draw_string(60, 200, "Press key to play again", WHITE);
}