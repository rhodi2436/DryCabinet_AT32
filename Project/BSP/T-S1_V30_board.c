/**
  **************************************************************************
  * File   : at32_board.c
  * Version: V1.2.2
  * Date   : 2020-07-01
  * Brief  : 1. Set of firmware functions to manage Leds, push-button and COM ports.
  *          2. initialize Delay Function and USB
  **************************************************************************
  */

#include "../Head file/includes.h" 



#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/*delay macros*/

  



/**
  * @brief  initialize UART1   
  * @param  bound: UART BaudRate
  * @retval None
  */
//void UART_Print_Init(uint32_t bound)
//{
//  GPIO_InitType GPIO_InitStructure;
//  USART_InitType USART_InitStructure;

//  /*Enable the UART Clock*/
//  RCC_APB2PeriphClockCmd(AT32_PRINT_UARTTX_GPIO_RCC | AT32_PRINT_UARTRX_GPIO_RCC, ENABLE);	
//  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);

//  /* Configure the UART1 TX pin */
//  GPIO_StructInit(&GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pins = AT32_PRINT_UARTTX_PIN; 
//  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
//  GPIO_Init(AT32_PRINT_UARTTX_GPIO, &GPIO_InitStructure);

//  /* Configure the UART1 RX pin */
//  GPIO_InitStructure.GPIO_Pins = AT32_PRINT_UARTRX_PIN;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(AT32_PRINT_UARTRX_GPIO, &GPIO_InitStructure);

//  /*Configure UART param*/
//  USART_StructInit(&USART_InitStructure);
//  USART_InitStructure.USART_BaudRate = bound;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

//  USART_Init(AT32_PRINT_UART, &USART_InitStructure); 
//  USART_INTConfig(AT32_PRINT_UART, USART_INT_RDNE, ENABLE);
//  USART_Cmd(AT32_PRINT_UART, ENABLE);   
//}

