/* game_controller.c - corrected version */

#define WELCOME         0
#define CATEGORY_SELECT 1
#define PLAYING         2
#define GUESS           3
#define RESULT          4

int state;
int prev_state;        // ← track previous state
int category;
int questions_asked;
int last_answer;

void run_game(void) {
    state      = WELCOME;
    prev_state = -1;       // -1 means "no previous state"

    while (1) {

        // ── WELCOME ──────────────────────────────────────────
        if (state == WELCOME) {
            if (prev_state != WELCOME) {    // just entered this state
                vga_draw_welcome();         // draw screen ONCE
                prev_state = WELCOME;
            }
            if (key_pressed())
                state = CATEGORY_SELECT;
        }

        // ── CATEGORY SELECT ──────────────────────────────────
        else if (state == CATEGORY_SELECT) {
            if (prev_state != CATEGORY_SELECT) {  // just entered
                vga_draw_category();              // draw screen ONCE
                prev_state = CATEGORY_SELECT;
            }
            category = read_category();
            if (category != -1) {
                hex_start_marquee(category);
                hex_set_questions(20);
                questions_asked = 0;
                bayes_init(category);
                state = PLAYING;
            }
        }

        // ── PLAYING ──────────────────────────────────────────
        else if (state == PLAYING) {
            if (prev_state != PLAYING) {    // just entered PLAYING
                vga_draw_background(category); // draw bg + box + YES/NO ONCE
                prev_state = PLAYING;
            }

            // now just swap the question text each round
            int q = question_engine_next();
            vga_draw_question(q);           // only redraws text
            audio_play_thinking();

            last_answer = wait_for_answer();

            bayes_update(q, last_answer);
            questions_asked++;
            hex_set_questions(20 - questions_asked);

            if (bayes_top_probability() > 0.90)
                state = GUESS;
            else if (questions_asked >= 20)
                state = GUESS;
        }

        // ── GUESS ────────────────────────────────────────────
        else if (state == GUESS) {
            if (prev_state != GUESS) {      // just entered GUESS
                int guess = bayes_top_candidate();
                vga_draw_guess(guess);      // draw screen ONCE
                audio_play_guess();
                prev_state = GUESS;
            }
            // just wait here until user acknowledges
            if (key_pressed())
                state = RESULT;
        }

        // ── RESULT ───────────────────────────────────────────
        else if (state == RESULT) {
            if (prev_state != RESULT) {     // just entered RESULT
                vga_draw_result(           
                    bayes_top_probability() > 0.90  // did akinator win?
                );
                prev_state = RESULT;
            }
            if (key_pressed())
                state = WELCOME;
        }
    }
}
"""
WELCOME entered --> vga_draw_welcome()        draws once
CATEGORY entered --> vga_draw_category()       draws once
PLAYING entered --> vga_draw_background()     draws once
  question 1 --> vga_draw_question(q1)     swaps text only
  question 2 --> vga_draw_question(q2)     swaps text only
  ...
GUESS entered --> vga_draw_guess()          draws once
RESULT entered  --> vga_draw_result()         draws once
WELCOME entered --> vga_draw_welcome()        draws once (restart)
"""