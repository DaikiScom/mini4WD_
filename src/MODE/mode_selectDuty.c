
/**
 * @file   mode_selectDuty.c
 * @brief  dutyを選択して走るモード <br>
 *  ////操作////<br>
 *  右SW: dutyの変更<br>
 *  左SW: 長押しでモード選択へ遷移<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

#include "mode_selectDuty.h"
#include "type.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"


void mode_selectDuty(void)
{
	int8_t motor_duty = 0;   //duty 0から100%まで1%刻み
	int8_t sign = 1;

	myPrintfUART("############ duty select mode ################\n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");
	myPrintfUART("  Right  SW: change motor duty .\n");
	myPrintfUART("  Left   SW: mode select .\n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");

	while(1)
	{
		if(getRightSWcount() > 1000 ) wait1msec(100);
		else wait1msec(1);

		//右SWでモータの状態を切り替え
		if( getRightSWcount() >0)
		{
			motor_duty ++;
			if(motor_duty == 101) sign *= -1;
			motor_duty = motor_duty %101;

			myPrintfUART("\t motor duty: %d % \n",motor_duty);
			//モーターのdutyを変える
			setDutyMotor( (float)(sign * motor_duty)*0.01);
			flashGreenLED(motor_duty,100 - motor_duty);

			while(getRightSWcount() != 0 ){
				if(getRightSWcount() > 1000)break;
			};
		}

		//モード選択へ遷移
		if( getLeftSWcount() == 1000)
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
