/************************************************************************
** 功能描述： 24c01-24c16读写驱动程序
*************************************************************************
** 模块名：   24cxx_module.c
** 输  入：   address：16Kbit(2Kbyte) 寻址空间 
** 输  出：
** 全局变量： 定时超时操作 
** 调用模块：
** 作  者：  黎昌其
** 日  期：  2014.8.6
** 修  改：
** 日  期：
** 版  本：   1.0
*************************************************************************
** 备 注:
** 16K 24c01-24c16 Device address

        bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0  
1K/2K	 1	   0	 1	   0	 A2	   A1	 A0	  R/W
4K 		 1	   0	 1	   0	 A2	   A1	 P0	  R/W
8K 		 1	   0	 1	   0	 A2	   P1	 P0	  R/W
16K 	 1	   0	 1	   0	 P2	   P1	 P0	  R/W

每页 2Kbit(256byte) == 8位寻址空间
P2/P1/P0：片内换页，A2/A1/A0：片间切换

** 读写切换处理：93C46在写完后可以直接读取，24CXX必须延时等待
** 考虑不同厂家器件可能存在区别，读写速度减半 2009-9-27 13:35

SERIAL CLOCK : (SCL)
 The SCL input is used to positive edge clock data into each
 EEPROM device and negative edge clock data out of each device.

SERIAL DATA : (SDA)
 The SDA pin is bidirectional for serial data transfer. 
 This pin is open-drain driven and may be wire-ORed with any 
 number of other open-drain or opencollector devices.

WRITE PROTECT : (WP)
 The AT24C01A/02/04/08A/16A has a Write Protect pin that
 provides hardware data protection. The Write Protect pin allows normal Read/Write
 operations when connected to ground (GND). When the Write Protect pin is connected
 to VCC, the write protection feature is enabled . 
 
 2012-6-27 9:20:01
 写保护必须在进来和出去，原来放在wd24()函数里不正确，造成AT24C08BN不能读写。
************************************************************************/

#include "../Head file/includes.h" 




// 保存参数
//#define	C24_ADDRESS_ADDR		(0x400+4)
#define	C24_ADDRESS_ADDR		(0x700+4)				 // 24C02 A0~A2均为1
#define	C24_FUNC1_ADDR			(C24_ADDRESS_ADDR + 1)
#define	C24_FUNC2_ADDR			(C24_ADDRESS_ADDR + 2)
#define	C24_FUNC3_ADDR			(C24_ADDRESS_ADDR + 3)
#define	C24_FUNC4_ADDR			(C24_ADDRESS_ADDR + 4)


/***********************************************************************/
//#define TimErr	Tim2.T2CNT		//超时定义


/************************************************************************
** 功能描述：START Data Transfer
 	A high-to-low transition of SDA with SCL high is a start condition
which must precede any other command

* Output Condition: scl = 1; scl = 1;
************************************************************************/
void s24(void) 
{ 		
	SETSDA_AT24();       //需在SCL之前设定
    SETSCL_AT24();       //硬件进入SDA检测状态
    Delay_us(5);   	 	 //延时至少4.7us
    RESETSDA_AT24();     //SDA由1->0,产生开始信号
    Delay_us(5);    	 //延时至少4us
    RESETSCL_AT24();     //SCL变为无效
}

/************************************************************************
** 功能描述： Stop Data Transfer
	A low-to-high transition of SDA with SCL high is a stop condition.
After a read sequence, the stop command will place the EEPROM in a standby 
power mode

* Output Condition: scl = 1; scl = 1;
************************************************************************/
void p24(void) 
{	
    RESETSDA_AT24();     //在SCL之前拉低
    SETSCL_AT24();       //硬件进入SDA检测状态
    Delay_us(5);    //至少延时4us
    SETSDA_AT24();       //SDA由0->1,产生结束信号
    Delay_us(5);    //延时至少4.7us
}

/************************************************************************
** 功能描述：复位24C08
************************************************************************/
void Ini_24C08(void)
{	
	INT16U	i;	

	s24();
	//24C08复位	
	for(i = 9;i > 0;i--)
	{				//用djnz指令完成循环	
		SETSCL_AT24()  ;Delay_us(10);
		SETSDA_AT24()  ;Delay_us(10);		
		RESETSCL_AT24();Delay_us(10);
	}
	s24();
	p24();
}


/*******************************************************************************
* 函数名  : EEPROM_Init
* 描述    : EEPROM初始化
*******************************************************************************/
void EEPROM_Init(void)
{

	Ini_24C08();
	DISWRITE_AT24();			   // 禁止写入
}

/************************************************************************
** 功能描述：字节读时序 
** 输入参数: void		             
** 输出参数: 返回8位数据
** 时序: SCL下降沿输出数据,数据改变必须在SCL低电平时
		 SCL 8个下降沿       
** data change at scl = 0;;
************************************************************************/

