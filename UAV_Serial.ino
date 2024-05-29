#include <HardwareSerial.h>

HardwareSerial mySerial(2);

const u_int8_t timeout = 1000;

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

bool initSerialConnection() {
  mySerial.println("knock knock!");

  unsigned long startMillis = millis();
  while (millis() - startMillis < timeout) {
    if (mySerial.available()) {
      String response = mySerial.readStringUntil('\n');
      if (response == "whos there?") {
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

  while (millis() - startMillis < timeout) {
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

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 16, 17);

  isConnected = initSerialConnection();
  if (isConnected) {
    Serial.println("Great success!!");
  } else {
    Serial.println("Boohoo :(");
  }
}

void loop() {
  isConnected = initSerialConnection();

  if (isConnected) {
    PositionData posData = askPosition();

    Serial.print("X: "); Serial.print(posData.X);
    Serial.print(" Y: "); Serial.print(posData.Y);
    Serial.print(" Z: "); Serial.print(posData.Z);
    Serial.print(" A: "); Serial.print(posData.A);
    Serial.print(" B: "); Serial.print(posData.B);
    Serial.print(" C: "); Serial.println(posData.C);
  } else {
    Serial.println("Lost ya");
  }

  delay(10);
}