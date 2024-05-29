void motorInit() {
    pinMode(MOTOR_SIDE_1, OUTPUT);
    pinMode(MOTOR_SIDE_2, OUTPUT);
    pinMode(MOTOR_SIDE_PWM, OUTPUT);
    
    pinMode(MOTOR_L_1, OUTPUT);
    pinMode(MOTOR_L_2, OUTPUT);
    pinMode(MOTOR_R_1, OUTPUT);
    pinMode(MOTOR_R_2, OUTPUT);

    digitalWrite(MOTOR_SIDE_1, LOW);
    digitalWrite(MOTOR_SIDE_2, LOW);
    digitalWrite(MOTOR_SIDE_PWM, LOW);

    digitalWrite(MOTOR_L_1, LOW);
    digitalWrite(MOTOR_L_2, LOW);
    digitalWrite(MOTOR_R_1, LOW);
    digitalWrite(MOTOR_R_2, LOW);
};

void motorLeft(int speed) {
  digitalWrite(MOTOR_L_1, LOW);
  digitalWrite(MOTOR_L_2, LOW);

  if (speed > 0) {
    digitalWrite(MOTOR_L_1, LOW);
    analogWrite(MOTOR_L_2, abs(speed));
  } else if (speed < 0) {
    digitalWrite(MOTOR_L_2, LOW);
    analogWrite(MOTOR_L_1, abs(speed));
  }
}

void motorRight(int speed) {
    digitalWrite(MOTOR_R_1, LOW);
    digitalWrite(MOTOR_R_2, LOW);

  if (speed > 0) {
    digitalWrite(MOTOR_R_1, LOW);
    analogWrite(MOTOR_R_2, abs(speed));
  } else if (speed < 0) {
    digitalWrite(MOTOR_R_2, LOW);
    analogWrite(MOTOR_R_1, abs(speed));
  }
}

void motorSide(int speed) {
  digitalWrite(MOTOR_SIDE_1, LOW);
  digitalWrite(MOTOR_SIDE_2, LOW);
  if (speed > 0) digitalWrite(MOTOR_SIDE_1, HIGH);
  if (speed < 0) digitalWrite(MOTOR_SIDE_2, HIGH);
  analogWrite(MOTOR_SIDE_PWM, abs(speed));
}

// void motorMaxon(int speedL, int speedR) {
//   if (speedL > 0) {
//     analogWrite(MOTOR_L_2, 0);
//     analogWrite(MOTOR_L_1, abs(speedL));
//   } else if (speedL < 0) {
//     analogWrite(MOTOR_L_1, 0);
//     analogWrite(MOTOR_L_2, abs(speedL));
//   } else {
//     digitalWrite(MOTOR_L_1, LOW);
//     digitalWrite(MOTOR_L_2, LOW);
//   }

//   if (speedR > 0) {
//     analogWrite(MOTOR_R_2, 0);
//     analogWrite(MOTOR_R_1, abs(speedR));
//   } else if (speedR < 0) {
//     analogWrite(MOTOR_R_1, 0);
//     analogWrite(MOTOR_R_2, abs(speedR));
//   } else {
//     digitalWrite(MOTOR_R_1, LOW);
//     digitalWrite(MOTOR_R_2, LOW);
//   }
// }