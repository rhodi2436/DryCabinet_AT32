/******************************************************************************
* 文件名: Dry_Module.c
* 作者  : 黎昌其
* 版本  : V1.00
* 日期  : 2026-04-1
* 描述  : 加热吹风除湿算法

PTC    加热弹簧        打开门
云母片 加热干燥剂       吸收湿气
风扇   吸收干燥剂湿气   排出湿气

风扇排湿气时，湿度下降的最快，43%-->13%, 15分钟
********************************************************************************/

#include "../Head file/includes.h"

// 定义加热云母片和PTC的加热时间（单位：毫秒）
#define MICA_HEAT_TIME  1000 * 60 * 28
#define PTC_HEAT_TIME    1000 * 60 * 1
//#define PTC_HEAT_DELAY  1000 * 60 * 8

// 定义风扇转动的时间（单位：毫秒）
//#define FAN_ON_TIME    1000 * 60 * 3
//#define FAN_OFF_TIME  1000 * 60 * 7
//#define FAN_CYCLE_NUM 7

// 定义热循环的时间（单位：毫秒）
#define MICA_CYCLE_TIME 1000 * 12
#define MICA_DELAY_TIME 1000 * 108


// 定义加快吸湿部分的时间（单位：毫秒）
#define FAST_CYCLE_TIME  1000 *  60 * 60 * 7
#define FAST_RUN_TIME  1000 * 60 * 5
#define FAST_HEAT_DELAY 1000 *   60 * 60 * 2
//#define FAST_FAN_DELAY   1000 * 60 * 7
//#define FAST_FAN_ON_TIME 1000 * 60 * 7

// 定义除湿部分的时间（单位：毫秒）
#define MINUTE_TIME			(1000 * 60)

#define DRY_PTC_HEAT_TIME   (1000 * 60 * 5)
#define DRY_MICA_HEAT_TIME  (1000 * 60 * 20)	//10//28  //28	//烘干时间
#define DRY_MICA_OFF_DELAY  (1000 * 60 * 1) 	//15
#define DRY_PTC_OFF_DELAY   (1000 * 60 * 17)	//15//10  //8		//冷却时间

// 定义允许湿度上升的时间（单位：毫秒）
#define CHECK_RISE_TIME   (1000 * 60 * 5) //9//2	//12


#ifndef testApp
#define INIT_FAN_TIME	60 * 3
// 定义加热云母片和PTC的加热时间（单位：毫秒）
#define MICA_HEAT_TIME  1 60 * 28
#define PTC_HEAT_TIME    160 * 1
//#define PTC_HEAT_DELAY   60 * 8

// 定义风扇转动的时间（单位：毫秒）
//#define FAN_ON_TIME    1000 * 60 * 3
//#define FAN_OFF_TIME  1000 * 60 * 7
//#define FAN_CYCLE_NUM 7

// 定义热循环的时间（单位：毫秒）
#define MICA_CYCLE_TIME  12
#define MICA_DELAY_TIME 108


// 定义加快吸湿部分的时间（单位：毫秒）
#define FAST_CYCLE_TIME  60 * 60 * 7
#define FAST_HEAT_DELAY  60 * 60 * 2
//#define FAST_FAN_DELAY   1000 * 60 * 7
//#define FAST_FAN_ON_TIME 1000 * 60 * 7

// 定义除湿部分的时间（单位：毫秒）
#define DRY_PTC_HEAT_TIME   60 * 1
#define DRY_MICA_HEAT_TIME   60 * 28
#define DRY_MICA_OFF_DELAY   60 * 15
#define DRY_PTC_OFF_DELAY   60 * 8

// 定义允许湿度上升的时间（单位：毫秒）
#define CHECK_RISE_TIME   60 * 5

#endif



// 定义控制开关的状态
#define SWITCH_ON  	1
#define SWITCH_OFF 	0

//extern unsigned char communicationflag;
//INT8U flag_step0 = 1;
//INT8U flag_step1 = 1;
//INT8U flag_step2 = 1;
//INT8U flag_step3 = 1;
//INT8U flag_hotstar = 1;
//INT8U flag_dry = 0;

INT8U dry_step = 0;		// 烘干步骤

SHT30_Humidity


// 除湿的状态检测及处理，中断函数里调用，1模式调用1次
// void  ChkHumiditySts(void)
// {
	
