

/**
 ******************************************************************************
 * File   : TMR/TimeBase/at32f4xx_it.c
 * Version: V1.2.2
 * Date   : 2020-07-01
 * Brief  : Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and peripherals
 *          interrupt service routine.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "../Head file/includes.h"
// #include "at32f4xx_it.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
 * @{
 */

/** @addtogroup TMR_TimeBase
 * @{
 */

// uint16_t capture = 0;
// extern __IO uint16_t CCR1_Val;
// extern __IO uint16_t CCR2_Val;
// extern __IO uint16_t CCR3_Val;
// extern __IO uint16_t CCR4_Val;

// void TMRingDelay_Decrement(void);
/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}
/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}
/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}
/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}
/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}
/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}
/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	//  TMRingDelay_Decrement();
	if (TRUE == Sound.CtlSts)
	{
		Sound.CtlSts = FALSE;
		//	  BEEP_OFF();				//关硬件
	}
	else
	{
		Sound.CtlSts = TRUE;
		//	  BEEP_ON()	;				//开硬件
	}
}

// void TMR2_GLOBAL_IRQHandler(void)
//{
//   if (TMR_GetINTStatus(TMR2, TMR_INT_CC1) != RESET)
//   {
//     TMR_ClearITPendingBit(TMR2, TMR_INT_CC1);

//    /* Pin PC.06 toggling with frequency = 73.24 Hz */
//    GPIO_WriteBit(GPIOC, GPIO_Pins_6, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_6)));
//    capture = TMR_GetCapture1(TMR2);
//    TMR_SetCompare1(TMR2, capture + CCR1_Val);
//  }
//  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC2) != RESET)
//  {
//    TMR_ClearITPendingBit(TMR2, TMR_INT_CC2);

//    /* Pin PC.07 toggling with frequency = 109.8 Hz */
//    GPIO_WriteBit(GPIOC, GPIO_Pins_7, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_7)));
//    capture = TMR_GetCapture2(TMR2);
//    TMR_SetCompare2(TMR2, capture + CCR2_Val);
//  }
//  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC3) != RESET)
//  {
//    TMR_ClearITPendingBit(TMR2, TMR_INT_CC3);

//    /* Pin PC.08 toggling with frequency = 219.7 Hz */
//    GPIO_WriteBit(GPIOC, GPIO_Pins_8, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_8)));
//    capture = TMR_GetCapture3(TMR2);
//    TMR_SetCompare3(TMR2, capture + CCR3_Val);
//  }
//  else
//  {
//    TMR_ClearITPendingBit(TMR2, TMR_INT_CC4);

//    /* Pin PC.09 toggling with frequency = 439.4 Hz */
//    GPIO_WriteBit(GPIOC, GPIO_Pins_9, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_9)));
//    capture = TMR_GetCapture4(TMR2);
//    TMR_SetCompare4(TMR2, capture + CCR4_Val);
//  }
//}
/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetIntStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearIntPendingBit(EXTI_Line0); ///< Clear the  EXTI line 0 pending bit
		iic_key.iic_key_type = iic_key_read();
	}
}

extern volatile enum STATE state;
enum HOT_STATE hotcycle_state;
// 定义计时器
uint32_t Dehumidification_steps_timer = 0; // 每个的计算步骤时间，单位ms
uint32_t hotcycle_timer = 0;			   // 烘干总执行时间，单位ms
uint32_t FAST_timer = 0;				   // 吸湿阶段运行时间，用来判断吸湿结束后是否进入到烘干阶段
INT8U Execute_onetime = 1;
INT8U Wait_Stauts_Flag = 0;
float Last_Work_Value = 0;

// 定义初始化开风扇时间
#define testApp
#ifdef testApp
#define INIT_FAN_TIME 1000 * 60 * 1
// 定义加热云母片和PTC的加热时间（单位：毫秒）
#define MICA_HEAT_TIME 1000 * 60 * 28
#define PTC_HEAT_TIME 1000 * 60 * 1
// #define PTC_HEAT_DELAY  1000 * 60 * 8

