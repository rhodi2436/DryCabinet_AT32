/**********************************************************************
* T-S1 中文电子标签                                              
*---------------------------------------------------------
* INFORMATION:                                            
* COPYRIGHT:     昊方控制设备有限公司      
* MICROCOMPUTER: AT32F403AVCT7，W25Q128JV，AT24C02
* TIME:          2020-08-20                                  
* DESIGNER:      黎昌其                                      
*---------------------------------------------------------
AT32F403AVCT7,USE_STDPERIPH_DRIVER,USE_FULL_ASSERT,AT_START_F403A_V1_0

*备注：
  1.  单3.0寸TFT彩色液晶屏，或者2.8寸TFT彩色液晶屏显示，240*400
  2.  W25Q128JV 16M byte SPI 数据线 nor_flash显示内存，4M字库


测试记录
1、完成1ms定时中断时间基准；
2、完成1us，1ms系统延时间基准；（受系统分频时钟影响）
3、RGB灯珠完成，原理图用的不是鸿利的封装，需要调整。
4、KEY ok
5、EEPROM ok
5、RS485 ok
6、XMC接口OK，核心频率必须要2分频，程序窗口需要赋初始值。
7、SPIM NorFlash接口OK，写入用ICP工具完成烧录。
8、2.8寸屏刷新整屏颜色（16ms），开始显示页面整屏颜色+字体（18ms），功能页面背景图+内容（74ms）
9、读取SPIM 40K用时（7ms）
10、TFT屏显示完成，背景图写入完成，字符串（字符和汉字）写入完成。
11、用AT-Link，ICP软件烧录NorFlash需要9分钟，烧录程序17秒，可以2个程序头统一用AT-Link烧录。

说明
1、字符库存在MCU的flash中
2、GB2312字库和图片存在NorFlash中

// 2020-9-10 T-ES1 V1.0，T-S1 V3.0功能完成
1、实现一个程序兼容2个TFT屏。程序自动识别液晶屏，并更改显示版本号。

// 2021-2-25 T-ES1 V1.20，功能完成
1、2.8寸屏驱动芯片由ILI9326更改成ILI9327。
2、鸿佳3.0寸屏读出ID为0x7793，2.8寸-9326读出为ID为0x9326
3、横屏可以完全兼容三款屏幕，竖屏位置有错，需要更改大量的数据，并做映射才可，暂不做更改
4、更换屏，主要更改初始化，还有设置显示模式，窗口设定，位置定义TFT3.55.h

// 2021-6-21 T-ES1 V1.20，MCU由403A改成403，主频240Mhz变成200Mhz

//2021年8月19日 T-ES1-V1.30_20210827

1、二维码显示根据输入的字符数，大小可自适应调节。
2、新增的二维码显示功能，显示位置与原有显示箭头位置共用。
3、新增的二维码显示功能，仅支持横屏显示，不支持竖屏，双屏显示。
4、二维码显示大小支持：21、25、29、33、37、41、45、49、53像素点
5、二维码显示使用21像素点效果最佳，输入8个中文或19个字符即可显示21像素点二维码。
6、上位机发送二维码字符最大长度为200(中文100个)，大于此长度后面的数据不做显示处理。

//2021年8月19日 T-ES1-V1.31_20210827
1、更改设备上电显示版本号为：V1.31 2021-08-27

//20240913



//2026-03-20更新_三十分钟的后显示为目标湿度值



//2026-03-31  
sht30_par.SHT30_Humidity    = 读取的当前湿度值
para.str.SHT30_Humidity_SET = 目标的设定湿度值
加入一个中间值 作为 当前值 Cur_SHT30_Humidity
1. 报警背景同步更改。
2. 显示数值必须小于设定值。
3. 开门后需恢复正常数值

4. 更改逻辑，发现快速除湿(开启风扇)下除湿效果最好，43%-->13%只要15分钟。 
   改成大部分时间采用快速除湿
***********************************************************************/

#define   GLOB_DEF
#include  "../Head file/includes.h" 
#include  "../Head file/Image_Const.h"
//#include  "../Head file/China_Dot_Lib.h"
#include  "../Head file/Dot_Lib.h"
#include  "../Head file/InfoImage_Lib.h"

#include "qrencode.h"
#include "qr_encode.h"

/**********************************************************************
**函数名:  Delay_us,  AT24C02用
**功能:    延时		
us = 1（1us），us = 10（10us），us = 100（100us），us = 1000（1000us）
// 这是系统1分频
27: us = 1（us），us = 10（us），us = 100（61us），us = 1000（us）
40: us = 1（us），us = 10（us），us = 100（87us），us = 1000（us）
46: us = 1（1.1us），us = 10（10.6us），us = 100（100us），us = 1000（us）
45: us = 1（1.1us），us = 10（10.6us），us = 100（97us），us = 1000（970us）
// 这是系统2分频
22: us = 1（1.2us），us = 10（10.3us），us = 100（101us），us = 1000（1020us）
21: us = 1（1.1us），us = 10（9.8us），us = 100（98us），us = 1000（980us）
***********************************************************************/
void Delay_us(INT16U us)
{	
	INT16U	j;
	for (; us > 0; us--)
	{	for(j = 0;j < 21;j++);    }	//8//46(系统1分频)
}

