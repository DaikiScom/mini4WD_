/**
 * @file   mode_debug.c
 * @brief  モード選択を実行する関数 <br>
 *
 * @author RTCorp. Ryota Takahashi
 */

#include "modeSelect.h"

#include "LPC13xx.h"
#include "core_cm3.h"
#include "type.h"

#include "UserInterface.h"
#include "SystemTickTimer.h"
#include "debug.h"
#include "usbTransmission.h"

//動作モード毎の処理
#include "mode_dutyMax.h"
#include "mode_selectDuty.h"
#include "mode_debug.h"
#include "mode_controlRun.h"
#include "mode_BluetoothSetting.h"


static void modeSelectInfo(void)
{

	myPrintfUART("############ mode select ################\n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");
	myPrintfUART("  Choose the mode .\n");
	myPrintfUART("  Right  SW: mode change \n");
	myPrintfUART("  Left   SW: enter       \n");
	myPrintfUART("++++++++++++++++++++++++++++++++\n");
	myPrintfUART("  0 output this info.      :Green LED OFF \n");
	myPrintfUART("  1 duty max          mode :Green LED ON \n");
	myPrintfUART("  2 select duty       mode :flashing Green LED 1Hz \n");
	myPrintfUART("  3 control run       mode :flashing Green LED 2Hz \n");
	myPrintfUART("  4 Bluetooth setting mode :flashing Green LED 4Hz \n");
	myPrintfUART("  5 debug             mode :flashing Green LED 8Hz \n");
	myPrintfUART("+++++++status of AICHIP++++++++++\n");
	myPrintfUART("  Lipo  Voltage : %f [V]    \n", getLipoVoltage());
	myPrintfUART("  Motor Voltage : %f [V]    \n", getMotorVoltage());
	myPrintfUART("  is USB Config : %d        \n", isUSBConfig() );
	myPrintfUART("  elapsed time  : %d [msec] \n", getElapsedTime() );
	myPrintfUART("++++++++++++++++++++++++++++++++\n");
}

void modeSelect(void)
{
    uint8_t md_select = 0;
    modeSelectInfo();

	while(1)
	{
		wait1msec(1);
		if( getRightSWcount() > 0 ) //右SWでモード選択
		{
			md_select++;
			md_select = md_select % 6;
			myPrintfUART("\t md_select = %d \n  ",md_select);
			if(md_select == 0)
			{
				modeSelectInfo();
			}
			while(getRightSWcount() != 0 );
		}

		if( getLeftSWcount() > 0 ){
			while(getRightSWcount() != 0 );
			break;  //左SWで決定
		}

		switch(md_select){
           case  0:  turnGreenLED(0);
           break;
           case  1:  turnGreenLED(1);
           break;
           case  2:  flashGreenLED(500,500);
           break;
           case  3:  flashGreenLED(250,250);
           break;
	       case  4:  flashGreenLED(125,125);
	       break;
	       case  5:  flashGreenLED(62,63);
		   	   break;
		  }
	}

	flashGreenLED(20,80);
	wait1msec(1000);
	turnGreenLED(0);
	myPrintfUART("\t enter \n");

	switch(md_select){
		case  0:
			break;
		case  1:  mode_dutyMax();
			break;
		case  2:  mode_selectDuty();
			break;
		case  3:  mode_controlRun();
			break;
		case  4:  mode_BluetoothSetting();
			break;
		case  5:  mode_debug();
			break;
	}

	flashGreenLED(160,40);
	wait1msec(1000);
	turnGreenLED(0);
}






/******************************************************************************
**                            End Of File
******************************************************************************/