// 定义风扇转动的时间（单位：毫秒）
// #define FAN_ON_TIME    1000 * 60 * 3
// #define FAN_OFF_TIME  1000 * 60 * 7
// #define FAN_CYCLE_NUM 7

// 定义热循环的时间（单位：毫秒）
#define MICA_CYCLE_TIME 1000 * 12
#define MICA_DELAY_TIME 1000 * 108

// 定义加快吸湿部分的时间（单位：毫秒）
#define FAST_CYCLE_TIME 1000 * 60 * 60 * 7
#define FAST_RUN_TIME 1000 * 60 * 5
#define FAST_HEAT_DELAY 1000 * 60 * 60 * 2
// #define FAST_FAN_DELAY   1000 * 60 * 7
// #define FAST_FAN_ON_TIME 1000 * 60 * 7

// 定义除湿部分的时间（单位：毫秒）
#define MINUTE_TIME (1000 * 60)

#define DRY_PTC_HEAT_TIME (1000 * 60 * 5)
#define DRY_MICA_HEAT_TIME (1000 * 60 * 20) // 10//28  //28	//烘干时间
#define DRY_MICA_OFF_DELAY (1000 * 60 * 1)	// 15
#define DRY_PTC_OFF_DELAY (1000 * 60 * 17)	// 15//10  //8		//冷却时间

// 定义允许湿度上升的时间（单位：毫秒）
#define CHECK_RISE_TIME (1000 * 60 * 5) // 9//2	//12

#endif
#ifndef testApp
#define INIT_FAN_TIME 60 * 3
// 定义加热云母片和PTC的加热时间（单位：毫秒）
#define MICA_HEAT_TIME 1 60 * 28
#define PTC_HEAT_TIME 160 * 1
// #define PTC_HEAT_DELAY   60 * 8

// 定义风扇转动的时间（单位：毫秒）
// #define FAN_ON_TIME    1000 * 60 * 3
// #define FAN_OFF_TIME  1000 * 60 * 7
// #define FAN_CYCLE_NUM 7

// 定义热循环的时间（单位：毫秒）
#define MICA_CYCLE_TIME 12
#define MICA_DELAY_TIME 108

// 定义加快吸湿部分的时间（单位：毫秒）
#define FAST_CYCLE_TIME 60 * 60 * 7
#define FAST_HEAT_DELAY 60 * 60 * 2
// #define FAST_FAN_DELAY   1000 * 60 * 7
// #define FAST_FAN_ON_TIME 1000 * 60 * 7

// 定义除湿部分的时间（单位：毫秒）
#define DRY_PTC_HEAT_TIME 60 * 1
#define DRY_MICA_HEAT_TIME 60 * 28
#define DRY_MICA_OFF_DELAY 60 * 15
#define DRY_PTC_OFF_DELAY 60 * 8

// 定义允许湿度上升的时间（单位：毫秒）
#define CHECK_RISE_TIME 60 * 5

#endif

// 定义控制开关的状态
#define SWITCH_ON 1
#define SWITCH_OFF 0

INT8U dry_step = 0; // 烘干步骤
// INT8U dry_run_flg = 0;	// 运行标志
// float change_Humidity = 0 ;       	// 变化的湿度值
INT32U StepTim = 0;
INT32U StateTim = 0;
float AvgHumi = 0;
UINT16 AvgHumiTim = 0;

// 更正逻辑后涉及变量
int Last_Humidity; // 上次记录的湿度值，判断湿度是否下降
INT8U ExecuteOnce_flag = 0;
INT8U KeepFanOn = 0;
uint32_t Humidity_rise_timer = 0;
uint8_t OpenFromState;	  // 记录进入 STATE_OPEN 的来源状态（STATE_FAST / STATE_DRY1 / STATE_DRY2）
uint32_t OpenEnterDryTim; // 从 DRY1/DRY2 转入 OPEN 时的 StateTim 值


