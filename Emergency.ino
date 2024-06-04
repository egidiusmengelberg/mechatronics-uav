void emergencyInit() {
  pinMode(SAFETY_AMPMETER, INPUT);
  pinMode(SAFETY_DISCHARGE, INPUT_PULLUP);
  
  pinMode(RELAY_ELEC, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(SAFETY_DISCHARGE), emergency, FALLING);

  digitalWrite(RELAY_ELEC, HIGH);
  delay(300);
}

void emergency() {
  digitalWrite(RELAY_ELEC, LOW);
  motorBlowerOff();
  Serial.println("Error 1: Amperage cutoff");
  displayNumber(1);
  noInterrupts();
  while (1);
}