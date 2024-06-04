#include <SPI.h>
#include <SD.h>

void sdInit() {
  if (!SD.begin(SD_CS)) {
    Serial.println("Error 4: Failed to boot sd");
    displayNumber(4);
    while (1);
  }
}

void sdWrite(char message[]) {
  File log = SD.open("log.txt", FILE_WRITE);
  
  if (!log) {
    Serial.println("Error 5: Failed to write to sd");
    displayNumber(5);
    return;
  }
  
  log.println(message);
  log.close();
}

