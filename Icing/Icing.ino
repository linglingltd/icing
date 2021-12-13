#include "./motors.h"
#define LED       13

#define FAN       9
#define PELTIER   8

#define ORN   A12
#define YLO   44
#define PNK   A10
#define BLU   A5

// X 55 - X 100
// Y 10 - Y 45
/* Controlling via Serial:
 *  
 *  N = Axis in {X, Y, Z}
 *  N HOME    Homing Axis N
 *  N 10      Moving Axis to 10mm absolute
 *  N +10      Moving Axis by +10mm
 *  N -10      Moving Axis to -10mm
 *  N s10      Moving Axis to 10 Steps absolute
 *  N +s10      Moving Axis by +10 Steps
 *  N -s10      Moving Axis to -10 Steps
 */

void printHeading(void);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(PELTIER, OUTPUT);
  pinMode(FAN, OUTPUT);

  pinMode(19, INPUT_PULLUP);

  initMotors();
  disableMotors();

  digitalWrite(FAN, HIGH);
  //digitalWrite(PELTIER, HIGH);

  
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n"); 
  for(uint8_t i=0;i<62;i++) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.print("\n");

  /*homeMotor('X', 10);
  moveMotor('X', 0, 10*200*16, 10);
  homeMotor('X', 25);
  moveMotor('X', 0, 5*200*32, 10);
  //disableMotors();
  homeMotor('Z', 10);
  moveMotor('Z', 0, 100*16, 90);
  homeMotor('Z', 450);
  moveMotor('Z', 0, 3*200*32, 90);

  //Put Y in correct position manually!
  delay(5000);
  moveMotor('Y', 1, 1, 100);*/
}

#define DELAYTIME 900
uint16_t steps = 0, i = 300;

uint8_t j = 0;

void loop() {
  while(Serial.available()) {
    handleSerial();
  }
  if(j == 1) {
    j = 0;
    uint32_t steps;
    homeMotor('X', 10);
    moveMotor('X', 0, 200*16, 10);
    homeMotor('X', 50);
    for(uint8_t i=0;i<60;i++) {
      steps = homeMotor('Z', 50);
      moveMotor('Z', 0, steps, 50);
      Serial.print((int32_t)steps-(3*200*32));
      Serial.print("\t");
      moveMotor('X', 0, 200*32, 25);
    }
    Serial.print("\n");
  } else if(j == 2) {
    j = 0;
    uint32_t steps;
    homeMotor('X', 10);
    moveMotor('X', 0, 10*200*16, 10);
    homeMotor('X', 25);
    moveMotor('X', 0, 5*200*32, 10);
    homeMotor('Z', 10);
    moveMotor('Z', 0, 100*16, 90);
    homeMotor('Z', 450);
    moveMotor('Z', 0, 2*200*32, 90);
    homeMotor('X', 25);
    for(uint8_t i=0;i<62;i++) {
      executeZscan();
      moveMotor('X', 0, 200*32, 25);
    }
    Serial.print("\n");
  } else {
    //disableMotors();
  }
  delay(100);
}

void printHeading(void) {
  // This function is obsolete, could be scrapped.
  for(uint8_t i=0;i<62;i++) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.print("\n");
}

