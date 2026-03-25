/* ----------------------------------------------------------
 * bayes_engine.c
 * Bayesian probability engine for Akinator
 * Reads from dataset.c, maintains probabilities internally
 * ----------------------------------------------------------*/

#include "bayes_engine.h"
#include "dataset.h"

/* ── PRIVATE STATE ─────────────────────────────────────────
 * Lives inside bayes_engine.c only
 * Nobody outside can see or touch these directly
 * ---------------------------------------------------------- */
static float probabilities[NUM_FOODS];   /* one per food, sums to 1.0  */
static int   asked[NUM_QUESTIONS];       /* 1 if question already asked */

/* ── bayes_init ────────────────────────────────────────────
 * Called once when entering PLAYING state
 * Sets all probabilities equal (1/NUM_FOODS each)
 * Resets asked[] array so all questions available
 * ---------------------------------------------------------- */
void bayes_init(void) {
    int i;
    float equal = 1.0f / NUM_FOODS;

    for (i = 0; i < NUM_FOODS; i++) {
        probabilities[i] = equal;
    }
    for (i = 0; i < NUM_QUESTIONS; i++) {
        asked[i] = 0;
    }
}

/* ── bayes_update ──────────────────────────────────────────
 * Called after every answer
 * q:      index of question that was asked (0 to NUM_QUESTIONS-1)
 * answer: 0 = YES, 1 = NO, 2 = SOMETIMES
 *
 * YES: multiply each food's prob by its property value
 * NO:  multiply each food's prob by (1 - property value)
 * SOMETIMES: multiply by 0.5 (neutral, soft update)
 *
 * Then renormalise so all probs still sum to 1.0
 * ---------------------------------------------------------- */
void bayes_update(int q, int answer) {
    int i;
    float sum = 0.0f;
    float weight;

    for (i = 0; i < NUM_FOODS; i++) {
        if (answer == 0) {
            /* YES: food matches if property is high */
            weight = properties[i][q];
        } else if (answer == 1) {
            /* NO: food matches if property is low */
            weight = 1.0f - properties[i][q];
        } else {
            /* SOMETIMES: neutral update */
            weight = 0.5f;
        }

        /* avoid multiplying by exactly 0 — keeps food alive with tiny prob */
        if (weight < 0.01f) weight = 0.01f;

        probabilities[i] *= weight;
        sum += probabilities[i];
    }

    /* renormalise so everything still sums to 1.0 */
    if (sum > 0.0f) {
        for (i = 0; i < NUM_FOODS; i++) {
            probabilities[i] /= sum;
        }
    }
}

/* ── bayes_next_question ───────────────────────────────────
 * Finds the best next question to ask
 * Best = yes_weight closest to 0.5 (most even split)
 *
 * yes_weight for question q =
 *   sum of (probabilities[food] * properties[food][q])
 *   for all foods
 *
 * Returns index of best unasked question
 * Marks it as asked so it won't be picked again
 * ---------------------------------------------------------- */
int bayes_next_question(void) {
    int q, i;
    int best_q = -1;
    float best_distance = 999.0f;
    float yes_weight;
    float distance;

    for (q = 0; q < NUM_QUESTIONS; q++) {
        if (asked[q]) continue;   /* skip already asked questions */

        /* calculate yes_weight for this question */
        yes_weight = 0.0f;
        for (i = 0; i < NUM_FOODS; i++) {
            yes_weight += probabilities[i] * properties[i][q];
        }

        /* how far from 50/50? closer to 0 = better question */
        distance = yes_weight - 0.5f;
        if (distance < 0.0f) distance = -distance;   /* absolute value */

        if (distance < best_distance) {
            best_distance = distance;
            best_q = q;
        }
    }

    /* mark as asked */
    if (best_q >= 0) {
        asked[best_q] = 1;
    }

    return best_q;
}

/* ── bayes_top_probability ─────────────────────────────────
 * Returns the highest probability in the array
 * game_controller checks: if > 0.90 → go to GUESS state
 * ---------------------------------------------------------- */
float bayes_top_probability(void) {
    int i;
    float best = 0.0f;

    for (i = 0; i < NUM_FOODS; i++) {
        if (probabilities[i] > best) {
            best = probabilities[i];
        }
    }

    return best;
}

/* ── bayes_top_candidate ───────────────────────────────────
 * Returns the INDEX of the food with highest probability
 * game_controller passes this to vga_draw_guess(index)
 * vga_display.c looks up food_names[index] to draw the name
 * ---------------------------------------------------------- */
int bayes_top_candidate(void) {
    int i;
    int best_i = 0;

    for (i = 1; i < NUM_FOODS; i++) {
        if (probabilities[i] > probabilities[best_i]) {
            best_i = i;
        }
    }

    return best_i;
}