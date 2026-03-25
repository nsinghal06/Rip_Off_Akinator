/* ----------------------------------------------------------
 * hex_display.c
 * Controls HEX0-5 on the DE10-SoC
 *
 * HEX0-3: scrolling marquee of category name
 *         e.g. "FOOD" scrolls across 4 digits left to right
 *
 * HEX4-5: questions remaining count (20 down to 00)
 *
 * Memory mapped addresses (from lecture 10):
 *   HEX0-3 base: 0xFF200020
 *   HEX4-5 base: 0xFF200030
 *
 * Each HEX digit is one byte, 7-segment active LOW
 * Writing 0 to a segment turns it ON
 * Writing 1 to a segment turns it OFF
 * ----------------------------------------------------------*/

#include "hex_display.h"

/* ── ADDRESSES ─────────────────────────────────────────── */
#define HEX03_BASE  0xFF200020   /* controls HEX0, HEX1, HEX2, HEX3 */
#define HEX45_BASE  0xFF200030   /* controls HEX4, HEX5              */

/* ── 7-SEGMENT ENCODINGS (active LOW) ──────────────────────
 *
 *  Segment layout:
 *       _
 *      |_|
 *      |_|
 *
 *  bit 0 = segment a (top)
 *  bit 1 = segment b (top right)
 *  bit 2 = segment c (bottom right)
 *  bit 3 = segment d (bottom)
 *  bit 4 = segment e (bottom left)
 *  bit 5 = segment f (top left)
 *  bit 6 = segment g (middle)
 *  bit 7 = always 0
 *
 *  0 = segment ON, 1 = segment OFF (active low)
 * ---------------------------------------------------------- */

/* digits 0-9 */
static const unsigned char seg7_digits[10] = {
    0x40, /* 0: all but middle    */
    0x79, /* 1: right two         */
    0x24, /* 2                    */
    0x30, /* 3                    */
    0x19, /* 4                    */
    0x12, /* 5                    */
    0x02, /* 6                    */
    0x78, /* 7                    */
    0x00, /* 8: all segments      */
    0x10, /* 9                    */
};

/* letters A-Z needed for category names
 * FOOD, ANIMAL, COUNTRY, MOVIE
 * letters needed: F O D A N I L C U T R Y M V E
 * plus space for padding */
static const unsigned char seg7_alpha[26] = {
    0x08, /* A */
    0x03, /* B */
    0x46, /* C */
    0x21, /* D */
    0x06, /* E */
    0x0E, /* F */
    0x42, /* G */
    0x09, /* H */
    0x7E, /* I — just two middle segments */
    0x61, /* J */
    0x09, /* K — approximate */
    0x47, /* L */
    0x28, /* M — approximate */
    0x2B, /* N — approximate */
    0x40, /* O */
    0x0C, /* P */
    0x18, /* Q — approximate */
    0x2F, /* R — approximate */
    0x12, /* S */
    0x07, /* T — approximate */
    0x41, /* U */
    0x41, /* V — same as U on 7seg */
    0x41, /* W — approximate */
    0x09, /* X — approximate */
    0x11, /* Y */
    0x24, /* Z */
};

#define SEG7_BLANK  0x7F   /* all segments OFF — blank digit */
#define SEG7_DASH   0x3F   /* just middle segment — dash     */

/* ── PRIVATE STATE ─────────────────────────────────────── */

/* marquee buffer — holds the category name padded with spaces
 * e.g. "    FOOD    " so it scrolls in and out cleanly
 * max category name = 7 chars (COUNTRY), pad 4 each side = 15 */
#define MARQUEE_BUF_SIZE 16
static unsigned char marquee_buf[MARQUEE_BUF_SIZE];
static int           marquee_len;       /* actual length of string in buf */
static int           marquee_pos;       /* current scroll position        */

/* ── HELPER: char to 7-segment encoding ────────────────── */
static unsigned char char_to_seg7(char c) {
    if (c >= '0' && c <= '9') return seg7_digits[c - '0'];
    if (c >= 'A' && c <= 'Z') return seg7_alpha[c - 'A'];
    if (c >= 'a' && c <= 'z') return seg7_alpha[c - 'a'];
    return SEG7_BLANK;   /* space or unknown = blank */
}

