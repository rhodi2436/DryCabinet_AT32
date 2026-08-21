/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_sram.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to drive the 
*                      IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include  "../Head file/FSMC_SRAM.h"
#include "../Head file/includes.h" 

/*******************************************************************************
* Function Name  : FSMC_SRAM_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
*******************************************************************************/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p,p1;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
	                     RCC_APB2Periph_GPIOF, ENABLE);
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
	                            GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* SRAM Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                            GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
	                            GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* NOE and NWE configuration */  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* NE3 NE2 NE4configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* NBL0, NBL1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	/*-- SRAM （TFT2.0）Configuration ------------------------------------------------------*/
	p.FSMC_AddressSetupTime = 0;
	p.FSMC_AddressHoldTime = 10;
	p.FSMC_DataSetupTime = 60;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	p1.FSMC_AddressSetupTime = 0;
	p1.FSMC_AddressHoldTime = 1;
	p1.FSMC_DataSetupTime = 9;
	p1.FSMC_BusTurnAroundDuration = 0;
	p1.FSMC_CLKDivision = 0;
	p1.FSMC_DataLatency = 0;
	p1.FSMC_AccessMode = FSMC_AccessMode_A;
	
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
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;	   //扩展模式使能位，1使能读写时序独立，0读写一致
	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p1;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	/*-- TFT 3.5 Configuration ------------------------------------------------------*/
	p.FSMC_AddressSetupTime = 0;
	p.FSMC_AddressHoldTime = 10;
	p.FSMC_DataSetupTime = 60;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	p1.FSMC_AddressSetupTime = 0;
	p1.FSMC_AddressHoldTime = 1;
	p1.FSMC_DataSetupTime = 9;
	p1.FSMC_BusTurnAroundDuration = 0;
	p1.FSMC_CLKDivision = 0;
	p1.FSMC_DataLatency = 0;
	p1.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE); 
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteBuffer
* Description    : Writes a Half-word buffer to the FSMC SRAM memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : SRAM memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite : number of half-words to write. 
*******************************************************************************/
void FSMC_SRAM_WriteBuffer(INT16U* pBuffer, INT32U WriteAddr, INT32U NumHalfwordToWrite)
{
	for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
	{
		/* Transfer data to the memory */
		*(INT16U *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
		
		/* Increment the address*/  
		WriteAddr += 2;
	}   
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadBuffer
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
*                  - NumHalfwordToRead : number of half-words to read.
*******************************************************************************/
void FSMC_SRAM_ReadBuffer(INT16U* pBuffer, INT32U ReadAddr, INT32U NumHalfwordToRead)
{
	/* while there is data to read */
	for(; NumHalfwordToRead != 0; NumHalfwordToRead--) 
	{
		/* Read a half-word from the memory */
		*pBuffer++ = *(INT16VU*) (Bank1_SRAM3_ADDR + ReadAddr);
		
		/* Increment the address*/  
		ReadAddr += 2;
	}  
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteImage
* 功能：写图片数据到RAM
*******************************************************************************/
void FSMC_SRAM_WriteImage(const INT8U* imageconst, INT32U WriteAddr, INT32U NumHalfwordToWrite)
{
	const INT8U *imagep;

	imagep = imageconst;
	for(; NumHalfwordToWrite != 0; NumHalfwordToWrite-=2) /* while there is data to write */
	{
		/* Transfer data to the memory */
		*(INT16U *) (Bank1_SRAM3_ADDR + WriteAddr) = ((*imagep)<<8)|(*(imagep+1));
		
		/* Increment the address*/  
		WriteAddr += 2;
		imagep += 2;
	}  
}




