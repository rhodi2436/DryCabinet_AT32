/******************************************************************************
* 文件名: FSMC_NOR_FLASH.c
* 作者  : xiaoweixian
* 版本  : V1.00
* 日期  : 19/07/2012
* 描述  : M29W128FH NOR FLASH驱动以及初始化.
********************************************************************************/
//#include "../Head file/FSMC_NORFlash.h"
#include "../Head file/includes.h" 

/*******************************************************************************
* Function Name  : FSMC_DMA_Init
* 功能：初始化FSMC的DMA
* 参数：PeripheralBaseAddr外设地址，即图片存储地址
		BufferSize为DMA传输数据量
*******************************************************************************/
extern void  DMA_NVICConfig(void);
void FSMC_DMA_Init(INT32U PeripheralBaseAddr,INT32U BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA channel6 configuration */
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(PeripheralBaseAddr+Bank1_NOR2_ADDR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TFT_DateBaseADDR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	//允许发送完成中断
	DMA_NVICConfig();
}

/*******************************************************************************
* Function Name  : FSMC_DMA_Init
* 功能：初始化FSMC的DMA
* 参数：PeripheralBaseAddr外设地址，即图片存储地址
		BufferSize为DMA传输数据量
*******************************************************************************/
extern void  DMA5_NVICConfig(void);
void FSMC_DMA5_Init(INT32U PeripheralBaseAddr,INT32U BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA channel6 configuration */
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(PeripheralBaseAddr+Bank1_NOR2_ADDR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TFT1_DateBaseADDR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	//允许发送完成中断
	DMA5_NVICConfig();
}
/*******************************************************************************
* 函数名  : FSMC_NORFLASH_Init
* 描述    : M29W128FH NOR FLASH 驱动初始化配置
*******************************************************************************/
void FSMC_NORFLASH_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p,p1;

	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
	                     RCC_APB2Periph_GPIOF, ENABLE);
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/* FSMC Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
	                            GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* FSMC Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                            GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);		 // A0~A9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                            GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);		 // A10~A15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);		 // A16~A18

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);		 // A19~A22
	
	/* NOE and NWE configuration */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* NE3 NE2 NE4configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* NBL0, NBL1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	/*-- NOR FLASH Configuration ------------------------------------------------------*/
	p.FSMC_AddressSetupTime = 1;
	p.FSMC_AddressHoldTime = 0;//0
	p.FSMC_DataSetupTime = 2;//2
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;//FSMC_AccessMode_B;//
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;		//禁止数据地址复用
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;					//存储器类型，NOR FLASH
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;			//存储数据宽带，8位和16位
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;		//禁止突发访问模式
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;	//等待信号极性
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;			//写使能位，1允许写入
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;			//读写时序一样
	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	/* Enable FSMC_Bank1_NORSRAM2 */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE); 


	/*-- SRAM （TFT2.0）Configuration ------------------------------------------------------*/
	// 读的时间
	p.FSMC_AddressSetupTime = 2;
	p.FSMC_AddressHoldTime = 0;
	p.FSMC_DataSetupTime = 30;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	// 写的时间
	p1.FSMC_AddressSetupTime = 1;//0;
	p1.FSMC_AddressHoldTime = 0;//1;
	p1.FSMC_DataSetupTime = 3;//9;
	p1.FSMC_BusTurnAroundDuration = 0;
	p1.FSMC_CLKDivision = 0;
	p1.FSMC_DataLatency = 0;
	p1.FSMC_AccessMode = FSMC_AccessMode_A;//FSMC_AccessMode_B;//
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	  //数据地址复用
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;				  //存储器类型
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		  //存储数据宽带，8位和16位
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	  //突发访问模式
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //等待信号极性
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	   //写使能位，1允许写入
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;	   //扩展模式使能位，1使能读写时序独立，0读写一致
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;	   //扩展模式使能位，1使能读写时序独立，0读写一致  ，这里读写时序不同
//	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p1;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	/*-- TFT 3.5 Configuration ------------------------------------------------------*/
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE); 
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

	// NorFlash-->TFT3.5	
//	FSMC_DMA_Init(Bank1_NOR2_ADDR,0);	  /* DMA Config */	
//	DMA_Cmd(DMA1_Channel6, ENABLE);		  /* ENABLE DMA  */
	// NorFlash-->TFT2.0
