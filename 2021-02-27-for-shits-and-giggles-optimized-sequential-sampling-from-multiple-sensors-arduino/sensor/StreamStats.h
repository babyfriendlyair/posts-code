#ifndef StreamStats_h
#define StreamStats_h

#include <Arduino.h>

class StreamStats
{
  public:
    StreamStats();
    void reportNumber(int n);
    double average();
    double variance();
    double stdDev();

  private:
    int nrValues_;
    double currentMean_;
    double currentVarianceHelperValue_;
};

#endif
