/**
  **************************************************************************
  * File   : T-S1_V30_board.h
  * Version: V3.0
  * Date   : 2020-08-20
  * Brief  : Header file for T-S1_V30 board
  *          1. Set of firmware functions to manage Leds, push-button and COM ports.
  *          2. initialize Delay Function and USB
  */

#ifndef __T_ES1_BOARD_H
#define __T_ES1_BOARD_H	 




/**************LED pins define **************/
// 好运来R-->鸿利B,好运来G-->鸿利R,好运来B-->鸿利G
//#if ( LED_TYPE == HL_RGB)	
// 鸿利灯珠

#define pLED_BLUE         	GPIO_Pins_4//GPIO_Pins_12
#define	LED_BLUE_PINSET	 	(pLED_BLUE)
#define LED_BLUE_PORT     	GPIOC                    // 蓝色	

#define pLED_GREEN        	GPIO_Pins_5//GPIO_Pins_11               // 绿色
#define	LED_GREEN_PINSET	(pLED_GREEN)
#define LED_GREEN_PORT    	GPIOC

#define pLED_RED          	GPIO_Pins_7//GPIO_Pins_10              // 红色
#define	LED_RED_PINSET	  	(pLED_RED)
#define LED_RED_PORT      	GPIOA

#define pLED_ORANGE          	GPIO_Pins_3//GPIO_Pins_10              // 红色
#define	LED_ORANGE_PINSET	  	(pLED_ORANGE)
#define LED_ORANGE_PORT      	GPIOA
//#else
// 好运来灯珠
//  #define LED_BLUE_PORT     GPIOC                    // 蓝色
//  #define pLED_BLUE         GPIO_Pin_11

//  #define LED_GREEN_PORT    GPIOC
//  #define pLED_GREEN        GPIO_Pin_10               // 绿色

//  #define LED_RED_PORT      GPIOB
//  #define pLED_RED          GPIO_Pin_9               // 红色
//    
//#endif


#define pLED_WRN     		GPIO_Pins_5
#define	LED_WRN_PINSET		(pLED_WRN)
#define LED_WRN_PORT     	GPIOE

//#define pLED_DO				GPIO_Pins_3
//#define	LED_DO_PINSET		(pLED_DO)
//#define LED_DO_PORT     	GPIOA

#define LED_R_ON()		GPIO_SetBits(LED_RED_PORT,pLED_RED)	 //0 亮
#define LED_R_OFF()		  GPIO_ResetBits(LED_RED_PORT,pLED_RED)

#define LED_G_ON()		GPIO_SetBits(LED_GREEN_PORT,pLED_GREEN)	 //0 亮
#define LED_G_OFF()		  GPIO_ResetBits(LED_GREEN_PORT,pLED_GREEN)

#define LED_B_ON()		GPIO_SetBits(LED_BLUE_PORT,pLED_BLUE)	 //0 亮
#define LED_B_OFF()		  GPIO_ResetBits(LED_BLUE_PORT,pLED_BLUE)

#define LED_WRN_ON()	GPIO_ResetBits(LED_WRN_PORT,pLED_WRN) //0 亮
#define LED_WRN_OFF()	  GPIO_SetBits(LED_WRN_PORT,pLED_WRN)
						
#define LED_O_ON()		GPIO_SetBits(LED_ORANGE_PORT,pLED_ORANGE)	 //0 亮
#define LED_O_OFF()	  GPIO_ResetBits(LED_ORANGE_PORT,pLED_ORANGE)
//#define READ_LED_O()	GPIO_ReadInputDataBit(LED_ORANGE_PORT,pLED_ORANGE)


/**************TFT  define **************/
#define pTFT_BL	    		GPIO_Pins_13	//背光控制脚
#define	TFT_BL_PINSET		(pTFT_BL)
#define TFT_BL_GPIO     	GPIOB

#define pTFT_REST	  		GPIO_Pins_12	//复位口
#define	TFT_RSET_PINSET		(pTFT_REST)
#define TFT_RSET_GPIO     	GPIOB

#define TFT1_BL_ON()		GPIO_ResetBits(TFT_BL_GPIO,pTFT_BL)	 //0 亮
#define TFT1_BL_OFF()		  GPIO_SetBits(TFT_BL_GPIO,pTFT_BL)


