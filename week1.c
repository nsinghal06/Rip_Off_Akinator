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
#define SEG_4   0b01100110
#define SEG_5   0b01101101
#define SEG_6   0b01111101
#define SEG_7   0b00000111
#define SEG_8   0b01111111
#define SEG_9   0b01101111

#define SEG_A   0b01110111
#define SEG_C   0b00111001
#define SEG_D   0b01011110
#define SEG_E   0b01111001
#define SEG_F   0b01110001
#define SEG_I   0b00000110
#define SEG_L   0b00111000
#define SEG_M   0b00110111
#define SEG_N   0b00110111
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
#define TIMER_BASE  0xFF202000
#define COUNT_DOWN  100000000 // 1 second at 100 MHz
#define COUNT_DOWN_LEDs  25000000	
	
void delay_seconds(int seconds) {
    volatile int *timer_ptr = (volatile int *) TIMER_BASE;
    for (int i = 0; i < seconds; i++) {
        *(timer_ptr + 1) = 0;
        *(timer_ptr) = 0;
        *(timer_ptr + 2) = COUNT_DOWN & 0xFFFF;
        *(timer_ptr + 3) = (COUNT_DOWN >> 16) & 0xFFFF;
        *(timer_ptr + 1) = 0x6;
        while ((*(timer_ptr) & 0x1) == 0); //poll the T0 bit
        *(timer_ptr) = 0; //clear
    }
}

void delay_LED(void) {
    volatile int *timer_ptr = (volatile int *) TIMER_BASE;
    *(timer_ptr + 1) = 0;
    *(timer_ptr) = 0;
    *(timer_ptr + 2) = COUNT_DOWN_LEDs & 0xFFFF;
    *(timer_ptr + 3) = (COUNT_DOWN_LEDs >> 16) & 0xFFFF;
    *(timer_ptr + 1) = 0x6;
    while ((*(timer_ptr) & 0x1) == 0);
    *(timer_ptr) = 0;
}

// ---------- HEX display ----------
void write_hex(unsigned int h5, unsigned int h4, unsigned int h3, unsigned int h2, unsigned int h1, unsigned int h0) {
    volatile int *hex_low  = (int *) HEX_BASE;
    volatile int *hex_high = (int *) HEX_HIGH;
    *hex_low  = ((h3 & 0xFF) << 24) | ((h2 & 0xFF) << 16) | ((h1 & 0xFF) << 8) | (h0 & 0xFF);
    *hex_high = ((h5 & 0xFF) <<  8) |  (h4 & 0xFF);
}

void blank_hex(void) {
    write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF);
}

void show_category(int category) {
    if (category == 0) write_hex(SEG_OFF, SEG_OFF, SEG_F, SEG_O, SEG_O, SEG_D);
    else if (category == 1) write_hex(SEG_A, SEG_N, SEG_I, SEG_M, SEG_A, SEG_L);
    else if (category == 2) write_hex(SEG_C, SEG_O, SEG_U, SEG_N, SEG_T, SEG_R);
    else if (category == 3) write_hex(SEG_M, SEG_M, SEG_O, SEG_U, SEG_I, SEG_E);
    else blank_hex();
}

void show_start(void) {
    write_hex(SEG_OFF, SEG_S, SEG_T, SEG_A, SEG_R, SEG_T);
}

void show_number(int n) {
    unsigned int seg[10] = {
        SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
        SEG_5, SEG_6, SEG_7, SEG_8, SEG_9
    };

    int tens = (n / 10) % 10; // 20/10 = 2 % 10 = 2
    int ones =  n % 10; //20 % 10 = 0

    int tens_seg;
    if (tens > 0) {
        tens_seg = seg[tens]; // show tens digit
    } else {
        tens_seg = SEG_OFF; // blank for no tens digit 
    }

    write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, tens_seg, seg[ones]);
}

// ---------- LEDs ----------
void set_leds(unsigned int mask) {
    volatile int *led_ptr = (volatile int *) LED_BASE;
    *led_ptr = mask;
}

int read_and_clear_edgecap(void) {
    volatile int *key_ptr = (volatile int *) KEY_BASE;
    int pressed = *(key_ptr + 3) & 0xF;
    *(key_ptr + 3) = 0xF; // clear edgecap
    return pressed;
}

//poll till keypress
void wait_for_keypress(void) {
    volatile int *key_ptr = (volatile int *) KEY_BASE;
    *(key_ptr + 3) = 0xF; //clear 
    while ((*(key_ptr + 3) & 0xF) == 0);
    *(key_ptr + 3) = 0xF; //clear 
}

//wait for key0 or key1
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
        if (sw == 0x1) return 0; //food
        if (sw == 0x2) return 1; //animal
        if (sw == 0x4) return 2; //country
        if (sw == 0x8) return 3; //movie
    }
}

// ---------- main ----------
int main(void) {
    int state = WELCOME;
    int category = -1;
    int q_count = 20; //question counter
    int led_bit = 0; //which led
    blank_hex();
    set_leds(0);

    while (1) {
        if (state == WELCOME) {
            wait_for_keypress();
            show_start(); //start on HEX
            delay_seconds(2);
            state = CATEGORY;
        }

        else if (state == CATEGORY) {
		while (1) {
			category = get_category_index();
			show_category(category);

			int pressed = *((volatile int *) KEY_BASE + 3) & 0xF;
			if (pressed) {
				*((volatile int *) KEY_BASE + 3) = 0xF;
				break;
			}
		}
		state = COUNTDOWN; // go straight to countdown, no delay
	}

        //----------countdown 3 2 1 ----------
        else if (state == COUNTDOWN) {
            //3
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_3);
            delay_seconds(1);
            //2
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_2);
            delay_seconds(1);
            //1
            write_hex(SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_1);
            delay_seconds(1);

            //starting question count
            q_count = 20;
            show_number(q_count);
            delay_seconds(1);

            led_bit = 0; // start with LED0
            state = QUESTIONS;
        }

        // ---------- QUESTIONS ----------
        else if (state == QUESTIONS) {
            if (q_count == 0) { //end of questions
                blank_hex();
                set_leds(0);
                while (1);
            }

            int led_mask; //actual pattern to write
			if (led_bit == 0) { //on LED0
				led_mask = 1;  //0001
			} else { //on LED1
				led_mask = 2;  //0010
			}
            volatile int *key_ptr = (int *) KEY_BASE;
            *(key_ptr + 3) = 0xF; //clear edgeCap

            //flashing loop
            while (1) {
                //flash on
                set_leds(led_mask);
                delay_LED();  

                //flash off
                set_leds(0);
                delay_LED();

                //check edgecap for KEY0 or KEY1 to break
                int pressed = *(key_ptr + 3) & 0x3;
                if(pressed) {
                    *(key_ptr + 3) = 0xF; //clear edgecap
                    break;
                }
            }

            set_leds(0); //LED off after answer

            //decrement counter update display
            q_count--;
            show_number(q_count);

            //alternate leds
            if (led_bit == 0) {
                led_bit = 1;
            } else {
                led_bit = 0;
            }

            delay_seconds(1);
        }
    }

    return 0;
}