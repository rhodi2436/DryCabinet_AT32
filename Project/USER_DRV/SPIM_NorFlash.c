/**
  ******************************************************************************
  * File   : FLASH/operate_SPIM/SPIM/SPIM_test.c
  * Version: V1.2.2
  * Date   : 2020-07-01
  * Brief  : This file contains the function SPIM_test used to test ext.flash
  ******************************************************************************
  
SPIM 说明
 SPIM (External SPI FLASH memory interface)最大地址段为0x08400000 - 0x093FFFFF(16MB)，是
 AT32 MCU独有的一种Flash访问方式.开启SPIM后可以作为Flash扩展,实现如下功能：
 1.在SPIM地址存放用户执行程序，类似于Bank1/Bank2一样执行程序。
 2.用户程序直接访问SPIM地址，作为存储字体库，图片等存储器使用。

 SPIM仅允许按字操作(32bit)， 在执行读、写、 擦除SPIM前， 必须首先执行初始化及解锁操作。
 读操作：直接按字(32bit)访问需要读取数据的地址段： 0x08400000 – 0x093FFFFF
 写操作：写操作步骤， 在AT32的BSP中已经封装成库函数，用户可以直接调用
   1) 打开编程操作， FLASH_CTRL3寄存器PRGM位置1
   2) 直接按字(32bit)在需要编程的地址写入数据
   3) 检查是否写入完成读取FLASH_STS3寄存器的BSY位是否清除，如果清除表示写入完成
   4) 关闭编程操作， FLASH_CTRL3寄存器PRGM位置0
   5) 检查是否写入成功，读取FLASH_STS3寄存器的WRPRTFLR和PRGMFLR位， 如果都为0则表
      示写入成功
 擦除操作：	

W25Q128JV 说明
 W25Q128JV （128M位）串行闪存为空间,阵列分为65,536个可编程页面，每个页面256字节。
 一次最多可以编程256个字节。可以按16组（4KB扇区擦除），128组（32KB块擦除），
 256组（64KB块擦除）或整个芯片（芯片擦除）擦除页面。 
 W25Q128JV分别具有4,096个可擦除扇区和256个可擦除块。 
 4KB的小扇区为需要数据和参数存储的应用程序提供了更大的灵活性。 
 W25Q128JV的SPI时钟频率支持高达133MHz，当使用快速读取双/四通道I/O时，双I/O的等效时钟速率为266MHz（133MHz x 2），
 四通道I/O的等效时钟速率为532MHz（133MHz x 4）。 这些传输速率可以超过标准的异步8位和16位并行闪存。
  */

/* Includes ------------------------------------------------------------------*/
//#include <string.h>
//#include <stdio.h>
 #include  "../Head file/includes.h" 

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_Operate_SPIM
  * @{
  */

/* Private variables ---------------------------------------------------------*/
u8 WriteBuffer[4096];  ///The content writed to ext.flash 
u8 ReadBuffer[4096];   ///The content read from ext.flash  		
u8 NorFlashFlg = 0;
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Read one page from a specified address in ext.flash
  * @param  adr: specifies the page starting address
  * @param  sz: page size in bytes
  * @param  buf: the content read from ext.flash
  * @retval None
  */
void ReadPage (unsigned long adr, unsigned long sz, unsigned char *buf)
{
  while (sz)
  {
    *(UINT32 *)buf = *(UINT32 *)adr;    
    sz -= sizeof(UINT32);
    adr += sizeof(UINT32);
    buf += sizeof(UINT32);
  }   
}

/**
  * @brief  To test ext.flash
  * @param  None
  * @retval None
  */


void SPIM_test(void)
{
  u16 i=0;
  INT32U	TimCnt;
  /* Configures the ext.flash */
//  printf("init SPIM.\r\n");   
//  FLASH_InitExtFlash();
	
  TimCnt = os_time.Systime_Count;
  for(i = 0; i < 10; i ++)
  {

	  ReadPage(SPIM_TEST_ADDR+i*SPIM_PAGE_SIZE, SPIM_PAGE_SIZE, ReadBuffer);	
  
  }
  LCD.TFT1_TimCnt1ms = os_time.Systime_Count - TimCnt;
  
//  /* Fill the content to be writed to ext.flash */
//  for(i=0;i<SPIM_PAGE_SIZE;i++)
//  {
//    WriteBuffer[i]=i%256;
//  }
//  
//  /* Erases an ext.flash page */
////  printf("erase one page.\r\n");
//  FLASH_ErasePage(SPIM_TEST_ADDR);
//  
//  /* Read an ext.flash page */
////  memset(ReadBuffer,0,SPIM_PAGE_SIZE);
//  ReadPage(SPIM_TEST_ADDR, SPIM_PAGE_SIZE, ReadBuffer);
//  
//  /* Check if the desired page are erased */
//  for(i=0;i<SPIM_PAGE_SIZE;i++)
//  {
//    if(ReadBuffer[i]!=0xff)
//    {
////      printf("operate SPIM fail.\r\n");
//	  NorFlashFlg = 1;
//      return;
//    }    
//  }
//  
//  /* Program an ext.flash page */
////  printf("write one page.\r\n");
//  i=0;
//  while(i<SPIM_PAGE_SIZE)
//  {
//    FLASH_ProgramWord (SPIM_TEST_ADDR+i,*(u32 *)(WriteBuffer+i)); 
//    i=i+4;  
//  }
//  
//  /* Read an ext.flash page */
////  printf("read one page.\r\n");

//  ReadPage(SPIM_TEST_ADDR, SPIM_PAGE_SIZE, ReadBuffer);
//  
//  /* Check if reading result and writing content are the same */
////  printf("compare the WriteBuffer/ReadBuffer.\r\n");
//  for(i=0;i<SPIM_PAGE_SIZE;i++)
//  {
//    if(ReadBuffer[i]!=WriteBuffer[i])
//    {

//	  NorFlashFlg = 2;	
//      return;
//    }    
//  }

}



  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