/************************************************************************
** 功能描述：IO配置	
**功能  ：RS485 引脚初始化
************************************************************************/
void USART_IOConfig(void)
{	
	GPIO_InitType GPIO_InitStructure;
	
	//GPIO结构的成员设置如下：  
	/*--------------485控制端初始化----------------*/  
	GPIO_InitStructure.GPIO_Pins = pCTRL485 ;  
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz; //50M时钟速度  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT_PP; //推挽输出  GPIO_Mode_Out_OD; 	//必须为开漏模式 
	GPIO_Init(CTRL485_GPIO, &GPIO_InitStructure);  

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pins  = pTXD485;				   //485_TX 
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			   //复用推挽输出
	GPIO_Init(TXD485_GPIO, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pins = pRXD485;				   //485_RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   //浮空输入，须加外部上拉
	GPIO_Init(RXD485_GPIO, &GPIO_InitStructure);
    
	// 初始化为接收模式
	RS485CTRL_RECV_EN();	
}

/************************************************************************
** 功能描述：USART配置		  
************************************************************************/
void USART_Configuration(void)
{
	USART_InitType USART_InitStructure;
//	USART_ClockInitType USART_ClockInitStructure; 
	
	/*Enable the UART Clock*/
	RCC_APB2PeriphClockCmd(AT32_PRINT_UARTTX_GPIO_RCC | AT32_PRINT_UARTRX_GPIO_RCC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);	
	
	  //USART_ClockInitTypeDef  USART_ClockInitStructure;
	/* RS485_UART configuration ------------------------------------------------------*/
	/* RS485_UART configured as follow:
	    - BaudRate = 38400 baud  
	    - Word Length = 9 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	    - USART Clock disabled
	    - USART CPOL: Clock is active low
	    - USART CPHA: Data is captured on the middle 
	    - USART LastBit: The clock pulse of the last data bit is not output to 
	                     the SCLK pin
	*/
//	USART_ClockStructInit(&USART_ClockInitStructure);
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
//	USART_ClockInit(RS485_UART, &USART_ClockInitStructure);
	
	
	/* Configure the RS485_UART synchronous paramters */
//	USART_StructInit(&USART_InitStructure);
//	USART_InitStructure.USART_BaudRate = 38400;						//设置USART的传输速率
//	USART_InitStructure.USART_WordLength = USART_WordLength_9b;		//在一帧中传输或接受9位数据位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//定义在帧的结尾传输一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;			//奇偶失能
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		  //指定硬件流控制模式RTS和CTS使能 
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//指定使能或失能发送和接受模式 Tx发送使能和Rx接收使能 
//	/* Configure USART1 basic and asynchronous paramters */
//	USART_Init(RS485_UART, &USART_InitStructure);

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 38400;						//设置USART的传输速率
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;		//在一帧中传输或接受9位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//定义在帧的结尾传输一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;			//奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		  //指定硬件流控制模式RTS和CTS使能 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//指定使能或失能发送和接受模式 Tx发送使能和Rx接收使能 
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(RS485_UART, &USART_InitStructure);
	
    /* Enable RS485_UART Receive interrupts */
    USART_INTConfig(RS485_UART, USART_INT_RDNE, ENABLE);			//接收中断使能
	USART_INTConfig(RS485_UART, USART_INT_TDE, DISABLE);			//发送中断禁止

    /* Enable RS485_UART */
  	USART_Cmd(RS485_UART, ENABLE);									//启动串口 使能USART1外设 

}


/************************************************************************
** 功能描述：初始化		  
************************************************************************/
void UART_Init(void)
{
	USART_IOConfig();					// IO口配置
	USART_Configuration();				// 寄存器配置
}

/***********************************************************************
**函数名:  GPIO_Configuration
**功能  ： TFT引脚初始化
************************************************************************/
void TFT_GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;
	
	/*防撕裂控制脚配置为推换输出*/
	/*背光控制脚配置为推换输出*/
	/*TFT背光*/
	GPIO_InitStructure.GPIO_Pins = TFT_BL_PINSET;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP; 
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(TFT_BL_GPIO, &GPIO_InitStructure);
		
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/*!< LCD Data lines configuration XMC D4-D12*/
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7 | GPIO_Pins_8 | GPIO_Pins_9  |GPIO_Pins_10 | GPIO_Pins_11 | GPIO_Pins_12 | GPIO_Pins_13 | GPIO_Pins_14| GPIO_Pins_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	/* XMC D2-D3,D13-D15,D0-D1*/
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0 | GPIO_Pins_1 | GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10 | GPIO_Pins_14| GPIO_Pins_15;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 


	/*!< LCD Reset lines configuration */
	/*TFT复位*/
	GPIO_InitStructure.GPIO_Pins = TFT_RSET_PINSET;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(TFT_RSET_GPIO, &GPIO_InitStructure);

	/*!< LCD RS/CS/WR/RD lines configuration */
	// RS， XMC_A16（only）
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	// CS，XMC_NE1
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	// WR，XMC_NWE
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	// RD，XMC_NOE
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 	

	// 关闭背光
	TFT1_BL_OFF();
}

/************************************************************************
** 功能描述：IO配置	
**功能  ： BELL引脚初始化
************************************************************************/
void BeepIO_Config(void)
{
//	GPIO_InitType GPIO_InitStructure;
//		

//	GPIO_InitStructure.GPIO_Pins = BEEP_PINSET;
//	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
//	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	
	// 关闭蜂鸣器
//	BEEP_OFF();									// 关硬件
}

/************************************************************************
** 功能描述：IO配置	
**功能  ： KEY引脚初始化
************************************************************************/
void   KeyIO_Config(void)
{
//	GPIO_InitType GPIO_InitStructure;
		

//	GPIO_InitStructure.GPIO_Pins = KEYPINSET;
////	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;//GPIO_Mode_IN_PD; 	//设置为输入上拉模式
//	GPIO_Init(KEY_GPIO, &GPIO_InitStructure);

}

/************************************************************************
** 功能描述：IO配置	
**功能  ： LED引脚初始化
************************************************************************/
void   LEDIO_Config(void)
{
	GPIO_InitType GPIO_InitStructure;
		
	
	// RED
	GPIO_InitStructure.GPIO_Pins = LED_RED_PINSET;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP; //必须为开漏模式
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);

	// GREEN
	GPIO_InitStructure.GPIO_Pins = LED_GREEN_PINSET;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD; //必须为开漏模式	
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);

	// BLUE
	GPIO_InitStructure.GPIO_Pins = LED_BLUE_PINSET;
	GPIO_Init(LED_BLUE_PORT, &GPIO_InitStructure);	

	// WRN
	GPIO_InitStructure.GPIO_Pins = LED_WRN_PINSET;
	GPIO_Init(LED_WRN_PORT, &GPIO_InitStructure);
	// DO-塔灯
	GPIO_InitStructure.GPIO_Pins = LED_ORANGE_PINSET;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD; //必须为开漏模式
	GPIO_Init(LED_ORANGE_PORT, &GPIO_InitStructure);


	
	// 关闭所有LED
	LED_R_OFF();
	LED_G_OFF();
	LED_B_OFF();
	LED_WRN_OFF();
	LED_O_OFF();		
}

