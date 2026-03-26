#include <stdio.h>
#include <string.h>
//#include <math.h>

#include "dataset.h"
#include "dataset.c"
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()

int any_pool_unlocked = 0;
static float probabilities[127];
static int   asked[130];
static int   pool_unlocked[8];

// Fast natural log approximation for embedded - no math.h needed
static float fast_log2(float x) {
    if (x <= 0.0f) return -100.0f;
    int exp = 0;
    while (x < 1.0f) { x *= 2.0f; exp--; }
    while (x >= 2.0f) { x *= 0.5f; exp++; }
    // Polynomial approximation of log2 for x in [1,2)
    float y = x - 1.0f;
    float result = y * (1.4426950f - y * (0.7213475f - y * 0.4808983f));
    return result + (float)exp;
}

//initialize probabilities to equal at game start
static void bayes_init(void) {
    int i;
    float equal = 1.0f / NUM_FOODS; //1/127 = 0.8%
    for (i = 0; i < NUM_FOODS;     i++) probabilities[i]  = equal;
    for (i = 0; i < NUM_QUESTIONS; i++) asked[i]          = 0;
    for (i = 0; i < NUM_POOLS;     i++) pool_unlocked[i]  = 0;
    any_pool_unlocked = 0;
}

// update probabilities after an answer
// answer: 0 = YES  1 = NO  2 = SOMETIMES
static void bayes_update(int q, int answer) {
    int i, p;
    float sum = 0.0f, weight;

    for (i = 0; i < NUM_FOODS; i++) {
        if (answer == 0)      weight = properties[i][q];
        else if (answer == 1) weight = 1.0f - properties[i][q];
        else                  weight = 0.5f;

        if (weight < 0.001f) weight = 0.001f;
        probabilities[i] *= weight;
        sum += probabilities[i];
    }

    if (sum > 0.0f)
        for (i = 0; i < NUM_FOODS; i++) probabilities[i] /= sum;

    // check if this answer unlocks any pool
    for (p = 0; p < NUM_POOLS; p++) {
        int unlock_q   = pool_defs[p][2];
        int unlock_ans = pool_defs[p][3];
        if (q == unlock_q && answer == unlock_ans) {
            pool_unlocked[p] = 1;
            any_pool_unlocked = 1;
        }
    }
}

static float bayes_top_probability(void) {
    float best = 0.0f;
    for (int i = 0; i < NUM_FOODS; i++)
        if (probabilities[i] > best) best = probabilities[i];
    return best;
}

