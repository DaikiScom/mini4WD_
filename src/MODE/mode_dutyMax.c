/**
 * @file   mode_debug.c
 * @brief  duty　maxで走らせるモード <br>
 * ////操作////<br>
 * 右SW: モータのon/off<br>
 * 左SW: 長押しでモード選択へ遷移<br>
 *
 * @author RTCorp. Ryota Takahashi
 */
#include "mode_dutyMax.h"
#include "type.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"


void mode_dutyMax(void)
{
	uint8_t flag_motor_state = 0;   //0:ストップ 1:回転

	myPrintfUART("############ duty max mode ################\n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");
	myPrintfUART("  Right  SW: change motor state .\n");
	myPrintfUART("  Left   SW: mode select .\n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");

	while(1)
	{
		wait1msec(1);

		//右SWでモータの状態を切り替え
		if( getRightSWcount() > 0)
		{
			flag_motor_state = 1 - flag_motor_state;
			myPrintfUART("\t motor state : %d \n",flag_motor_state);
			while(getRightSWcount() != 0 );
		}

		//モーターの状態を変える
		if( flag_motor_state == 1)
		{
			turnGreenLED(1);
			setDutyMotor(1.0);
		}
		else
		{
			turnGreenLED(0);
			setDutyMotor(0.0);
		}

		//モード選択へ遷移
		if( getLeftSWcount() > 1000)
		{
			setDutyMotor(0.0);
			myPrintfUART("\t return mode select \n");
			break;
		}

	}

}

/******************************************************************************
**                            End Of File
******************************************************************************/
