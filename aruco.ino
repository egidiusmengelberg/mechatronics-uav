#include <HardwareSerial.h>

HardwareSerial mySerial(2);

const long cyclustijd = 10;                       // [ms] 
long t_oud, t_nw;                                 // [ms]
float dt = 1;                                     // [s] 

const float m = 1.122;                            // [kg] Mass of UAV
const float Iz = 0.008;                           // [kgm^2] Moment of inertia
const float d = 0.188;                            // [m] Distance between motors

const int heli_moment = 0.0;                      // [Nm] Moment of blower heli effect

const float motor_l_min = -0.177734118, motor_l_max = 0.131569412;    // [N] Min and Max thrust of left motor
const float motor_r_min = -0.173117647, motor_r_max = 0.133877647;    // [N] Min and Max thrust of right motor
const float motor_s_min = -0.1397925, motor_s_max = 0.103005;         // [N] Min and Max thrust of side motor

const float re_x = 1, im_x = 2, pole_x = 4;         // Pole placing X Translation
const float re_y = 1, im_y = 2, pole_y = 4;         // Pole placing Y Translation
const float re_rot = 1, im_rot = 2, pole_rot = 4;   // Pole placing Rotation

const float Kp_x = (re_x * re_x + im_x * im_x + 2 * re_x * pole_x) * m;
const float Kd_x = (2 * re_x + pole_x) * m;
const float Ki_x = (re_x * re_x + im_x * im_x) * pole_x * m;

const float Kp_y = (re_y * re_y + im_y * im_y + 2 * re_y * pole_y) * m;
const float Kd_y = (2 * re_y + pole_y) * m;
const float Ki_y = (re_y * re_y + im_y * im_y) * pole_y * m;

const float Kp_rot = (re_rot * re_rot + im_rot * im_rot + 2 * re_rot * pole_rot) * Iz;
const float Kd_rot = (2 * re_rot + pole_rot) * Iz;
const float Ki_rot = (re_rot * re_rot + im_rot * im_rot) * pole_rot * Iz;

float error_x = 0, error_x_previous = 0, error_x_delta = 0, error_x_sum = 0, resulting_force_x = 0;
float error_y = 0, error_y_previous = 0, error_y_delta = 0, error_y_sum = 0, resulting_force_y = 0;
float error_rot = 0, error_rot_previous = 0, error_rot_delta = 0, error_rot_sum = 0, resulting_moment = 0;

struct arucoOutput {
  float motor_l = 0.0;
  float motor_r = 0.0;
  float motor_s = 0.0;
};

struct PositionData {
  float X;
  float Y;
  float Z;
  float A;
  float B;
  float C;
};

PositionData prevData = {0, 0, 0, 0, 0, 0};
bool isConnected = false;
bool setMarker = false;

bool initSerialConnection() {
  mySerial.println("knock knock!");

  unsigned long startMillis = millis();
  while (millis() - startMillis < 1000) {
    if (mySerial.available()) {
      String response = mySerial.readStringUntil('\n');
      if (response == "whos there?") {
        return true;
      }
    }
  }
  return false;
}

bool setArucoType(String type) {
  mySerial.println("aruco_" + type);

  unsigned long startMillis = millis();
  while (millis() - startMillis < 1000) {
    if (mySerial.available()) {
      String response = mySerial.readStringUntil('\n');
      if (response == "invalid") {
        return false;
      } else if (response == "accepted") {
        return true;
      }
    }
  }
  return false;
}

PositionData splitString(String data) {
  PositionData posData;
  int index = 0;
  int prevIndex = 0;

  index = data.indexOf('_', prevIndex);
  posData.X = data.substring(prevIndex, index).toFloat();
  prevIndex = index + 1;

  index = data.indexOf('_', prevIndex);
  posData.Y = data.substring(prevIndex, index).toFloat();
  prevIndex = index + 1;

  index = data.indexOf('_', prevIndex);
  posData.Z = data.substring(prevIndex, index).toFloat();
  prevIndex = index + 1;

  index = data.indexOf('_', prevIndex);
  posData.A = data.substring(prevIndex, index).toFloat();
  prevIndex = index + 1;

  index = data.indexOf('_', prevIndex);
  posData.B = data.substring(prevIndex, index).toFloat();
  prevIndex = index + 1;

  posData.C = data.substring(prevIndex).toFloat();
  return posData;
}

PositionData askPosition() {
  while (mySerial.available()) {
    mySerial.read();
  }

  mySerial.println("where am I?");

  unsigned long startMillis = millis();
  bool responseReceived = false;
  PositionData posData;

  while (millis() - startMillis < 1000) {
    if (mySerial.available()) {
      String positionData = mySerial.readStringUntil('\n');
      responseReceived = true;

      posData = splitString(positionData);
      prevData = posData;
      break;
    }
  }

  if (!responseReceived) {
    Serial.println("Nobody home");
    posData = prevData;
  }
  return posData;
}