#include "./motors.h"
#define LED       13

#define FAN       9
#define PELTIER   8

#define ORN   A12
#define YLO   44
#define PNK   A10
#define BLU   A5

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

  homeMotor('X', 10);
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
  moveMotor('Y', 1, 1, 100);
}

#define DELAYTIME 900
uint16_t steps = 0, i = 300;

uint8_t j = 0;
char rcvc = 0, rcv[20] = {0};
uint8_t rcvct = 0;
void loop() {
  while(Serial.available()) {
    rcvc = Serial.read();
    if(rcvc == '\n') {
      Serial.print(rcv);
      if(0 == strcmp(rcv, "P=1")) {
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
      steps = homeMotor('Z', 50);
      moveMotor('Z', 0, steps, 50);
      Serial.print((int32_t)steps-(2*200*32));
      Serial.print("\t");
      moveMotor('X', 0, 200*32, 25);
    }
    Serial.print("\n");
  } else {
    //disableMotors();
  }
  delay(100);
}

void printHeading(void) {
  for(uint8_t i=0;i<62;i++) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.print("\n");  
}