INT8U rd24(void) 
{	
	INT8U i,dat = 0;


	RESETSCL_AT24();Delay_us(4);
	SETSDA_AT24()  ;Delay_us(1);		//写1，做为输入 防止钳位
	
	for(i = 8; i > 0; i--)
	{			//这样写就会用djnz指令
		SETSCL_AT24(); Delay_us(2);
		(0 != READSDABIT_AT24()) ? (dat = ((dat << 1) | 0x01)) 
				   : (dat = ((dat << 1) & 0xfe));
		Delay_us(3) ;
		RESETSCL_AT24(); Delay_us(5);
	}
	SETSCL_AT24(); Delay_us(3);
	SETSDA_AT24(); Delay_us(2);
	RESETSCL_AT24(); Delay_us(3);
	return(dat);
}   
    
/************************************************************************
** 功能描述：字节写时序 ，fCLK Max ＝ 400KHz
** 输入参数: 8位数据dat		             
** 输出参数: 返回bit, 代表写入是否正确, true则写入ok ,false则写入失败 
** 时序: SCL上升沿传送数据,数据改变必须在SCL低电平时
		 SCL 9个上升沿 其中8个为传送数据, 第9个为检测应答信号用        
** 检测应答信号,并作10ms的超时保护
** data change at scl = 0;
************************************************************************/

INT8U wd24(INT8U dat) 
{		
	INT8U i;
	INT8U flg;
	
	ENWRITE_AT24();	//Delay1us(10);	// 允许写
	for(i = 8; i > 0; i--)			// 这样写就会用djnz指令
	{				
		RESETSCL_AT24(); Delay_us(2);
		
		//编译器漏洞：加括号，确保 & 运算在 == 之前
		if((dat  & 0x80) == 0x80) 
			{ SETSDA_AT24(); }
		else{ RESETSDA_AT24(); }

		dat <<= 1;Delay_us(3) ;
		
		SETSCL_AT24(); Delay_us(5);	
	}
	
	//检测应答信号
	//ACKNOWLEDGE：写数据时，应答信号由24LCXX发出 
	RESETSCL_AT24();Delay_us(4);
	SETSDA_AT24();  Delay_us(1);
	SETSCL_AT24();  Delay_us(5);
	
	//While the SCL is high,then ADDRESS OR ACKNOWLEDGE VALID
	flg = FALSE;
	for(i = 200; i > 0; i--)	//这样写就会用djnz指令
	{						
		if(READSDABIT_AT24() == 0)
		{flg = TRUE; break;}			//Acknowledge Succ
		else{ Delay_us(250); }			//250us
	}
	
	RESETSCL_AT24();Delay_us(3);
	SETSDA_AT24();//Delay1us(2);
//	pWP2408  = 1;Delay1us(5);			//禁止写
	return(flg);						//Acknowledge Err
}   
     
/************************************************************************
** 功能描述： Random Read ,任意地址读出
** 输入参数:  16位地址address(取11位) 	
** 输出参数:  返回b8位的数据
** 时序 	   :  START + Device address + Word address + 
              START + Device address(读) + Data + STOP 
 时间为476us             
************************************************************************/

INT8U read1_24cxx(INT16U address)
{	
	INT8U dat;

	dat = ((address&0x7ff)/256) << 1;
			
	s24();									//START               
	if( (FALSE == wd24(0xa0|dat))			//Write Device Address
	  ||(FALSE == wd24((INT8U)address)) )	//Write Word Address  	 
	{ p24();return(0xAA); }   
	 		  		
	s24();                              	//START
	if(FALSE == wd24(0xa1|dat))		 
	{ p24();return(0xAA); }    				//Write Device Address(读)               
	dat = rd24();							//Read 
	p24();                      			//STOP

	return(dat);
}
/************************************************************************
** 功能描述： 任意地址读出
** 输入参数:  16位地址address(取11位) 	
** 输出参数:  返回b8位的数据        
************************************************************************/
INT8U read_24cxx(INT16U address)
{	
	INT8U dat;

	ENWRITE_AT24(); Delay_us(20);		// 允许写
	
	dat = read1_24cxx(address);

	DISWRITE_AT24(); Delay_us(20);		// 禁止写
	return(dat);
}
/************************************************************************
** 功能描述：写一个字节进24C08指定地址，Byte Write ,字节写入24C08 
** 输入参数: 16位地址address(取11位),高8位为硬件地址，低8位为RAM地址
             8位数据dat		
** 输出参数: 返回写入状态 
             true则写入ok;                 false则写入Memory A失败;

** 时序    : START + Device address + Word address + Data + STOP 
时间为348us
************************************************************************/
INT8U write1_24cxx(INT16U address,INT8U dat)
{	
	INT8U page;
	INT8U  flg = TRUE;
	page = ((address&0x7ff)/256) << 1;
			
	s24();									//START
	if( (FALSE == wd24(0xa0|page))			//Write Device Address
	  ||(FALSE == wd24((INT8U)address))		//Write Word Address
	  ||(FALSE == wd24(dat)) )	 			//Write Data
	{ flg = FALSE; }	
	p24();                    				//STOP

	return(flg);
}

