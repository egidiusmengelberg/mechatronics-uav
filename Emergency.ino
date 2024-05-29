void emergencyInit() {
  pinMode(SAFETY_AMPMETER, INPUT);
  pinMode(SAFETY_DISCHARGE, INPUT_PULLUP);
  
  pinMode(RELAY_ELEC, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(SAFETY_DISCHARGE), emergency, FALLING);

  digitalWrite(RELAY_ELEC, HIGH);
}

void emergency() {
  Serial.println("Error 1: Amperage cutoff");
  displayNumber(1);
  digitalWrite(RELAY_ELEC, LOW);
  motorBlowerOff();
  while (1);
}