/* ── HELPER: write 4 bytes to HEX0-3 ───────────────────── */
static void write_hex03(unsigned char h0, unsigned char h1,
                        unsigned char h2, unsigned char h3) {
    volatile int *ptr = (int *) HEX03_BASE;
    /* pack 4 bytes into one 32-bit write
     * byte 0 = HEX0 (rightmost), byte 3 = HEX3 (leftmost) */
    *ptr = (h3 << 24) | (h2 << 16) | (h1 << 8) | h0;
}

/* ── HELPER: write 2 bytes to HEX4-5 ───────────────────── */
static void write_hex45(unsigned char h4, unsigned char h5) {
    volatile int *ptr = (int *) HEX45_BASE;
    *ptr = (h5 << 8) | h4;
}

/* ── hex_start_marquee ──────────────────────────────────────
 * Called once in CATEGORY_SELECT when category is confirmed
 * Builds the scroll buffer for the chosen category name
 * category: 0=Food 1=Animal 2=Country 3=Movie
 * ---------------------------------------------------------- */
void hex_start_marquee(int category) {
    const char *names[4] = {"FOOD", "ANIMAL", "COUNTRY", "MOVIE"};
    const char *name;
    int i, j;

    if (category < 0 || category > 3) return;
    name = names[category];

    /* build buffer: 4 spaces + name + 4 spaces
     * this lets the name scroll fully in and out */
    i = 0;
    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';

    for (j = 0; name[j] != '\0' && i < MARQUEE_BUF_SIZE - 1; j++) {
        marquee_buf[i++] = name[j];
    }

    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';
    marquee_buf[i++] = ' ';

    marquee_len = i;
    marquee_pos = 0;
}

/* ── hex_tick_marquee ───────────────────────────────────────
 * Call this in a loop or from a timer to advance the scroll
 * Each call shifts the displayed window one position left
 * Wraps around when it reaches the end
 *
 * game_controller.c calls this each iteration of the
 * PLAYING while loop so the scroll keeps moving
 * ---------------------------------------------------------- */
void hex_tick_marquee(void) {
    unsigned char d0, d1, d2, d3;
    int i0, i1, i2, i3;

    if (marquee_len == 0) return;

    /* get 4 characters at current window position
     * HEX3 is leftmost, HEX0 is rightmost
     * so we show buf[pos+3] buf[pos+2] buf[pos+1] buf[pos+0] */
    i3 = (marquee_pos + 3) % marquee_len;
    i2 = (marquee_pos + 2) % marquee_len;
    i1 = (marquee_pos + 1) % marquee_len;
    i0 = (marquee_pos + 0) % marquee_len;

    d3 = char_to_seg7(marquee_buf[i3]);
    d2 = char_to_seg7(marquee_buf[i2]);
    d1 = char_to_seg7(marquee_buf[i1]);
    d0 = char_to_seg7(marquee_buf[i0]);

    write_hex03(d0, d1, d2, d3);

    /* advance scroll position, wrap around */
    marquee_pos = (marquee_pos + 1) % marquee_len;
}

/* ── hex_set_questions ──────────────────────────────────────
 * Displays questions remaining on HEX4-5
 * n: 0 to 20
 * e.g. n=20 → HEX5 shows '2', HEX4 shows '0'
 *      n=7  → HEX5 shows ' ', HEX4 shows '7'
 *      n=0  → HEX5 shows '0', HEX4 shows '0'
 * ---------------------------------------------------------- */
void hex_set_questions(int n) {
    unsigned char h4, h5;

    if (n < 0)  n = 0;
    if (n > 20) n = 20;

    h4 = seg7_digits[n % 10];          /* ones digit  → HEX4 */
    h5 = (n >= 10) ? seg7_digits[n / 10] : SEG7_BLANK; /* tens → HEX5 */

    write_hex45(h4, h5);
}

/* ── hex_clear ──────────────────────────────────────────────
 * Blanks all 6 HEX displays
 * Call at start of WELCOME state to clear leftover display
 * ---------------------------------------------------------- */
void hex_clear(void) {
    write_hex03(SEG7_BLANK, SEG7_BLANK, SEG7_BLANK, SEG7_BLANK);
    write_hex45(SEG7_BLANK, SEG7_BLANK);
    marquee_len = 0;
    marquee_pos = 0;
}