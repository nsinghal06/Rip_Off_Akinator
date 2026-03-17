/* game_controller.c
 * top-level state machine, calls all other modules, decides which state
 * States: WELCOME, CATEGORY_SELECT, PLAYING, GUESS, RESULT
 */

// States
#define WELCOME 0
#define CATEGORY_SELECT 1
#define PLAYING 2
#define GUESS 3
#define RESULT 4

int state; // current state
int category; // 0 = Food, 1 = Animal, 2 = Country, 3 = Movie
int questions_asked; // 0 to 20
int last_answer; // 0=YES, 1=NO

void run_game(void) {
    state = WELCOME;

    while (1) {
        // ----------- WELCOME -----------
        if(state == WELCOME) {
            vga_draw_welcome(); // calls vga_display.c
            // wait for any KEY press to continue
            if (key_pressed())        // calls answer_input.c
                state = CATEGORY_SELECT;
        }

        // ----------- CATEGORY SELECT -----------
        else if (state == CATEGORY_SELECT) {
            vga_draw_category();      // calls vga_display.c
            category = read_category(); // calls category_select.c
            if (category != -1) {
                hex_start_marquee(category); // calls hex_display.c
                hex_set_questions(20);       // calls hex_display.c
                questions_asked = 0;
                bayes_init(category);        // calls bayes_engine.c
                state = PLAYING;
            }
        }

        // ── PLAYING ──────────────────────────────
        else if (state == PLAYING) {
            int q = question_engine_next(); // calls question_engine.c
            vga_draw_question(q);           // calls vga_display.c
            audio_play_thinking();          // calls audio_output.c

            last_answer = wait_for_answer();// calls answer_input.c
                                            // blocks until KEY0 or KEY1

            bayes_update(q, last_answer);   // calls bayes_engine.c
            questions_asked++;
            hex_set_questions(20 - questions_asked); // calls hex_display.c

            // check win conditions
            if (bayes_top_probability() > 0.90)
                state = GUESS;
            else if (questions_asked >= 20)
                state = GUESS;
        }

        // ── GUESS ────────────────────────────────
        else if (state == GUESS) {
            int guess = bayes_top_candidate(); // calls bayes_engine.c
            vga_draw_guess(guess);             // calls vga_display.c
            audio_play_guess();                // calls audio_output.c
            state = RESULT;
        }

        // ── RESULT ───────────────────────────────
        else if (state == RESULT) {
            // wait for KEY press, then restart
            if (key_pressed())                 // calls answer_input.c
                state = WELCOME;
        }
    }
}
