/************************************************************************
** 功能描述：定时器配置，使能
*************************************************************************
** 模块名： 定时器模块   
** 作  者： xiaoweixian
** 日  期： 2011.1.19  
** 修  改：
** 日  期：
** 版  本： 1.0  
*************************************************************************
** 备 注:
************************************************************************/
//#include "stm32f10x_lib.h"
//#include "../Head file/Typedef.h"
//#include "../Head file/Glob.h"
#include "../Head file/includes.h" 

/*********************************************************************
函数名: TIM2_Configuration 
描述：  定时器2配置
时间：	2011.01.08
**********************************************************************/
void TIM2_Configuration(void)
{
	TMR_TimeBaseInitType  TIM_TimeBaseStructure;
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	 //clk = f/71+1	 //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_Period = 2000;	 //2ms
	TIM_TimeBaseStructure.TIM_Period = 1000;	 //1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	TIM_Cmd(TIM2,DISABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);   //清除中断标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	 //使能中断
}

/*********************************************************************
函数名: TIM3_Configuration 
描述：  定时器2配置
时间：	2011.01.08
**********************************************************************/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = (72*10-1);	 //clk = f/71+1	 //	10us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 65000;	 				
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3,DISABLE);
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);   //清除中断标志位
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	 //使能中断
}

/*********************************************************************
函数名: TIM1_Configuration 
描述：  定时器1配置
时间：	2011.01.08
**********************************************************************/
void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	 //clk = f/71+1	 //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 100;	     //20us
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_Cmd(TIM4,DISABLE);
//	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);   //清除中断标志位
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);	 //使能中断
}

/*********************************************************************
函数名: Systick_Configuration 
描述：  嘀嗒时钟配置
**********************************************************************/
void Systick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//时钟为72/8 = 9MHz
	  /* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
//    SysTick_SetReload(9000);								//校正值为9000时为1MS时基
	SysTick_SetReload(1667);								//校正值为186us,2.7K蜂鸣器频率
//	SysTick_SetReload(1125);								//4K蜂鸣器频率
//	SysTick_SetReload(1800);								//200us，2.5K蜂鸣器频率
    /* Enable SysTick interrupt */
//    SysTick_ITConfig(ENABLE);							    //使能中断
	SysTick_CounterCmd(SysTick_Counter_Enable);
}

/*********************************************************************
函数名: Timer_Configuration 
描述：  时钟配置
**********************************************************************/
void Timer_Configuration(void)
{
	TIM2_Configuration();			// 1ms系统时钟
//	TIM3_Configuration();
//	TIM4_Configuration();
	Systick_Configuration();		// 186us蜂鸣器频率
}



