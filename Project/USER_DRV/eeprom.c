 /*=============================================================================
// 模块说明

外部调用函数
void LoadParaInEeprom(void):  从EEPROM读出参数，如果参数超出范围则恢复出厂设置  
void SaveParaToBlock(void):   保存参数进入EEPROM区，如果参数超出则恢复默认值再写入
                              写入3个区域，一次全参数写入
                              加入根据参数，设置显示内容

内部函数
void ChkParaOverToDefVal(void): 检查参数是否超出范围，超出则恢复默认值,并存入备份区
void ReadParaInEeprom(void):   读出3片区域，如果有不一致的则取校验和正确那块区域，
                               如果都不一致取第一片数据
==============================================================================*/

#define AP_EPROM_GLOBALS	
#include  "../Head file/includes.h"


uint16_t RdPageBuff[PAGE_SIZE/2];	//最多是2K字节
uint16_t WrPageBuff[PAGE_SIZE/2];	//最多是2K字节

//读取指定地址的半字(16位数据)
//ReadAddress:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
static uint16_t FLASH_ReadHalfWord(uint32_t ReadAddress)
{
	return *(uint16_t*)ReadAddress; 
}

/*************************************************************************
	检查用哪个块，因为每个块最大写入次数为1万次
	with verify
**************************************************************************/
u16 ChkVliadBlock(void)
{
	u16 i,WriteCnt,addr;
	for( i = 0; i < PAGE_NUM; i++ )
	{
		addr = i*PAGE_SIZE;
		WriteCnt = FLASH_ReadHalfWord( EEPROM_START_ADDRESS + addr );
        if(( WriteCnt < 9000 )||( WriteCnt == 0xffff ))
		{ break; }	
	}
	return(addr);
}

/*************************************************************************
	                    Read len bytes from eeprom
// 整页读取
// beginAddr:起始地址
**************************************************************************/

void eeprom_read_block( UINT16 beginAddr )
{
    u16 i;
	u32 PageAddr = beginAddr+EEPROM_START_ADDRESS;
    for (i = 0; i < PAGE_SIZE/2; i++)
    {
        RdPageBuff[i] = FLASH_ReadHalfWord(PageAddr);
		PageAddr += 2;//偏移2个字节.
    }   
}

/*************************************************************************
	                    write len bytes to eeprom
**************************************************************************/

void eeprom_write_block(UINT16 beginAddr)
{
	u16 i;  
	u32 PageAddr = beginAddr+EEPROM_START_ADDRESS;
    /* 解锁FLASH */
    FLASH_Unlock();	
	
	// 擦除这个扇区
	FLASH_ErasePage( PageAddr );
	
	//写入整个扇区    
	for(i = 0;i < PAGE_SIZE/2; i++)
	{
		// 不检查的写入
		FLASH_ProgramHalfWord(PageAddr,WrPageBuff[i]);
	    PageAddr+=2;//地址增加2.
	} 
	
	/* 上锁FLASH */	
    FLASH_Lock();
}


/*************************************
	Read Parameter from inn EEPROM
	with verify
*************************************/

void ReadParaInEeprom(void)
{
	u16 i,data;
    u16 sum1 = 0,sum2 = 0,sum3 = 0;
	u16 beginAddr;
	
    //EEPROM解除保护。
    FLASH_Unlock();	
	// 检查用哪一块数据
	beginAddr = ChkVliadBlock();
	// 读出区域
    eeprom_read_block( beginAddr );
    //EEPROM加上保护。
    FLASH_Lock();	
	
    // 取校验和符合要求的区域数据,异或值
    for (i = 0;i < (MAX_CFGPARA+1); i++)
    {
      sum1 ^= RdPageBuff[i+SECTOR_0_ADDR];
      sum2 ^= RdPageBuff[i+SECTOR_1_ADDR];
      sum3 ^= RdPageBuff[i+SECTOR_2_ADDR];
    }    

    // 取校验和正确的区域数值，存入参数区
    for(i = 0;i < (MAX_CFGPARA+1); i++)
    {    
        if ( sum1 == 0 )
        { data = RdPageBuff[i+SECTOR_0_ADDR]; }
        else if ( sum2 == 0 )
        { data = RdPageBuff[i+SECTOR_1_ADDR]; }
        else 
        { data = RdPageBuff[i+SECTOR_2_ADDR]; }

        para.Byte[i] = (u8)data;
    }	
}

