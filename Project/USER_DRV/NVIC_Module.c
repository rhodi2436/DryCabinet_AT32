/************************************************************************
** 功能描述：中断配置
*************************************************************************
** 模块名：  中断模块 
** 作  者：  xiaoweixian
** 日  期：  2011.2.11 
** 修  改：
** 日  期：
** 版  本：  1.0 
*************************************************************************
** 备 注:
************************************************************************/
//#include "stm32f10x_lib.h"
//#include "../Head file/Typedef.h"
//#include "../Head file/Glob.h"
#include "../Head file/includes.h" 

void  USART1_NVICConfig(void);
void  USART2_NVICConfig(void);
void  TIM2_NVICConfig(void);
void  TIM3_NVICConfig(void);
void  TIM4_NVICConfig(void);
void  RTC_NVICConfig(void);
void  DMA_NVICConfig(void);
void  DMA5_NVICConfig(void);
/*******************************************************************************
* 函数名  : NVIC_Configuration
* 描述    : 中断向量配置
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		//1位抢占优先级，3位子级
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//2位抢占优先级，2位子级
	
	TIM2_NVICConfig();									// 定时时钟
//	TIM3_NVICConfig();
//	TIM4_NVICConfig();
//    USART2_NVICConfig();
	USART1_NVICConfig();
//    RTC_NVICConfig();
//	DMA_NVICConfig();
//	DMA5_NVICConfig();

//	NVIC_SetPriority(SysTick_IRQn, 4);					 //设置抢占等级为1
//    SysTick_ITConfig(ENABLE);							 //使能中断
}

/*******************************************************************************
* 函数名  : RTC_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void RTC_NVICConfig(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    /* Enable the RTC Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : UART1_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void USART1_NVICConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;      // 通道设置为串口1中断（故后面应选择在“void USART1_IRQHandler(void)”开中断）  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 // 中断占先等级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 // 中断响应优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 打开中断 
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : UART1_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void USART2_NVICConfig(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;        
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 //	抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //	子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //	使能
//	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : TIM2_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void TIM2_NVICConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 //	抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //	子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //	使能
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : TIM3_NVICConfig
* 描述    : TIMER3中断配置
*******************************************************************************/
void TIM3_NVICConfig(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;        
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 //	抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //	子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //	使能
//	NVIC_Init(&NVIC_InitStructure);
} 

/*******************************************************************************
* 函数名  : TIM3_NVICConfig
* 描述    : TIMER3中断配置
*******************************************************************************/
void TIM4_NVICConfig(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel;        
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 //	抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //	子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //	使能
//	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : DMA_NVICConfig
* 描述    : DMA中断配置
*******************************************************************************/
void DMA_NVICConfig(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	  
//	/* Enable DMA channel6 IRQ Channel */
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQChannel;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //	抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				 //	子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					 //	使能
//	NVIC_Init(&NVIC_InitStructure);
//	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
}

/*******************************************************************************
* 函数名  : DMA_NVICConfig
* 描述    : DMA中断配置
*******************************************************************************/
void DMA5_NVICConfig(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	  
//	/* Enable DMA channel6 IRQ Channel */
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQChannel;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//	抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//	子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//	使能
//	NVIC_Init(&NVIC_InitStructure);
//	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
}