/************************************************************************
** 功能描述：Byte Write ,字节写入24C08 ,写入2个24C08
** 输入参数: 16位地址address(取11位),高8位为硬件地址，低8位为RAM地址
             8位数据dat		
** 输出参数: 代表写入是否正确, 
             00则写入ok;                 01则写入Memory A失败;
             02则写入Memory B失败;       03则写入Memory A B失败
** 时序    : START + Device address + Word address + Data + STOP  

备注:
** 预置重写次数 3次，每次写完毕后读取当前写入字节，如不符则重写
** 改为先读出值如果和要写入一致则不再写入
** 改为A保存功能和灵敏度值，B为参数记录
************************************************************************/
// 地址可代表要写入哪个24C08
// 如果写完再读，发现读出有错，要读2次才对
// 读3次写一次的时间为6。88ms

INT8U write_24cxx(INT16U address,INT8U dat)
{	
	INT8U  again;
	INT8U  stats;
    stats = 0xff;
	
	ENWRITE_AT24(); Delay_us(20);		// 允许写
   	                                   			
	for( again = 0;again < 3;again++ )
	{ 	//最大重写次数 3		
	    //Write A 	       
		if(dat != read1_24cxx(address))
		{
			stats = 2;
			if( FALSE == write1_24cxx(address,dat) )
			{ stats += 1; }	
			read1_24cxx(address);
		}
		else{stats = 0;break;}					
	}
		
	DISWRITE_AT24(); Delay_us(20);		// 禁止写
	return(stats);
}

/*******************************************************************************
	 		   检查参数是否超出范围，超出则恢复默认值,并存入备份区
	input:   none
	output:  none					
*******************************************************************************/
//												1            2            3            4			 5			   6
//INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR,MAX_FINISHEDLED,MAX_WTTIME,MAX_FNDEFINE,MAX_DISPMODE,MAX_KEYPROCLED };
//INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR,DEF_FINISHEDLED,DEF_WTTIME,DEF_FNDEFINE,DEF_DISPMODE,DEF_KEYPROCLED };

//INT8U	CODE	Para_MaxTB[] = { MAX_ADDR ,MAX_FINISHEDLED ,MAX_WTTIME ,MAX_FNDEFINE ,MAX_DISPMODE ,MAX_KEYPROCLED };
//INT8U	CODE	Para_DefTB[] = { DEF_ADDR ,DEF_FINISHEDLED ,DEF_WTTIME,DEF_FNDEFINE  ,DEF_DISPMODE ,DEF_KEYPROCLED };

//INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR ,MAX_FINISHED_LED_DISP };//,MAX_KEYPROCLED };
//INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR ,DEF_FINISHED_LED_DISP };//,DEF_KEYPROCLED };

void	ChkParaOverToDefVal(void)
{
	UINT8 i;

	// 判断参数，如果超出范围则回复默认值

//	for (i = 0; i < MAX_CFGPARA; i++)
//	{ 
//	  	// 判断是否超出范围
//		if(para.Byte[i] > Para_MaxTB[i])
//	  	{ para.Byte[i] = Para_DefTB[i]; }

//		para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
//	}

//    // 判断1个字节存两个参数的值是否越界，就判断低4位即可    
//    if( (para.str.FinishedLED & 0x0f) > MAX_FINISHEDLED )
//    { para.str.FinishedLED = (para.str.FinishedLED & 0xf0) + DEF_FINISHEDLED; }
//    para_bk.str.FinishedLED = para.str.FinishedLED;
}

/*******************************************************************************
	 		    		Load Parameter from EEPROM
	input:   none		   
	output:  none					
*******************************************************************************/


//void LoadParaInEeprom(void)
//{
//	// 读出参数
//	INT8U	i;
//	for(i = 0;i < MAX_CFGPARA;i++)
//	{ para.Byte[i] = read_24cxx(C24_ADDRESS_ADDR+i); }

//	// 检查参数是否溢出，是则恢复，并备份入备份区中
//	if( (para.str.address == 0) )//||(addr == 0) )  
//    { para.str.address = DEF_ADDR;  }     //默认加载参数

//	ChkParaOverToDefVal();			   // 检查参数是否超出范围，超出则恢复默认值,并存入备份区
//}

/*******************************************************************************
	 		    		save Parameter to EEPROM
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock(void)
{

	INT8U i;
	// 检查参数是否溢出，是则恢复，并备份入备份区中
	ChkParaOverToDefVal();
	
	// 写入	eeprom	
	for(i = 0;i < MAX_CFGPARA;i++)
	{ 
        write_24cxx(C24_ADDRESS_ADDR+i,para.Byte[i]); 
    }

//	LCD_Dsip_Mode();		// 设置显示模式
}

//=============================================End=================================










