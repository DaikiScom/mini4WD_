/* ------------------------------------------------------------ *
File io.c

IOポートの設定
* ------------------------------------------------------------ */
#include "io.h"
#include "LPC13xx.h"
#include "core_cm3.h"
#include "type.h"

/* ------------------------------------------------------------ *
  Input  void
  Output void

  IOポートの設定.
  SW1,SW2,LED1,LED2に繋がっているIOピンを設定
* ------------------------------------------------------------ */
void initIO (void)
{

	//SW1 (Left)
	LPC_IOCON->PIO2_0 = 0x0000;   //IOConfig PIO2_0
	LPC_GPIO2->DIR &= 0xfffe;     //Data Direction(1 output , 0 input)   [0bit] SW1

	//SW2(Right)
	LPC_IOCON->PIO0_1 = 0x0000;   //IOConfig PIO0_1
	LPC_GPIO0->DIR &= 0xfffd;     //Data Direction(1 output , 0 input)   [1bit] SW2

	//LED1 (GR)
	LPC_IOCON->PIO1_8 = 0x0000;   //IOConfig PIO1_8
	LPC_GPIO1->DIR |= 0x0100;     //Data Direction(1 output , 0 input)   [8bit] LED1

	//LED2(RD)
	LPC_IOCON->SWDIO_PIO1_3 = 0x00C1; //IOConfig PIO1_3
	LPC_GPIO1->DIR |= 0x0008;     //Data Direction(1 output , 0 input)   [3bit] LED2

	//モータードライバ NSLEEP
	LPC_IOCON->PIO1_11 = 0x00C8;//IOConfig NSLEEP
	LPC_GPIO1->DIR |= 0x0800;     //Data Direction(1 output , 0 input)   [3bit] NSLEEP

	//モーターの正転/負転切り替え MOT_CW_CCW
	LPC_IOCON->PIO0_7 = 0x0000;
	LPC_GPIO0->DIR |=0x0080;

};
/******************************************************************************
**                            End Of File
******************************************************************************/
