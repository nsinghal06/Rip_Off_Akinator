/* vga_display.h
 * #include "vga_display.h"
 */

#ifndef VGA_DISPLAY_H
#define VGA_DISPLAY_H

// ---------- COLOURS (RGB565) ---------- 
#define BLACK      0x0000
#define WHITE      0xFFFF
#define RED        0xF800
#define GREEN      0x07E0
#define BLUE       0x001F
#define YELLOW     0xFFE0
#define CYAN       0x07FF
#define MAGENTA    0xF81F
#define DARK_BLUE  0x0010
#define DARK_GREY  0x4208
#define ORANGE     0xFC00

#define SCREEN_W   320
#define SCREEN_H   240

void vga_draw_welcome(void);
// draws: akinator logo, background, "press any key" prompt     

void vga_draw_category(void);
// draws: category selection screen, switch instructions        

void vga_draw_background(int category);
// draws: themed background for category, question box,             
// YES/NO prompts at bottom. called ONCE on entering PLAYING.   
// category: 0 = Food, 1 = Animal, 2 = Country, 3 = Movie                

void vga_draw_question(int q);
// draws: only the question text inside the box.                
// erases old text first. called every question in PLAYING.      
// q: index into question_strings[] in dataset.c                 

void vga_draw_guess(int candidate);
// draws: full guess screen with candidate name                  
// candidate: index into candidate_names[] in dataset.c         

void vga_draw_result(int akinator_won);
// draws: win or lose screen depending on akinator_won flag      
// akinator_won: 1 if akinator guessed correctly, 0 if user won  

#endif