#include "./motors.h"
#define A0   (54)
#define A1   (55)
#define A2   (56)
#define A3   (57)
#define A4   (58)
#define A5   (59)
#define A6   (60)
#define A7   (61)
#define A8   (62)
#define A9   (63)
#define A10  (64)
#define A11  (65)
#define A12  (66)
#define A13  (67)
#define A14  (68)
#define A15  (69)

#define E0STEP   26
#define E0EN     24
#define E0DIR    28

#define E1STEP   36
#define E1EN     30
#define E1DIR    34

#define LED       13

#define FAN       9
#define PELTIER   8

#define ORN   A12
#define YLO   44
#define PNK   A10
#define BLU   A5

#define ZMIN     18
#define ZMAX     19

void moveStepper(char id, uint8_t dir, uint32_t steps);
uint32_t homeStepper(char id);
void printHeading(void);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(PELTIER, OUTPUT);
  pinMode(FAN, OUTPUT);

  pinMode(ORN, OUTPUT);
  pinMode(YLO, OUTPUT);
  pinMode(PNK, OUTPUT);
  pinMode(BLU, OUTPUT);

  pinMode(19, INPUT_PULLUP);

  initMotors();

  disableMotors();

  digitalWrite(BLU, LOW);
  digitalWrite(PNK, LOW);
  digitalWrite(YLO, LOW);
  digitalWrite(ORN, LOW);

  
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n"); 
  for(uint8_t i=0;i<60;i++) {
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

/*void loop() {
  if(i == 0) {
    Serial.print("Slow Testing (fixed pos.)\n");
  } else if(i == 100) {
    Serial.print("Fast Testing (fixed pos.)\n");
  } else if(i == 200) {
    Serial.print("Fast Testing (Var X pos.)\n");
  } else if(i == 300) {
    Serial.print("Fast Testing (Var X & Y pos.)\n");
  }
  
  if(i < 100) {
    steps = homeMotor('Z', 50);
    moveMotor('Z', 0, steps, 50);
    Serial.print(i);
    Serial.print("\t");
    Serial.print((int32_t)steps-(3*200*32));
    Serial.print("\n");
    delay(10);
    i++;
  } else if(i < 200) {
    steps = homeMotor('Z', 25);
    moveMotor('Z', 0, steps, 25);
    Serial.print(i-100);
    Serial.print("\t");
    Serial.print((int32_t)steps-(3*200*32));
    Serial.print("\n");
    delay(10);
    i++;
  } else if(i < 300) {
    steps = homeMotor('Z', 50);
    moveMotor('Z', 0, steps, 50);
    moveMotor('X', 0, 2*200*32, 25);
    moveMotor('X', 1, 2*200*32, 25);
    Serial.print(i-200);
    Serial.print("\t");
    Serial.print((int32_t)steps-(3*200*32));
    Serial.print("\n");
    delay(10);
    i++;
  } else if(i < 400) {
    steps = homeMotor('Z', 50);
    moveMotor('Z', 0, steps, 50);
    moveMotor('X', 0, 2*200*32, 25);
    moveMotor('Y', 0, 100*32, 100);
    moveMotor('X', 1, 2*200*32, 25);
    moveMotor('Y', 1, 100*32, 100);
    Serial.print(i-300);
    Serial.print("\t");
    Serial.print((int32_t)steps-(3*200*32));
    Serial.print("\n");
    delay(10);
    i++;
  } else {
    Serial.print("Overflow. Restarting test routine!\n");
    i = 0;
  }
}*/

/*void loop() {
    steps = homeMotor('Z', 50);
    moveMotor('Z', 0, steps, 50);
    moveMotor('X', 0, 2*200*32, 25);
    moveMotor('X', 1, 2*200*32, 25);
    Serial.print(i);
    Serial.print("\t");
    Serial.print((int32_t)steps-(3*200*32));
    Serial.print("\n");
    delay(10);
    i++;
}*/
uint8_t j = 0;
void loop() {
  uint32_t steps;
  homeMotor('X', 10);
  moveMotor('X', 0, 200*16, 10);
  homeMotor('X', 50);
  //disableMotors();
  for(uint8_t i=0;i<60;i++) {
    //disableMotors();
    steps = homeMotor('Z', 50);
    moveMotor('Z', 0, steps, 50);
    Serial.print((int32_t)steps-(3*200*32));
    //moveMotor('Z', 0, 5*200*32, 90);
    Serial.print("\t");
    //disableMotors();
    moveMotor('X', 0, 200*32, 25);
    //disableMotors();
  }
  Serial.print("\n");
  /*if(j < 10) {
    j++;
    moveMotor('Y', 1, Y_STEPS_PER_MM, 100);
  } else {
    j=0;
    moveMotor('Y', 0, j*Y_STEPS_PER_MM, 100);
  }*/
  
  delay(1000);
}

void printHeading(void) {
  for(uint8_t i=0;i<60;i++) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.print("\n");  
}

uint32_t homeStepper(char id) {
  uint32_t steps = 0;
  while(digitalRead(ZMAX)) {
    steps++;
    moveStepper(id, 0, 1);
  }
  return steps;
}

void moveStepper(char id, uint8_t dir, uint32_t steps) {
  switch(id) {
    case 'z':
      break;
    default:
      return;
      break;
  }

  if(dir == 0) {
    while(steps) {
      steps--;      
      digitalWrite(PNK, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(BLU, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(YLO, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(PNK, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(ORN, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(YLO, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(BLU, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(ORN, LOW);
      delayMicroseconds(DELAYTIME);
    }    
  } else if(dir == 1) {
    while(steps) {
      steps--;
      digitalWrite(ORN, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(BLU, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(YLO, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(ORN, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(PNK, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(YLO, LOW);
      delayMicroseconds(DELAYTIME);
      digitalWrite(BLU, HIGH);
      delayMicroseconds(DELAYTIME);
      digitalWrite(PNK, LOW);
      delayMicroseconds(DELAYTIME);

    }    
  }
}
