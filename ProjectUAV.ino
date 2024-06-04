#include "config.h"

enum State {
  PAUSE,
  MOORING,
  PARALLELTOF,
  PARALLELGYRO,
  PARALLELFORWARD,
  FORWARDDISTANCE,
  ROTATE,
  ARUCO
};

struct SensorData {
  uint16_t tof1 = 0;
  uint16_t tof2 = 0;
  uint16_t tof3 = 0;
  int8_t tofAngle = 0;
  float gyro = 0;
  float gyroDegrees = 0;
};

struct MotorData {
  int16_t motorL = 0;
  int16_t motorR = 0;
  int16_t motorS = 0;
};

int time = 0;
int lastWriteCycleTime = 0;
int lastFastReadCycleTime = 0;
int lastSlowReadCycleTime = 0;

enum State state = PAUSE;
struct SensorData sensors;
struct MotorData motors;

void setup() {
  Serial.begin(115200);
  
  emergencyInit();
  displayInit();
  gyroInit();
  tofInit(); 
  motorInit();
  // sdInit();
}

void loop() {
  time = millis();

  if(analogRead(SAFETY_AMPMETER) > SAFETY_AMP_CONSTANT){
    emergency();
  }

  if (time - lastWriteCycleTime > CYCLE_TIME_WRITE) {
    motorL(motors.motorL);
    motorR(motors.motorR);
    motorS(motors.motorS);
    lastWriteCycleTime = time;
  }

  if (time - lastFastReadCycleTime > CYCLE_TIME_READ_FAST) {
    sensors.gyro = gyroRead();
    sensors.gyroDegrees = sensors.gyroDegrees + (sensors.gyro / (time - lastFastReadCycleTime));
    sensors.tof1 = tof1Read();
    sensors.tof2 = tof2Read();
    sensors.tof3 = tof3Read();
    sensors.tofAngle = tofAngleRead();
    lastFastReadCycleTime = time;
  }

  if (time - lastSlowReadCycleTime > CYCLE_TIME_READ_SLOW) {
    // TODO: read slow sensors
    Serial.println(sensors.tof1);
    lastSlowReadCycleTime = time;
  }

  switch (state) {
    case PAUSE:
      motorBlowerOff();
      motors.motorL = 0;
      motors.motorR = 0;
      motors.motorS = 0;
      break;
    case MOORING:
      break;
    case PARALLELTOF:
      break;
    case PARALLELGYRO:
      break;
    case PARALLELFORWARD:
      break;
    case FORWARDDISTANCE:
      break;
    case ROTATE:
      break;
    case ARUCO:
      break;
    default:
      Serial.println("Error 0: got to default state, that should not be possible");
      displayNumber(0);
      while (1) {}
  }

}