// 	Dehumidification_steps_timer++;		// 步骤时间累计
// 	hotcycle_timer++;					// 烘干时间累计
	
// 	//	if((communicationflag)&&(sht30_par.SHT30_Humidity != 0))
// 	{
// 		switch (state) 
// 		{
// 			//--------------------------------------------------------------------
// 			// 上电状态，最长就开启风扇3分钟
// 			case STATE_INIT:
// 				// 刚上电时的初始化
// 				if (sht30_par.SHT30_Humidity >= para.str.SHT30_Humidity_SET + 3) 
// 				{	// 湿度大于H+3，开启风扇
// 					fan_switch(SWITCH_ON);
// 					if(Dehumidification_steps_timer >= INIT_FAN_TIME)	//3min， 1分钟
// 					{
// 						fan_switch(SWITCH_OFF);						
// 						//ptc_switch(SWITCH_ON);
// 						//LED_R_ON();
// 						//state = STATE_DRY;
// 						state = STATE_FAST;
// 						Dehumidification_steps_timer = 0;
// 					}
// 				} 
// 				else 
// 				{// 湿度小于H+3，进入加快吸湿部分
// 					state = STATE_FAST;
// 				}
// 			break;
// 			//--------------------------------------------------------------------
// 			// 正常情况下都是在该阶段，快速加湿状态（只开启风扇）
// 			case STATE_FAST:	
// 				// 加快吸湿部分

// 				if(Execute_onetime == 1)
// 				{	// 执行1次干燥回来，则保存当前湿度值
// 					Last_humidity = (int)(sht30_par.SHT30_Humidity);
// 					Execute_onetime = 0;
// 				}
				
// 				// if ( sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET + 3 ) 
// 				// {
// 				// 	Dehumidification_steps_timer = 0;	//防止在抽湿阶段开柜门湿度上升，无法及时进入到烘干
// 				// 	state = STATE_DRY;
// 				// }

// 				if (sht30_par.SHT30_Humidity >= para.str.SHT30_Humidity_SET + 1) 
// 				{ // 当前湿度大于设定湿度+1
					
// 					// 根据内部PTC是否打开来启动风扇
// 					if(READ_DOOR_BIT == 0)		//读PTC的口是否关闭， 0=打开
// 						fan_switch(SWITCH_OFF);	//风扇关
// 					else
// 						fan_switch(SWITCH_ON);	//开风扇
					
// 					// 湿度持续上升
// 					if(Last_humidity < (int)(sht30_par.SHT30_Humidity)) //增加检测阶段，如果湿度一致不降，超过一定时间，就会进入除湿阶段
// 					{	// 最小的湿度值
// 						//Humidity_rise_timer++;
// 						Humidity_rise_timer += ((int)sht30_par.SHT30_Humidity - Last_humidity);
// 					}
// 					else if	(Last_humidity > (int)(sht30_par.SHT30_Humidity))
// 					{
// 						// Last_humidity = (int)(sht30_par.SHT30_Humidity);
// 						//Humidity_rise_timer = 0;
// 						if(Humidity_rise_timer > 0)
// 						 Humidity_rise_timer--;
// 					}

// 					// 当湿度累计上升时间超过 5分钟
// 					if(Humidity_rise_timer > CHECK_RISE_TIME)	//5min
// 					{
// 						Humidity_rise_timer = 0;

// //						fan_switch(SWITCH_OFF);		// 关风扇
// 						//ptc_switch(SWITCH_ON);	//可能重复	修改为先预热云母片4分钟
// //						mica_switch(SWITCH_ON);		// 打开云母加热
// 						//LED_R_ON();
						
// 						// 切换状态 到烘干状态
// 						Dehumidification_steps_timer = 0;	// 定时清0
// 						dry_step = 0;						// 步骤从0步开始
// 						state = STATE_DRY;					// 进入烘干状态
// 						hotcycle_timer = 0;					// 烘干执行时间清0
// 						Execute_onetime = 1;
// //						flag_dry = 1;
						
// 					}					
// 				}								
// 				else if (sht30_par.SHT30_Humidity < para.str.SHT30_Humidity_SET - 1) 
// 				{	// 当前湿度大于设定湿度-1，关闭风扇
// 				 	fan_switch(SWITCH_OFF);
// 					Dehumidification_steps_timer = 0;
// 				} 
// 				else
// 				{	// 偏差在-1%~1%之间
// 					if(READ_DOOR_BIT == 0)		//内部门关//读PTC的口是否关闭
// 					{ fan_switch(SWITCH_OFF);}	//风扇关
// 				}
// //				else if(sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET + 2)
// //				{


