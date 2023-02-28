/**
 * @file   AICHIPFunction.c
 * @brief  モーターの回転, 機体データの送信, コマンドの受信 <br>
 *         10msec毎に割り込み関数で処理を実行
 *
 * @author RTCorp. Ryota Takahashi
 */


#include "LPC13xx.h"
#include "core_cm3.h"
#include "type.h"

#include "timer.h"
#include "pwm.h"
#include "AICHIPFunction.h"
#include "UserInterface.h"

#include "mpu9250.h"
#include "debug.h"
#include "uart.h"
#include "tinyMathFunction.h"

#include "usbTransmission.h"


///////uart.cに宣言があり/////////////////////////
extern volatile uint32_t UARTCount;
extern volatile uint8_t  UARTBuffer[BUFSIZE];
///////プロトタイプ宣言/////////////////////////////
static void moterEnable(uint8_t en);
static void execUARTCommand();
static void execCommand(uint8_t *cmd);
//////////////////////////////////////////////

volatile static float motor_duty = 0.0;
volatile static float degree = 0.0;
volatile static uint8_t flag_send_data_enable = 0;

volatile static uint32_t time;
volatile static uint8_t flag_slope = 0; //1:坂を上っている 0:坂を上っていない
volatile static uint8_t flag_curve = 0; //1:curving 0:not curving
volatile static uint8_t flag_stop = 0;  //1:stop 0:moving


/**
 * AICHIPFunction.c内の関数を使用できるようにする
 *
 * @param void
 * @return void
 */
void initAICHIP(void)
{
	startTimer32_1();
}

/**
 * 機体データの送信をするかどうかを選択
 *
 * @param en 1:送信 0:送信じ禁止
 * @return void
 */
void setSendDataEnable(uint8_t en)
{
	flag_send_data_enable = en;
}

/**
 * モータードライバーのスリープの設定
 *
 * @param en 1:動作 0:スリープ
 * @return void
 */
static void moterEnable(uint8_t en)
{
	if(en == 1) LPC_GPIO1->DATA |=   0x0800 ; //NSLEEP ON
	if(en == 0) LPC_GPIO1->DATA &= ~(0x0800); //NSLEEP OFF
}


/**
 *  モーターの正転,負転の切り替え
 *
 * @param  dir  1:正転 -1:負転
 * @return void
 */
static void setRotationDir(int8_t dir)
{
	if(dir ==  1) LPC_GPIO0->DATA |=0x0080;
	if(dir == -1) LPC_GPIO0->DATA &= ~(0x0080);


}

/**
 *  モータを指定したdutyで回転させる
 *
 * @param  pwm_duty -1.0 to 1.0
 * @return void
 */
void setDutyMotor(float pwm_duty)
{

	if(pwm_duty == 0.0)
	{
		moterEnable(0);
		motor_duty = pwm_duty;
	}
	if(pwm_duty < 0.0)
	{
		if(pwm_duty < - 1.0) pwm_duty = -1.0;
		motor_duty = pwm_duty;
		Init_PWM2();

		moterEnable(1);
		setRotationDir(-1);
		setDutyPWM2 (1.0-(-1.0 * pwm_duty));
	}
	if(pwm_duty > 0.0)
	{
		if(pwm_duty >  1.0) pwm_duty = 1.0;
		motor_duty = pwm_duty;
		Init_PWM2();

		moterEnable(1);
		setRotationDir(1);
		setDutyPWM2 (1.0-pwm_duty );
	}

}

/**
 * motorに設定した現在のdutyを返す
 *
 * @param  void
 * @return 現在のduty -1.0から1.0
 */
float getDutyMotor(void)
{
	return motor_duty;
}

/**
 * 10msec毎に行われる処理 <br>
 *         <ul>
 *          <li>9軸センサの観測値をUART,USBで送信</li>
 *          <li>角速度の積分 </li>
 *          <li>各種機体情報をUART,USBで送信 </li>
 *          <li>UART経由でのコマンドの受付 </li>
 *         </ul>
 * @param  void
 * @return void
 */
