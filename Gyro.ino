#include <Wire.h>
#include <MPU9250_WE.h>

MPU9250_WE gyro = MPU9250_WE(0x68);

void gyroInit() {
  Wire.begin();
  
  if(!gyro.init()){
    Serial.println("Error 3: Failed to boot gyro");
    displayNumber(3);
    while(1);
  }

  gyro.autoOffsets();
  gyro.enableGyrDLPF();
  gyro.setGyrDLPF(MPU9250_DLPF_6);
  gyro.setSampleRateDivider(5);
  gyro.setGyrRange(MPU9250_GYRO_RANGE_250);
  gyro.setAccRange(MPU9250_ACC_RANGE_2G);
  gyro.enableAccDLPF(true);
  gyro.setAccDLPF(MPU9250_DLPF_6);
  gyro.setMagOpMode(AK8963_CONT_MODE_100HZ);
}



float gyroRead() {
  xyzFloat gyr = gyro.getGyrValues();
  return gyr.z;
}