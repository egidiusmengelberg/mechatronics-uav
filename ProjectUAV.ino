#include "config.h"

enum State {
  BOOTING,
  RUNNING
};

struct SensorData {
  uint16_t tof1 = 0;
  uint16_t tof2 = 0;
  uint16_t tof3 = 0;
  int8_t tofAngle = 0;
  float gyro = 0;
};

struct MotorData {
  uint8_t motorL = 0;
  uint8_t motorR = 0;
  uint8_t motorS = 0;
};

int time = 0;
int lastWriteCycleTime = 0;
int lastFastReadCycleTime = 0;
int lastSlowReadCycleTime = 0;

enum State state = BOOTING;
struct SensorData sensors;
struct MotorData motors;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(AMPMETER, INPUT);
  pinMode(DISCHARGE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DISCHARGE), emergency, FALLING);

  pinMode(RELAY_BLOWERS, OUTPUT);
  pinMode(RELAY_ELEC, OUTPUT);
  digitalWrite(RELAY_ELEC, HIGH);
  
  displayInit();
  gyroInit();
  // tofInit(); 

  motorInit();
}

void loop() {
  time = millis();

  if(analogRead(AMPMETER) > 632){
    emergency();
  }

  if (time - lastWriteCycleTime > CYCLE_TIME_WRITE) {
    lastWriteCycleTime = time;
    // TODO: write outputs with motors structs
  }

  if (time - lastFastReadCycleTime > CYCLE_TIME_READ_FAST) {
    lastFastReadCycleTime = time;
    sensors.gyro = gyroRead();
    sensors.tof1 = tof1Read();
    sensors.tof2 = tof2Read();
    sensors.tof3 = tof3Read();
    sensors.tofAngle = tofAngleRead();
  }

  if (time - lastSlowReadCycleTime > CYCLE_TIME_READ_SLOW) {
    lastSlowReadCycleTime = time;
    // TODO: read slow sensors
  }

  switch (state) {
    case BOOTING:
      motorSide(255);
      state = RUNNING;
      break;
    case RUNNING:
      break;
    default:
      Serial.println("Error 0: got to default state, that should not be possible");
      displayNumber(0);
      while (1) {}
  }

}

void emergency() {
  Serial.println("Error 1: Amperage cutoff");
  displayNumber(1);
  digitalWrite(RELAY_ELEC, LOW);
  digitalWrite(RELAY_BLOWERS, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  while (1);
}
