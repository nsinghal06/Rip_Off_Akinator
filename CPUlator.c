/* ============================================================
   AKINATOR 2.0 - Complete single file for CPUlator
   ============================================================ */

/* ── ADDRESSES ─────────────────────────────────────────────── */
#define PIXEL_BUF  0x8000000
#define KEY_BASE   0xFF200050
#define SW_BASE    0xFF200040

/* ── SCREEN ─────────────────────────────────────────────────── */
#define SCREEN_W   320
#define SCREEN_H   240

/* ── COLOURS RGB565 ─────────────────────────────────────────── */
#define BLACK      0x0000
#define WHITE      0xFFFF
#define RED        0xF800
#define GREEN      0x07E0
#define BLUE       0x001F
#define YELLOW     0xFFE0
#define DARK_BLUE  0x0010
#define CYAN       0x07FF
#define GREY       0x8410
#define ORANGE     0xFD20
#define PURPLE     0x780F

/* ── STATES ─────────────────────────────────────────────────── */
#define WELCOME         0
#define CATEGORY_SELECT 1
#define PLAYING         2
#define DONE            3

/* ============================================================
   FONT 8x8 — ASCII 32 (space) to 90 (Z)
   ============================================================ */
static const unsigned char font8x8[91][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* space */
    {0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00}, /* !     */
    {0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00}, /* "     */
    {0x66,0xFF,0x66,0x66,0xFF,0x66,0x00,0x00}, /* #     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* $     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* %     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* &     */
    {0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00}, /* '     */
    {0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00}, /* (     */
    {0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00}, /* )     */
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, /* *     */
    {0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00}, /* +     */
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30}, /* ,     */
    {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00}, /* -     */
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00}, /* .     */
    {0x06,0x0C,0x18,0x30,0x60,0x40,0x00,0x00}, /* /     */
    {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, /* 0     */
    {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00}, /* 1     */
    {0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00}, /* 2     */
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, /* 3     */
    {0x0E,0x1E,0x36,0x66,0x7F,0x06,0x06,0x00}, /* 4     */
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, /* 5     */
    {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, /* 6     */
    {0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00}, /* 7     */
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, /* 8     */
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}, /* 9     */
    {0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00}, /* :     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* ;     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* <     */
    {0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00}, /* =     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* >     */
    {0x3C,0x66,0x06,0x1C,0x18,0x00,0x18,0x00}, /* ?     */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* @     */
    {0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00}, /* A     */
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, /* B     */
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, /* C     */
    {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, /* D     */
    {0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00}, /* E     */
    {0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00}, /* F     */
    {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, /* G     */
    {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, /* H     */
    {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, /* I     */
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00}, /* J     */
    {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, /* K     */
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, /* L     */
    {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, /* M     */
    {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00}, /* N     */
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, /* O     */
    {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, /* P     */
    {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x06,0x00}, /* Q     */
    {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00}, /* R     */
    {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00}, /* S     */
    {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, /* T     */
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, /* U     */
    {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, /* V     */
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, /* W     */
    {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, /* X     */
    {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00}, /* Y     */
    {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}, /* Z     */
};

/* ============================================================
   VGA DRAWING
   ============================================================ */

static void draw_pixel(int x, int y, short colour) {
    volatile short *ptr;
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return;
    ptr = (short *)(PIXEL_BUF + (y << 10) + (x << 1));
    *ptr = colour;
}

static void draw_rect(int x, int y, int w, int h, short colour) {
    int r, c;
    for (r = y; r < y + h; r++)
        for (c = x; c < x + w; c++)
            draw_pixel(c, r, colour);
}

static void draw_char(int x, int y, char ch, short colour) {
    int row, col;
    unsigned char bits;
    int idx = (int)ch - 32;
    if (idx < 0 || idx > 90) return;
    for (row = 0; row < 8; row++) {
        bits = font8x8[idx][row];
        for (col = 0; col < 8; col++)
            if (bits & (0x80 >> col))
                draw_pixel(x + col, y + row, colour);
    }
}

static void draw_string(int x, int y, const char *s, short colour) {
    while (*s) {
        draw_char(x, y, *s, colour);
        x += 8;
        s++;
    }
}

/* ── SCREENS ─────────────────────────────────────────────────── */

static void vga_draw_welcome(void) {
    draw_rect(0, 0, SCREEN_W, SCREEN_H, DARK_BLUE);

    /* title box */
    draw_rect(60, 70, 200, 40, CYAN);
    draw_string(76, 83, "AKINATOR 2.0", DARK_BLUE);

    /* subtitle */
    draw_string(44, 130, "THE MIND READER", WHITE);

    /* prompt */
    draw_string(52, 190, "PRESS ANY KEY", YELLOW);
}

static void vga_draw_category(void) {
    draw_rect(0, 0, SCREEN_W, SCREEN_H, WHITE);

    /* title bar */
    draw_rect(0, 0, SCREEN_W, 30, DARK_BLUE);
    draw_string(68, 10, "PICK A CATEGORY", WHITE);

    /* options — matches your image exactly */
    draw_string(88, 60,  "FOOD    : SW0", DARK_BLUE);
    draw_string(88, 90,  "ANIMAL  : SW1", DARK_BLUE);
    draw_string(88, 120, "COUNTRY : SW2", DARK_BLUE);
    draw_string(88, 150, "MOVIE   : SW3", DARK_BLUE);

    draw_string(52, 210, "FLIP SWITCH TO SELECT", GREY);
}

static void vga_draw_playing(int category) {
    short bg;
    const char *cat;

    if      (category == 0) { bg = ORANGE; cat = "FOOD";    }
    else if (category == 1) { bg = GREEN;  cat = "ANIMAL";  }
    else if (category == 2) { bg = BLUE;   cat = "COUNTRY"; }
    else                    { bg = PURPLE; cat = "MOVIE";   }

    /* background */
    draw_rect(0, 0, SCREEN_W, SCREEN_H, bg);

    /* top bar with category name */
    draw_rect(0, 0, SCREEN_W, 22, DARK_BLUE);
    draw_string(4, 7, cat, WHITE);

    /* question box */
    draw_rect(10, 50, 300, 100, DARK_BLUE);
    draw_string(20, 65, "QUESTION 1:", WHITE);
    draw_string(20, 90, "IS IT A DESSERT?", YELLOW);

    /* answer buttons */
    draw_rect(10,  180, 130, 35, GREEN);
    draw_rect(180, 180, 130, 35, RED);
    draw_string(28,  191, "KEY0 = YES", DARK_BLUE);
    draw_string(196, 191, "KEY1 = NO",  WHITE);
}

static void vga_draw_done(void) {
    draw_rect(0, 0, SCREEN_W, SCREEN_H, BLACK);
}

/* ============================================================
   INPUT
   ============================================================ */

static int key_pressed(void) {
    volatile int *key_ptr = (int *) KEY_BASE;
    return (*key_ptr & 0x3) != 0;   /* KEY0 or KEY1 down */
}

static int read_category(void) {
    volatile int *sw_ptr = (int *) SW_BASE;
    int sw = *sw_ptr & 0xF;
    if (sw == 0x1) return 0;   /* SW0 = Food    */
    if (sw == 0x2) return 1;   /* SW1 = Animal  */
    if (sw == 0x4) return 2;   /* SW2 = Country */
    if (sw == 0x8) return 3;   /* SW3 = Movie   */
    return -1;
}

/* ============================================================
   GAME CONTROLLER — state machine
   ============================================================ */

static void run_game(void) {
    int state      = WELCOME;
    int prev_state = -1;
    int category   = -1;

    while (1) {

        /* ── WELCOME ──────────────────────────────────── */
        if (state == WELCOME) {
            if (prev_state != WELCOME) {
                vga_draw_welcome();
                prev_state = WELCOME;
            }
            if (key_pressed())
                state = CATEGORY_SELECT;
        }

        /* ── CATEGORY SELECT ──────────────────────────── */
        else if (state == CATEGORY_SELECT) {
            if (prev_state != CATEGORY_SELECT) {
                vga_draw_category();
                prev_state = CATEGORY_SELECT;
            }
            category = read_category();
            if (category != -1) {
                prev_state = -1;
                state = PLAYING;
            }
        }

        /* ── PLAYING ──────────────────────────────────── */
        else if (state == PLAYING) {
            if (prev_state != PLAYING) {
                vga_draw_playing(category);
                prev_state = PLAYING;
            }
            if (key_pressed()) {
                prev_state = -1;
                state = DONE;
            }
        }

        /* ── DONE (black screen) ──────────────────────── */
        else if (state == DONE) {
            if (prev_state != DONE) {
                vga_draw_done();
                prev_state = DONE;
            }
            /* sit here, next step will be RESULT screen */
        }
    }
}

/* ============================================================
   MAIN
   ============================================================ */

int main(void) {
    run_game();
    return 0;
}