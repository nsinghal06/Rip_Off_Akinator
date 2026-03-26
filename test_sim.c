/*
 * test_sim.c  —  Akinator Food Auto-Simulation Harness
 *
 * Compile:  gcc -O2 -o test_sim test_sim.c -lm
 * Run:      ./test_sim
 *
 * For each food, the sim answers every question using the food's own
 * property weights:
 *   >= 0.70  → YES
 *   <= 0.30  → NO
 *   otherwise → SOMETIMES (0.5 weight)
 *
 * Output:  PASS / FAIL / WARN per food, plus a summary report.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ── tuneable limits ── */
#define MAX_Q_LIMIT      20      /* fail if more questions needed     */
#define CONFIDENCE_WIN   0.70f   /* stop asking once top > this       */
#define WARN_THRESHOLD   0.50f   /* warn if correct food never passes this */

/* ── dataset sizes (must match your dataset.h) ── */
#define NUM_FOODS      114
#define NUM_QUESTIONS  130
#define NUM_POOLS        8
#define P1_END           8

/* ── paste-in / include your data ── */
#include "dataset.h"
#include "dataset.c"

/* ════════════════════════════════════════════════════════
   Minimal self-contained Bayes engine (copy of your logic)
   ════════════════════════════════════════════════════════ */

static float  probs[NUM_FOODS];
static int    asked[NUM_QUESTIONS];
static int    pool_unlocked[NUM_POOLS];
static int    any_pool_unlocked_sim;

static float fast_log2_sim(float x) {
    if (x <= 0.0f) return -100.0f;
    int exp = 0;
    while (x < 1.0f) { x *= 2.0f; exp--; }
    while (x >= 2.0f) { x *= 0.5f; exp++; }
    float y = x - 1.0f;
    return y * (1.4426950f - y * (0.7213475f - y * 0.4808983f)) + (float)exp;
}

static void sim_init(void) {
    float eq = 1.0f / NUM_FOODS;
    for (int i = 0; i < NUM_FOODS;     i++) probs[i]         = eq;
    for (int i = 0; i < NUM_QUESTIONS; i++) asked[i]         = 0;
    for (int i = 0; i < NUM_POOLS;     i++) pool_unlocked[i] = 0;
    any_pool_unlocked_sim = 0;
}

static void sim_update(int q, int answer) {
    float sum = 0.0f;
    for (int i = 0; i < NUM_FOODS; i++) {
        float w;
        if      (answer == 0) w = properties[i][q];
        else if (answer == 1) w = 1.0f - properties[i][q];
        else                  w = 0.5f;
        if (w < 0.001f) w = 0.001f;
        probs[i] *= w;
        sum += probs[i];
    }
    if (sum > 0.0f)
        for (int i = 0; i < NUM_FOODS; i++) probs[i] /= sum;

    for (int p = 0; p < NUM_POOLS; p++) {
        if (q == pool_defs[p][2] && answer == pool_defs[p][3]) {
            pool_unlocked[p] = 1;
            any_pool_unlocked_sim = 1;
        }
    }
}