/**********************************************************************
**函数名:  Delay_ms
**功能:    延时		
ms = 1（32us），ms = 10（318us），ms = 100（3150us），ms = 1000（31.5ms）
250*4: ms = 1（us），ms = 10（205us），ms = 100（2080us），ms = 1000（31.5ms）
250*5: ms = 1（31us），ms = 10（300us），ms = 100（3120us），ms = 1000（ms）
1300:  ms = 1（us），ms = 10（us），ms = 100（3240us），ms = 1000（ms）

改成真实 1ms
***********************************************************************/
void Delay_ms(INT32U ms)
{
//	INT32U i;
//	INT16U  j; 

//	for (i=0; i<ms; i++)
//	{	for (j=0;j<250*4;j++);    }
	

	while(ms--)
	{
//		Delay_us(31);	
//		for (j=0;j<250*5;j++); 
		Delay_us(1000);			// 970us（45），1000us（46）
	}	
}

/*******************************************************************************
	 					清除所有显示
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Clear(void)
{	
	Key.f_LongFunc         = FALSE; 	// 退出配置密码状态	
	KeyMode.f_Config       = OFF;		// 退出配置状态
	KeyMode.f_BackDisp 	   = OFF;		// 退出回显状态
	KeyMode.f_ChangeValue1 = OFF; 		// 退出任务1修改状态
	KeyMode.f_ChangeValue2 = OFF; 		// 退出任务2修改状态
	Key.f_IsOkPressed  	   = OFF; 		// 清除OK按键信息
	Key.f_IsOkPressed1     = OFF; 		// 清除900U OK按键信息	
	KeyMode.f_F1BackDisp   = OFF;       // 清F1触发的回显标志

	SetLcdLay( PAGE_OFF );				// 显示运行页面// 灭显示
	LED_ALL_OFF();						// 灭LED指示灯
}

/*********************************************************************
函数名：System_Init
功能：	开机初始化
**********************************************************************/
void System_Init(void)
{
	OsCmdSet.SystemLocked = ALL_LOCK;			// 锁定系统、解锁系统；不允许按键工作

//	EEPROM_Init();						// AT24C08初始化，模拟I2C接口
	 
	LoadParaInEeprom();					// 读EEPROM系统保存的参数

	// 开机显示颜色画面			
 	TFTLCD_Init();       				// 显示初始化，要根据设置参数做横竖屏显示
	
	bk_colour_true = aBLACK	;
	SetLcdLay( PAGE_START );			
	LCD_Scan();
	Delay_ms(25000);

	LED_R_ON();									
	Delay_ms(1000);
	LED_R_OFF();
	LED_G_ON();		
	Delay_ms(1000);
	LED_G_OFF();
  LED_B_ON();	
	Delay_ms(1000);
	LED_B_OFF();
	LED_O_ON();	
	Delay_ms(1000);
	LED_O_OFF();	
				
	OsCmdSet.CurDispMode  = DISPMODE_NONE;  	// 当前任务状态设为非运行状态
	OsCmdSet.SystemLocked = ALL_UNLOCK;				// 锁定系统、解锁系统；默认解锁		// 允许按键工作
	BackTask.num = 0;			   				// 回显备份区的保存总个数
	os_time.Base_100ms    = TIMER_BASE_100MS;	// 基准时间


	//握手通信
//	RESETCTRL485();		 				// 关闭发送DE端口，打开接收/RE485端口
//	sys_data_reset();					// 初始化任务相关变量
//	uart.rFlag   = OFF;					// 已处理
//	uart.rIndex  = 0;					// 接收个数清0



	Display_Clear();						// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
}


void SHT30_sys_Init(void)
{
	SHT30_read_result(0x44);
//	Delay_ms(80);
//	SHT3X_ReadMeasurementBuffer(&sht30_par.SHT30_Temprature,&sht30_par.SHT30_Humidity);
}
/**********************************************************************
**函数名:  主程序
**功能:    		
***********************************************************************/
#ifdef DEBUG_SYS_1US
INT16U usss = 1;

Debug_test_us(void)
{
	LED_WRN_ON(); 
//		Delay_ms(usss);
	Delay_us(usss);
	LED_WRN_OFF(); 
//		Delay_ms(usss);
	Delay_us(usss);	
}	
#endif	

//unsigned char communicationflag = 0;
int main(void)
{
	Board_Init();   		// 板级初始化，MCU资源的配置及初始化
	System_Init();			// 系统初始化，参数及开机显示
	SHT30_sys_Init();
	lcd_background.TIMER_SHT_ENABLED = 1;
	lcd_background.bar_interrupt_recover = 0;
//	communicationflag = 1;
	LCD_Scan();
	
	while(1)
	{
//		CMD_led_flash();		// LED灯珠扫描
		LCD_Scan();				// TFT根据刷新标志刷新,如无刷新标志则根据定时刷新标志定时刷新	
		check_state();		
//		state_test_485();
//	    Cmd_Process();				            //处理主机下发命令	// 和显示有冲突
	
	}
}

