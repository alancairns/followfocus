#include <Wire.h>
#include <string.h>

//added by AC
#include <Arduino.h>
//added by AC

#undef int
#include <stdio.h>
  
#define VERSION "0.6.0"
#define VERSIONINFO "(Simplified)"

//added by AC; used to power-off motor to save battery
unsigned long motorStoppedTimeMark = 0;
unsigned long motorStoppedTimeInterval = 5000; // 5 sec delay to disable L6226 H-Bridge after motor stops
  
int bridgeEnableA = 6;
int bridgeEnableB = 7;
long oldcurrentPosition = 0; 
//added by AC

// output pins which are connected to the H-bridge
int pinA =  3;    
int pinB =  9; 
int pinC =  10;    
int pinD =  11;    

// some timing data
int del = 5; // 25000;
int delMilli = 0;
int stepsize = 2;

static uint8_t nunchuck_buf[6];   // array to store nunchuck data,

boolean requestedData = false;
int joy_x_axis = 0;
int joy_y_axis = 0;
boolean z_button = false;
boolean c_button = false;
int analog_axis_y = 0;

// overflows and multipliers contains the speed table 
int overflows[21];
int multipliers[21];

// if high-speed is enabled, we will support a non-microstepped mode.
volatile boolean enableHighSpeed = false;
volatile boolean invertDirection = true; // because my motor is mounted facing camera

#define fastAnalogWriteA(val) if (invertDirection) OCR1A = val; else OCR2B = val; 
#define fastAnalogWriteB(val) if (invertDirection) OCR2B = val; else OCR1A = val; 
#define fastAnalogWriteC(val) OCR1B = val; 
#define fastAnalogWriteD(val) OCR2A = val;

// this controls the sinewave
const int NUMBER_OF_MICROSTEPS =128;
const int NUMBER_OF_MICROSTEPS_BIT = 7; // which bit is set in NUMBER_OF_MICROSTEPS
const long PHASE_MODULO = 4 * NUMBER_OF_MICROSTEPS;
int sinValues[NUMBER_OF_MICROSTEPS];

