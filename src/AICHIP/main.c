/**
 * @file   main.c
 * @brief  RT-AICHIPのサンプルプログラム<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

//OHTER
#include "LPC13xx.h"
#include "core_cm3.h"
#include "type.h"
//PERIHERAL
#include "ad.h"
#include "debug.h"
#include "i2c.h"
#include "io.h"
#include "pwm.h"
//#include "ssp.h"
#include "SystemTickTimer.h"
#include "timer.h"
#include "usbTransmission.h"
#include "uart.h"
//AICHIP
#include "AICHIPFunction.h"
#include "mpu9250.h"
#include "UserInterface.h"
//MODE
#include "modeSelect.h"
//USB
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

/////プロトタイプ宣言//////////////
void init(void);
/////////////////////////////

/**
 * main関数
 *
 * @param void
 * @return 0
 */
int main (void)
{
	volatile uint8_t dummy;
	init();

//	tryUSBConnect();        //USB接続を試みる
	if(isUSBConfig() == 1) flashRedLED(300,300);//USBの接続使用
	else flashRedLED(100,100); //USBの接続不使用

	//左スイッチが押されていた場合は外部からのコマンド入力受付と機体情報の出力のみを行う
    wait1msec(50);
	if(getLeftSWcount()>0){
    	setSendDataEnable(1); //マシンデータを送信開始
    	//LEDを一回だけ点滅させる
    	turnRedLED(1);
    	turnGreenLED(1);
    	wait1msec(500);
    	turnRedLED(0);
    	turnGreenLED(0);
    	wait1msec(500);
    	while(1)
    	{
    		dummy++; //Infinite loop
    	}
    }


	setSendDataEnable(1); //マシンデータを送信開始

	while(1) //Infinite loop
	{
		modeSelect();     //モード選択を実行
	};

	return 0;
}


/**
 * 各種ペリフェラルの初期化
 *
 * @param  void
 * @return void
 */
void init(void)
{
	//IOポートの初期化
	initIO();
	//LEDを一回だけ点滅させる
	turnRedLED(1);
	turnGreenLED(1);
	wait1msec(500);
	turnRedLED(0);
	turnGreenLED(0);
	wait1msec(500);
	//各ペリフェラルの初期化
	USBIOClkConfig();
	initTimer32_0();
	initTimer32_1();
	Init_ad();
//	SSPInit();
//	USB_Init();
	UARTInit(115200);
	while (!I2CInit((uint32_t)I2CMASTER)); // I2Cの初期化が終わるまで待つ
	//9軸センサの初期化
	initMPU9250();
	initAK8963();
	calibOmegaOffset();  //ジャイロのオフセットを取得
	initOffsetByEEPROM(); //EEPROM
	//UIの初期化
	initUI();
	initAICHIP();
}

/******************************************************************************
**                            End Of File
******************************************************************************/
