#include "rolling.h"
#include <stdlib.h>

float Rolling::upd(float newVal) {
  total -= val[count];
  val[count] = newVal;
  total += val[count];
  avg = total / samples;
  count++;
  if (count == samples) count=0;
  return total / samples;
}

void Rolling::init(float init) {
  int i;
  for (i=0; i < samples; i++) {
    val[i] = init;
  }
  total = init * samples;
  avg = init;
  return;
}

Rolling::Rolling(int size) {
  samples = size;
  val = (float*) malloc(samples*sizeof(float));
  count = 0;
}