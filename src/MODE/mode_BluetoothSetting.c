/**
 * @file   mode_BluetoothSetting.c
 * @brief  Bluetoothモジュールのボーレート等を設定する.<br>
 *   //設定項目//<br>
 *   Bluetoothデバイス名:RT-AICHIP <br>
 *   ボーレート:115200bps<br>
 *   動作モード:スレーブモード<br>
 *   設定変更後リブートコマンドを送る.<br>
 *   操作:<br>
 *    右SW: モータのdutyを変更<br>
 *    左SW: 長押しでモード選択へ繊維<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

#include "mode_debug.h"
#include "type.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"


void mode_BluetoothSetting(void)
{
	turnGreenLED(1);
	myPrintfUART("############ Bluetooth setting mode ################\n");
	wait1msec(300);
	myPrintfUART("$$$");    //コマンドモードに入る
	wait1msec(2000);
	myPrintfUART("SM,0\r");   //動作モードをスレーブモードに変更
	wait1msec(2000);
	myPrintfUART("SU,115K\r");//ボーレートを115200bpsに変更
	wait1msec(2000);
	myPrintfUART("SN,RT-AICHIP\r");//デバイス名の変更
	wait1msec(2000);
	myPrintfUART("R,1\r"); //リブート
	wait1msec(2000);

	while(1)
	{
		wait1msec(100);
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
