#include "config.h"
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X tof1 = Adafruit_VL53L0X();
Adafruit_VL53L0X tof2 = Adafruit_VL53L0X();
Adafruit_VL53L0X tof3 = Adafruit_VL53L0X();

void tofInit() {
  pinMode(TOF_2_XSHUT, OUTPUT);
  pinMode(TOF_3_XSHUT, OUTPUT);
  digitalWrite(TOF_2_XSHUT, LOW);    
  digitalWrite(TOF_3_XSHUT, LOW);
  delay(10);

  
  if(!tof1.begin(TOF_1_ADDR)) {
    Serial.println("Error 2: Failed to boot first ToF");
    displayNumber(2);
    while(1);
  } 

  delay(10);
  digitalWrite(TOF_2_XSHUT, HIGH);
  delay(10);

  if(!tof2.begin(TOF_2_ADDR)) {
    Serial.println("Error 2: Failed to boot second ToF");
    displayNumber(2);
    while(1);
  }

  delay(10);
  digitalWrite(TOF_3_XSHUT, HIGH);
  delay(10);

  if(!tof3.begin(TOF_3_ADDR)) {
    Serial.println("Error 2: Failed to boot third ToF");
    displayNumber(2);
    while(1);
  }
}

uint16_t tof1Read() {
  VL53L0X_RangingMeasurementData_t measurement;
  tof1.rangingTest(&measurement, false);
  if(measurement.RangeStatus != 4) {     // if not out of range
    return measurement.RangeMilliMeter;
  }
  return 0;
}

uint16_t tof2Read() {
  VL53L0X_RangingMeasurementData_t measurement;
  tof2.rangingTest(&measurement, false);
  if(measurement.RangeStatus != 4) {     // if not out of range
    return measurement.RangeMilliMeter;
  }
  return 0;
}

uint16_t tof3Read() {
  VL53L0X_RangingMeasurementData_t measurement;
  tof3.rangingTest(&measurement, false);
  if(measurement.RangeStatus != 4) {     // if not out of range
    return measurement.RangeMilliMeter;
  }
  return 0;
}

int8_t tofAngleRead() {
  uint16_t d1 = tof1Read();
  uint16_t d2 = tof2Read();
  // TODO: replace with cordic algorithm
  if (d1 > d2) {
    return -round( atan2 (d1 - d2, TOF_DISTANCE_BETWEEN_SENSORS) * 180/3.14159265 );
  }
  return round( atan2 (d2 - d1, TOF_DISTANCE_BETWEEN_SENSORS) * 180/3.14159265 );
}