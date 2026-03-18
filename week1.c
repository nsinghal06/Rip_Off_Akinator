//Week 1 Main
/*
Switch encoding
    0001 Food
    0010 Animal
    0100 Country
    1000 Movie
    other invalid / none
 */
//memory maps
#define HEX_BASE  0xFF200020  //HEX3 - 0
#define HEX_HIGH  0xFF200030  //HEX5 - 4
#define SW_BASE   0xFF200040  //SW9 - 0
#define KEY_BASE  0xFF200050  //KEY3 - 0
#define LED_BASE  0xFF200000  //LEDR9 - 0

//HEX display bit codes
#define SEG_OFF 0b00000000
#define SEG_0   0b00111111
#define SEG_1   0b00000110
#define SEG_2   0b01011011
#define SEG_3   0b01001111
#define SEG_A   0b01110111
#define SEG_C   0b00111001
#define SEG_D   0b01011110
#define SEG_E   0b01111001
#define SEG_F   0b01110001
#define SEG_I   0b00000110
#define SEG_L   0b00111000
#define SEG_M   0b00110111
#define SEG_N   0b01100111
#define SEG_O   0b00111111
#define SEG_R   0b01010000
#define SEG_S   0b01101101
#define SEG_T   0b01111000
#define SEG_U   0b00111110
#define SEG_V   0b00011100
#define SEG_Y   0b01101110

//game states
#define WELCOME   0
#define CATEGORY  1
#define COUNTDOWN 2
#define QUESTIONS 3

// ---------- delay ----------
// Busy-wait ~1 second (calibrate DELAY_1S for your board's clock if needed)
#define DELAY_1S 2500000

void delay_seconds(int seconds) {
    volatile int i;
    for (int s = 0; s < seconds; s++)
        for (i = 0; i < DELAY_1S; i++);
}

// ---------- HEX display ----------
void write_hex(unsigned int h5, unsigned int h4, unsigned int h3,
               unsigned int h2, unsigned int h1, unsigned int h0) {
    volatile int *hex_low  = (volatile int *) HEX_BASE;
    volatile int *hex_high = (volatile int *) HEX_HIGH;
    *hex_low  = ((h3 & 0xFF) << 24) | ((h2 & 0xFF) << 16) | ((h1 & 0xFF) << 8) | (h0 & 0xFF);
    *hex_high = ((h5 & 0xFF) <<  8) |  (h4 & 0xFF);
}

void blank_hex(void) {
    write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF);
}

void show_category(int category) {
    if      (category == 0) write_hex(SEG_OFF, SEG_OFF, SEG_F,   SEG_O,   SEG_O,   SEG_D);
    else if (category == 1) write_hex(SEG_A,   SEG_N,   SEG_I,   SEG_M,   SEG_A,   SEG_L);
    else if (category == 2) write_hex(SEG_C,   SEG_O,   SEG_U,   SEG_N,   SEG_T,   SEG_R);
    else if (category == 3) write_hex(SEG_OFF, SEG_M,   SEG_O,   SEG_V,   SEG_I,   SEG_E);
    else                    blank_hex();
}

void show_start(void) {
    write_hex(SEG_OFF, SEG_S, SEG_T, SEG_A, SEG_R, SEG_T);
}

// Display a 1 or 2 digit number on HEX1..HEX0
void show_number(int n) {
    // Segment lookup for digits 0-9
    unsigned int seg[10] = {
        SEG_0, SEG_1, SEG_2, SEG_3,
        0b01100110, // 4
        SEG_S,      // 5 (same as S)
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };
    int tens = (n / 10) % 10;
    int ones =  n % 10;
    // Only show tens digit if non-zero
    write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF,
              (tens > 0) ? seg[tens] : SEG_OFF,
              seg[ones]);
}

// ---------- LEDs ----------
void set_leds(unsigned int mask) {
    volatile int *led_ptr = (volatile int *) LED_BASE;
    *led_ptr = mask;
}

