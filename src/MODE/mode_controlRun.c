/**
 * @file   mode_ControlRun.c
 * @brief  コースの状況に応じてモーターのdutyを変えながら走るモード <br>
 *  ////操作////<br>
 *  右SW: スタート,ストップの切り替え<br>
 *  左SW: 長押しでモード選択へ遷移<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

#include "mode_debug.h"
#include "type.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"


void mode_controlRun(void)
{
	float duty_straight = 1.0;
	float duty_curve = 0.3;
	float duty_slope = 0.4;

	turnGreenLED(1);

	myPrintfUART("############ control run mode ################\n");
	wait1msec(1000);
	while(1)
	{
		wait1msec(1);
		if( getLeftSWcount()>0 ){
			while(getRightSWcount() != 0 );
			break;  //左SWを押したら走り始める
		}

	}
	resetElapsedTime();
	setDegree(0.0);

	while(1)
	{
		wait1msec(10);

		if(isSlope() == 1) setDutyMotor(duty_slope);
		else if(isCurve == 1) setDutyMotor(duty_curve);
		else setDutyMotor(duty_straight);

		//モード選択へ遷移
		if( getLeftSWcount() > 1000)
		{
			setDutyMotor(0.0);
			turnGreenLED(0);
			myPrintfUART("\t return mode select \n");
			break;
		}
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