static int sim_next_question(void) {
    int q, i, p;
    int best_q  = -1;
    float best_ig = -1.0f;

    /* Phase-1 block */
    int all_p1_asked = 1;
    for (i = 0; i < P1_END; i++) if (!asked[i]) { all_p1_asked = 0; break; }

    if (!all_p1_asked) {
        float H = 0.0f;
        for (i = 0; i < NUM_FOODS; i++)
            if (probs[i] > 1e-9f) H -= probs[i] * fast_log2_sim(probs[i]);

        float best_ig_p1 = -1.0f; int best_p1 = -1;
        for (q = 0; q < P1_END; q++) {
            if (asked[q]) continue;
            float py = 0.0f;
            for (i = 0; i < NUM_FOODS; i++) py += probs[i] * properties[i][q];
            float pn = 1.0f - py;
            float Hy = 0.0f, Hn = 0.0f;
            if (py > 1e-6f)
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = probs[i] * properties[i][q] / py;
                    if (post > 1e-9f) Hy -= post * fast_log2_sim(post);
                }
            if (pn > 1e-6f)
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = probs[i] * (1.0f - properties[i][q]) / pn;
                    if (post > 1e-9f) Hn -= post * fast_log2_sim(post);
                }
            float ig = H - (py * Hy + pn * Hn);
            if (ig > best_ig_p1) { best_ig_p1 = ig; best_p1 = q; }
        }
        if (best_p1 >= 0) { asked[best_p1] = 1; return best_p1; }
    }

    float H = 0.0f;
    for (i = 0; i < NUM_FOODS; i++)
        if (probs[i] > 1e-9f) H -= probs[i] * fast_log2_sim(probs[i]);

    /* Top-3 for focused mode */
    int   top3[3]  = {-1,-1,-1};
    float tp3[3]   = { 0, 0, 0};
    for (i = 0; i < NUM_FOODS; i++) {
        if      (probs[i] > tp3[0]) { top3[2]=top3[1]; tp3[2]=tp3[1]; top3[1]=top3[0]; tp3[1]=tp3[0]; top3[0]=i; tp3[0]=probs[i]; }
        else if (probs[i] > tp3[1]) { top3[2]=top3[1]; tp3[2]=tp3[1]; top3[1]=i; tp3[1]=probs[i]; }
        else if (probs[i] > tp3[2]) { top3[2]=i; tp3[2]=probs[i]; }
    }
    int focused = (tp3[1] > 0.25f && tp3[0] < 0.75f && (tp3[0]-tp3[1]) < 0.15f);
    int fc = (tp3[2] < 0.05f) ? 2 : 3;

    int cur_best_pool = -1; float max_pp = -1.0f;
    for (p = 0; p < NUM_POOLS; p++) {
        if (!pool_unlocked[p]) continue;
        float s = 0.0f;
        for (i = 0; i < NUM_FOODS; i++)
            if (properties[i][pool_defs[p][2]] > 0.8f) s += probs[i];
        if (s > max_pp) { max_pp = s; cur_best_pool = p; }
    }

    for (q = 0; q < NUM_QUESTIONS; q++) {
        if (asked[q]) continue;
        int qpool = -1;
        for (p = 0; p < NUM_POOLS; p++)
            if (q >= pool_defs[p][0] && q < pool_defs[p][1]) { qpool = p; break; }
        if (cur_best_pool != -1 && qpool != cur_best_pool) {
            int left = 0;
            for (int k = pool_defs[cur_best_pool][0]; k < pool_defs[cur_best_pool][1]; k++)
                if (!asked[k]) left++;
            if (left > 0) continue;
        }
        int unlocked = 0;
        for (p = 0; p < NUM_POOLS; p++)
            if (q >= pool_defs[p][0] && q < pool_defs[p][1] && pool_unlocked[p]) { unlocked = 1; break; }
        if (!unlocked) continue;

        float py = 0.0f, tw = 0.0f;
        if (focused) {
            for (int t = 0; t < fc; t++) { if (top3[t]<0) continue; py += probs[top3[t]]*properties[top3[t]][q]; tw += probs[top3[t]]; }
        } else {
            for (i = 0; i < NUM_FOODS; i++) if (probs[i] > 0.001f) { py += probs[i]*properties[i][q]; tw += probs[i]; }
        }
        if (tw > 0) py /= tw;
        float pn = 1.0f - py;

        float Hy = 0.0f, Hn = 0.0f;
        if (py > 1e-6f) {
            if (focused) { for (int t=0;t<fc;t++) { if(top3[t]<0) continue; float post=probs[top3[t]]*properties[top3[t]][q]/py; if(post>1e-9f) Hy-=post*fast_log2_sim(post); } }
            else         { for (i=0;i<NUM_FOODS;i++) { float post=probs[i]*properties[i][q]/py; if(post>1e-9f) Hy-=post*fast_log2_sim(post); } }
        }
        if (pn > 1e-6f) {
            if (focused) { for (int t=0;t<fc;t++) { if(top3[t]<0) continue; float post=probs[top3[t]]*(1.0f-properties[top3[t]][q])/pn; if(post>1e-9f) Hn-=post*fast_log2_sim(post); } }
            else         { for (i=0;i<NUM_FOODS;i++) { float post=probs[i]*(1.0f-properties[i][q])/pn; if(post>1e-9f) Hn-=post*fast_log2_sim(post); } }
        }

        float ig = H - (py * Hy + pn * Hn);
        if (focused && top3[0] >= 0 && top3[1] >= 0) {
            float sep = properties[top3[0]][q] - properties[top3[1]][q];
            if (sep < 0) sep = -sep;
            ig += sep * 0.8f;
        }
        if (ig > best_ig) { best_ig = ig; best_q = q; }
    }
    if (best_q >= 0) asked[best_q] = 1;
    return best_q;
}

