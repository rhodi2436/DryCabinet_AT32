
#ifndef _LED_H_
#define _LED_H_

/**************pins define **************/

#define HYL_RGB     0
#define HL_RGB      1

//#define LED_TYPE    HYL_RGB
#define LED_TYPE    HL_RGB


//// 好运来R-->鸿利B,好运来G-->鸿利R,好运来B-->鸿利G
//#if ( LED_TYPE == HL_RGB)	
//// 鸿利灯珠
//  #define LED_BLUE_PORT     GPIOB                    // 蓝色
//  #define pLED_BLUE         GPIO_Pin_9

//  #define LED_GREEN_PORT    GPIOC
//  #define pLED_GREEN        GPIO_Pin_11               // 绿色

//  #define LED_RED_PORT      GPIOC
//  #define pLED_RED          GPIO_Pin_10              // 红色
//#else
//// 好运来灯珠
//  #define LED_BLUE_PORT     GPIOC                    // 蓝色
//  #define pLED_BLUE         GPIO_Pin_11

//  #define LED_GREEN_PORT    GPIOC
//  #define pLED_GREEN        GPIO_Pin_10               // 绿色

//  #define LED_RED_PORT      GPIOB
//  #define pLED_RED          GPIO_Pin_9               // 红色
//    
//#endif

////#define		pLED_RED		GPIO_Pin_9	
////#define		LEDPINSET1		(pLED_RED)
////#define     LEDPORT1     	GPIOB

////#define     pLED_GREEN     	GPIO_Pin_10
////#define		pLED_BLUE		GPIO_Pin_11	
////#define		LEDPINSET2		(pLED_GREEN|pLED_BLUE)
////#define     LEDPORT2     	GPIOC

//#define     pLED_WRN     	GPIO_Pin_1
//#define		LEDPINSET3		(pLED_WRN)
//#define     LEDPORT3     	GPIOA

//#define     pLED_RT			GPIO_Pin_6
//#define		LEDPINSET4		(pLED_RT)
//#define     LEDPORT4     	GPIOF





//	
////#define LED_R_ON()		GPIO_ResetBits(LEDPORT1,pLED_RED)	 //0 亮
////#define LED_R_OFF()		  GPIO_SetBits(LEDPORT1,pLED_RED)

////#define LED_G_ON()		GPIO_ResetBits(LEDPORT2,pLED_GREEN)	 //0 亮
////#define LED_G_OFF()		  GPIO_SetBits(LEDPORT2,pLED_GREEN)

////#define LED_B_ON()		GPIO_ResetBits(LEDPORT2,pLED_BLUE)	 //0 亮
////#define LED_B_OFF()		  GPIO_SetBits(LEDPORT2,pLED_BLUE)

//#define LED_R_ON()		GPIO_ResetBits(LED_RED_PORT,pLED_RED)	 //0 亮
//#define LED_R_OFF()		  GPIO_SetBits(LED_RED_PORT,pLED_RED)

//#define LED_G_ON()		GPIO_ResetBits(LED_GREEN_PORT,pLED_GREEN)	 //0 亮
//#define LED_G_OFF()		  GPIO_SetBits(LED_GREEN_PORT,pLED_GREEN)

//#define LED_B_ON()		GPIO_ResetBits(LED_BLUE_PORT,pLED_BLUE)	 //0 亮
//#define LED_B_OFF()		  GPIO_SetBits(LED_BLUE_PORT,pLED_BLUE)

//#define LED_WRN_ON()	GPIO_ResetBits(LEDPORT3,pLED_WRN) //0 亮
//#define LED_WRN_OFF()	  GPIO_SetBits(LEDPORT3,pLED_WRN)

//#define LED_RT_ON()		GPIO_ResetBits(LEDPORT4,pLED_RT)	 //0 亮
//#define LED_RT_OFF()	  GPIO_SetBits(LEDPORT4,pLED_RT)




#define		LEDON	 1
#define		LEDOFF	 0

//led light 引脚配置
#define LED_NONE	0
#define LED_RED		1
#define LED_GREEN	2
#define LED_BLUE	3

#define LED_RG		4
#define LED_RB		5
#define LED_GB		6
#define LED_RGB		7

//led 灯闪烁比例模式
//9种亮灭时间比例
// LED_FLASH_MODE1：常亮；
// LED_FLASH_MODE2：亮灭时间：1V1，表示亮1*100ms，灭1*100ms；
// LED_FLASH_MODE3：亮灭时间：1V2，表示亮1*100ms，灭2*100Tms；
// LED_FLASH_MODE4：亮灭时间：1V5;    表示亮1*100ms，灭5*100ms；
// LED_FLASH_MODE5：亮灭时间：1V10；
// LED_FLASH_MODE6：亮灭时间：2V1；
// LED_FLASH_MODE7：亮灭时间：5V1；
// LED_FLASH_MODE8：亮灭时间：10V1；


#define LED_FLASH_MODE_ALWAYS		0			// 常亮；
#define LED_FLASH_MODE_1V1			1			// 表示亮1* 100ms，灭1* 100ms；
#define LED_FLASH_MODE_1V2			2			// 表示亮1* 100ms，灭2* 100Tms；
#define LED_FLASH_MODE_1V5			3
#define LED_FLASH_MODE_1V10			4
#define LED_FLASH_MODE_2V1			5			// 表示亮2* 100ms，灭1* 100Tms；
#define LED_FLASH_MODE_5V1			6
#define LED_FLASH_MODE_10V1			7


#define LED_FLASH_MAX_MODE9	    LED_FLASH_MODE9

#define LED_FLASH_MODE10	10
#define LED_FLASH_MODE11	11
#define LED_FLASH_MODE12	12
#define LED_FLASH_MODE13	13

#define LED_FLASH_TIME_MAX       4



#endif

