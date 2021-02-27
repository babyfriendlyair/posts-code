#include "SamplesPerSensorOptimizer.h"
#include "Arduino.h"
#include "StreamStats.h"

SamplesPerSensorOptimizer::SamplesPerSensorOptimizer(DigitalSensorReaders digitalSensorReaders, 
                                                     AnalogSensorReaders analogSensorReaders, 
                                                     int maxTimeForSamplingInMs,
                                                     int nrSamplesForCalibration) {
  digitalSensorReaders_ = digitalSensorReaders;
  analogSensorReaders_ = analogSensorReaders;
  maxTimeForSamplingInMs_ = maxTimeForSamplingInMs;
  nrSamplesForCalibration_ = nrSamplesForCalibration;
}

void SamplesPerSensorOptimizer::printOptProblemForWolframAlpha() {
  int nrSensors = analogSensorReaders_.size;
  
  double stdDevs[nrSensors];
  estimateStdDevs(stdDevs);
  
  int maxSamples = estimateMaxNrSamples();

  // min (10 / sqrt(a) + 30 / sqrt(b)) s.t. a + b <= 9600
  Serial.print("min (");
  for (auto i = 0; i < nrSensors; i++) {
    if (i > 0) {
      Serial.print(" + ");
    }
    Serial.print(stdDevs[i]);
    Serial.print(" / sqrt(");
    Serial.print((char) (97 + i));
    Serial.print(")");
  }
  
  Serial.print(") s.t. ");
  for (auto i = 0; i < nrSensors; i++) {
    Serial.print((char) (97 + i));
    if (i < nrSensors - 1) {
      Serial.print(" + ");
    }
  }
  
  Serial.print(" <= ");
  Serial.println(maxSamples);
}

void SamplesPerSensorOptimizer::estimateStdDevs(double stdDevs[]) {
    auto nrSensors = analogSensorReaders_.size;
    for (auto i = 0; i < nrSensors; i++) {
      auto analogSensorReader = analogSensorReaders_.readers[i];
      StreamStats streamStats {};
      
      for (auto j = 0; j < nrSamplesForCalibration_; j++) {
        streamStats.reportNumber(analogSensorReader.read());
      }
      
      stdDevs[i] = streamStats.stdDev();
    }
}

int SamplesPerSensorOptimizer::estimateMaxNrSamples() {
  unsigned long tReadDigital = 0;
  for (auto i = 0; i < digitalSensorReaders_.size; i++) {
    unsigned long t0 = micros();
    digitalSensorReaders_.readers[i]();
    tReadDigital += micros() - t0;
  }

  int tMax = maxTimeForSamplingInMs_ - (tReadDigital / 1000);

  auto analogReadFn = analogSensorReaders_.readers[0].read;
  StreamStats analogReadStats {};
  for (auto i = 0; i < 1000; i++) {
    unsigned long t0 = micros();
    analogReadFn();
    analogReadStats.reportNumber(micros() - t0);
  }

  return tMax / (analogReadStats.average() / 1000);
}