static int sim_top_candidate(void) {
    int best = 0;
    for (int i = 1; i < NUM_FOODS; i++)
        if (probs[i] > probs[best]) best = i;
    return best;
}

/* ════════════════════════════════════════
   Auto-answer: use the food's own weights
   ════════════════════════════════════════ */
static int auto_answer(int food_idx, int q) {
    float w = properties[food_idx][q];
    if (w >= 0.70f) return 0;   /* YES       */
    if (w <= 0.30f) return 1;   /* NO        */
    return 2;                    /* SOMETIMES */
}

/* ════════════════════════
   Result codes
   ════════════════════════ */
#define R_PASS  0   /* correct guess within MAX_Q_LIMIT           */
#define R_WARN  1   /* correct but took > MAX_Q_LIMIT questions   */
#define R_FAIL  2   /* wrong food guessed                         */
#define R_LOST  3   /* ran out of questions entirely              */

typedef struct {
    int  code;
    int  questions_used;
    int  guessed_idx;
    float correct_prob_at_end;
    float guessed_prob_at_end;
    /* trail of questions asked */
    int  q_trail[MAX_Q_LIMIT + 10];
    int  a_trail[MAX_Q_LIMIT + 10];
    int  trail_len;
} SimResult;

static SimResult run_sim(int food_idx) {
    SimResult r = {0};
    sim_init();

    int q, answer;
    int qcount = 0;

    while (qcount < MAX_Q_LIMIT + 5) {   /* +5 slack to see where it goes */
        q = sim_next_question();
        if (q < 0) break;

        answer = auto_answer(food_idx, q);
        sim_update(q, answer);
        qcount++;

        /* store trail */
        if (r.trail_len < MAX_Q_LIMIT + 9) {
            r.q_trail[r.trail_len] = q;
            r.a_trail[r.trail_len] = answer;
            r.trail_len++;
        }

        /* check confidence */
        if (probs[sim_top_candidate()] > CONFIDENCE_WIN) break;
    }

    r.questions_used       = qcount;
    r.guessed_idx          = sim_top_candidate();
    r.correct_prob_at_end  = probs[food_idx];
    r.guessed_prob_at_end  = probs[r.guessed_idx];

    if (r.guessed_idx == food_idx) {
        r.code = (qcount <= MAX_Q_LIMIT) ? R_PASS : R_WARN;
    } else if (q < 0 || qcount >= MAX_Q_LIMIT + 5) {
        r.code = R_LOST;
    } else {
        r.code = R_FAIL;
    }
    return r;
}

/* ════════════════════════
   Helpers
   ════════════════════════ */
static const char *answer_str(int a) {
    if (a == 0) return "YES ";
    if (a == 1) return "NO  ";
    return "SOME";
}

static const char *pool_label(int q) {
    if (q < P1_END) return "PHASE1 ";
    const char *n[] = {"DESSERT","FRUIT  ","VEG    ","ASIAN  ","SNACK  ","SOUP   ","MEAT   ","WESTERN"};
    for (int p = 0; p < NUM_POOLS; p++)
        if (q >= pool_defs[p][0] && q < pool_defs[p][1]) return n[p];
    return "?      ";
}

/* ════════════════════════
   MAIN
   ════════════════════════ */
