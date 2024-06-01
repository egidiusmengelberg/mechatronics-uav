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

  //init balancing
  float kp = 0.5;
  float ki = 0.2;
  float kd = 0.136;
  float setpoint = 0;
  float integral = 0;
  float previous_error = 0;
  float offsett = 0;

  const float Iz_hovercraft = 0.008;
  const float max_force = 0.12;
  const float distance_motors = 0.188;
  const float max_torque = 2 * max_force * distance_motors;
  const float min_torque = -max_torque;

  float times = 0;
  float angles = 0;
  float angle = 0;
  float gyroZ = 0;  // This will be directly measured
  float dt = 0.1;
  float angle_old = 0;
  float error_old = 0;
  float error_sum = 0;
  float motorOutput = 0;
  float Motor_R = 0;
  float Motor_L = 0;
  int time2 = millis();


void setup() {
  Serial.begin(115200);
  
  emergencyInit();
  displayInit();
  gyroInit();
  tofInit(); 
  motorInit();
  //sdInit();




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
    lastSlowReadCycleTime = time;
  }


  

  //balancing code
  int gyroz = sensors.gyro;

  //Serial.print(gyroz);
    // Calculate PID
  float angle = gyroz * (0.1);  //HELP CYCLE TIJD LUKT ME NIET MET JOU GEDOE
  //Serial.print(angle);
  float error = setpoint - angle;

  

  float proportional = kp * error;
  float derivative = kd * (error - previous_error) / dt;
  float motorOutput = offsett + proportional + derivative;
  
  if (motorOutput >= min_torque && motorOutput <= max_torque) {
    error_sum += error * dt;
    integral = ki * error_sum;
    motorOutput += integral;
  }
  
    // Limit the motor output
  //if (motorOutput > max_torque) {
  //  motorOutput = max_torque;
  //} else if (motorOutput < min_torque) {
  //  motorOutput = min_torque;
  //}
  //Serial.print(motorOutput);
  Serial.println();
    // Calculate the motor outputs for left and right motors
  motorOutput =  motorOutput / 2;

  float Motor_L = -motorOutput; //* Iz_hovercraft / distance_motors;
  float Motor_R = motorOutput; //* Iz_hovercraft / distance_motors;
  
 Serial.print(Motor_L);

  switch (state) {
    case PAUSE:
      motorBlowerOff();
      motors.motorL = -255;
      motors.motorR = -255;
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
