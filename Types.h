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
  float tofAngle = 0;
  float gyro = 0;
  float gyroDegrees = 0;
};

struct OutputData {
  int e = 0;
  int16_t motorL = 0;
  int16_t motorR = 0;
  int16_t motorS = 0;
};

struct LookupTable {
  float force;
  int16_t pwm;
};