int main(void) {
    SimResult results[NUM_FOODS];
    int pass=0, warn=0, fail=0, lost=0;

    printf("Running simulation for all %d foods...\n\n", NUM_FOODS);

    for (int f = 0; f < NUM_FOODS; f++)
        results[f] = run_sim(f);

    /* ── SUMMARY TABLE ── */
    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("  %-28s  %6s  %4s  %-28s  %s\n",
           "FOOD", "STATUS", " Qs", "GUESSED AS", "CORRECT%");
    printf("═══════════════════════════════════════════════════════════════════════\n");

    for (int f = 0; f < NUM_FOODS; f++) {
        SimResult *r = &results[f];
        const char *status;
        switch (r->code) {
            case R_PASS: status = "PASS ✓"; pass++; break;
            case R_WARN: status = "WARN ⚠"; warn++; break;
            case R_FAIL: status = "FAIL ✗"; fail++; break;
            default:     status = "LOST ~"; lost++; break;
        }
        printf("  %-28s  %-6s  %3d   %-28s  %.1f%%\n",
               food_names[f],
               status,
               r->questions_used,
               food_names[r->guessed_idx],
               r->correct_prob_at_end * 100.0f);
    }

    printf("═══════════════════════════════════════════════════════════════════════\n");
    printf("\n  RESULTS:  PASS=%d  WARN=%d  FAIL=%d  LOST=%d  (total=%d)\n\n",
           pass, warn, fail, lost, NUM_FOODS);

    /* ── DETAILED FAILURE REPORT ── */
    int has_issues = (warn + fail + lost) > 0;
    if (has_issues) {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("  DETAILED FAILURE / WARNING REPORT\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

        for (int f = 0; f < NUM_FOODS; f++) {
            SimResult *r = &results[f];
            if (r->code == R_PASS) continue;

            const char *tag = (r->code==R_WARN) ? "⚠ WARN" :
                              (r->code==R_FAIL) ? "✗ FAIL" : "~ LOST";

            printf("  [%s]  %s\n", tag, food_names[f]);
            if (r->code == R_FAIL)
                printf("        Guessed: %s (%.1f%%)  |  Correct: %.1f%%\n",
                       food_names[r->guessed_idx],
                       r->guessed_prob_at_end * 100.0f,
                       r->correct_prob_at_end  * 100.0f);
            else if (r->code == R_WARN)
                printf("        Took %d questions (limit=%d), correct at %.1f%%\n",
                       r->questions_used, MAX_Q_LIMIT,
                       r->correct_prob_at_end * 100.0f);
            else
                printf("        Ran out of questions. Correct at %.1f%%\n",
                       r->correct_prob_at_end * 100.0f);

            printf("        Question trail:\n");
            for (int t = 0; t < r->trail_len; t++) {
                int q  = r->q_trail[t];
                int a  = r->a_trail[t];
                float w = properties[f][q];
                printf("          Q%02d [%s] [%s] (weight=%.2f)  %s\n",
                       t+1, pool_label(q), answer_str(a), w, questions[q]);
            }
            printf("\n");
        }
    }

    /* ── DUPLICATE FOOD NAME WARNING ── */
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  DUPLICATE NAME CHECK\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    int dup_found = 0;
    for (int a = 0; a < NUM_FOODS; a++)
        for (int b = a+1; b < NUM_FOODS; b++)
            if (strcasecmp(food_names[a], food_names[b]) == 0) {
                printf("  DUPLICATE: \"%s\" at index %d and %d\n",
                       food_names[a], a, b);
                dup_found++;
            }
    if (!dup_found) printf("  No duplicates found.\n");

    /* ── AMBIGUOUS WEIGHT CHECK ── */
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  AMBIGUOUS WEIGHT CHECK  (weights 0.40–0.60 in important questions)\n");
    printf("  These are properties that will answer SOMETIMES — review if intended\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for (int f = 0; f < NUM_FOODS; f++) {
        int printed_header = 0;
        /* only flag phase-1 and pool-defining questions */
        for (int q = 0; q < P1_END; q++) {
            float w = properties[f][q];
            if (w > 0.35f && w < 0.65f) {
                if (!printed_header) { printf("  %s:\n", food_names[f]); printed_header = 1; }
                printf("    Q%02d (weight=%.2f) [PHASE1] %s\n", q, w, questions[q]);
            }
        }
    }

    printf("\nDone.\n");
    return 0;
}