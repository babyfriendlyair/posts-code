#ifndef SamplesPerSensorOptimizer_h
#define SamplesPerSensorOptimizer_h

typedef void (*DigitalSensorRead)();
typedef int (*AnalogSensorRead)();
typedef void (*PrintStdOut)(char* message);

struct DigitalSensorReaders {
  DigitalSensorRead* readers;
  int size;
};

struct AnalogSensorReader {
  int id;
  AnalogSensorRead read;
};

struct AnalogSensorReaders {
  AnalogSensorReader* readers;
  int size;
};

class SamplesPerSensorOptimizer {
  public:
    SamplesPerSensorOptimizer(DigitalSensorReaders digitalSensorReaders,
                              AnalogSensorReaders analogSensorReaders,
                              int maxTimeForSamplingInMs,
                              int nrSamplesForCalibration);
    /**
       Prints in a format that can be pasted to https://www.wolframalpha.com/
       which than gives you the number of samples per sensor to get.
    */
    void printOptProblemForWolframAlpha();

  private:
    DigitalSensorReaders digitalSensorReaders_;
    AnalogSensorReaders analogSensorReaders_;
    int maxTimeForSamplingInMs_;
    int nrSamplesForCalibration_;
    void estimateStdDevs(double stdDevs[]);
    int estimateMaxNrSamples();
};

#endif