// //				}
// 			break;
// 			//----------------------------------------------------------------------------------------
// 			// 开启干燥(烘干)模式
// 			case STATE_DRY:
				
// //				if(READ_DOOR_BIT == 0)  		//读PTC的口是否关闭, =0 打开
// //					fan_switch(SWITCH_OFF);
				
// 				sht30_par.Gled = 1;		// 干燥中，则绿色 LED 点亮
				
// 				switch( dry_step )
// 				{
// 					case 0:
// 						{
// 							fan_switch(SWITCH_OFF);				// 关风扇
// 							mica_switch(SWITCH_ON);				// 打开云母加热
// 							Dehumidification_steps_timer = 0;	// 步骤计时时间清0
// 							dry_step = 1;						// 步骤切换
// 						}
// 						break;
					
// 					case 1:					
// 						if (Dehumidification_steps_timer >= (MINUTE_TIME * 5))//DRY_PTC_HEAT_TIME)
// 						{	// 5分钟后，打开PTC加热, 打开弹簧
// 							ptc_switch(SWITCH_ON);				// 打开PTC加热				
// 							Dehumidification_steps_timer = 0;	// 步骤计时时间清0
// 							dry_step = 2;						// 步骤切换
// 						}
// 						break;
					
// 					case 2:
// 						if (Dehumidification_steps_timer >= (MINUTE_TIME))//(1000 * 60 * 1))
// 						{	// 1分钟后，打开加热云母片
// 							mica_switch(SWITCH_ON);				// 打开云母加热
// 							Dehumidification_steps_timer = 0;	// 步骤计时时间清0
// 							dry_step = 3;						// 步骤切换
// 						}	
// 						break;	

// 					case 3:
// 						if (Dehumidification_steps_timer >= (MINUTE_TIME * 20))//DRY_MICA_HEAT_TIME)
// 						{	// 20分钟后, 停止加热云母片
// 							mica_switch(SWITCH_OFF);			// 关闭云母加热
// 							Dehumidification_steps_timer = 0;	// 步骤计时时间清0
// 							dry_step = 4;						// 步骤切换
// 						}	
// 						break;	

// 					case 4:
// 						if (Dehumidification_steps_timer >= (MINUTE_TIME))//DRY_MICA_OFF_DELAY)
// 						{	// 1分钟后, 停止加热云母片
// 							ptc_switch(SWITCH_OFF);;			// 关闭PTC加热
// 							Dehumidification_steps_timer = 0;	// 步骤计时时间清0
// 							dry_step = 5;						// 步骤切换
// 						}						
// 						break;	

// 					case 5:
// 					default:
// 						if (Dehumidification_steps_timer >= DRY_PTC_OFF_DELAY)
// 						{	// 冷却17分钟后, 返回加快加湿部分
// 							Dehumidification_steps_timer = 0;
// 							state = STATE_FAST;
// 						}						
// 						break;							
// 				}
				
// 				// 如果湿度先达到设定值，则提前结束步骤
// 				if( (dry_step < 5)
// 				  &&(hotcycle_timer >= DRY_PTC_HEAT_TIME)
// 				  &&(sht30_par.SHT30_Humidity <= (para.str.SHT30_Humidity_SET + 1))
// 				  )
// 				{ // 当前湿度小于等于目标湿度+1，且 烘干时间超过 5分钟 则 停止烘干，进入加快吸湿部分
// 					ptc_switch(SWITCH_OFF);				// 关闭PTC加热
// 					mica_switch(SWITCH_OFF);			// 关闭云母加热
					
// 					dry_step = 5;						// 直接进入结束步骤
					
// 					// 延时1分钟进入加快吸湿模式
// 					Dehumidification_steps_timer = DRY_PTC_OFF_DELAY - DRY_MICA_OFF_DELAY;
// //					if(hotcycle_timer > DRY_PTC_HEAT_TIME*5)
// //					{ Dehumidification_steps_timer = 0; }
										
// 					//LED_R_OFF();
// 					//state = STATE_FAST;
// 				}				
				

// 				break;
// 			}

// 	}
// }