char rcvc = 0, rcv[20] = {0};
uint8_t rcvct = 0;
void handleSerial(void) {
  char axis = '0';
  rcvc = Serial.read();
  if(rcvc == '\n') {
    Serial.print(rcv);
    Serial.print(": ");
    if(0 == strncmp(rcv, "X ", 2)) axis = 'X';
    else if(0 == strncmp(rcv, "Y ", 2)) axis = 'Y';
    else if(0 == strncmp(rcv, "Z ", 2)) axis = 'Z';

    // Controlling individual Axis
    if(axis != '0') {
      
      if(0 == strncmp(rcv+2, "HOME", 4)) {  // Homing
        homeMotor(axis, (axis == 'Y') ? 100 : 25);
        Serial.print("Homed.\n");
      } else if(rcv[2] == '-' || rcv[2] == '+') {  // Relative Bewegung
        uint32_t stp = (rcv[3] == 's') ? strToInt(rcv+4) : strToInt(rcv+3);
        moveMotor(axis, 
            (rcv[2] == '-') ? 1 : 0,
            stp * ((rcv[3] == 's') ? 1 : stepsPerMm[axis-'X']),
            (axis == 'Y') ? 100 : 25);
        Serial.print("Moved relative.\n");
      } else {  // Absolte Bewegung
        uint32_t stp = (rcv[2] == 's') ? strToInt(rcv+3) : strToInt(rcv+2);
        moveMotorAbsolute(axis, 
            stp * ((rcv[2] == 's') ? 1 : stepsPerMm[axis-'X']), 
            (axis == 'Y') ? 100 : 25);
        Serial.print("Moved absolute.\n");
      }
    } else if(0 == strcmp(rcv, "SCAN")) {
      Serial.print("Exec. area scan.\n");
      areaScan((uint32_t)3200*55, (uint32_t)164*10, (uint32_t)3200, (uint32_t)164, 45, 30);
    } else if(0 == strcmp(rcv, "FASTSCAN")) {
      Serial.print("Exec. area scan.\n");
      areaScan((uint32_t)3200*55, (uint32_t)164*10, (uint32_t)3200*5, (uint32_t)164*5, 10, 8);
      Serial.print("\n\n");
    } else if(0 == strcmp(rcv, "FASTSCANL")) {
      Serial.print("Exec. area scan.\n");
      areaScan((uint32_t)3200*57, (uint32_t)164*10, (uint32_t)3200, (uint32_t)82*5, 10, 15);
      Serial.print("\n\n");
    } else if(0 == strcmp(rcv, "FASTSCANR")) {
      Serial.print("Exec. area scan.\n");
      areaScan((uint32_t)3200*+62, (uint32_t)164*10, (uint32_t)3200*3, (uint32_t)82*5, 13, 15);
      Serial.print("\n\n");
    } else if(0 == strcmp(rcv, "P=1")) {
      digitalWrite(PELTIER, HIGH);
      Serial.print("P set to 1\n");
    } else if(0 == strcmp(rcv, "P=0")) {
      digitalWrite(PELTIER, LOW);
      Serial.print("P set to 0\n");
    } else if(0 == strcmp(rcv, "X-Sweep")) {
      j = 1;
      Serial.print("Starting X-Sweep\n");
    } else if(0 == strcmp(rcv, "X-Sweep2")) {
      j = 2;
      Serial.print("Starting X-Sweep2\n");
    } else {
      // Nope
    }
    
    while(rcvct > 0) {
      rcv[rcvct-1] = 0; 
      rcvct--;
    }
  }
  else if(rcvct < 19) {
    rcv[rcvct] = rcvc;
    rcvct++;
  }
}

uint32_t strToInt(char *txt) {
  uint32_t ret = 0;
  while(*txt) {
    if(*txt >= '0' && *txt <= '9') {
      ret *= 10;
      ret += *txt - '0';
      txt++;
    } else {
      break;
    }
  }
  Serial.print(ret);
  return ret;
}

void areaScan(uint32_t ax, uint32_t ay, uint32_t stepx, uint32_t stepy, uint8_t nx, uint8_t ny) {
  // @@@ Probably do some Z-Homing before running
  
  // Goto start
  moveMotorAbsolute('X', ax, 25);
  moveMotorAbsolute('Y', ay, 25);

  // Start scanning
  for(uint8_t y=0; y<ny; y++) {
    if(y != 0) {
      moveMotor('Y', 0, stepy, 100);
      moveMotorAbsolute('X', ax, 25); // Do this last due to wobble on y.
    }
    for(uint8_t x=0; x<nx; x++) {
      if(x != 0) moveMotor('X', 0, stepx, 25);
      executeZscan();
    } 
  }
  
  moveMotorAbsolute('X', 10*X_STEPS_PER_MM, 25);
  moveMotorAbsolute('Y', 10*Y_STEPS_PER_MM, 25);
}

void executeZscan() {
  steps = homeMotor('Z', 50);
  moveMotor('Z', 0, steps, 50);
  Serial.print((int32_t)steps-(2*200*32));
  Serial.print("\t");
}