void HumidityDispProcess(void)
{
	//	const float k = 1.15f; // 1 + 0.25，即斜率

	//	if (sht30_par.SHT30_Humidity > 20.0f)
	//	{
	//		Cur_SHT30_Humidity = sht30_par.SHT30_Humidity;
	//	}													
	//	else
	//	{
	//		Cur_SHT30_Humidity = k * sht30_par.SHT30_Humidity - 3.0f;
	//	}
	//
	//	if(Cur_SHT30_Humidity < (para.str.SHT30_Humidity_SET - 0.5f)){
	//		float frac = sht30_par.SHT30_Humidity - (int)sht30_par.SHT30_Humidity;
	//		Cur_SHT30_Humidity = para.str.SHT30_Humidity_SET + frac;
	//	}
	
	Cur_SHT30_Humidity = sht30_par.SHT30_Humidity;
}


void ChkHumiditySts(void)
{
	StateTim++;
	HumidityDispProcess();
	switch (state)
	{
	//----------------------------------------------------------
	// STATE_INIT：上电初始化
	//   80分钟强制烘干后按湿度决定下一状态
	//----------------------------------------------------------
	case STATE_INIT:
		if (StateTim <= 30 * 60 * 1000)
		{
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_ON);
			mica_switch(SWITCH_ON);
		}
		else if (StateTim <= 50 * 60 * 1000)
		{
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_OFF);
			
			mica_switch(SWITCH_OFF);
		}
		else if (StateTim <= 70 * 60 * 1000)
		{
			fan_switch(SWITCH_ON);
			ptc_switch(SWITCH_OFF);
			mica_switch(SWITCH_OFF);
		}
		else
		{
			if (sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET)
			{
				// 湿度大于设定值，先快速除湿
				ExecuteOnce_flag = 1;
				state = STATE_FAST;
			}
			else
			{
				// 湿度已达标，进入保湿状态
				ExecuteOnce_flag = 1;
				state = STATE_KEEP;
			}
			StateTim = 0;
		}
		break;

	//----------------------------------------------------------
	// STATE_FAST：快速除湿（仅风扇）
	//   时刻监测湿度，下降更新基准值
	//   湿度较基准值上升 > 3% → OPEN
	//   湿度降到设定值-1以下 → KEEP
	//   7min 未下降且上升 ≤ 3% → DRY2；上升 > 3% → OPEN
	//----------------------------------------------------------
	case STATE_FAST:	
		fan_switch(SWITCH_ON);
		ptc_switch(SWITCH_OFF);
		mica_switch(SWITCH_OFF);

		if (ExecuteOnce_flag == 1)
		{
			Last_Humidity = (int)sht30_par.SHT30_Humidity;
			ExecuteOnce_flag = 0;
			Humidity_rise_timer = 0;
		}

		// 时刻检测：湿度下降更新基准值并重置计时
		if ((int)sht30_par.SHT30_Humidity < Last_Humidity)
		{
			Humidity_rise_timer = 0;
			Last_Humidity = (int)sht30_par.SHT30_Humidity;
		}
		else
		{
			Humidity_rise_timer++;
		}

		// 湿度上升超过基准值3%，转向 OPEN
		if ((int)sht30_par.SHT30_Humidity > Last_Humidity + 3)
		{
			OpenFromState = STATE_FAST;
			Humidity_rise_timer = 0;
			state = STATE_OPEN;
			StateTim = 0;
			break;
		}

		// 湿度降到设定值-1以下，进入保湿状态
		if (sht30_par.SHT30_Humidity < para.str.SHT30_Humidity_SET )
		{
			ExecuteOnce_flag = 1;
			Humidity_rise_timer = 0;
			StateTim = 0;
			state = STATE_KEEP;
			break;
		}

		// 7 分钟湿度未下降
		if (Humidity_rise_timer >= 1000 * 60 * 7)
		{
			Humidity_rise_timer = 0;

			if ((int)sht30_par.SHT30_Humidity <= Last_Humidity + 3)
			{
				// 上升不超过3%，转强除湿
				state = STATE_DRY;
				ExecuteOnce_flag = 1;
			}
			else
			{
				// 湿度上升过多，转向 OPEN
				OpenFromState = STATE_FAST;
				state = STATE_OPEN;
			}
			StateTim = 0;
		}
		break;

	//----------------------------------------------------------
	// STATE_KEEP：保湿状态（湿度 ≤ 设定值 时进入）
	//   等待湿度回升到设定值 → 开风扇5min
	//   5min 内降到设定值以下 → 停止风扇回到等待，重复此过程
	//   5min 后仍 ≥ 设定值 → 进入 DRY2 强除湿
	//   湿度较最小湿度值上升 > 4% → OPEN（与 FAST 进入 OPEN 处理相同）
	//----------------------------------------------------------
	case STATE_KEEP:
		if (ExecuteOnce_flag == 1)
		{
			Last_Humidity = (int)sht30_par.SHT30_Humidity; // 记录最小湿度基准
			ExecuteOnce_flag = 0;
			KeepFanOn = 0;
			StateTim = 0;
		}

		// 维持最小湿度值
		if ((int)sht30_par.SHT30_Humidity < Last_Humidity)
		{
			Last_Humidity = (int)sht30_par.SHT30_Humidity;
		}

		// 湿度高于最小湿度值4%，转 OPEN（开门等异常）
		if ((int)sht30_par.SHT30_Humidity > Last_Humidity + 4)
		{
			OpenFromState = STATE_KEEP;
			state = STATE_OPEN;
			StateTim = 0;
			break;
		}

		if (KeepFanOn == 0)
		{
			// 风扇阶段：开风扇5min
			fan_switch(SWITCH_ON);
			ptc_switch(SWITCH_OFF);
			mica_switch(SWITCH_OFF);

			if(StateTim >= 5 * 60 * 1000){
				//5min到再判断
				if(sht30_par.SHT30_Humidity < para.str.SHT30_Humidity_SET)
				{
					//已降到设定值以下，回到等到阶段重复
					KeepFanOn = 1;
					StateTim = 0;
				}
				else
				{
					//仍大于设定值，进入DRY
					ExecuteOnce_flag = 1;
					state = STATE_DRY;
					StateTim = 0;
				}
			}
		}
		else
		{
			// 等待阶段：全部关闭
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_OFF);
			mica_switch(SWITCH_OFF);

			if (sht30_par.SHT30_Humidity >= para.str.SHT30_Humidity_SET)
			{
				// 湿度回升到设定值，开风扇5min
				KeepFanOn = 0;
				StateTim = 0;
			}
		}
		break;

	//----------------------------------------------------------
	// STATE_DRY2：强除湿
	//   监测湿度，较基准值上升 > 12% → OPEN
	//---------------------------------------------------------- 
	case STATE_DRY:
		if (ExecuteOnce_flag == 1)
		{
			Last_Humidity = (int)sht30_par.SHT30_Humidity;
			ExecuteOnce_flag = 0;
		}

		// 监测湿度变化
		if ((int)sht30_par.SHT30_Humidity < Last_Humidity)
		{
			Last_Humidity = (int)sht30_par.SHT30_Humidity;
		}
		else if ((int)sht30_par.SHT30_Humidity > Last_Humidity + 12)
		{
			// 湿度上升超过 12%，转向 OPEN
			OpenFromState = STATE_DRY;
			OpenEnterDryTim = StateTim;
			state = STATE_OPEN;
			StateTim = 0;
			break;
		}

		if (StateTim < 22 * 60 * 1000)
		{
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_ON);
			mica_switch(SWITCH_ON);
		}