/*******************************************************************************
	 		   检查参数是否超出范围，超出则恢复默认值,并存入备份区
	input:   none
	output:  none					
*******************************************************************************/
//												0          1 合并2个 参数        2             3            4			  5			      6               7                8             9
INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR ,	MAX_HUMIDITYSET,	MAX_HUM_CAL_SET,	MAX_TEM_CAL_SET,	MAX_LANGUAGE_SWITCH };//,MAX_KEYPROCLED };
INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR ,	DEF_HUMIDITYSET,	DEF_HUMCALSET,		DEF_TEMCALSET,		DEF_LANGUAGE_SWITCH };//,DEF_KEYPROCLED };

void	ChkParaOverToDefVal(void)
{
	UINT8 i;

	// 判断参数，如果超出范围则回复默认值

	for (i = 0; i < MAX_CFGPARA; i++)
	{ 
	  	// 判断是否超出范围，超出则恢复默认值
		if(para.Byte[i] > Para_MaxTB[i])
	  	{ para.Byte[i] = Para_DefTB[i]; }

		if(para.Byte[i] < Para_DefTB[i])
	  	{ para.Byte[i] = Para_DefTB[i]; }

		para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
	}
    
    // 判断1个字节存两个参数的值是否越界，就判断低4位即可    
    if( (para.str.FinishedLED & 0x0f) > MAX_FINISHEDLED )
    { para.str.FinishedLED = (para.str.FinishedLED & 0xf0) + DEF_FINISHEDLED; }
    para_bk.str.FinishedLED = para.str.FinishedLED;
    
}
/*******************************************************************************
	函数说明： 		   保存参数进入EEPROM区，如果参数超出则恢复默认值再写入
                    写入3个区域，一次全参数写入
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock(void)
{
	u16 i,dat,sum = 0;
	u16 beginAddr;
    
	// 检查参数是否溢出，是则恢复，并备份入备份区中
	ChkParaOverToDefVal();
		
	
    // 计算校验和
    for (i = 0; i < MAX_CFGPARA; i++)
    { 
		dat = para.Byte[i];
		sum ^= dat; 
		WrPageBuff[i+SECTOR_0_ADDR] = dat;
		WrPageBuff[i+SECTOR_1_ADDR] = dat;
		WrPageBuff[i+SECTOR_2_ADDR] = dat;
    }
	WrPageBuff[MAX_CFGPARA+SECTOR_0_ADDR] = sum;
	WrPageBuff[MAX_CFGPARA+SECTOR_1_ADDR] = sum;
	WrPageBuff[MAX_CFGPARA+SECTOR_2_ADDR] = sum;

    
	// 检查用哪一块数据
	beginAddr = ChkVliadBlock();
	// 读出区域
    eeprom_read_block(beginAddr);	
		
	// 写入	eeprom
    // 取校验和正确的区域数值，存入参数区
    for(i = 0;i < MAX_CFGPARA+1; i++)
    {    

		if ( (WrPageBuff[i+SECTOR_0_ADDR] != RdPageBuff[i+SECTOR_0_ADDR])
		   ||(WrPageBuff[i+SECTOR_1_ADDR] != RdPageBuff[i+SECTOR_1_ADDR])
		   ||(WrPageBuff[i+SECTOR_2_ADDR] != RdPageBuff[i+SECTOR_2_ADDR]) )
		{ break; }
    }
	
	// 如果待写入的数据和读取的数据不一致则写入
	if( i < MAX_CFGPARA+1)
	{
		WrPageBuff[0] = RdPageBuff[0]+1;	//写入次数加一
		eeprom_write_block (beginAddr);
	}
    // 根据设定值改变库位位数和数量位数  
//	sys_disp_set();		     
}

/*******************************************************************************
	函数说明： 		   从EEPROM读出参数，如果参数超出范围则恢复出厂设置                
	input:   none		   
	output:  none					
*******************************************************************************/
void LoadParaInEeprom(void)
{
	u8 i;  


    // 读出所有参数
    ReadParaInEeprom();                    


	// 检查参数是否溢出，是则恢复，并备份入备份区中
	if( (para.str.address == 0) )//||(addr == 0) )  
    {   
      // 恢复默认值
      for (i = 0; i < MAX_CFGPARA; i++)      
	  { para.Byte[i] = Para_DefTB[i]; }
      
      SaveParaToBlock();      
    }     //默认加载参数

	ChkParaOverToDefVal();
}

/*******************************************************************************
								END					
*******************************************************************************/