/*******************************************************************************
* 函数名  : SPIM_init
* 描述    : Spim IO脚初始化, 寄存器初始化

初始化及解锁操作
初始化及解锁的步骤，在AT32的BSP中已经封装成库函数，用户可以直接调用
1) 使能GPIOA、 GPIOB和AFIO RCC时钟。
2) 配置对应pin PA8、 PA11、 PA12、 PB1、 PB6、 PB7为推挽复用输出模式。
3) AFIO_MAP2寄存器中使能SPIM接口。
4) 设置FLASH_SELECT寄存器选择配置SPIM Flash的类型。
5) 使用KEY解锁SPIM：写FLASH_FCKEY3寄存器2次，按顺序分别为0x45670123和
0xCDEF89AB。
6) 检查SPIM是否解锁成功， 读取FLASH_CTRL3寄存器的LOCK位，如果被清除为0， 则可以开始
操作SPIM。
*******************************************************************************/
void SPIM_init(void)
{
	GPIO_InitType  GPIO_InitStructure; 
	 
	/* Enable ext.flash GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO|RCC_APB2PERIPH_GPIOA|RCC_APB2PERIPH_GPIOB, ENABLE);	
		
	/* Configure ext.flash pin */
	GPIO_StructInit(&GPIO_InitStructure);
	// PA8 = SPIM_NSS, PA11 = SPIM_IO0, PA12 = SPIM_IO1
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_11 | GPIO_Pins_12;			
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// PB1 = SPIM_SCK, PB7 = SPIM_IO2, PB6 = SPIM_IO3,
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_1 | GPIO_Pins_6 | GPIO_Pins_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/* Select PB10, PB11 as SPIF_TX and SPIF_RX */
	GPIO_PinsRemapConfig(GPIO_Remap_EXT_FLASH, ENABLE);	
	// 当XMC和SPIM同时使用时，PB7会被XMC_NADV默认开启占用，导致SPIM工作异常
	// 需手动配置AFIO_MAP2寄存器器关闭XMC_NADV功能。
	GPIO_PinsRemapConfig(GPIO_Remap_XMC_NADV,ENABLE);	
	
	/* In this example, use on-board EN25QH128A as ext.flash */
	FLASH->B3SEL = FLASH_SPIM_TYPE2;

	/* Unlock the ext.flash program erase controller */  
	while (BIT_READ(FLASH->STS3, FLASH_STS_BSY));
	FLASH->FCKEY3 = FLASH_KEY1;
	FLASH->FCKEY3 = FLASH_KEY2;
	while (BIT_READ(FLASH->CTRL3, FLASH_CTRL_LCK));

	/* If the data written to ext.flash need to be scrambled, please specify the scrambled range */
	// AT32 MCU根据FLASH_DA寄存器值作为加密范围分界, 小于0x8400000+FLASH->DA地址段的数据为密文，其余范围数据为明文
	FLASH->DA = 0;	
}


/*******************************************************************************
* 函数名  : AT24C08IO_Configuration
* 描述    : AT24C08IO配置
*******************************************************************************/
void AT24C08IO_Configuration(void)
{
//	GPIO_InitType  GPIO_InitStructure; 
//	 
//   	/* Configure I2C1 pins: WP*/
//	GPIO_InitStructure.GPIO_Pins =   pSDA_AT24;//pWP_AT24 | pSCL_AT24 | pSDA_AT24;
//	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD; //必须为开漏模式
//	GPIO_Init(AT24_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pins   =  pWP_AT24 | pSCL_AT24 ;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT_PP; //推挽输出
//	GPIO_Init(AT24_GPIO, &GPIO_InitStructure);
}

