void motorInit() {
  pinMode(RELAY_BLOWERS, OUTPUT);

  pinMode(MOTOR_SIDE_1, OUTPUT);
  pinMode(MOTOR_SIDE_2, OUTPUT);
  pinMode(MOTOR_SIDE_PWM, OUTPUT);
  
  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);

  digitalWrite(RELAY_BLOWERS, LOW);

  digitalWrite(MOTOR_SIDE_1, LOW);
  digitalWrite(MOTOR_SIDE_2, LOW);
  analogWrite(MOTOR_SIDE_PWM, 0);

  analogWrite(MOTOR_L_1, 0);
  analogWrite(MOTOR_L_2, 0);
  analogWrite(MOTOR_R_1, 0);
  analogWrite(MOTOR_R_2, 0);
}

void motorL(int16_t speed) {
  if (speed > 0) {
    analogWrite(MOTOR_L_1, 0);
    analogWrite(MOTOR_L_2, abs(speed));
  } else if (speed < 0) {
    analogWrite(MOTOR_L_1, abs(speed));
    analogWrite(MOTOR_L_2, 0);
  } else {
    analogWrite(MOTOR_L_1, 0);
    analogWrite(MOTOR_L_2, 0);
  }
}

void motorR(int16_t speed) {
  if (speed > 0) {
    analogWrite(MOTOR_R_1, 0);
    analogWrite(MOTOR_R_2, abs(speed));
  } else if (speed < 0) {
    analogWrite(MOTOR_R_1, abs(speed));
    analogWrite(MOTOR_R_2, 0);
  } else {
    analogWrite(MOTOR_R_1, 0);
    analogWrite(MOTOR_R_2, 0);
  }
}

void motorS(int16_t speed) {
  if (speed > 0) {
    digitalWrite(MOTOR_SIDE_1, HIGH);
    digitalWrite(MOTOR_SIDE_2, LOW);
  } else if (speed < 0) {
    digitalWrite(MOTOR_SIDE_1, LOW);
    digitalWrite(MOTOR_SIDE_2, HIGH);
  } else {
    digitalWrite(MOTOR_SIDE_1, LOW);
    digitalWrite(MOTOR_SIDE_2, LOW);
  }
  analogWrite(MOTOR_SIDE_PWM, abs(speed));
}

void motorBlowerOn() {
  digitalWrite(RELAY_BLOWERS, HIGH);
}

void motorBlowerOff() {
  digitalWrite(RELAY_BLOWERS, LOW);
}