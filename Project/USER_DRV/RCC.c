/*********************************************************************/
//所有系统初始化函数的定义文件
/*********************************************************************/
//#include "../Head file/Glob.h"
#include "../Head file/includes.h" 

#define    SETCPUCLK_72M
/*********************************************************************
函数名：RCC_Configuration	
时间：	2011.01.08
**********************************************************************/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;
	// RCC system reset(for debug purpose) 
	RCC_DeInit();	
	// Enable HSE 	  // 使能外部晶振
	RCC_HSEConfig(RCC_HSE_ON);
	// Wait till HSE is ready 	 // 等待外部晶振稳定
	HSEStartUpStatus = RCC_WaitForHSEStartUp();	
	// 如果外部晶振启动成功，则进行下一步操作
	if(HSEStartUpStatus == SUCCESS)
	{
//		// Enable Prefetch Buffer 	   // 开启FLASH预取指功能
//	    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//	    // Flash 0 wait state 		   // FLASH时序控制，推荐0~24M 取0，24~48M 取1，48~72M 取2
//	    FLASH_SetLatency(FLASH_Latency_0);

		// HCLK = SYSCLK/2,AHB总线频率 // 设置HCLK(AHB时钟)
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//		RCC_HCLKConfig(RCC_SYSCLK_Div2); 

		// PCLK1 = HCLK/2  	(APB1)
		RCC_PCLK1Config(RCC_HCLK_Div2);	

		// PCLK2 = HCLK 	(APB2)
		RCC_PCLK2Config(RCC_HCLK_Div1); 	//串口波特率的确定	

		// ADCCLK = PCLK2/2 36M 
		RCC_ADCCLKConfig(RCC_PCLK2_Div2); 	

		// RTCCLK = HSE/128 
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
//
//		// MCOCLK = HSE 
//		RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);
//
//		// USBCLK = 48MHZ 
//		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

		// Flash 2 wait state 	    // FLASH时序控制，推荐0~24M 取0，24~48M 取1，48~72M 取2
		FLASH_SetLatency(FLASH_Latency_2);
		// Enable Prefetch Buffer 	// 开启FLASH预取指功能
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 
		#ifdef    SETCPUCLK_72M
		    // PLLCLK = 8MHz * 9 = 72 MHz
		    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		#else
		    // PLLCLK = 8MHz * 7 = 56 MHz
		    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
		#endif
		// Enable PLL 	 // 使能PLL
		RCC_PLLCmd(ENABLE);
		// Wait till PLL is ready 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		// Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// Wait till PLL is used as system clock source 
		while(RCC_GetSYSCLKSource() != 0x08);
	}
	// Enable peripheral clocks/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);		  // 使能FSMC时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
	// Enable ADC1 and GPIOC clock 

	// 使能GPIO时钟
	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_GPIOB 
							| RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD
							| RCC_APB2Periph_GPIOE
							| RCC_APB2Periph_GPIOF
							| RCC_APB2Periph_GPIOG
							| RCC_APB2Periph_AFIO
							//| RCC_APB2Periph_ADC1
							//| RCC_APB2Periph_ADC2
							| RCC_APB2Periph_TIM1
							//| RCC_APB2Periph_SPI1
							//| RCC_APB2Periph_TIM8
							| RCC_APB2Periph_USART1,ENABLE);

	RCC_APB1PeriphClockCmd(   RCC_APB1Periph_PWR            
							| RCC_APB1Periph_BKP
							| RCC_APB1Periph_TIM2
							| RCC_APB1Periph_TIM3
							| RCC_APB1Periph_TIM4
//							| RCC_APB1Periph_I2C1
							| RCC_APB1Periph_USART2
							| RCC_APB1Periph_USART3,ENABLE);
}