/*********************************************************************
函数名：RCC_Configuration	
时间：	2011.01.08
**********************************************************************/
void RCC_Configuration(void)
{
	// Enable HSE 	  // 使能外部晶振
	RCC_HSEConfig(RCC_HSE_ENABLE);
//	// 等待外部晶振稳定
//	if( SUCCESS == RCC_WaitForHSEStable() )
//	{	// 外部晶振启动成功，执行下一步操作

//		/* HCLK = SYSCLK/1 */
////		RCC_AHBCLKConfig(RCC_SYSCLK_Div1);		// AHB的时钟（HCLK）
//		RCC_AHBCLKConfig(RCC_SYSCLK_Div2);		// AHB的时钟（HCLK）
////		RCC_AHBCLKConfig(RCC_SYSCLK_Div4);		// AHB的时钟（HCLK）
//		
//		/* PCLK1 = HCLK/4 */
////		RCC_APB1CLKConfig(RCC_AHBCLK_Div4);		// APB1的时钟，TMR2，TMR3
//		RCC_APB1CLKConfig(RCC_AHBCLK_Div2);		// APB1的时钟，TMR2，TMR3	
////		RCC_APB1CLKConfig(RCC_AHBCLK_Div1);		// APB1的时钟，TMR2，TMR3			
//		/* PCLK2 = HCLK/4 */
////		RCC_APB2CLKConfig(RCC_AHBCLK_Div4);		// APB2的时钟，UART1			
////		RCC_APB2CLKConfig(RCC_AHBCLK_Div2);		// APB2的时钟，UART1	
//		RCC_APB2CLKConfig(RCC_AHBCLK_Div2);		// APB2的时钟，UART1
//		
//		// PLLConfig，PLLCLK = 8MHz * 30 = 240 MHz
//		RCC_PLLConfig(RCC_PLLRefClk_HSE_Div1,RCC_PLLMult_30,RCC_Range_GreatThan_72Mhz);
//		// Enable PLL 	 // 使能PLL
//		RCC_PLLCmd(ENABLE);
//		// Wait till PLL is ready 
//		while(RCC_GetFlagStatus(RCC_FLAG_PLLSTBL) == RESET);
//		// 使能
//		RCC_StepModeCmd(ENABLE);
//		// Select PLL as system clock source
//		RCC_SYSCLKConfig(RCC_SYSCLKSelction_PLL);
//		// Wait till PLL is used as system clock source 
//		while(RCC_GetSYSCLKSelction() != 0x08);		
//		// 禁止
//		RCC_StepModeCmd(DISABLE);
//				
//	}	
	
	// Enable peripheral clocks/
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_XMC, ENABLE);		  // 使能FSMC时钟

	
	 /* TMR3 clock enable */
	RCC_APB1PeriphClockCmd(   RCC_APB1PERIPH_PWR            
							| RCC_APB1PERIPH_BKP
							| RCC_APB1PERIPH_TMR2 
							| RCC_APB1PERIPH_TMR3,ENABLE);		
	
	// 使能GPIO时钟
	RCC_APB2PeriphClockCmd(   RCC_APB2PERIPH_GPIOA 
							| RCC_APB2PERIPH_GPIOB 
							| RCC_APB2PERIPH_GPIOC
							| RCC_APB2PERIPH_GPIOD
							| RCC_APB2PERIPH_GPIOE
							| RCC_APB2PERIPH_AFIO
							//| RCC_APB2Periph_ADC1
							//| RCC_APB2Periph_ADC2
							| RCC_APB2PERIPH_TMR1
							//| RCC_APB2Periph_SPI1
							| RCC_APB2PERIPH_USART1,ENABLE);
														
}