void TIMER32_1_IRQHandler(void)
{

	static uint16_t count = 0;
	GETDAT_T send_data;

	static uint8_t timestamp;
	uint8_t i;
	float acc_vec[3];
	float acc_norm = 0.0;
	float acc_norm_LPF = 0.0;
	float gain_LPF = 0.01;
	float omega_vec[3];
	float omega_str_LPF_vec[3]; //強いローパスフィルタがかかった値を格納
	float diff_omega[3];        //強いローパスフィルタをかけたジャイロの値と現在のジャイロの値の差分

	int16_t  deg;
	uint16_t duty;
    uint16_t v_lipo;
    uint16_t v_bat;


	LPC_TMR32B1->IR=0x08; //clear interrupt flag

	//送信データ格納用変数のヘッダ
	send_data.num  = 43;
	send_data.d[0] = 0xff;
	send_data.d[1] = 0xff;
	send_data.d[2] = 0x52;
	send_data.d[3] = 0x54;
	send_data.d[4] = 0x34;
	send_data.d[5] = 0x57;
	send_data.d[6] = 0x00;
	send_data.d[7] = timestamp;

	if(timestamp == 0xff) timestamp = 0;
	else timestamp++;

	//9軸センサのデータの更新
	updateMPU9250Data();
	updateAK8963Data();

	//9軸センサの観測値を送信用配列に格納
	getDataFromMPU9250( (uint8_t *)(&send_data.d[8]) );
	getDataFromAK8963( (uint8_t *)(&send_data.d[22]) );

	//9軸センサの観測値を変数に格納
	getOmega(&omega_vec[0]);
	getAcc(&acc_vec[0]);
	getOmegaStrLPF(&omega_str_LPF_vec[0]);
	//静止状態の判定 omega < 0.1 deg/sec かつ |1-|a||<0.01
	//もし静止状態ならば角速度を0.0にしてしまう.
	acc_norm = tSqrt(acc_vec[0]*acc_vec[0]+acc_vec[1]*acc_vec[1]+acc_vec[2]*acc_vec[2]);
	acc_norm_LPF = gain_LPF * acc_norm_LPF + (1.0-gain_LPF)*acc_norm;

	flag_stop = 0;
	if( ABS(acc_norm_LPF - acc_norm )<0.02 )
	{
		for(i=0;i<3;i++)
		{

			diff_omega[i] =  omega_str_LPF_vec[i] - omega_vec[i] ;
			if(ABS(diff_omega[i]) < DEG2RAD(0.3))
			{
				omega_vec[i] = 0.0;
			}

			if(ABS(diff_omega[0]) < DEG2RAD(0.3) &&
			   ABS(diff_omega[1]) < DEG2RAD(0.3) &&
			   ABS(diff_omega[2]) < DEG2RAD(0.3)
			) flag_stop = 1;
		}
	}

	if(ABS(omega_vec[2]) > DEG2RAD(100)) flag_curve = 1;
	else flag_curve = 0;

	if(ABS( 1- ABS(acc_vec[2])) >0.1  ) flag_slope = 1;
	else flag_slope = 0;
	//角度の積分処理
	if(isStop() == 0)
	{
		degree += 0.01* omega_vec[2];
/*		while(degree < 0){
			degree += 2*PI;
		}
		while(degree > 2*PI)
		{
			degree -= 2*PI;
		}
		while(degree > PI)
		{
			degree -= 2*PI;
		}
*/
	}

	//角度情報を送信用配列に格納
	deg = (int16_t)(degree/ (2*PI) * 32767.0);
	send_data.d[28]  =  0x00ff & deg;
	send_data.d[29]  = (0xff00 & deg)>>8;
	//duty情報を送信用配列に格納
	duty = (int16_t)(motor_duty * 32767.0);
	send_data.d[30] =  0x00ff & duty;
	send_data.d[31] = (0xff00 & duty)>>8;
	//isStop(),isCurve(),isSlope()の情報を送信用配列に格納
	send_data.d[32] = isStop();
	send_data.d[33] = isCurve();
	send_data.d[34] = isSlope();
	//経過時間の情報を送信用配列に格納
	time = getElapsedTime();
	send_data.d[35] =  0x000000ff & time;
	send_data.d[36] = (0x0000ff00 & time)>>8;
	send_data.d[37] = (0x00ff0000 & time)>>16;
	send_data.d[38] = (0xff000000 & time)>>24;
    //Lipoの電圧を送信用配列に格納
	v_lipo = (uint16_t)(getLipoVoltage()  * 13107);  //0.0~5.0を0から65535に変換
	send_data.d[39] =  0x000000ff & v_lipo;
	send_data.d[40] = (0x0000ff00 & v_lipo)>>8;
	//Batteryの電圧を送信用配列に格納
	v_bat  = (uint16_t)(getMotorVoltage() * 13107);  //0.0~5.0を0から65535に変換
	send_data.d[41] =  0x000000ff & v_bat;
	send_data.d[42] = (0x0000ff00 & v_bat)>>8;


	//20Hzの周期でデータを送る (割り込み周期100Hz 5回に一回)
	if(count % 5 == 0 && flag_send_data_enable == 1)
	{
		//UART経由でデータを送信
		LPC_UART->IER = IER_THRE | IER_RLS;			    // Disable RBR
		UARTSend(&send_data.d[0], send_data.num);
		LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	// Re-enable RBR

		if(isUSBConfig() == 1) //USB接続確立
		{
			//USB経由でデータを送信
			VCOM_SendData(send_data);

		}
	}

	count++;

	//UART経由での受信データの処理
	execUARTCommand();

}

