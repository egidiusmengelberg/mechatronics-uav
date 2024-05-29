#ifndef CONFIG_H
#define CONFIG_H

// CYCLE TIMES
#define CYCLE_TIME_WRITE 50       // ms
#define CYCLE_TIME_READ_FAST 50   // ms
#define CYCLE_TIME_READ_SLOW 100  // ms

// SAFETY
#define AMPMETER A0
#define DISCHARGE 2

// RELAYS
#define RELAY_BLOWERS 3
#define RELAY_ELEC 4

// MOTORS
#define MOTOR_R_1 8
#define MOTOR_R_2 9
#define MOTOR_L_1 10
#define MOTOR_L_2 11

#define MOTOR_SIDE_1 5
#define MOTOR_SIDE_2 6
#define MOTOR_SIDE_PWM 7

// TOF
#define TOF_2_XSHUT 12
#define TOF_3_XSHUT 13
#define TOF_1_ADDR 0x50
#define TOF_2_ADDR 0x51
#define TOF_3_ADDR 0x52
#define TOF_DISTANCE_BETWEEN_SENSORS 141 // mm

// SDCARD
#define SD_DI 50;
#define SD_DO 51;
#define SD_SCK 52;
#define SD_CS 53;

// DISPLAY
#define DISPLAY_SUPPLY_1 34
#define DISPLAY_SUPPLY_2 35
#define DISPLAY_SEGMENT_A   37
#define DISPLAY_SEGMENT_B   39
#define DISPLAY_SEGMENT_C   36
#define DISPLAY_SEGMENT_D   32
#define DISPLAY_SEGMENT_E   30
#define DISPLAY_SEGMENT_F   33
#define DISPLAY_SEGMENT_G   31
#define DISPLAY_SEGMENT_DOT 38

#endif