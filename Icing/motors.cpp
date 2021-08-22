#include <Arduino.h>
#include "./motors.h"

const uint32_t motorMax[3] = { (uint32_t)3200*140, (uint32_t)164*100, (uint32_t)3200*70 };
uint32_t motorPos[3];
uint8_t initialized = 0;
uint8_t homed;

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
  homed = 0;
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
      homed &= ~0x01;
      break;
    case 'Y':
      pin = YEN;
      homed &= ~0x02;
      break;
    case 'Z':
      pin = ZEN;
      homed &= ~0x04;
      break;
    default:
      return;
  }
  digitalWrite(pin, HIGH);
}


void moveMotorAbsolute(char id, uint32_t pos, uint16_t dly) {
  uint32_t currPos = motorPos[id - 'X'];
  
  // Never exceed Axis-Maximum
  if(pos > motorMax[id - 'X']) return;

  // Move motor accordingly
  if(currPos < pos) {
    return moveMotor(id, 0, pos-currPos, dly);
  } else if(currPos > pos) {
    return moveMotor(id, 1, currPos-pos, dly);
  }
}

void moveMotor(char id, uint8_t dir, uint32_t steps, uint16_t dly) {
  int pinStep, pinDir;
  // Home, if Axis has not been homed.
  /*Serial.print(id);
  Serial.print(" is at ");
  Serial.print(motorPos[id - 'X']);
  Serial.print(" should move by ");
  Serial.print(steps);
  Serial.print('\n');*/
  
  if((homed & (0x01 << (id - 'X'))) != (0x01 << (id - 'X'))) {
    homeMotor(id, dly);
  }

  // Check if movement is acceptable
  if(dir == 1 && steps > motorPos[id - 'X']) {
    return;
  } else if(dir == 0 && steps+motorPos[id - 'X'] > motorMax[id - 'X']) {
    return;
  }

  // Movement acceptable - do some moving
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

  // Recalc Position of Axis
  if(dir == 1) {
    motorPos[id - 'X'] -= steps;
    //Serial.print("Lowered Axis to ");
    //Serial.print(motorPos[id - 'X']);
  } else if(dir == 0) {
    motorPos[id - 'X'] += steps;
    //Serial.print("Increased Axis to ");
    //Serial.print(motorPos[id - 'X']);
  }  
  // Move it
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
      motorPos[0] = 0;
      homed |= 0x01;
      break;
    case 'Y':
      pinStep = YSTEP;
      pinDir = YDIR;
      pinEnd = YMIN;
      motorPos[1] = 0;
      homed |= 0x02;
      break;
    case 'Z':
      pinStep = ZSTEP;
      pinDir = ZDIR;
      pinEnd = ZMIN;
      motorPos[2] = 0;
      homed |= 0x04;
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
