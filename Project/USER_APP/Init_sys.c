//#include  "../Head file/Glob.h"

#include "../Head file/includes.h" 
/*********************************************************************
函数名：DeviceConfig_Init
功能：	MCU资源初始化
**********************************************************************/
void DeviceConfig_Init(void)
{
	
    RCC_Configuration();			   // 时钟配置		/*System clocks configuration*/
	NVIC_Configuration();			   // 中断向量配置	  /* 中断配置*/
//	FSMC_SRAM_Init();				   // FSMC_SRAM 配置
	FSMC_NORFLASH_Init();			   // FSMC_FLASH 配置
	Timer_Configuration();			   // 定时器 配置	  
//  	SPI_FLASH_Init();				   // Initialize the SPI FLASH driver 

	UART_Init();					   // 串口 配置
}

/*********************************************************************
函数名：System_Init
功能：	开机初始化
**********************************************************************/


void System_Init(void)
{
//	Delay_ms(1000);

	OsCmdSet.SystemLocked = ALL_LOCK;			// 锁定系统、解锁系统；不允许按键工作
 	Key_Init();						   	// 按键功能初始化，IO口配置
	Beep_Init();						// 蜂鸣器初始化，IO口配置
	LED_Init();							// LED初始化
	EEPROM_Init();						// AT24C08初始化，模拟I2C接口

	 
	LoadParaInEeprom();					// 读EEPROM系统保存的参数
	// 开机显示颜色画面			
 	TFTLCD_Init();       				// 显示初始化，要根据设置参数做横竖屏显示

//	AlarmSet(ALMBAT);					// 蜂鸣器声音
	AlarmSet(ALMKEY);
	
	bk_colour_true = aBLACK	;
	SetLcdLay( PAGE_START );			// 显示开机页面
	LCD_Scan();
	Delay_ms(25000);
	
	led_light_ctrl(LED_RED);
	bk_colour_true = aRED	;
//	DispColor1(aRED);  	  // 红色
	SetLcdLay( PAGE_START );			// 显示开机页面
	LCD_Scan();
  	Delay_ms(25000);

	led_light_ctrl(LED_GREEN);
	bk_colour_true = aGREEN	;
//	DispColor1(aGREEN);	  // 绿色
	SetLcdLay( PAGE_START );			// 显示开机页面
	LCD_Scan();
	Delay_ms(25000);

	led_light_ctrl(LED_BLUE);
	bk_colour_true = aBLUE	;
//	DispColor1(aBLUE);  	  // 蓝色
	SetLcdLay( PAGE_START );			// 显示开机页面
	LCD_Scan();
	Delay_ms(25000);

//	led_light_ctrl(LED_RG);
////	DispColor1(BLUE1); DispColor2(BLUE1); 	  // 黄色
//	Delay_ms(10000);
//
//	led_light_ctrl(LED_RB);
////	DispColor1(BLUE1); DispColor2(BLUE1); 	  // 粉色
//	Delay_ms(10000);
//
//	led_light_ctrl(LED_GB);
////	DispColor1(BLUE1); DispColor2(BLUE1); 	  // 青色
//	Delay_ms(10000);

////	led_light_ctrl(LED_RGB);
////	DispColor1(WHITE1);DispColor2(WHITE1);	  // 白色
////	Delay_ms(10000);

	LED_ALL_OFF();						// 关所有的灯珠
			
//	OsCmdSet.CurDispMode  = DISPMODE_NONE;  	// 当前任务状态设为非运行状态
	OsCmdSet.SystemLocked = ALL_UNLOCK;				// 锁定系统、解锁系统；默认解锁		// 允许按键工作
	BackTask.num = 0;			   				// 回显备份区的保存总个数
	os_time.Base_100ms    = TIMER_BASE_100MS;	// 基准时间


	//握手通信
	RESETCTRL485();		 				// 关闭发送DE端口，打开接收/RE485端口
	sys_data_reset();					// 初始化任务相关变量
	uart.rFlag   = OFF;					// 已处理
	uart.rIndex  = 0;					// 接收个数清0


	// 开机显示AD,F1,F2,F3,F4，灭显示	
	SetLcdLay( PAGE_CFGADDR );				// 显示地址页面
	LCD_Scan();	Delay_ms(35000);			// 显示	  ，延时630ms
	SetLcdLay( PAGE_CFGFUNC1 );				// 显示F1页面
	LCD_Scan();	Delay_ms(35000);			// 显示	  ，延时630ms
	SetLcdLay( PAGE_CFGFUNC2 );				// 显示F2页面
	LCD_Scan();	Delay_ms(35000);			// 显示	  ，延时630ms
	SetLcdLay( PAGE_CFGFUNC3 );				// 显示F3页面
	LCD_Scan();	Delay_ms(35000);			// 显示	  ，延时630ms
	SetLcdLay( PAGE_CFGFUNC4 );				// 显示F4页面
	LCD_Scan();	Delay_ms(35000);			// 显示	  ，延时630ms


	Display_Clear();						// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
}