//	FSMC_DMA5_Init(Bank1_NOR2_ADDR,0);
//	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/******************************************************************************
* Function Name  : FSMC_NOR_GetStatus
* Description    : Returns the NOR operation status.
* Input          : - Timeout: NOR progamming Timeout
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_GetStatus(u32 Timeout)
{ 
	u16 val1 = 0x00, val2 = 0x00;
	NOR_Status status = NOR_ONGOING; 
	u32 timeout = Timeout;
	
	/* Poll on NOR memory Ready/Busy signal ------------------------------------*/
	while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) != RESET) && (timeout > 0)) 
	{	timeout--;	 }
	
	timeout = Timeout;
	
	while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == RESET) && (timeout > 0))   
	{	timeout--;	 }
	
	/* Get the NOR memory operation status -------------------------------------*/
	while((Timeout != 0x00) && (status != NOR_SUCCESS))
	{
		Timeout--;
		/* Read DQ6 and DQ5 */
		val1 = *(vu16 *)(Bank1_NOR2_ADDR);
		val2 = *(vu16 *)(Bank1_NOR2_ADDR);
		/* If DQ6 did not toggle between the two reads then return NOR_Success */
		if((val1 & 0x0040) == (val2 & 0x0040)) {	return NOR_SUCCESS;	   }
		
		if((val1 & 0x0020) != 0x0020){	 status = NOR_ONGOING;	 }
		
		val1 = *(vu16 *)(Bank1_NOR2_ADDR);
		val2 = *(vu16 *)(Bank1_NOR2_ADDR);
			
		if((val1 & 0x0040) == (val2 & 0x0040)) 
		{	return NOR_SUCCESS;	  }
		else if((val1 & 0x0020) == 0x0020)
		{	return NOR_ERROR;	}
	}
	
	if(Timeout == 0x00)
	{	status = NOR_TIMEOUT;	} 
	
	/* Return the operation status */
	return (status);
}

/******************************************************************************
* Function Name  : FSMC_NOR_ReadID
* Description    : Reads NOR memory's Manufacturer and Device Code.
* Input          : - NOR_ID: pointer to a NOR_IDTypeDef structure which will hold
*                    the Manufacturer and Device Code.
*******************************************************************************/
void FSMC_NOR_ReadID(NOR_IDTypeDef* NOR_ID)
{
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x0090);
	
	NOR_ID->Manufacturer_Code = *(vu16 *)ADDR_SHIFT(0x0000);
	NOR_ID->Device_Code1 = *(vu16 *)ADDR_SHIFT(0x0001);
	NOR_ID->Device_Code2 = *(vu16 *)ADDR_SHIFT(0x000E);
	NOR_ID->Device_Code3 = *(vu16 *)ADDR_SHIFT(0x000F);
}

/*******************************************************************************
* Function Name  : FSMC_NOR_EraseBlock
* Description    : Erases the specified Nor memory block.
* Input          : - BlockAddr: address of the block to erase.
* Output         : None
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_EraseBlock(INT32U BlockAddr)
{
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE((Bank1_NOR2_ADDR + BlockAddr), 0x30);
	
	return (FSMC_NOR_GetStatus(BlockErase_Timeout));
}

/*******************************************************************************
* Function Name  : FSMC_NOR_EraseChip
* Description    : Erases the entire chip.
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_EraseChip(void)
{
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x0080);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x0010);
	
	return (FSMC_NOR_GetStatus(ChipErase_Timeout));
}

/******************************************************************************
* Function Name  : FSMC_NOR_WriteHalfWord
* Description    : Writes a half-word to the NOR memory. 
* Input          : - WriteAddr : NOR memory internal address to write to.
*                  - Data : Data to write. 
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_WriteHalfWord(u32 WriteAddr, u16 Data)
{
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00A0);
	NOR_WRITE((Bank1_NOR2_ADDR + WriteAddr), Data);
	
	return (FSMC_NOR_GetStatus(Program_Timeout));
}

/*******************************************************************************
* Function Name  : FSMC_NOR_WriteBuffer
* Description    : Writes a half-word buffer to the FSMC NOR memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : NOR memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite : number of Half words to write. 
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_WriteBuffer(INT16U* pBuffer, INT32U WriteAddr, INT32U NumHalfwordToWrite)
{
	NOR_Status status = NOR_ONGOING; 
	
	do
	{
		/* Transfer data to the memory */
		status = FSMC_NOR_WriteHalfWord(WriteAddr, *pBuffer++);
		WriteAddr = WriteAddr + 2;
		NumHalfwordToWrite--;
	}
	while((status == NOR_SUCCESS) && (NumHalfwordToWrite != 0));
	
	return (status); 
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteImage
* 功能：写图片数据到RAM
*******************************************************************************/
void FSMC_NOR_8BitBuffer(const INT8U* imageconst, INT32U WriteAddr, INT32U NumHalfwordToWrite)
{
	INT16U dattmp;
	const INT8U *imagep;
	NOR_Status status = NOR_ONGOING; 

	imagep = imageconst;
	do
	{
		/* Transfer data to the memory */
		dattmp = ((*imagep)<<8)|(*(imagep+1));
		status = FSMC_NOR_WriteHalfWord(WriteAddr, dattmp);
		WriteAddr = WriteAddr + 2; 
		NumHalfwordToWrite -= 2;
		imagep += 2;
	}
	while((status == NOR_SUCCESS) && (NumHalfwordToWrite != 0));
}

