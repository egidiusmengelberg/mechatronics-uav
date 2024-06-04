#include "config.h"

const uint8_t segment_array[8] = {
    DISPLAY_SEGMENT_A, 
    DISPLAY_SEGMENT_B, 
    DISPLAY_SEGMENT_C, 
    DISPLAY_SEGMENT_D, 
    DISPLAY_SEGMENT_E, 
    DISPLAY_SEGMENT_F, 
    DISPLAY_SEGMENT_G, 
    DISPLAY_SEGMENT_DOT
};

void displayInit() {
    pinMode(DISPLAY_SUPPLY_1, OUTPUT);      digitalWrite(DISPLAY_SUPPLY_1, HIGH);
    pinMode(DISPLAY_SUPPLY_2, OUTPUT);      digitalWrite(DISPLAY_SUPPLY_2, HIGH);
    pinMode(DISPLAY_SEGMENT_A, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_A, HIGH);
    pinMode(DISPLAY_SEGMENT_B, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_B, HIGH);
    pinMode(DISPLAY_SEGMENT_C, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_C, HIGH);
    pinMode(DISPLAY_SEGMENT_D, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_D, HIGH);
    pinMode(DISPLAY_SEGMENT_E, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_E, HIGH);
    pinMode(DISPLAY_SEGMENT_F, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_F, HIGH);
    pinMode(DISPLAY_SEGMENT_G, OUTPUT);     digitalWrite(DISPLAY_SEGMENT_G, HIGH);
    pinMode(DISPLAY_SEGMENT_DOT, OUTPUT);   digitalWrite(DISPLAY_SEGMENT_DOT, HIGH);
}

void displayDot(bool state){
  displaySetAllSegments(0);
  digitalWrite(segment_array[7], !state);
}

void displayNumber(uint8_t number) {
    displaySetAllSegments(0);
    switch(number){
        case 0:
        displaySetSegments(1,1,1,1,1,1,0);
        break;
        case 1:
        displaySetSegments(0,1,1,0,0,0,0);
        break;
        case 2:
        displaySetSegments(1,1,0,1,1,0,1);
        break;
        case 3:
        displaySetSegments(1,1,1,1,0,0,1);
        break;
        case 4:
        displaySetSegments(0,1,1,0,0,1,1);
        break;
        case 5:
        displaySetSegments(1,0,1,1,0,1,1);
        break;
        case 6:
        displaySetSegments(1,0,1,1,1,1,1);
        break;
        case 7:
        displaySetSegments(1,1,1,0,0,0,0);
        break;
        case 8:
        displaySetSegments(1,1,1,1,1,1,1);
        break;
        case 9:
        displaySetSegments(1,1,1,1,0,1,1);
        break;
    }
}

void displaySetAllSegments(bool state){
    for(int j = 0; j<8; j++){
        digitalWrite(segment_array[j], !state);
    }
}

void displaySetSegments(bool s1, bool s2, bool s3, bool s4, bool s5, bool s6, bool s7) {
  digitalWrite(segment_array[0], !s1);
  digitalWrite(segment_array[1], !s2);
  digitalWrite(segment_array[2], !s3);
  digitalWrite(segment_array[3], !s4);
  digitalWrite(segment_array[4], !s5);
  digitalWrite(segment_array[5], !s6);
  digitalWrite(segment_array[6], !s7);
}