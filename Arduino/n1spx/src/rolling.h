#ifndef _ROLLING_H_
#define _ROLLING_H_

class Rolling {
  public:
    float avg;
    float upd(float newVal);
    void init(float init);
    Rolling(int samples);

  private:
    float total;
    float* val;
    int count;
    int samples;
};

#endif