/*******************************************************************************
* 函数名  : UART1_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void USART1_NVICConfig(void)
{
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;      		 // 通道设置为串口1中断（故后面应选择在“void USART1_IRQHandler(void)”开中断）  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 // 中断占先等级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 // 中断响应优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // 打开中断 
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名  : TIM2_NVICConfig
* 描述    : TIMER2中断配置
*******************************************************************************/
void TIM3_NVICConfig(void)
{
	
	NVIC_InitType NVIC_InitStructure;

	/* Enable the TMR3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TMR3_GLOBAL_IRQn;			// 中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				// 子优先级  1=低优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 使能
	NVIC_Init(&NVIC_InitStructure);									// 配置中断
	
}
/**
  * @brief  EXTI0 Config.Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI0_Config(void)
{
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                       ///<Configure PA.00 pin as input floating
	
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         ///<Enable AFIO clock
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinsSource0); ///<Connect EXTI0 Line to PA0 pin

	EXTI_InitType EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

}
/*******************************************************************************
* 函数名  : EXIT0_NVICConfig
* 描述    : 外部中断配置
*******************************************************************************/
void EXIT0_NVICConfig(void)
{
	
	NVIC_InitType NVIC_InitStructure;

	/* Enable the TMR3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			// 中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				// 子优先级  1=低优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 使能
	NVIC_Init(&NVIC_InitStructure);									// 配置中断
	
}
/*******************************************************************************
* 函数名  : NVIC_Configuration
* 描述    : 中断向量配置
*******************************************************************************/
void NVIC_Configuration(void)
{
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//1位抢占优先级，3位子级
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//2位抢占优先级，2位子级
	
	TIM3_NVICConfig();									// 定时时钟
	EXIT0_NVICConfig();
	USART1_NVICConfig();	
}

/*******************************************************************************
* 函数名  : XMC（FSMC）初始化
* 描述    : 中断向量配置
*******************************************************************************/
void XMC_init(void)
{
	XMC_NORSRAMInitType  XMC_NORSRAMInitStruct;
	XMC_NORSRAMTimingInitType  XMC_WriteInitStruct, XMC_ReadInitStruct;

	/* Enable the XMC Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_XMC, ENABLE);	

	/*-- FSMC Configuration ------------------------------------------------------*/
	// 读的时间
	XMC_ReadInitStruct.XMC_AdrOpTime = 0x02;	
	XMC_ReadInitStruct.XMC_AdrHoldTime = 0x00;
	XMC_ReadInitStruct.XMC_DataOpTime = 30;
	XMC_ReadInitStruct.XMC_IntervalBetweenOP = 0x00;	
	XMC_ReadInitStruct.XMC_CLKPsc = 0x00;	
	XMC_ReadInitStruct.XMC_DataStableTime = 0x00;
	XMC_ReadInitStruct.XMC_Mode = XMC_Mode_A;	
	// 写的时间
	XMC_WriteInitStruct.XMC_AdrOpTime = 0x02;	
	XMC_WriteInitStruct.XMC_AdrHoldTime = 0x00;
	XMC_WriteInitStruct.XMC_DataOpTime = 0x03;
	XMC_WriteInitStruct.XMC_IntervalBetweenOP = 0x00;	
	XMC_WriteInitStruct.XMC_CLKPsc = 0x00;	
	XMC_WriteInitStruct.XMC_DataStableTime = 0x00;
	XMC_WriteInitStruct.XMC_Mode = XMC_Mode_A;	
	

	XMC_NORSRAMInitStruct.XMC_Bank = XMC_Bank1_NORSRAM1;
	XMC_NORSRAMInitStruct.XMC_BusType = XMC_BusType_16b;								// 存储数据宽带，8位和16位
	XMC_NORSRAMInitStruct.XMC_DataAdrMux = XMC_DataAdrMux_Disable;						// 数据地址复用, 禁止
	XMC_NORSRAMInitStruct.XMC_Dev = XMC_Dev_SRAM;										// 存储器类型 SRAM
	XMC_NORSRAMInitStruct.XMC_EnableAsynWait = XMC_AsynWait_Disable;
	XMC_NORSRAMInitStruct.XMC_EnableBurstMode = XMC_BurstMode_Disable;					// 突发访问模式，禁止
	XMC_NORSRAMInitStruct.XMC_EnableBurstModeSplit = XMC_BurstModeSplit_Disable;		// Wrap_Mode，禁止
	XMC_NORSRAMInitStruct.XMC_EnableWaitSignal = XMC_WaitSignal_Disable;				// Wait_Signal，禁止
	XMC_NORSRAMInitStruct.XMC_EnableWrite = XMC_WriteOperation_Enable;					// Write_Operation，允许			
	XMC_NORSRAMInitStruct.XMC_EnableWriteTiming = XMC_WriteTiming_Enable;				// Extended_Mode，允许，读写时序独立，0读写一致  ，这里读写时序不同
	XMC_NORSRAMInitStruct.XMC_WaitSignalConfig = XMC_WaitSignalConfig_BeforeWaitState;	// Wait_Timing
	XMC_NORSRAMInitStruct.XMC_WaitSignalLv = XMC_WaitSignalLv_Low;						// 等待信号极性, low
	XMC_NORSRAMInitStruct.XMC_WriteBurstSyn = XMC_WriteBurstSyn_Disable;				// Write_Burst，禁止
	XMC_NORSRAMInitStruct.XMC_WTimingStruct  = &XMC_WriteInitStruct;
	XMC_NORSRAMInitStruct.XMC_RWTimingStruct = &XMC_ReadInitStruct;
	XMC_NORSRAMInit(&XMC_NORSRAMInitStruct);
	/*!< Enable FSMC Bank1_SRAM Bank */
	XMC_NORSRAMCmd(XMC_Bank1_NORSRAM1, ENABLE);
}

/************************************************************************
** 功能描述：温湿度传感器		  
************************************************************************/
void SHT30_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pins = sda_pin | scl_pin;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;//GPIO_Mode_IN_PD; 	//设置为输入上拉模式
	GPIO_Init(sda_gpio, &GPIO_InitStructure);
	

}
/************************************************************************
** 功能描述：触摸按键传感器		  
************************************************************************/
void IICKEY_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pins = sdakey_pin | sclkey_pin;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;//GPIO_Mode_IN_PD; 	//设置为输入上拉模式	
	GPIO_Init(sdakey_gpio, &GPIO_InitStructure);
	
	
}
/************************************************************************
** 功能描述：机芯控制		  
************************************************************************/
void MOVEMENT_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pins = HOT | DOOR | FAN;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;//GPIO_Mode_IN_PD; 	//设置为输入上拉模式	
	GPIO_Init(HOT_PORT, &GPIO_InitStructure);
	
	FAN_OFF;
	DOOR_OFF;
	HOT_OFF;
}
///*********************************************************************
//函数名: TIM2_Configuration 
//描述：  定时器2配置
//时间：	2011.01.08
//**********************************************************************/
//void TIM2_Configuration(void)
//{	
//	TMR_TimerBaseInitType  TMR_TMReBaseStructure;
//	/* Compute the prescaler value */
//	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;

