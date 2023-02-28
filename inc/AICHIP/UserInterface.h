/* ------------------------------------------------------------ *
File UserInterface.h

* ------------------------------------------------------------ */
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

void initUI(void);

uint8_t getStateLeftSW(void);
uint8_t getStateRightSW(void);
uint16_t getLeftSWcount(void);
uint16_t getRightSWcount(void);

uint32_t getElapsedTime(void);
void resetElapsedTime(void);

void turnRedLED(uint8_t state);
void turnGreenLED(uint8_t state);

void flashGreenLED(uint16_t on_count, uint16_t off_count);
void flashRedLED(uint16_t on_count, uint16_t off_count);

float getLipoVoltage(void);
float getMotorVoltage(void);
float getSensorVoltage(void);

void debugUI(void);

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