// concept of entropy: measures how confused the computation is
//information gain: how much a question reduces entropy/confusion, compares between current entropy and expected future entropy after asking a question
// If all candidates have equal probability, entropy is high
// If one candidate has 99% probability, entropy is low
// We want to pick questions that reduce entropy the most (gives the most information).
static int bayes_next_question(void) {
    int q, i, p;
    int best_q  = -1;
    float best_ig = -1.0f;

    // 1. PHASE 1 use information gain to pick best phase 1 question
    int all_p1_asked = 1;
    for (i = 0; i < P1_END; i++) { //checking all pools
        if (!asked[i]) { all_p1_asked = 0; break; } //ask all pools
    }

    if (!all_p1_asked) { //still asking all pools, this will pick best phase 1 question 
        float best_ig_p1 = -1.0f;
        int   best_p1_q  = -1;
        float H_current_p1 = 0.0f; //current entropy 
        for (i = 0; i < NUM_FOODS; i++) {
            if (probabilities[i] > 1e-9f)
                H_current_p1 -= probabilities[i] * (fast_log2(probabilities[i])); //formula to calculate entropy of current system
        }

        //information gain = current confusion - expected future confusion
        //highest information gain means reduces confusion the most
        for (q = 0; q < P1_END; q++) {
            if (asked[q]) continue;
            float p_yes = 0.0f;
            for (i = 0; i < NUM_FOODS; i++)
                p_yes += probabilities[i] * properties[i][q];
            float p_no = 1.0f - p_yes;
            float H_yes = 0.0f, H_no = 0.0f;
            if (p_yes > 1e-6f) {
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = (probabilities[i] * properties[i][q]) / p_yes;
                    if (post > 1e-9f) H_yes -= post * (fast_log2(post));
                }
            }
            if (p_no > 1e-6f) {
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = (probabilities[i] * (1.0f - properties[i][q])) / p_no;
                    if (post > 1e-9f) H_no -= post * (fast_log2(post));
                }
            }
            float ig = H_current_p1 - (p_yes * H_yes + p_no * H_no);
            if (ig > best_ig_p1) { best_ig_p1 = ig; best_p1_q = q; }
        }
        if (best_p1_q >= 0) { asked[best_p1_q] = 1; return best_p1_q; }
    }

    // 2. ENTROPY CALCULATION
    // Phase 1 done, calculate current system entropy
    float H_current = 0.0f;
    for (i = 0; i < NUM_FOODS; i++) {
        if (probabilities[i] > 1e-9f)
            H_current -= probabilities[i] * (fast_log2(probabilities[i]));
    }

    // 3. FOCUSED MODE
    // Find top 3 candidates
    int   top3[3]      = {-1, -1, -1};
    float top3_prob[3] = { 0,  0,  0};
    for (i = 0; i < NUM_FOODS; i++) {
        if (probabilities[i] > top3_prob[0]) {
            top3[2] = top3[1]; top3_prob[2] = top3_prob[1];
            top3[1] = top3[0]; top3_prob[1] = top3_prob[0];
            top3[0] = i;       top3_prob[0] = probabilities[i];
        } else if (probabilities[i] > top3_prob[1]) {
            top3[2] = top3[1]; top3_prob[2] = top3_prob[1];
            top3[1] = i;       top3_prob[1] = probabilities[i];
        } else if (probabilities[i] > top3_prob[2]) {
            top3[2] = i;       top3_prob[2] = probabilities[i];
        }
    }

    // focused mode triggers when top 2 are both high and close together and no clear winner yet
    int focused_mode = 0;
    float gap = top3_prob[0] - top3_prob[1];
    if (top3_prob[1] > 0.25f && top3_prob[0] < 0.75f && gap < 0.15f) {
        focused_mode = 1;
    }

    // if top 3rd candidate is very low, focus on just top 2
    int focus_count = 3;
    if (top3_prob[2] < 0.05f) focus_count = 2;

    // 4. SMART POOL-BASED QUESTION SELECTION
    // find which unlocked pool has the most probability 
    // makes AI stick to the most likely category
    int current_best_pool = -1;
    float max_pool_prob   = -1.0f;

    for (p = 0; p < NUM_POOLS; p++) {
        if (!pool_unlocked[p]) continue;
        float current_p_sum = 0.0f;
        for (i = 0; i < NUM_FOODS; i++) {
            if (properties[i][pool_defs[p][2]] > 0.8f)
                current_p_sum += probabilities[i];
        }
        if (current_p_sum > max_pool_prob) {
            max_pool_prob     = current_p_sum;
            current_best_pool = p;
        }
    }

    for (q = 0; q < NUM_QUESTIONS; q++) {
        if (asked[q]) continue;

        // --------------- ENFORCED POOL STICKINESS ---------------
        int q_pool = -1;
        for (p = 0; p < NUM_POOLS; p++) {
            if (q >= pool_defs[p][0] && q < pool_defs[p][1]) {
                q_pool = p;
                break;
            }
        }

        // skip questions not in the best pool unless that pool is finished
        if (current_best_pool != -1 && q_pool != current_best_pool) {
            int questions_left_in_best = 0;
            for (int k = pool_defs[current_best_pool][0]; k < pool_defs[current_best_pool][1]; k++) {
                if (!asked[k]) questions_left_in_best++;
            }
            if (questions_left_in_best > 0) continue;
        }

        // safety: skip questions not in any unlocked pool
        int is_unlocked = 0;
        for (p = 0; p < NUM_POOLS; p++) {
            if (q >= pool_defs[p][0] && q < pool_defs[p][1] && pool_unlocked[p]) {
                is_unlocked = 1;
                break;
            }
        }
        if (!is_unlocked) continue;

        // 5. INFORMATION GAIN CALCULATION
        // focused mode: only consider top 2 or 3 candidates
        // normal mode: consider all foods
        float p_yes = 0.0f;
        float total_weight = 0.0f;

        //if focused only focus on top 3 
        if (focused_mode) {
            for (int t = 0; t < focus_count; t++) {
                if (top3[t] < 0) continue;
                p_yes        += probabilities[top3[t]] * properties[top3[t]][q];
                total_weight += probabilities[top3[t]];
            }
        } else {
            for (i = 0; i < NUM_FOODS; i++) {
                if (probabilities[i] > 0.001f) {
                    p_yes        += probabilities[i] * properties[i][q];
                    total_weight += probabilities[i];
                }
            }
        }
        if (total_weight > 0) p_yes /= total_weight; //normalizing it
        float p_no = 1.0f - p_yes;

        //recalculate actual entropy of yes and no branches after the question
        float H_yes = 0.0f;
        if (p_yes > 1e-6f) {
            if (focused_mode) {
                for (int t = 0; t < focus_count; t++) {
                    if (top3[t] < 0) continue;
                    float post = (probabilities[top3[t]] * properties[top3[t]][q]) / p_yes;
                    if (post > 1e-9f) H_yes -= post * (fast_log2(post));
                }
            } else {
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = (probabilities[i] * properties[i][q]) / p_yes;
                    if (post > 1e-9f) H_yes -= post * (fast_log2(post));
                }
            }
        }

        float H_no = 0.0f;
        if (p_no > 1e-6f) {
            if (focused_mode) {
                for (int t = 0; t < focus_count; t++) {
                    if (top3[t] < 0) continue;
                    float post = (probabilities[top3[t]] * (1.0f - properties[top3[t]][q])) / p_no;
                    if (post > 1e-9f) H_no -= post * (fast_log2(post));
                }
            } else {
                for (i = 0; i < NUM_FOODS; i++) {
                    float post = (probabilities[i] * (1.0f - properties[i][q])) / p_no;
                    if (post > 1e-9f) H_no -= post * (fast_log2(post));
                }
            }
        }

        // calculate information gain and track best question
        float expected_H = p_yes * H_yes + p_no * H_no; //expected future confusion after asking this question
        float ig = H_current - expected_H; //how much this question reduces confusion

        if (focused_mode && top3[0] >= 0 && top3[1] >= 0) {
            float prop_top1 = properties[top3[0]][q];
            float prop_top2 = properties[top3[1]][q];
            float separation = (prop_top1 - prop_top2);
            if (separation < 0) separation = -separation;
            ig += separation * 0.8f;
        }


        if (ig > best_ig) { //track to find best question
            best_ig = ig;
            best_q  = q;
        }

    } 

    if (best_q >= 0) asked[best_q] = 1;
    return best_q; 
}

