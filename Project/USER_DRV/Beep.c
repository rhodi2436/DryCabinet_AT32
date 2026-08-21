/************************************************************************
** 功能描述：蜂鸣器配置，控制
*************************************************************************
** 模块名：  蜂鸣器模块 
** 作  者：  黎昌其
** 日  期：  2014.8.6 
** 修  改：
** 日  期：
** 版  本：  1.0 
*************************************************************************
** 备 注:
************************************************************************/
//#include  "../Head file/Glob.h"
#include "../Head file/includes.h" 

/*--------------------------------pins define--------------------------*/




/************************************************************************
** 功能描述：蜂鸣器打开		  
************************************************************************/
void BEEP_OPEN(void)
{
	if(FALSE == Sound.BellON)
	{
	  Sound.BellON = TRUE;
//	  SysTick_ITConfig(ENABLE);					// 开中断
	  Sound.CtlSts = TRUE;
	} 
}

/************************************************************************
** 功能描述：蜂鸣器关闭		  
************************************************************************/
void BEEP_CLOSE(void)
{
//	SysTick_ITConfig(DISABLE);					// 关中断
	Sound.BellON = FALSE;						// 清蜂鸣器打开标志
//	BEEP_OFF();									// 关硬件
}



//------------------------------------------------------------------------
//表里的数据格式意义
//ALMSTP代表结束 
//最高位代表开关峰鸣器 低7位代表开通时间= 低7位*1ms
const INT8U  Almdb_key[] = {0x26,ALMSTP}; //按键声音 38ms
const INT8U  Almdb_err[] = {0x6E,0xEE,0x6E,0xEE,0x6E,ALMSTP};   //出错报警声 110ms,3响2停	 			
const INT8U  Almdb_sel[] = {0x64,0x64,0x64,0x64,ALMSTP};//分版声 400ms
//const INT8U  Almdb_bat[] = {0xFF,0xff,0xFF,0xff,0x4A,0xCA,0x4A,0xCA,0x4A,0xFF,0xff,0xff,0xff,0xFF,0xff,0xff,0xff,ALMSTP};   //预置数不够 74ms,3响2停，间隔1.5S	
const INT8U  Almdb_bat[] = {0x7E,0xA0,0x7E,0xA0,0x7E,ALMSTP};   //127ms响,32ms停，3响2停
const INT8U  Almdb_cnt[] = {0xFF,0xFF,0x28,0xA8,0x28,0xA8,0x28,0xA8,0x28,0xA8,0x28,ALMSTP};   //计数不准报警声// 2013-03-11 加入
/************************************************************************
* 峰鸣器鸣叫方式设置
************************************************************************/
void AlarmSet(INT8U Type)
{		
	Sound.Next  	= 0x00;					//表偏移量初始化
	Sound.TimCnt 	= 0x00;					//计时器初始化	
	Sound.Type 		= Type;					//峰鸣器鸣叫声类型			
}

/************************************************************************
* 对数据表的意义进行重新定义
* Almdo 在中断里调用，注意临界函数
************************************************************************/
void Almdo(void)
{		
	INT8U tmp;
				
	//蜂鸣器停止模式	
	if( ALMSTP == Sound.Type ) 	
	{ BEEP_CLOSE(); return ; }			

	//蜂鸣器运行模式			   		
	tmp = Sound.Next;			
   	switch(Sound.Type)
   	{		   					
		case	ALMKEY: tmp = Almdb_key[tmp];	break;
		case	ALMERR: tmp = Almdb_err[tmp];	break;					
		case	ALMSEL: tmp = Almdb_sel[tmp];	break;
		case	ALMBAT: tmp = Almdb_bat[tmp];	break;	
		case	ALMCNT: tmp = Almdb_cnt[tmp];	break;				
		default	 : 		tmp = ALMSTP;			break;	// 结束符											
    }	
    //判断是否结束
	if(ALMSTP == tmp)
	{ Sound.Type = ALMSTP; BEEP_CLOSE(); return; }
	//蜂鸣器有节奏的开关
	//根据高位开关蜂鸣器
	if(0 == (tmp&0x80)) { BEEP_OPEN(); }
	else				{ BEEP_CLOSE(); }	
							   
	if(Sound.TimCnt++ >= ((tmp&0x7F)*2))
	{					
		Sound.TimCnt = 0x00;	//备份系统时钟    	   
		Sound.Next ++ ; 		//表偏移＋1
	}	 	   
}


