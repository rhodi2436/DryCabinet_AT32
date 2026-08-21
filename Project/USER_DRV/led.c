/************************************************************************
** 功能描述：LED 指示模块
*************************************************************************
** 器件描述：红绿蓝状态灯，和通讯状态指示灯
** 作  者：  黎昌其
** 日  期：  2014.8.6
** 版  本：  1.0 
*************************************************************************
** 备 注:
************************************************************************/
#include "../Head file/includes.h" 


/*******************************************************************************
	 					清除所有显示
	input:   none     
	output:  none					
*******************************************************************************/
void LED_ALL_OFF(void)
{
    led.light_type = LED_NONE;
	led_light_ctrl(LED_NONE);
}

/*******************************************************************************
	 					打开所有显示
	input:   none     
	output:  none					
*******************************************************************************/
void LED_ALL_ON(void)
{
	led.light_type = LED_RGB;
	led_light_ctrl(LED_RGB);
}



/*******************************************************************************
	                    给当前显示led供电            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/

void led_light_ctrl(INT8U ucValue)
{
	switch (ucValue)
	{
		case LED_RED:	LED_G_OFF(); LED_B_OFF(); LED_R_ON();  break;		
		case LED_GREEN:	LED_R_OFF(); LED_B_OFF(); LED_G_ON();  break;		
		case LED_BLUE:	LED_R_OFF(); LED_G_OFF(); LED_B_ON();  break;

		case LED_RG:	LED_B_OFF(); LED_R_ON();  LED_G_ON();  break;		
		case LED_RB:	LED_G_OFF(); LED_R_ON();  LED_B_ON();  break;		
		case LED_GB:	LED_R_OFF(); LED_G_ON();  LED_B_ON();  break;	
		case LED_RGB:	LED_R_ON();  LED_G_ON();  LED_B_ON();  break;			
		default:		LED_R_OFF(); LED_G_OFF(); LED_B_OFF(); break;
	}
}

/*******************************************************************************
	 					获取灯亮时间，单位ms
	参数说明
	    led_flash_period:      闪烁时间间隔
	    led_flash_mode:        闪烁模式	     
	output:  返回亮灯时间					
*******************************************************************************/

//UINT16 GetLedOnTime(void)
void GetLedOnTime(void)
{
	UINT16 unRet = led.flash_period;
	switch(led.flash_mode)
	{
//		case LED_FLASH_MODE_ALWAYS:		break;					  		//0 常亮模式，不会调用此函数
//		case LED_FLASH_MODE_1V1:								  		//1 表示亮1*100ms，灭1*100ms；
//		case LED_FLASH_MODE_1V2:								  		//2 表示亮1*100ms，灭2*100Tms；
//		case LED_FLASH_MODE_1V5:								  		//3	表示亮1*100ms，灭5*100ms；
//		case LED_FLASH_MODE_1V10:	unRet = led_flash_period; 	break;	//4	表示亮1*100ms，灭10*100ms；
		case LED_FLASH_MODE_2V1:  	unRet = unRet*2;	break;	//5 表示亮2*100ms，灭1*100ms；
		case LED_FLASH_MODE_5V1:	unRet = unRet*5; break;	//6 表示亮5*100ms，灭1*100ms；
		case LED_FLASH_MODE_10V1:  	unRet = unRet*10;break;	//7 表示亮10*100ms，灭1*100ms；
		default:                                break;
	}
//	return( unRet );
	led.time_refresh = unRet;		// 开灯闪烁时间
}
/*******************************************************************************
	 					获取灯灭时间，单位ms
	参数说明
	    led_flash_period:      闪烁时间间隔
	    led_flash_mode:        闪烁模式	   
	output:  返回灭灯时间					
*******************************************************************************/

//UINT16 GetLedOffTime()
void GetLedOffTime(void)
{
	UINT16 unRet = led.flash_period;
	switch(led.flash_mode)
	{
//		case LED_FLASH_MODE_ALWAYS:		break;					  		//0 常亮模式，不会调用此函数
//		case LED_FLASH_MODE_1V1:								  		//1 表示亮1*100ms，灭1*100ms；
//		case LED_FLASH_MODE_2V1:  										//5 表示亮2*100ms，灭1*100ms；
//		case LED_FLASH_MODE_5V1:										//6 表示亮5*100ms，灭1*100ms；
//		case LED_FLASH_MODE_10V1: unRet = led_flash_period;		break; 	//7 表示亮10*100ms，灭1*100ms；
		case LED_FLASH_MODE_1V2:  unRet = unRet*2;	break;	//2 表示亮1*100ms，灭2*100Tms；
		case LED_FLASH_MODE_1V5:  unRet = unRet*5;	break;	//3	表示亮1*100ms，灭5*100ms；
		case LED_FLASH_MODE_1V10: unRet = unRet*10;	break;	//4	表示亮1*100ms，灭10*100ms；
		default:							break;
	}
//	return( unRet );
	led.time_refresh = unRet;		// 关灯闪烁时间
}
/*******************************************************************************
	 					指示灯闪烁控制模式
参数说明  
	    led_flash_period:      闪烁时间间隔
	    led_flash_NO:          闪烁灯序号
	    led_flash_mode:        闪烁模式					
*******************************************************************************/
void CMD_led_flash(void)
{
    //LED指示灯控制
    if( led.light_type == LED_NONE )	//当前为灭
	{  led_light_ctrl(LED_NONE); return; }

	//常亮显示
	if(led.flash_mode == LED_FLASH_MODE_ALWAYS)	
	{ led_light_ctrl(led.light_type); return; }

	//闪烁显示
	if(led.time_refresh == 0)
	{ // 时间到状态改变
	  //当前灯为灭状态
		if(led.flash_status == 0)
		{
			//切换到灯亮状态，并且设置改变灯亮周期参数
			led.flash_status = 1;			// 切换到灯亮状态
			led_light_ctrl(led.light_type);	// 仅点亮对应颜色			
			GetLedOnTime();					// 取亮灯时间，系统时钟里自减到0
		}
		//当前为亮灯
		else
		{
			//切换到灯灭状态，并且设置改变灯灭周期参数
			led.flash_status = 0;			//切换到灯亮状态
			led_light_ctrl(LED_NONE);		// 全灭	
			GetLedOffTime();	// 取灭灯时间，系统时钟里自减到0
		}
	}
}

/*******************************************************************************
								END					
*******************************************************************************/

