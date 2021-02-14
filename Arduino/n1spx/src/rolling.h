#pragma once

// This set the last X number of samples to include
// in the rolling average
// Default to 10 unless this was defined already by
// the calling program
#ifndef ROLL_COUNT
  #define ROLL_COUNT 10
#endif

typedef struct roll {
  float total;
  float val[ROLL_COUNT];
  int count;
  float avg;
} rolling;

float updateRoll(rolling* roll, float newVal);

void initRoll(rolling* roll, float init);