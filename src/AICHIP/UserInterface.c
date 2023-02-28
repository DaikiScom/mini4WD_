/**
 * @file   UserInterface.c
 * @brief  LED,SW,電源電圧の監視等の機能の実装 <br>
 *
 * @author RTCorp. Ryota Takahashi
 */


#include "LPC13xx.h"
#include "core_cm3.h"
#include "timer.h"
#include "type.h"
#include "pwm.h"
#include "UserInterface.h"
#include "ad.h"

//LEDが点滅状態か常時点灯状態かのフラグ 1:点滅 　0:常時点灯(消灯も含む)
volatile static uint8_t flag_flash_redLED = 0;
volatile static uint8_t flag_flash_greenLED = 0;
//SWの押下状態のカウント
volatile static uint16_t leftSW_count  = 0;
volatile static uint16_t rightSW_count = 0;
//LEDの点滅用カウント
volatile static uint16_t redLED_count   = 0;
volatile static uint16_t greenLED_count = 0;
//経過時間のカウント
volatile static uint32_t elapsed_time_count = 0;
//LEDの点滅時間のon/off時間のカウント
volatile static uint16_t on_count_redLED    = 0;
volatile static uint16_t off_count_redLED   = 1;
volatile static uint16_t on_count_greenLED  = 0;
volatile static uint16_t off_count_greenLED = 1;
//電池の電圧値[V]
volatile static float voltageLipo  = 0.0;
volatile static float voltageMotor = 0.0;


/**
 * LEDの点滅, AD変換でのバッテリーの電圧取得, スイッチの押下状態の監視<br>
 * 機能の初期化 <br>
 * この関数は各ペリフェラルの初期化後に呼ぶこと <br>
 *
 * この関数を呼ぶと <br>
 *         <ul>
 *          <li>getLeftSWcount()</li>
 *          <li>getRightSWcount() </li>
 *          <li>getElapsedTime() </li>
 *          <li>flashRedLED() </li>
 *          <li>flashGreenLED()</li>
 *          <li>getLipoVoltage()</li>
 *          <li>getMotorVoltage()</li>
 *         </ul>
 * が有効となる.
 *
 * @param  void
 * @return void
 */
void initUI(void)
{
	startTimer32_0();
}

/*----------------------------------------------------------------------------
  Input  void
  Output state 1:on 0:off

  SW(Left)の状態取得
 *---------------------------------------------------------------------------*/

/**
 * 左スイッチの押下状態取得
 *
 * @param void
 * @return 1:押している 0:押していない
 */
uint8_t getStateLeftSW(void)
{
	uint32_t state;
	state = LPC_GPIO2->DATA & 0x1;
	state = 1 - state;
	return state;
}

/**
 * 右スイッチの押下状態取得
 *
 * @param void
 * @return 1:押している 0:押していない
 */
uint8_t getStateRightSW(void)
{
	uint32_t state;
	state = (LPC_GPIO0->DATA & 0x2)>>1;
	state = 1 - state;
	return state;
}

/**
 * 左スイッチの押下状態取得
 *
 * @param void
 * @return 1:押している 0:押していない
 */

uint16_t getLeftSWcount(void)
{
	return leftSW_count;
}

/**
 * 右スイッチの押下時間を返す
 *
 * @param void
 * @return 右スイッチを押してた 秒数 [msec]
 */
uint16_t getRightSWcount(void)
{
	return rightSW_count;
}

/**
 * initUI()を呼んでからの経過時間を返す.
 * elapsed_time_count * 1msec
 *
 * @param void
 * @return 経過した秒数 [msec]
 */
uint32_t getElapsedTime(void)
{
	return elapsed_time_count;
}

/**
 * 経過した秒数をリセットする
 *
 * @param  void
 * @return void
 */
void resetElapsedTime(void)
{
	elapsed_time_count = 0;
}

/**
 * 右LEDをon/offする
 *
 * @param  state 1:点灯 0:消灯
 * @return void
 */
static void setStateGreenLED(uint8_t state)
{
	if(state == 1)
	{
		LPC_GPIO1->DATA |= 0x0100; //LED1 on
	}
	else
	{
		LPC_GPIO1->DATA &= ~0x0100; //LED1 off
	}
}

/**
 *  赤LEDの点灯と消灯 LED2
 *
 * @param  state 1:点灯 0:消灯
 * @return void
 */
static void setStateRedLED(uint8_t state)
{
	if(state == 1)
	{
		LPC_GPIO1->DATA |= 0x0008; //LED2 on
	}
	else
	{
		LPC_GPIO1->DATA &= ~0x0008; //LED2 off
	}
}

/**
 * 赤LEDの点灯と消灯.  LEDが点滅状態で
 * 呼んだ場合は常時点灯(消灯)状態になる.
 *
 * @param  state 1:点灯 0:消灯
 * @return void
 */
