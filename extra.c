
void vga_draw_category(void) {
    draw_rect(0, 0, SCREEN_W, SCREEN_H, WHITE);
    draw_rect(0, 0, SCREEN_W, 30, DARK_BLUE);
    draw_string(68, 10, "PICK A CATEGORY", WHITE);
    draw_string(88, 60,  "FOOD    : SW0", DARK_BLUE);
    draw_string(88, 90,  "ANIMAL  : SW1", DARK_BLUE);
    draw_string(88, 120, "COUNTRY : SW2", DARK_BLUE);
    draw_string(88, 150, "MOVIE   : SW3", DARK_BLUE);
    draw_string(52, 210, "FLIP SWITCH TO SELECT", GREY);
}

void vga_draw_playing(int category) {
    short bg;
    const char *cat;
    if      (category == 0) { bg = ORANGE; cat = "FOOD";    }
    else if (category == 1) { bg = GREEN;  cat = "ANIMAL";  }
    else if (category == 2) { bg = BLUE;   cat = "COUNTRY"; }
    else                    { bg = PURPLE; cat = "MOVIE";   }

    draw_rect(0, 0, SCREEN_W, SCREEN_H, bg);
    draw_rect(0, 0, SCREEN_W, 22, DARK_BLUE);
    draw_string(4, 7, cat, WHITE);
    draw_rect(10, 50, 300, 100, DARK_BLUE);
    draw_string(20, 65, "QUESTION 1:", WHITE);
    draw_string(20, 90, "IS IT A DESSERT?", YELLOW);
    draw_rect(10,  180, 130, 35, GREEN);
    draw_rect(180, 180, 130, 35, RED);
    draw_string(28,  191, "KEY0 = YES", DARK_BLUE);
    draw_string(196, 191, "KEY1 = NO",  WHITE);
}