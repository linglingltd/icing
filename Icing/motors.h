#ifndef MOTORS_H
#define MOTORS_H


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

#define XSTEP    A0
#define XMIN     3
#define XMAX     2
#define XEN      38
#define XDIR     A1

#define YSTEP    A6
#define YMIN     14
#define YMAX     15
#define YEN      A2
#define YDIR     A7

#define ZSTEP    46
#define ZMIN     18
#define ZMAX     19
#define ZEN      A8
#define ZDIR     48

#define E0STEP   26
#define E0EN     24
#define E0DIR    28

#define E1STEP   36
#define E1EN     30
#define E1DIR    34

#define X_STEPS_PER_MM  3200
#define Y_STEPS_PER_MM  164

void initMotors(void);

void enableMotor(char id);
void disableMotors(void);
void disableMotor(char id);

void homeMotors();
uint32_t homeMotor(char id, uint16_t dly);

void moveMotor(char id, uint8_t dir, uint32_t steps, uint16_t dly);
void moveMotorAbsolute(char id, uint32_t pos, uint16_t dly);

#endif
