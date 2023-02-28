/**
 * @file   mode_debug.c
 * @brief  デバッグ用モード. データを出力し続ける  <br>
 * ////操作////<br>
 * 左SW: 長押しでモード選択へ遷移<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

#include "mode_debug.h"
#include "type.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"


void mode_debug(void)
{
	turnGreenLED(1);
	setSendDataEnable(0); //マシンデータを送信停止
	myPrintfUART("############ debug mode ################\n");
	wait1msec(1000);

	while(1)
	{
		wait1msec(100);
		debugUI();
		debugMPU9250();
		debugAICHIP();

		//モード選択へ遷移
		if( getLeftSWcount() > 1000)
		{
			setDutyMotor(0.0);
			turnGreenLED(0);
			myPrintfUART("\t return mode select \n");
			setSendDataEnable(1); //マシンデータを送信開始
			break;
		}
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
