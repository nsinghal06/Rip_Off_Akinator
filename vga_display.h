#ifndef VGA_DISPLAY_H
#define VGA_DISPLAY_H

#define BLACK     0x0000
#define WHITE     0xFFFF
#define RED       0xF800
#define GREEN     0x07E0
#define BLUE      0x001F
#define YELLOW    0xFFE0
#define DARK_BLUE 0x0010
#define CYAN      0x07FF
#define GREY      0x8410
#define ORANGE    0xFD20
#define PURPLE    0x780F

#define SCREEN_W  320
#define SCREEN_H  240

void vga_draw_welcome(void);
void vga_draw_category(void);
void vga_draw_playing(int category);
void vga_draw_done(void);

#endif