#define TFT1_REST_ON()		  GPIO_SetBits(TFT_RSET_GPIO,pTFT_REST)	 //1 亮
#define TFT1_REST_OFF()		GPIO_ResetBits(TFT_RSET_GPIO,pTFT_REST)

/**************** SHT30 printf ****************/
#define sda_pin					GPIO_Pins_8
#define sda_gpio				GPIOB

#define scl_pin					GPIO_Pins_9
#define scl_gpio				GPIOB

#define     SET_SDA_BIT()	GPIO_SetBits(sda_gpio,sda_pin)
#define     RESET_SDA_BIT()	GPIO_ResetBits(sda_gpio,sda_pin)
#define     READ_SDA_BIT	GPIO_ReadInputDataBit(sda_gpio,sda_pin)

#define     SET_SCL_BIT()	GPIO_SetBits(scl_gpio,scl_pin)
#define     RESET_SCL_BIT()	GPIO_ResetBits(scl_gpio,scl_pin)
/**************** IIC_KEY30 printf ****************/
#define sdakey_pin				GPIO_Pins_1
#define sdakey_gpio				GPIOA

#define sclkey_pin				GPIO_Pins_2
#define sclkey_gpio				GPIOA

#define     SET_SDAKEY_BIT()	GPIO_SetBits(sdakey_gpio,sdakey_pin)
#define     RESET_SDAKEY_BIT()	GPIO_ResetBits(sdakey_gpio,sdakey_pin)
#define     READ_SDAKEY_BIT	GPIO_ReadInputDataBit(sdakey_gpio,sdakey_pin)

#define     SET_SCLKEY_BIT()	GPIO_SetBits(sclkey_gpio,sclkey_pin)
#define     RESET_SCLKEY_BIT()	GPIO_ResetBits(sclkey_gpio,sclkey_pin)
/**************** movement printf ****************/
#define	HOT			 GPIO_Pins_4
#define HOT_PORT     GPIOA

#define	DOOR		GPIO_Pins_5
#define DOOR_PORT    GPIOA

#define	FAN			 GPIO_Pins_6
#define FAN_PORT     GPIOA 

#define HOT_ON				GPIO_ResetBits(HOT_PORT, HOT)			// 云母片控制，低电平开通
#define HOT_OFF				GPIO_SetBits(HOT_PORT, HOT)
#define READ_HOT_BIT		GPIO_ReadInputDataBit(HOT_PORT, HOT)

#define DOOR_ON			    GPIO_ResetBits(DOOR_PORT, DOOR) 		// PTC加热控制，低电平开通
#define DOOR_OFF		    GPIO_SetBits(DOOR_PORT, DOOR)
#define READ_DOOR_BIT		GPIO_ReadInputDataBit(DOOR_PORT, DOOR)  // 读PA5的值

#define FAN_ON				GPIO_ResetBits(FAN_PORT, FAN)  			// 风扇控制，低电平开通
#define FAN_OFF				GPIO_SetBits(FAN_PORT, FAN)
#define READ_FAN_BIT		GPIO_ReadInputDataBit(FAN_PORT, FAN)


/**************BEEP pins define **************/
//#define 	pBEEP      	   	GPIO_Pins_6
//#define	  	BEEP_PINSET		(pBEEP)
//#define 	BEEP_GPIO      	GPIOE

//#define 	BEEP_ON()	     GPIO_SetBits(BEEP_GPIO,pBEEP) 	//1 开
//#define 	BEEP_OFF()	   GPIO_ResetBits(BEEP_GPIO,pBEEP)


/**************RS485 pins define **************/
#define    RS485_UART       USART1

#define    pCTRL485	    	GPIO_Pins_9	
#define    CTRL485_GPIO  	GPIOC

#define    pTXD485	    	GPIO_Pins_9				
#define    TXD485_GPIO    	GPIOA

#define    pRXD485	    	GPIO_Pins_10
#define    RXD485_GPIO    	GPIOA

//#define	   SETCTRL485() 	GPIO_SetBits(CTRL485_GPIO,pCTRL485)  
//#define	   RESETCTRL485()	GPIO_ResetBits(CTRL485_GPIO,pCTRL485)  

