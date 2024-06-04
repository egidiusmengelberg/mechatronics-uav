#include "config.h"

struct PositionData {
  float X;
  float Y;
  float Z;
  float A;
  float B;
  float C;
};

void initRaspberry() {
  Serial2.begin(115200);

  Serial2.println("knock knock!");

  unsigned long startMillis = millis();
  while (millis() - startMillis < RASPBERRY_TIMEOUT) {
    if (Serial2.available()) {
      String response = Serial2.readStringUntil('\n');
      if (response == "whos there?") return;
    }
  }
  Serial.println("Error 6: Failed to init raspberry");
  displayNumber(6);
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

PositionData raspberryPosition() {
  while (raspberrySerial.available()) {
    raspberrySerial.read();
  }

  raspberrySerial.println("where am I?");

  unsigned long startMillis = millis();
  bool responseReceived = false;
  PositionData posData;

  while (millis() - startMillis < timeout) {
    if (raspberrySerial.available()) {
      String positionData = raspberrySerial.readStringUntil('\n');
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