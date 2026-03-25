#ifndef DATASET_H
#define DATASET_H

#define NUM_FOODS     127
#define NUM_QUESTIONS 130
#define NUM_POOLS     8
#define P1_END        8

extern const char *food_names[127];
extern const char *questions[130];
extern int pool_defs[8][4];
extern float properties[127][130];

#endif