#include <Arduino.h>
#include "./motors.h"

uint32_t motorsLocation[3];
uint8_t initialized = 0;

void initMotors(void) {
  pinMode(XMIN, INPUT_PULLUP);
  pinMode(XMAX, INPUT_PULLUP);
  pinMode(XSTEP, OUTPUT);
  pinMode(XEN, OUTPUT);
  pinMode(XDIR, OUTPUT);
  digitalWrite(XEN, HIGH);
  
  pinMode(YMIN, INPUT_PULLUP);
  pinMode(YMAX, INPUT_PULLUP);
  pinMode(YSTEP, OUTPUT);
  pinMode(YEN, OUTPUT);
  pinMode(YDIR, OUTPUT);
  digitalWrite(YEN, HIGH);
  
  pinMode(ZMIN, INPUT_PULLUP);
  pinMode(ZMAX, INPUT_PULLUP);
  pinMode(ZSTEP, OUTPUT);
  pinMode(ZEN, OUTPUT);
  pinMode(ZDIR, OUTPUT);
  digitalWrite(ZEN, HIGH);
  
}

void enableMotor(char id) {
  int pin;
  switch(id) {
    case 'X': 
      pin = XEN;
      break;
    case 'Y':
      pin = YEN;
      break;
    case 'Z':
      pin = ZEN;
      break;
    default:
      return;
  }
  digitalWrite(pin, LOW);
}


void disableMotors(void) {
  disableMotor('X');
  disableMotor('Y');
  disableMotor('Z');
}

void disableMotor(char id) {
  int pin;
  switch(id) {
    case 'X': 
      pin = XEN;
      break;
    case 'Y':
      pin = YEN;
      break;
    case 'Z':
      pin = ZEN;
      break;
    default:
      return;
  }
  digitalWrite(pin, HIGH);
}

void moveMotor(char id, uint8_t dir, uint32_t steps, uint16_t dly) {
  int pinStep, pinDir;
  switch(id) {
    case 'X': 
      pinStep = XSTEP;
      pinDir = XDIR;
      break;
    case 'Y':
      pinStep = YSTEP;
      pinDir = YDIR;
      break;
    case 'Z':
      pinStep = ZSTEP;
      pinDir = ZDIR;
      break;
    default:
      return;
  }
  steps = steps;
  enableMotor(id);
  digitalWrite(pinDir, dir);
  while(steps > 0) {
    steps--;
    digitalWrite(pinStep, HIGH);
    delayMicroseconds(dly);
    digitalWrite(pinStep, LOW);
    delayMicroseconds(dly);
  }
}

void homeMotors(void) {
  homeMotor('X', 25);
  homeMotor('Y', 25);
  homeMotor('Z', 25);
}

uint32_t homeMotor(char id, uint16_t dly) {
  uint8_t pinStep, pinDir, pinEnd;
  uint16_t steps = 0;
  uint32_t stp = 0;
  switch(id) {
    case 'X': 
      pinStep = XSTEP;
      pinDir = XDIR;
      pinEnd = XMIN;
      break;
    case 'Y':
      pinStep = YSTEP;
      pinDir = YDIR;
      pinEnd = YMIN;
      break;
    case 'Z':
      pinStep = ZSTEP;
      pinDir = ZDIR;
      pinEnd = ZMIN;
      break;
    default:
      return 0;
  }
  enableMotor(id);
  digitalWrite(pinDir, 1);
  while(steps < X_STEPS_PER_MM*200 && digitalRead(pinEnd) == 1) {
    digitalWrite(pinStep, HIGH);
    delayMicroseconds(dly);
    digitalWrite(pinStep, LOW);
    delayMicroseconds(dly);
    stp++;
  }
  return stp;
}
