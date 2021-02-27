#include <DHT_U.h>
#include <DHT.h>

#include "SamplesPerSensorOptimizer.h"
#include "StreamStats.h"

DHT dht(2, DHT11);

#define VARIABLE_NAME samplesPerSensorOptimizer
#define MAX_TIME_IN_MS_TO_SAMPLE 1000
#define SAMPLES_FOR_CALIBRATION 1000

// All digital sensors go here
DigitalSensorRead digitalSensorReadersArray[] = {
  []()->void { dht.readHumidity(); },
  []()->void { dht.readTemperature(); }
};

AnalogSensorReader analogSensorReaderArray[] = {
  {A0, []()->int { return analogRead(A0); }},
  {A1, []()->int { return analogRead(A1); }},
  {A2, []()->int { return analogRead(A2); }}
};

// TL;DR Boilerplate below
// @todo Get rid of it :)

DigitalSensorReaders digitalSensorReaders {
  digitalSensorReadersArray,
  sizeof(digitalSensorReadersArray) / sizeof(digitalSensorReadersArray[0])
};

AnalogSensorReaders analogSensorReaders {
  analogSensorReaderArray,
  sizeof(analogSensorReaderArray) / sizeof(analogSensorReaderArray[0])
};

SamplesPerSensorOptimizer VARIABLE_NAME { 
  digitalSensorReaders, 
  analogSensorReaders, 
  MAX_TIME_IN_MS_TO_SAMPLE,
  SAMPLES_FOR_CALIBRATION
};

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    // samplesPerSensorOptimizer.printOptProblemForWolframAlpha();
    
    int samplesToTake[] {3253, 2142, 3211};
    int nrSensors = sizeof(analogSensorReaderArray) 
                  / sizeof(analogSensorReaderArray[0]);
    for (auto i = 0; i < nrSensors; i++) {
      StreamStats stats {};
      
      auto sensor = analogSensorReaderArray[i];
      for (auto j = 0; j < samplesToTake[i]; j++) {
        stats.reportNumber(sensor.read());
        if (j == 29) {
          Serial.print(stats.average());
          Serial.print(",");
        }
      }

      Serial.print(stats.average());
      Serial.print(",");
      Serial.print(sensor.read());
      Serial.print(",");
    }

    Serial.print(dht.readHumidity());
    Serial.print(",");
    Serial.print(dht.readTemperature());
    
    Serial.println();
}