//	/* TMRe base configuration */
//	TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
//	TMR_TMReBaseStructure.TMR_Period = 1000;		//1ms
//	TMR_TMReBaseStructure.TMR_DIV = 0;
//	TMR_TMReBaseStructure.TMR_ClockDivision = 0;
//	TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;

//	TMR_TimeBaseInit(TMR2, &TMR_TMReBaseStructure);	
//	
//	/* Prescaler configuration */
//	TMR_DIVConfig(TMR2, PrescalerValue, TMR_DIVReloadMode_Immediate);	
//	

////	TMR_ARRPreloadConfig(TMR2,ENABLE);

//	/* TMR IT flg clear */
//	TMR_ClearITPendingBit(TMR2,TMR_INT_Overflow);
//	/* TMR IT enable */
//	TMR_INTConfig(TMR2,TMR_INT_Overflow,ENABLE);		
//	/* TMR2 enable counter */
//	TMR_Cmd(TMR2, ENABLE);							
//}

/*********************************************************************
函数名: TIM6_Configuration 
描述：  定时器6配置

**********************************************************************/
//void TIM6_Configuration(void)
//{	
//	TMR_TimerBaseInitType  TMR_TMReBaseStructure;
//	/* Compute the prescaler value */
////	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;

//	
//  /* 
//    * The TIMxCLK is fixed to 240 MHZ, the Prescaler is equal to 2 so the TMRx clock 
//      counter is equal to 80 MHz.
//      The Three Timers are running at: 
//      TMRx frequency = TMRx clock counter/ 2*(TIMx_Period + 1) = 500 KHz. 
//  */
//	/* TMRe base configuration */	
//	TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
//	TMR_TMReBaseStructure.TMR_Period = 40000-1;		//1ms, 1Khz
//	TMR_TMReBaseStructure.TMR_DIV = 2;
//	TMR_TMReBaseStructure.TMR_ClockDivision = 0;
//	TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;

//	TMR_TimeBaseInit(TMR6, &TMR_TMReBaseStructure);	
//	
//	/* Prescaler configuration */
////	TMR_DIVConfig(TMR6, PrescalerValue, TMR_DIVReloadMode_Update);	
//	

////	TMR_ARRPreloadConfig(TMR2,ENABLE);

//	/* TMR IT flg clear */
//	TMR_ClearITPendingBit(TMR6,TMR_INT_Overflow);
//	/* TMR IT enable */
//	TMR_INTConfig(TMR6,TMR_INT_Overflow,ENABLE);		
//	/* TMR enable counter */
//	TMR_Cmd(TMR6, ENABLE);							
//}