/*******************************************************************************
* Function Name  : FSMC_NOR_ProgramBuffer
* Description    : Writes a half-word buffer to the FSMC NOR memory. This function 
*                  must be used only with S29GL128P NOR memory.
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr: NOR memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite: number of Half words to write.
*                    The maximum allowed value is 32 Half words (64 bytes).
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
NOR_Status FSMC_NOR_ProgramBuffer(INT16U* pBuffer, INT32U WriteAddr, INT32U NumHalfwordToWrite)
{       
	u32 lastloadedaddress = 0x00;
	u32 currentaddress = 0x00;
	u32 endaddress = 0x00;
	
	/* Initialize variables */
	currentaddress = WriteAddr;
	endaddress = WriteAddr + NumHalfwordToWrite - 1;
	lastloadedaddress = WriteAddr;
	
	/* Issue unlock command sequence */
	NOR_WRITE(ADDR_SHIFT(0x005555), 0x00AA);
	
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);  
	
	/* Write Write Buffer Load Command */
	NOR_WRITE(ADDR_SHIFT(WriteAddr), 0x0025);
	NOR_WRITE(ADDR_SHIFT(WriteAddr), (NumHalfwordToWrite - 1));
	
	/* Load Data into NOR Buffer */
	while(currentaddress <= endaddress)
	{
		/* Store last loaded address & data value (for polling) */
		lastloadedaddress = currentaddress;
		
		NOR_WRITE(ADDR_SHIFT(currentaddress), *pBuffer++);
		currentaddress += 1; 
	}
	
	NOR_WRITE(ADDR_SHIFT(lastloadedaddress), 0x29);
	
	return(FSMC_NOR_GetStatus(Program_Timeout));
}

/******************************************************************************
* Function Name  : FSMC_NOR_ReadHalfWord
* Description    : Reads a half-word from the NOR memory. 
* Input          : - ReadAddr : NOR memory internal address to read from.
* Output         : None
* Return         : Half-word read from the NOR memory
*******************************************************************************/
u16 FSMC_NOR_ReadHalfWord(u32 ReadAddr)
{
	NOR_WRITE(ADDR_SHIFT(0x005555), 0x00AA); 
	NOR_WRITE(ADDR_SHIFT(0x002AAA), 0x0055);  
	NOR_WRITE((Bank1_NOR2_ADDR + ReadAddr), 0x00F0 );
	
	return (*(vu16 *)((Bank1_NOR2_ADDR + ReadAddr)));
}

/*******************************************************************************
* Function Name  : FSMC_NOR_ReadBuffer
* Description    : Reads a block of data from the FSMC NOR memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the NOR memory.
*                  - ReadAddr : NOR memory internal address to read from.
*                  - NumHalfwordToRead : number of Half word to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_NOR_ReadBuffer(INT16U* pBuffer, INT32U ReadAddr, INT32U NumHalfwordToRead)
{
	NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
	NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
	NOR_WRITE((Bank1_NOR2_ADDR + ReadAddr), 0x00F0);
	
	for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) /* while there is data to read */
	{
		/* Read a Halfword from the NOR */
		*pBuffer++ = *(vu16 *)((Bank1_NOR2_ADDR + ReadAddr));
		ReadAddr = ReadAddr + 2; 
	}  
}

/******************************************************************************
* Function Name  : FSMC_NOR_ReturnToReadMode
* Description    : Returns the NOR memory to Read mode.
* Input          : None
* Output         : None
* Return         : NOR_SUCCESS
*******************************************************************************/
NOR_Status FSMC_NOR_ReturnToReadMode(void)
{
	NOR_WRITE(Bank1_NOR2_ADDR, 0x00F0);
	
	return (NOR_SUCCESS);
}

/******************************************************************************
* Function Name  : FSMC_NOR_Reset
* Description    : Returns the NOR memory to Read mode and resets the errors in
*                  the NOR memory Status Register.
* Input          : None
* Output         : None
* Return         : NOR_SUCCESS
*******************************************************************************/
NOR_Status FSMC_NOR_Reset(void)
{
	NOR_WRITE(ADDR_SHIFT(0x005555), 0x00AA); 
	NOR_WRITE(ADDR_SHIFT(0x002AAA), 0x0055); 
	NOR_WRITE(Bank1_NOR2_ADDR, 0x00F0); 
	
	return (NOR_SUCCESS);
}
