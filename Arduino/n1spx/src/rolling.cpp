#include "rolling.h"

float updateRoll(rolling* roll, float newVal) {
  roll->total -= roll->val[roll->count];
  roll->val[roll->count] = newVal;
  roll->total += roll->val[roll->count];
  roll->avg = roll->total / ROLL_COUNT;
  roll->count++;
  if (roll->count == ROLL_COUNT) roll->count=0;
  
  return roll->total / ROLL_COUNT;
}

void initRoll(rolling* roll, float init) {
  int i;
  for (i=0; i < ROLL_COUNT; i++) {
    roll->val[i] = init;
  }
  roll->total = init * ROLL_COUNT;
  roll->avg = init;
  return;
}