/**
 * UARTの受信バッファーにコマンドが入っていないかを調べ, コマンドを実行
 *
 * @param  void
 * @return void
 */
static void execUARTCommand()
{
	uint16_t i;

	if(UARTCount >9)
	{
		//UARTBufferにデータが10byte以上入っていてかつ'cmd'のパターンを見つけたら
		//コマンドを実行
		for(i=0;i< UARTCount-9; i++)
		{
				if(UARTBuffer[i]   == 'c' &&
				   UARTBuffer[i+1] == 'm' &&
				   UARTBuffer[i+2] == 'd' &&
				   UARTCount -i >9)
				{
					execCommand( &UARTBuffer[i] );
				}
		}
	}

	//UARTBufferのデータを10byte分だけ残して消去
	//10byte分残すのはバッファにコマンドの断片が混じっている可能性があるため

	if(UARTCount >9){
		for(i=0;i<10;i++)
		{
			UARTBuffer[9-i] = UARTBuffer[UARTCount-1-i];
		}
		UARTCount = 10;
	}
	//残した10byte分が意味のあるコマンドの場合は既に実行されているはずなのでバッファを空にする
	if(  UARTBuffer[0] == 'c' &&
		 UARTBuffer[1] == 'm' &&
		 UARTBuffer[2] == 'd' &&
		 UARTCount == 10)
		{
			UARTCount = 0;
		}

}

/**
 * 固定長10byteのコマンドの実行<br>
 * AI-CHIPのコマンドは固定長10byteで書式は以下 <br>
 *         0byte目 99   (c) <br>
 *         1byte目 109  (m) <br>
 *         2byte目 100　  (d) <br>
 *         3byte目 id　             <br>
 *         4byte目 -9byte目 データフィールド(使わないフィールドは0をセット) <br>
 * 定義されているコマンドは以下<br>
 *         <ul>
 *          <li>id 0 モータのdutyをセット </li>
 *          <li>id 1 左(赤)LEDの制御</li>
 *          <li>id 2 左(赤)LEDの制御</li>
 *          <li>id 3 右(緑)LEDの点滅制御</li>
 *          <li>id 4 左(赤)LEDの点滅制御</li>
 *          <li>id 5 車体角度のセット</li>
 *         </ul>
 *
 * @param  *cmd 10byteのコマンドが格納された配列
 * @return void
 */
static void execCommand(uint8_t *cmd)
{
	uint8_t id = cmd[3];

	int16_t duty_int;
	int16_t deg_int;
	switch(id)
	{
		case 0:
			duty_int = cmd[4] + (cmd[5]<<8);
			if (duty_int > 32767) duty_int -=  65536;
			setDutyMotor((float)(duty_int)/32767.0 );
			break;
		case 1: turnGreenLED(cmd[4]);
			break;
		case 2: turnRedLED(cmd[4]);
			break;
		case 3: flashGreenLED( cmd[4]+(cmd[5]<<8), cmd[6]+(cmd[7]<<8) );
			break;
		case 4: flashRedLED( cmd[4]+(cmd[5]<<8), cmd[6]+(cmd[7]<<8) );
			break;
		case 5:
			deg_int = cmd[4] + (cmd[5]<<8);
			if (deg_int > 32767) deg_int -=  65536;
			setDegree( (float)(deg_int)/32767.0 * PI );
			break;
	}//switch

}


/**
 * 現在の角度を返す
 *
 * @param  void
 * @return void
 */
float getDegree(void)
{
	return (degree*180/PI);
};


/**
 * 角度をセットする
 *
 * @param  deg [radian]
 * @return void
 */
void setDegree(float deg)
{
	degree = deg;
};


/**
 * 静止状態かどうかを返す
 *
 * @param  void
 * @return void
 */
uint8_t isStop(void)
{
	return flag_stop;
}


/**
 * カーブしているかどうかを返す
 *
 * @param  void
 * @return void
 */
uint8_t isCurve(void)
{
	return flag_curve;
}


/**
 * 斜面にいるかどうかを返す
 *
 * @param  void
 * @return void
 */
uint8_t isSlope(void)
{
	return flag_slope;
}


/**
 * デバッグ用関数
 *
 * @param  void
 * @return void
 */
void debugAICHIP()
{
	myPrintfUART("########debug AICHIP#######\n");
	myPrintfUART("motor duty : %f \n", getDutyMotor());
	myPrintfUART("degree     : %f \n", getDegree());
	myPrintfUART("is stop    : %d \n", isStop());
	myPrintfUART("is curve   : %d \n", isCurve());
	myPrintfUART("is slope   : %d \n", isSlope());
}


/******************************************************************************
**                            End Of File
******************************************************************************/
