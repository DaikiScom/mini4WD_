/* ------------------------------------------------------------ *
File AICHIPFunction.h

* ------------------------------------------------------------ */
#ifndef AICHIPFUNCTION_H
#define AICHIPFUNCTION_H

void initAICHIP(void);
void setSendDataEnable(uint8_t en);
void setDutyMotor(float pwm_duty);
float getDutyMotor(void);
void setDutyRate(float targetDuty, float dutyRate);

float getDegree(void);
void  setDegree(float deg);
uint8_t isStop(void);
uint8_t isCurve(void);
uint8_t isSlope(void);

void debugAICHIP(void);

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
