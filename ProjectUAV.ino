#include "Types.h"
#include "config.h"

void setup() {
  Serial.begin(115200);
  
  safetyInit();
  displayInit();
  gyroInit();
  tofInit(); 
  motorInit();
}

void loop() {
  static unsigned int dt = 0;
  static unsigned int time = millis();
  
  static int lastError = 0;
  static unsigned int lastWriteCycleTime = 0;
  static unsigned int lastFastReadCycleTime = 0;
  static unsigned int lastSlowReadCycleTime = 0;

  static enum State state = PAUSE;
  static struct SensorData sensors;
  static struct OutputData output;
  
  if(analogRead(SAFETY_AMPMETER) > SAFETY_AMP_CONSTANT){
    safetyAmps();
  }

  if (time - lastWriteCycleTime > CYCLE_TIME_WRITE) {
    motorL(output.motorL);
    motorR(output.motorR);
    motorS(output.motorS);
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
    lastSlowReadCycleTime = time;
  }

  switch (state) {
    case PAUSE:
      motorBlowerOff();
      output.motorL = 0;
      output.motorR = 0;
      output.motorS = 0;
      break;
    case MOORING:
      motorBlowerOn();
      output = mooring(dt, sensors);
      break;
    case PARALLELTOF:
      motorBlowerOn();
      output = parallelTof(dt, sensors);
      break;
    case PARALLELGYRO:
      motorBlowerOn();
      output = parallelGyro(dt, sensors);
      break;
    case PARALLELFORWARD:
      motorBlowerOn();
      output = parallelForward(dt, sensors);
      break;
    case FORWARDDISTANCE:
      motorBlowerOn();
      output = forwardDistance(dt, sensors);
      break;
    case ROTATE:
      motorBlowerOn();
      output = rotate(dt, sensors);
      break;
    case ARUCO:
      motorBlowerOn();
      output = aruco(dt, sensors);
      break;
    default:
      Serial.println("Error 0: got to default state, that should not be possible");
      displayNumber(0);
      while (1);
  }

  lastError = output.e;
  dt = millis() - time;
}