// ─── Keys (always use edgecap register, offset +3) ────────────────────────────
// Returns bitmask of which keys were pressed (KEY0=bit0, KEY1=bit1, etc.)
int read_and_clear_edgecap(void) {
    volatile int *key_ptr = (volatile int *) KEY_BASE;
    int pressed = *(key_ptr + 3) & 0xF;
    *(key_ptr + 3) = 0xF; // clear edgecap
    return pressed;
}

void wait_for_keypress(void) {
    volatile int *key_ptr = (volatile int *) KEY_BASE;
    *(key_ptr + 3) = 0xF; // clear first
    while ((*(key_ptr + 3) & 0xF) == 0);
    *(key_ptr + 3) = 0xF; // clear after
}

// Wait for KEY0 or KEY1 only, returns 0 for KEY0, 1 for KEY1
int wait_for_answer(void) {
    volatile int *key_ptr = (volatile int *) KEY_BASE;
    *(key_ptr + 3) = 0xF; // clear edgecap first
    while (1) {
        int pressed = *(key_ptr + 3) & 0x3; // only KEY0 and KEY1
        if (pressed & 0x1) { *(key_ptr + 3) = 0xF; return 0; } // KEY0 = No
        if (pressed & 0x2) { *(key_ptr + 3) = 0xF; return 1; } // KEY1 = Yes
    }
}

// ---------- category select ----------
int get_category_index(void) {
    volatile int *sw_ptr = (volatile int *) SW_BASE;
    int sw;
    while (1) {
        sw = *(sw_ptr) & 0xF;
        if (sw == 0x1) return 0;  // Food
        if (sw == 0x2) return 1;  // Animal
        if (sw == 0x4) return 2;  // Country
        if (sw == 0x8) return 3;  // Movie
    }
}

// ---------- main ----------
int main(void) {
    int state    = WELCOME;
    int category = -1;
    int q_count  = 20;      // question counter
    int led_bit  = 0;       // alternates between LED0 and LED1 each question

    blank_hex();
    set_leds(0);

    while (1) {

        if (state == WELCOME) {
            wait_for_keypress();
            show_start();
            state = CATEGORY;
        }

        else if (state == CATEGORY) {
            category = get_category_index();
            show_category(category);
            delay_seconds(2);   // show category name briefly before countdown
            state = COUNTDOWN;
        }

        //----------countdown 3 2 1 ----------
        else if (state == COUNTDOWN) {
            // Show 3
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_3);
            delay_seconds(1);
            // Show 2
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_2);
            delay_seconds(1);
            // Show 1
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_1);
            delay_seconds(1);

            // Show starting question count (20)
            q_count = 20;
            show_number(q_count);
            delay_seconds(1);

            led_bit = 0;        // start with LED0
            state = QUESTIONS;
        }

        // ---------- QUESTIONS ----------
        else if (state == QUESTIONS) {

            if (q_count == 0) {
                // All done — blank everything and stop
                blank_hex();
                set_leds(0);
                while (1);      // halt
            }

    
            unsigned int led_mask = (1 << led_bit); // LED0 or LED1
            volatile int *key_ptr = (volatile int *) KEY_BASE;
            *(key_ptr + 3) = 0xF; // clear edgecap before flashing loop

            while (1) {
                // Flash on
                set_leds(led_mask);
                delay_seconds(1);  // adjust for faster flash if desired

                // Flash off
                set_leds(0);
                delay_seconds(1);

                // Check edgecap for KEY0 or KEY1 — break on either
                int pressed = *(key_ptr + 3) & 0x3;
                if (pressed) {
                    *(key_ptr + 3) = 0xF; // clear edgecap
                    break;
                }
            }

            set_leds(0); // ensure LED is off after answer

            // Decrement counter and update display
            q_count--;
            show_number(q_count);

            // Alternate LED for next question
            led_bit = 1 - led_bit; // toggles 0 → 1 → 0 → ...

            delay_seconds(1); // brief pause before next question's flash starts
        }
    }

    return 0;
}