//next best candidate after asking questions, final guess of computer
static int bayes_top_candidate(void) {
    int i, best = 0;
    for (i = 1; i < NUM_FOODS; i++)
        if (probabilities[i] > probabilities[best]) best = i;
    return best;
}

/* ----------------------- TERMINAL UI ----------------------- */

static const char *pool_name(int q) {
    if (q < P1_END) return "PHASE1";
    int p;
    for (p = 0; p < NUM_POOLS; p++)
        if (q >= pool_defs[p][0] && q < pool_defs[p][1]) {
            const char *names[] = {"DESSERT","FRUIT","VEG","ASIAN","SNACK","SOUP","MEAT","WESTERN"};
            return names[p];
        }
    return "?";
}

static void print_top5(void) {
    int i, j;
    float used[5];
    for (i = 0; i < 5; i++) used[i] = -1.0f;
    printf("\n  Top candidates:\n");
    for (i = 0; i < 5; i++) {
        float best = -1.0f; int best_idx = -1;
        for (j = 0; j < NUM_FOODS; j++) {
            int skip = 0, k;
            for (k = 0; k < i; k++) if ((int)used[k] == j) skip = 1;
            if (!skip && probabilities[j] > best) {
                best = probabilities[j]; best_idx = j;
            }
        }
        if (best_idx < 0) break;
        used[i] = (float)best_idx;
        printf("  %d. %-28s %.1f%%\n",
               i+1, food_names[best_idx], probabilities[best_idx]*100.0f);
    }

    // show which pools are unlocked
    printf("  Unlocked pools: ");
    const char *pnames[] = {"DESSERT","FRUIT","VEG","ASIAN","SNACK","SOUP","MEAT","WESTERN"};
    int any = 0;
    int p;
    for (p = 0; p < NUM_POOLS; p++)
        if (pool_unlocked[p]) { printf("%s ", pnames[p]); any = 1; }
    if (!any) printf("none yet");
    printf("\n\n");
}

