void safetyInit() {
  pinMode(SAFETY_AMPMETER, INPUT);
  pinMode(SAFETY_DISCHARGE, INPUT_PULLUP);
  
  pinMode(RELAY_ELEC, OUTPUT);
  digitalWrite(RELAY_ELEC, HIGH);
  
  delay(300);
  
  attachInterrupt(digitalPinToInterrupt(SAFETY_DISCHARGE), safetyDischarge, FALLING);
}

void safetyDischarge() {
  digitalWrite(RELAY_ELEC, LOW);
  motorBlowerOff();
  displayNumber(1);
  Serial.println("Error 1: Battery cutoff due to voltage");
  noInterrupts();
  while (1);
}

void safetyAmps() {
  digitalWrite(RELAY_ELEC, LOW);
  motorBlowerOff();
  displayNumber(1);
  Serial.println("Error 1: Battery cutoff due to amperage");
  noInterrupts();
  while (1);
}