void turnRedLED(uint8_t state)
{
	flag_flash_redLED = 0;
	setStateRedLED(state);
}

/**
 * 緑LEDの点灯と消灯.  LEDが点滅状態で
 * 呼んだ場合は常時点灯(消灯)状態になる.
 *
 * @param  state 1:点灯 0:消灯
 * @return void
 */
void turnGreenLED(uint8_t state)
{
	flag_flash_greenLED = 0;
	setStateGreenLED(state);
}

/**
 * 緑LEDを指定した時間間隔で点滅させる <br>
 * 例.LEDを100msecの間on, 50msecの間offで点滅させるには <br>
 *    on_count = 100, off_count = 50 <br>
 * LEDを常時点灯する場合は <br>
 *    turnGreenLED(1) <br>
 * LEDを消灯する場合は <br>
 *    turnGreenLED(0) <br>
 *
 * @param  on_count  [msec]
 * @param  off_count [msec]
 * @return void
 */

void flashGreenLED(uint16_t on_count, uint16_t off_count)
{
	flag_flash_greenLED = 1;
	on_count_greenLED    = on_count;
	off_count_greenLED   = off_count;
}

/**
 * 赤LEDを指定した時間間隔で点滅させる <br>
 * 例.LEDを100msecの間on, 50msecの間offで点滅させるには <br>
 *    on_count = 100, off_count = 50 <br>
 * LEDを常時点灯する場合は <br>
 *    turnRedLED(1) <br>
 * LEDを消灯する場合は <br>
 *    turnRedLED(0) <br>
 *
 * @param  on_count  [msec]
 * @param  off_count [msec]
 * @return void
 */
void flashRedLED(uint16_t on_count, uint16_t off_count)
{
	flag_flash_redLED = 1;
	on_count_redLED    = on_count;
	off_count_redLED   = off_count;
}

/**
 * Lipoバッテリーの電圧取得
 *
 * @param void
 * @return Lipoの電圧
 */
float getLipoVoltage(void)
{
	return voltageLipo;
}

/**
 * モーター用バッテリー(単三電池2直列)の電圧取得
 *
 * @param void
 * @return モーターの電圧
 */
float getMotorVoltage(void)
{
	return voltageMotor;
}

/*----------------------------------------------------------------------------
  Input  void
  Output void

　 32bitタイマー0の割り込み関数
    各種countのインクリメントとAD変換を実行
 *---------------------------------------------------------------------------*/

/**
 * UserInterface内の関数を使えるようにするための割り込み関数<br>
 * 1msec毎に実行される
 *
 * @param void
 * @return void
 */
void TIMER32_0_IRQHandler(void)
{
	int16_t ad_val[8];
	LPC_TMR32B0->IR=0x08; //clear interrupt flag

	if(flag_flash_redLED == 1)
	{
		//赤LEDの点滅
		if(redLED_count < on_count_redLED ) setStateRedLED(1);
		else setStateRedLED(0);
		if( (on_count_redLED + off_count_redLED) < redLED_count ) redLED_count = 0;
		redLED_count ++;
	}

	//緑LEDの点滅
	if(flag_flash_greenLED == 1)
	{
		if(greenLED_count < on_count_greenLED ) setStateGreenLED(1);
		else setStateGreenLED(0);
		if( (on_count_greenLED + off_count_greenLED) < greenLED_count  ) greenLED_count = 0;
		greenLED_count ++;
	}
	//時間経過
	elapsed_time_count ++;
	//左スイッチの押下
	if(getStateLeftSW() == 1) leftSW_count ++;
	else leftSW_count = 0;

	//右スイッチの押下
	if(getStateRightSW() == 1) rightSW_count ++;
	else rightSW_count = 0;

	//AD変換と電池の電圧計算
	storeAD2Array(&ad_val[0]);
	voltageLipo  = (float)(ad_val[1])/1024.0 * 3.0 * 2.0;
	voltageMotor = (float)(ad_val[0])/1024.0 * 3.0 * 2.0;

}

/**
 * debug用関数
 *
 * @param void
 * @return void
 */
void debugUI(void)
{
	myPrintfUART("#######debug UI####### \n");
	myPrintfUART("RSW state %d \n", getStateRightSW());
	myPrintfUART("RSW count %d msec \n", getRightSWcount());
	myPrintfUART("LSW state %d \n", getStateLeftSW());
	myPrintfUART("LSW count %d msec \n", getLeftSWcount());
	myPrintfUART("Elap Time %d msec \n", getElapsedTime());
	myPrintfUART("V Motor   %f V \n", getMotorVoltage());
	myPrintfUART("V Lipo    %f V \n", getLipoVoltage());
}

/******************************************************************************
**                            End Of File
******************************************************************************/