#define	   RS485CTRL_SEND_EN() 	GPIO_SetBits(CTRL485_GPIO,pCTRL485)  
#define	   RS485CTRL_RECV_EN()	GPIO_ResetBits(CTRL485_GPIO,pCTRL485) 
	   

/**************Key pins define **************/

#define		pKEY_OK		  GPIO_Pins_2
#define		pKEY_CFG	  GPIO_Pins_3	
#define		pKEY_SHIFT	GPIO_Pins_4
#define		pKEY_ADD	  GPIO_Pins_5

//#define		KEYPINSET	  (pKEY_OK|pKEY_CFG|pKEY_SHIFT|pKEY_ADD)
//#define   	KEY_GPIO    GPIOE

//#define     SETKEYOKBIT()	GPIO_SetBits(KEY_GPIO,pKEY_OK)
//#define     READKEYOKBIT()	GPIO_ReadInputDataBit(KEY_GPIO,pKEY_OK)  

//#define     SETKEYCFGBIT()	GPIO_SetBits(KEY_GPIO,pKEY_CFG)
//#define     READKEYCFGBIT()	GPIO_ReadInputDataBit(KEY_GPIO,pKEY_CFG)

//#define     SETKEYSHIFTBIT()	GPIO_SetBits(KEY_GPIO,pKEY_SHIFT)
//#define     READKEYSHIFTBIT()	GPIO_ReadInputDataBit(KEY_GPIO,pKEY_SHIFT)

//#define     SETKEYADDBIT()	GPIO_SetBits(KEY_GPIO,pKEY_ADD)
//#define     READKEYADDBIT()	GPIO_ReadInputDataBit(KEY_GPIO,pKEY_ADD)

/***********************EEPROM pin_define************************/
#define  pWP_AT24   	GPIO_Pins_1
#define  pSCL_AT24		GPIO_Pins_2
#define  pSDA_AT24		GPIO_Pins_3

#define  AT24_GPIO	GPIOC

#define	   SETSDA_AT24()	      GPIO_SetBits(AT24_GPIO,pSDA_AT24)
#define	   RESETSDA_AT24()		GPIO_ResetBits(AT24_GPIO,pSDA_AT24)  
#define	   SETSCL_AT24()	      GPIO_SetBits(AT24_GPIO,pSCL_AT24) 
#define	   RESETSCL_AT24()		GPIO_ResetBits(AT24_GPIO,pSCL_AT24)
#define	   READSDABIT_AT24()  GPIO_ReadInputDataBit(AT24_GPIO,pSDA_AT24)
#define	   DISWRITE_AT24()		  GPIO_SetBits(AT24_GPIO,pWP_AT24)
#define	   ENWRITE_AT24()		GPIO_ResetBits(AT24_GPIO,pWP_AT24)


/**************** UART printf ****************/
#define AT32_PRINT_UART                USART1
#define USARTx_IRQn                    USART1_IRQn
#define USARTx_IRQ_Handler             USART1_IRQHandler
#define AT32_PRINT_UART_RCC            RCC_APB2PERIPH_USART1

/*Tx*/
#define AT32_PRINT_UARTTX_PIN          GPIO_Pins_9 
#define AT32_PRINT_UARTTX_GPIO         GPIOA
#define AT32_PRINT_UARTTX_GPIO_RCC     RCC_APB2PERIPH_GPIOA

/*Rx*/
#define AT32_PRINT_UARTRX_PIN          GPIO_Pins_10
#define AT32_PRINT_UARTRX_GPIO         GPIOA
#define AT32_PRINT_UARTRX_GPIO_RCC     RCC_APB2PERIPH_GPIOA
/**************** End UART printf ****************/



#define CLOSE_SYS_TMR_INT()		TMR_INTConfig(TMR3,TMR_INT_Overflow,DISABLE)	//禁止系统时钟中断
#define  OPEN_SYS_TMR_INT()		TMR_INTConfig(TMR3,TMR_INT_Overflow,ENABLE)	 	//使能系统时钟中断

//#define	 DEBUG_INT_1MS
//#define	 DEBUG_SYS_1US

//void UART_Print_Init(uint32_t bound);

//void TFT_GPIO_Config(void);

/*Delay function*/
void Delay_init(void);

#endif

/****************** (C) COPYRIGHT 2018 ArteryTek *********END OF FILE*********/