/*********************************************************************
函数名: TIM3_Configuration 
描述：  定时器3配置

**********************************************************************/
void TIM3_Configuration(void)
{
	TMR_TimerBaseInitType  TMR_TimeBaseStructure;	
	uint16_t PrescalerValue;
  /* ---------------------------------------------------------------
    TMR3 Configuration: 
    TMR3CLK = SystemCoreClock / 4,
    The objective is to get TMR3 counter clock at 1 KHz:
     - Prescaler = (TIM3CLK / TMR3 counter clock) - 1

	TMRx clock counter = TIMxCLK / (TMR_DIV+1)
  --------------------------------------------------------------- */	
	/*Compute the prescaler value */
//	PrescalerValue = (uint16_t) (SystemCoreClock / 4000) - 1;
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;	// 设定计数器时钟为1Mhz	
	/* Time base configuration */
	TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);  
//	TMR_TimeBaseStructure.TMR_Period = 1;//65535;	// 设定为1时才是1Khz
//	TMR_TimeBaseStructure.TMR_DIV = PrescalerValue;
	TMR_TimeBaseStructure.TMR_Period = 1000;		// 指定要加载的周期重载值,1000us，  TMRx_AR，值范围0~65535  ，设定计数重载值
	TMR_TimeBaseStructure.TMR_DIV = PrescalerValue;	// 指定用于分频TMR时钟的预分频器值，TMRx_DIV，值范围 0~65535，设定计数器时钟频率	
	
	TMR_TimeBaseStructure.TMR_ClockDivision = 0;
	TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;

	TMR_TimeBaseInit(TMR3, &TMR_TimeBaseStructure);

//	TMR_ARPreloadConfig(TMR3, ENABLE);
	/* TMR IT flg clear */
	TMR_ClearITPendingBit(TMR3,TMR_INT_Overflow);
	/* TMR IT enable */
	TMR_INTConfig(TMR3,TMR_INT_Overflow,ENABLE);		
	/* TMR enable counter */
	TMR_Cmd(TMR3, ENABLE);		
}
/*********************************************************************
函数名: Systick_Configuration 
描述：  嘀嗒时钟配置
**********************************************************************/
void Systick_Configuration(void)
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		// 时钟为72/8 = 9MHz
	
	/*Config Systick*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		//时钟为240/8 = 30MHz

	
	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
//    SysTick_SetReload(9000);								//校正值为9000时为1MS时基
//	SysTick_SetReload(5555);								//校正值为186us,2.7K蜂鸣器频率 ,9M = 1667, 9000K/（2.7K*2）
//	SysTick_SetReload(1125);								//4K蜂鸣器频率
//	SysTick_SetReload(1800);								//200us，2.5K蜂鸣器频率
    /* Enable SysTick interrupt */
//    SysTick_ITConfig(ENABLE);							    //使能中断
//	SysTick_CounterCmd(SysTick_Counter_Enable);
}

/************************************************************************
** 功能描述：板级硬件的初始化	
**功能  ： LED引脚初始化
************************************************************************/
void Board_Init(void)
{
	/*System clocks configuration*/
    RCC_Configuration();			   // 时钟配置		
	  

	/*Configure TFT */
	TFT_GPIO_Config();
	
	/*Configure LED */
	LEDIO_Config();
	
	/*Configure BEEL */
	BeepIO_Config();

	/*Configure KEY */
	KeyIO_Config();
	
	/*Configure EEPROM */
//	AT24C08IO_Configuration();	
		
	/*Configure XMC(FSMC) TFT */
	XMC_init();

	/*Configure SPIM Nor Flash */
//	SpimIO_Config();
	SPIM_init();
	
    SHT30_Init();

	/*Configure RS485 */
	UART_Init();
	
	/*Configure RS485 */
	TIM3_Configuration();			// 1ms系统时钟
//	Systick_Configuration();		// 186us蜂鸣器频率	
	
	NVIC_Configuration();			   // 中断向量配置	  /* 中断配置*/
	
	EXTI0_Config();
	
	IICKEY_Init();
	
	MOVEMENT_Init();
//	FSMC_SRAM_Init();				   // FSMC_SRAM 配置
//	FSMC_NORFLASH_Init();			   // FSMC_FLASH 配置
//	Timer_Configuration();			   // 定时器 配置	
}
	





