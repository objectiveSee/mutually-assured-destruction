#include <avr/pgmspace.h>

/**
 *  Patterns are defined in the following way:
 *  
 *  Patterns alternate specifying the OFF and ON durations in deciseconds (10ms) 
 *  For example: {<time off>, <time on>, <time off>...} 
 *  {0,10,20} would be off for 0 seconds, on for 100ms, then off for 200ms.
 *  Values must be between 0 and 254 inclusively.
 *  The value of 255 is reserved for a stop character so cannot be used, therefor the max duration of a burst is 2540ms or 2.54 seconds
 *  
 */

#define STOP 0xFF

// Static Memory (Patterns)
// Rapid Bursts
#define RAPID_ON_DURATION 20
#define RAPID_OFF_DURATION 20
const unsigned char RAPID_BURST_A[] PROGMEM = {0, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, STOP};
const unsigned char RAPID_BURST_B[] PROGMEM = {RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, RAPID_OFF_DURATION, RAPID_ON_DURATION, STOP};

#define RAPID_ON_DURATION_1 4
#define RAPID_OFF_DURATION_1 4
const unsigned char RAPID_BURST_40MS[] PROGMEM = {0, RAPID_ON_DURATION_1, RAPID_OFF_DURATION_1, RAPID_ON_DURATION_1, RAPID_OFF_DURATION_1, RAPID_ON_DURATION_1, RAPID_OFF_DURATION_1, RAPID_ON_DURATION_1, RAPID_OFF_DURATION_1, STOP};

#define RAPID_ON_DURATION_2 10
#define RAPID_OFF_DURATION_2 10
const unsigned char RAPID_BURST_100MS[] PROGMEM = {0, RAPID_ON_DURATION_2, RAPID_OFF_DURATION_2, RAPID_ON_DURATION_2, RAPID_OFF_DURATION_2, RAPID_ON_DURATION_2, RAPID_OFF_DURATION_2, RAPID_ON_DURATION_2, RAPID_OFF_DURATION_2, STOP};

#define RAPID_ON_DURATION_3 3
#define RAPID_OFF_DURATION_3 3
const unsigned char RAPID_BURST_SUPERFAST[] PROGMEM = {0, RAPID_ON_DURATION_3, RAPID_OFF_DURATION_3, RAPID_ON_DURATION_3, RAPID_OFF_DURATION_3, RAPID_ON_DURATION_3, RAPID_OFF_DURATION_3, RAPID_ON_DURATION_3, RAPID_OFF_DURATION_3, STOP};

const unsigned char SINGLE_BURST[] PROGMEM = {0, 200, STOP};

const unsigned char JAWS_LEFT[] PROGMEM = {0,62,118,0,254,52,121,20,60,20,53,12,59,14,13,12,8,9,10,11,8,STOP};
const unsigned char JAWS_RIGHT[] PROGMEM = {241,75,209,40,83,17,60,19,47,16,40,8,13,9,10,9,9,11,7,STOP};

const unsigned char SHAVE_LEFT[] PROGMEM = {0,24,77,20,56,42,65,24,29,22,64,STOP};
const unsigned char SHAVE_RIGHT[] PROGMEM = {62,26,29,31,134,25,28,24,63,STOP};


// L/R/Middle
const unsigned char LRM_A[] PROGMEM = {0, 80, 40, 80, STOP};
const unsigned char LRM_B[] PROGMEM = {40, 80, 40, 80, STOP};

// Single Button Press Pattern
const unsigned char SINGLE_BUTTON_PRESS[] PROGMEM = {0, 10, STOP};

// Danny G
const unsigned char GOLDBERG_A[] PROGMEM = {100, 4, 4, 8, 100, 4, 4, 8, 1, 1, 2, 2, 4, 4, 8, 8, 16, 16, 32, 32, 64, 64, 128, 128, 254, 128, 100, 50, STOP};
const unsigned char GOLDBERG_B[] PROGMEM = {100, 4, 4, 8, 100, 4, 4, 8 + 2 + 4 + 8 + 16 + 32 + 64, 128, 128, 128, 32, 32, 32, 32, 254, 100, 50, STOP};


// Mikes Version
// const unsigned char BEETHOVEN_LEFT[] PROGMEM = {97, 6, 13, 6, 13, 6, 13, 175, 47, 7, 15, 7, 15, 7, 15, 285, 236, 6, 12, 6, 12, 6, 12, 99, 129, 6, 12, 6, 12, 6, 12, 86, 0, 6, 11, 6, 11, 6, 11, 60, 9, 6, 11, 6, 11, 6, 11, 6, 11, 60, 9, 6, 11, 6, 11, 6, 11, 6, 11, 30, 42, 32, 44, 37, 206, 7, 14, 7, 14, 7, 14, 308, 38, 6, 11, 6, 11, 6, 11, 197, 9, 6, 11, 6, 11, 6, 11, 6, 11, 197, 9, 6, 11, 6, 11, 6, 11, 6, 11, 59, 8, 6, 11, 6, 11, 6, 11, 6, 11, 59, 8, 6, 11, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 17, 0, 6, 11, 6, 11, 6, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 28, 107, 36, 344, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 294, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, 37, 0, STOP};
// const unsigned char BEETHOVEN_RIGHT[] PROGMEM = {97, 6, 13, 6, 13, 6, 13, 175, 47, 7, 15, 7, 15, 7, 15, 285, 96, 6, 12, 6, 12, 6, 12, 228, 0, 6, 12, 6, 12, 6, 12, 132, 9, 155, 0, 6, 11, 6, 11, 6, 11, 86, 0, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 30, 42, 32, 44, 37, 206, 7, 14, 7, 14, 7, 14, 308, 106, 6, 11, 6, 11, 6, 11, 129, 9, 29, 57, 6, 11, 6, 11, 6, 11, 129, 9, 6, 11, 6, 11, 6, 11, 6, 11, 84, 0, 6, 11, 6, 11, 6, 11, 84, 0, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 6, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 5, 11, 28, 107, 36, 344, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, 74, 0, STOP};

// Dannys Version
const unsigned char BEETHOVEN_LEFT[] PROGMEM = {0,18,12,16,14,13,20,53,253,9,9,8,11,8,11,12,132,12,8,11,8,9,10,14,125,9,8,8,11,7,8,10,137,8,11,8,9,9,11,12,156,26,100,17,18,19,18,18,23,66,45,STOP};
const unsigned char BEETHOVEN_RIGHT[] PROGMEM = {220,11,18,11,17,12,21,44,135,7,11,8,10,7,9,8,140,11,8,8,11,7,14,19,137,7,11,6,11,6,11,9,168,20,193,19,18,19,18,18,21,68,45,STOP};