static int get_answer(void) {
    char buf[16];
    while (1) {
        printf("  Your answer (y/n/s): ");
        fflush(stdout);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 1;
        if (buf[0]=='y'||buf[0]=='Y') return 0;
        if (buf[0]=='n'||buf[0]=='N') return 1;
        if (buf[0]=='s'||buf[0]=='S') return 2;
        printf("  Please enter y, n, or s\n");
    }
}

/* ----------------------- MAIN ----------------------- */
int main(void) {
    srand(time(NULL));

    int q, answer, questions_asked, candidate;
    char buf[16];

    int  debug_target_idx = -1;
    char target_name[64];
    printf("DEBUG: Which food are you testing for? ");
    fgets(target_name, sizeof(target_name), stdin);
    target_name[strcspn(target_name, "\n")] = 0;

    for (int i = 0; i < NUM_FOODS; i++) {
        if (strcasecmp(food_names[i], target_name) == 0) {
            debug_target_idx = i;
            break;
        }
    }

    printf("\n");
    printf("  ╔══════════════════════════════════╗\n");
    printf("  ║        AKINATOR 2.0  v2          ║\n");
    printf("  ║   Think of a food item...        ║\n");
    printf("  ╚══════════════════════════════════╝\n\n");
    printf("  %d foods  |  %d questions  |  %d category pools\n",
           NUM_FOODS, NUM_QUESTIONS, NUM_POOLS);
    printf("  y=yes  n=no  s=sometimes\n\n");

    do {
        bayes_init();
        questions_asked = 0;

        printf("  Think of a food. Press Enter when ready...");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);

        while (1) {
            q = bayes_next_question();
            if (q < 0) break;

            printf("  Q%02d [%-7s]: %s\n", questions_asked+1, pool_name(q), questions[q]);
            answer = get_answer();
            bayes_update(q, answer);
            questions_asked++;

            if (debug_target_idx != -1) {
                printf("\n  [WATCHDOG] %s probability: %.2f%%\n",
                       food_names[debug_target_idx],
                       probabilities[debug_target_idx] * 100.0f);
            }
            print_top5();

            if (bayes_top_probability() > 0.70f) {
                printf("  >> Confidence > 70%% reached!\n\n");
                break;
            }
        }

        candidate = bayes_top_candidate();
        float top_prob = probabilities[candidate];

        if (top_prob < 0.30f) {
            printf("  I couldn't narrow it down enough (best: %.1f%%).\n",
                   top_prob * 100.0f);
            printf("  What was your food? ");
            fflush(stdout);
            fgets(buf, sizeof(buf), stdin);
        } else {
            printf("  ╔══════════════════════════════════╗\n");
            printf("  ║  I think it is:                  ║\n");
            printf("  ║  %-32s║\n", food_names[candidate]);
            printf("  ║  Confidence:   %-18.1f%%║\n", top_prob * 100.0f);
            printf("  ╚══════════════════════════════════╝\n\n");

            printf("  Was I right? (y/n): ");
            fflush(stdout);
            fgets(buf, sizeof(buf), stdin);
            if (buf[0]=='y'||buf[0]=='Y')
                printf("  Akinator wins!\n\n");
            else
                printf("  You win! Tell me what it was so we can fix the dataset.\n\n");
        }

        printf("  Play again? (y/n): ");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
    } while (buf[0]=='y'||buf[0]=='Y');

    printf("  Thanks for playing!\n\n");
    return 0;
}