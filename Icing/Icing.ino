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

uint8_t j = 0;
void loop() {
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
  delay(1000);
}

void printHeading(void) {
  for(uint8_t i=0;i<60;i++) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.print("\n");  
}
