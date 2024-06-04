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

  analogWrite(MOTOR_L_1, 255);
  analogWrite(MOTOR_L_2, 255);
  analogWrite(MOTOR_R_1, 0);
  analogWrite(MOTOR_R_2, 0);
}

void motorL(float force) {
  int16_t pwm = motorLInterpolate(force);
  if (pwm > 0) {
    analogWrite(MOTOR_L_1, 0);
    analogWrite(MOTOR_L_2, 255-pwm);
  } else if (pwm < 0) {
    analogWrite(MOTOR_L_1, 255+pwm);
    analogWrite(MOTOR_L_2, 0);
  } else {
    analogWrite(MOTOR_L_1, 255);
    analogWrite(MOTOR_L_2, 255);
  }
}

void motorR(float force) {
  int16_t pwm = motorLInterpolate(force);
  if (pwm < 0) {
    analogWrite(MOTOR_R_1, 0);
    analogWrite(MOTOR_R_2, abs(pwm));

  } else if (pwm > 0) {
    analogWrite(MOTOR_R_1, abs(pwm));
    analogWrite(MOTOR_R_2, 0);
    
  } else {
    analogWrite(MOTOR_R_1, 0);
    analogWrite(MOTOR_R_2, 0);
  }
}

void motorS(float force) {
  int16_t pwm = motorLInterpolate(force);
  if (pwm > 0) {
    digitalWrite(MOTOR_SIDE_1, HIGH);
    digitalWrite(MOTOR_SIDE_2, LOW);
  } else if (pwm < 0) {
    digitalWrite(MOTOR_SIDE_1, LOW);
    digitalWrite(MOTOR_SIDE_2, HIGH);
  } else {
    digitalWrite(MOTOR_SIDE_1, LOW);
    digitalWrite(MOTOR_SIDE_2, LOW);
  }
  analogWrite(MOTOR_SIDE_PWM, abs(pwm));
}

void motorBlowerOn() {
  digitalWrite(RELAY_BLOWERS, HIGH);
}

void motorBlowerOff() {
  digitalWrite(RELAY_BLOWERS, LOW);
}

int16_t motorLInterpolate(float force) {
  const LookupTable motorLCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, 0},
    {0.018465882, 20},
    {0.036931765, 40},
    {0.050781176, 60},
    {0.064630588, 80},
    {0.073863529, 100},
    {0.087712941, 120},
    {0.092329412, 140},
    {0.101562353, 160},
    {0.125644706, 180},
    {0.108487059, 200},
    {0.113103529, 220},
    {0.120028235, 240},
    {0.131569412, 255}
  };

  const LookupTable motorLCCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, -0},
    {0.02308235, -20},
    {0.050781176, -40},
    {0.076171765, -60},
    {0.092329412, -80},
    {0.108487059, -100},
    {0.120028235, -120},
    {0.136185882, -140},
    {0.143110588, -160},
    {0.152343529, -180},
    {0.161576471, -200},
    {0.163884706, -220},
    {0.170809412, -240},
    {0.177734118, -255}
  };
  
  return motorInterpolate(force, motorLCW, motorLCCW);
}

int16_t motorRInterpolate(float force) {
  const LookupTable motorRCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, 0},
    {0.020774118, 20},
    {0.03924, 40},
    {0.055397647, 60},
    {0.069247059, 80},
    {0.080788235, 100},
    {0.090021176, 120},
    {0.103870588, 140},
    {0.113103529, 160},
    {0.120028235, 180},
    {0.124644706, 200},
    {0.126952941, 220},
    {0.131569412, 240},
    {0.133877647, 255}
  };

  const LookupTable motorRCCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, -0},
    {0.025390588, -20},
    {0.048472941, -40},
    {0.073863529, -60},
    {0.092329412, -80},
    {0.108487059, -100},
    {0.11772, -120},
    {0.133877647, -140},
    {0.145418824, -160},
    {0.147727059, -180},
    {0.150035294, -200},
    {0.15696, -220},
    {0.166192941, -240},
    {0.173117647, -255}
  };
  
  return motorInterpolate(force, motorRCW, motorRCCW);
}

int16_t motorSInterpolate(float force) {
  const LookupTable motorSCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, 0},
    {0.0073575, 20},
    {0.0220725, 40},
    {0.02943, 60},
    {0.0367875, 80},
    {0.044145, 100},
    {0.044145, 120},
    {0.05886, 140},
    {0.0662175, 160},
    {0.073575, 180},
    {0.0809325, 200},
    {0.08829, 220},
    {0.0956475, 240},
    {0.103005, 255}
  };

  const LookupTable motorSCCW[MOTOR_LOOKUP_SIZE] PROGMEM = {
    {0.0, -0},
    {0.0073575, -20},
    {0.014715, -40},
    {0.02943, -60},
    {0.044145, -80},
    {0.0515025, -100},
    {0.0662175, -120},
    {0.073575, -140},
    {0.0809325, -160},
    {0.0956475, -180},
    {0.1103625, -200},
    {0.1250775, -220},
    {0.132435, -240},
    {0.1397925, -255}
  };
  
  return motorInterpolate(force, motorSCW, motorSCCW);
}

int16_t motorInterpolate(float force, LookupTable *lotCW, LookupTable *lotCCW) {
  if (force > 0) {
    if (force <= lotCW[0].force) {return lotCW[0].pwm;}
    else if (force >= lotCW[0].force) {return lotCW[MOTOR_LOOKUP_SIZE - 1].pwm;}
    else {
      for (char i = 0; i < MOTOR_LOOKUP_SIZE - 1; i++) {
        if (force >= lotCW[i].force && force <= lotCW[i + 1].force) {
          float fraction = (force - lotCW[i].force) / (lotCW[i + 1].force - lotCW[i].force);
          return lotCW[i].pwm + fraction * (lotCW[i + 1].pwm - lotCW[i].pwm);
        }
      }
    }
  } else if (force < 0) {
    if (force <= lotCCW[0].force) {return lotCCW[0].pwm;}
    else if (force >= lotCCW[0].force) {return lotCCW[MOTOR_LOOKUP_SIZE - 1].pwm;}
    else {
      for (char i = 0; i < MOTOR_LOOKUP_SIZE - 1; i++) {
        if (force >= lotCCW[i].force && force <= lotCCW[i + 1].force) {
          float fraction = (force - lotCCW[i].force) / (lotCCW[i + 1].force - lotCCW[i].force);
          return lotCCW[i].pwm + fraction * (lotCCW[i + 1].pwm - lotCCW[i].pwm);
        }
      }
    }
  } else {
    return 0;
  }
}

