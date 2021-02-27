#include "StreamStats.h"
#include <math.h>

StreamStats::StreamStats() {
  nrValues_ = 0;
  currentMean_ = -1;
  currentVarianceHelperValue_ = 0;
}

void StreamStats::reportNumber(int n) {
  if (nrValues_ == 0) {
    currentMean_ = n;
  } else {
    double prevMean = currentMean_;
    currentMean_ += ((n - prevMean) / (nrValues_ + 1));
    currentVarianceHelperValue_ += (n - prevMean) * (n - currentMean_);
  }
  nrValues_++;
}

double StreamStats::average() {
  return currentMean_;
}

double StreamStats::variance() {
  if (nrValues_ > 1) {
    return currentVarianceHelperValue_ / (nrValues_ - 1);
  } else {
    return 0;
  }
}

double StreamStats::stdDev() {
  return sqrt(variance());
}