//		else if (StateTim < 23 * 60 * 1000)
//		{
//			fan_switch(SWITCH_OFF);
//			ptc_switch(SWITCH_ON);
//			mica_switch(SWITCH_OFF);
//		}
		else if	(StateTim < 27 * 60 * 1000){
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_ON);
			mica_switch(SWITCH_OFF);
		}
		else if (StateTim < 43 * 60 * 1000)
		{
			fan_switch(SWITCH_OFF);
			ptc_switch(SWITCH_OFF);
			mica_switch(SWITCH_OFF);
		}
		else
		{
			ExecuteOnce_flag = 1;
			state = STATE_FAST;
			StateTim = 0; 
		}
		break;

	//----------------------------------------------------------
	// STATE_OPEN：湿度异常（开门）处理													
	//   根据来源状态采用不同的处理流程
	//----------------------------------------------------------
	case STATE_OPEN:
	{
		uint32_t all_off_time = 0;
		uint32_t fan_time = 20 * 60 * 1000;
		uint32_t total_time;
		uint32_t threshold;

		if (OpenFromState == STATE_FAST || OpenFromState == STATE_KEEP)
		{
			// 从 FAST / KEEP 进入：仅风扇 20min
			fan_switch(SWITCH_ON);
			ptc_switch(SWITCH_OFF);
			mica_switch(SWITCH_OFF);
			total_time = fan_time;
		}
		else // STATE_DRY
		{
			threshold = 23 * 60 * 1000;

			if (OpenEnterDryTim < threshold)
			{
				all_off_time = 10 * 60 * 1000;
			}
			else
			{
				// 超过 23min 的部分，从 10min 全关时间中扣除			
				uint32_t excess = OpenEnterDryTim - threshold;
				if (excess < 10 * 60 * 1000)
				{
					all_off_time = 10 * 60 * 1000 - excess;
				}
				else
				{
					all_off_time = 0;
				}
			}
			total_time = all_off_time + fan_time;

			if (StateTim < all_off_time)
			{
				fan_switch(SWITCH_OFF);
				ptc_switch(SWITCH_OFF);
				mica_switch(SWITCH_OFF);
			}
			else
			{
				fan_switch(SWITCH_ON);
				ptc_switch(SWITCH_OFF);
				mica_switch(SWITCH_OFF);
			}
		}

		// OPEN 阶段结束，按湿度决定下一状态
		if (StateTim >= total_time)
		{
			if (sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET - 1)
			{
				// 湿度仍不达标，继续快速除湿
				ExecuteOnce_flag = 1;
				state = STATE_FAST;
			}
			else
			{
				// 湿度已达标，进入保湿状态
				ExecuteOnce_flag = 1;
				state = STATE_KEEP;
			}
			StateTim = 0;
		}
	}
	break;

	default:
		state = STATE_FAST;
		ExecuteOnce_flag = 1;
		StateTim = 0;
		break;
	}
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
// 1ms 定时
*******************************************************************************/

void TMR3_GLOBAL_IRQHandler(void)
{
	if (TMR_GetINTStatus(TMR3, TMR_INT_Overflow) != RESET)
	{

		TMR_ClearITPendingBit(TMR3, TMR_INT_Overflow); // 清除定时器3中断
		os_time.Systime_Count++;					   // 32位的总计时

		os_time.Systime_s++;
		if (os_time.Systime_s >= 1000)
		{ // 1S读取1次
			if (lcd_background.TIMER_SHT_ENABLED == 1)
			//				SHT3X_ReadMeasurementBuffer(&sht30_par.SHT30_Temprature,&sht30_par.SHT30_Humidity);
			{
				SHT3X_ReadMeasurementBuffer(&sht30_par);
			}
			os_time.Systime_s = 0;
			os_time.Systime_Min++; //
		}

		if (os_time.Systime_Min >= 60)
		{
			os_time.Systime_Min = 0;
			os_time.Systime_Min_flag = 1; // 一分钟标志位
		}
		// 串口判断
		if (uart.rIndex != 0) // 串口缓冲区有数据
		{
			if (uart.rTimer-- == 0)
			{
				uart.rFlag = ON;  // 说明接收完成，未处理
				Cmd_Process();	  // 处理主机下发命令	// 和显示有冲突
				uart.rFlag = OFF; // 已处理
				uart.rIndex = 0;  // 接收个数清0
			}
		}

		if (Key.SysCnt != 0) // 扫描功能按键时间未到，提前处理OK按键
		{
			Key.SysCnt--;
			if (RUN_REFRESH != LCD.f_LcdEnable) // 扫描功能按键时间到
			{
				if (Key.SysCnt % TIMER_KEY5ms == 0) // 扫描OK按键时间到
				{
					Key.f_Enable = TRUE;
					Key.f_KeyOK_En = TRUE;
				}
			}
		}
		else
		{
			if (RUN_REFRESH != LCD.f_LcdEnable) // 扫描功能按键时间到
			{
				Key.SysCnt = TIMER_COUNT_32ms; // TIMER_KEY60ms ;
				Key.f_Enable = TRUE;
				Key.f_KeyFunc_En = TRUE;
			}
		}
		Key_Scan();

		if (led.time_refresh != 0) // 16位Led 闪烁时间，自减1
		{
			led.time_refresh--;
		}
		if (Disp.TimeRefresh != 0) // 16位定时更新显示缓存计数器，自减1,焦点位闪烁计数器
		{
			Disp.TimeRefresh--;
		}
		if (DispRoll.RollDelayCnt != 0) // 16位数码管 滚屏时间，自减1
		{
			DispRoll.RollDelayCnt--;
		}

		if (os_time.Base_100ms-- == 0) // 100ms 计时
		{
			os_time.Base_100ms = TIMER_BASE_100MS; // 单位为100ms
			LED_G_OFF();
			LED_R_OFF();
			LED_O_OFF();
			LED_B_OFF();
			if (Disp.TatolTimeRefresh8 != 0)
			{
				Disp.TatolTimeRefresh8--; // 8位超时递减计算器，自减1	//配置状态下 和 作业时修改数值状态下 共用做为超时退出
			}

			if (Disp.TatolTimeRefresh16 != 0)
			{
				Disp.TatolTimeRefresh16--; // 16位超时递减计算器，自减1	// 进度等待时间
			}
		}

		if (sht30_par.SHT30_Humidity >= para.str.SHT30_Humidity_SET + 3)
		{
			LED_B_ON();
		}
		else
		{
			LED_B_OFF();
		}

		ChkHumiditySts(); // 除湿的状态检测及切换

		//		Almdo();							            //调蜂鸣器,无时0.4us，有时最大1.4us
	}
}

/*******************************************************************************
 * Function Name  : USART1_IRQHandler
 * Description    : This function handles USART1 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 * RS485 接收中断函数
 *******************************************************************************/
#define RCV_OVER_TIMER 4 // 串口接收超时时间 4ms

void USART1_IRQHandler(void)
{
	INT8U tmp;

	if (USART_GetITStatus(USART1, USART_INT_RDNE) != RESET) // 接收中断
	{
		tmp = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_INT_RDNE);
		if ((uart.rIndex < UART_REVBUF_LEN) && (uart.rFlag != ON)) // 接收数据长度小于最大接收数据量，且未接收已处理完毕
		{
			uart.rBuffer[uart.rIndex++] = tmp;
			uart.rTimer = RCV_OVER_TIMER;
		}
	}
	else
	{
		uart.ErrCnt++;
	}
	if (USART_GetITStatus(USART1, USART_INT_TDE) != RESET) // 发送中断
	{
		USART_ClearFlag(USART1, USART_INT_TDE);
	}
}

/******************************************************************************/
/*                 AT32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/
/**
 * @}
 */

/**
 * @}
 */
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
