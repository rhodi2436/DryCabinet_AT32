/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : spi_flash.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Header for spi_flash.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
//#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL)
// //#define USE_STM3210B_EVAL
// #define USE_STM3210E_EVAL
//#endif

//#ifdef USE_STM3210B_EVAL
 #define GPIO_CS                  GPIOA
 #define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOA
 #define GPIO_Pin_CS              GPIO_Pin_4 
//#else /* USE_STM3210E_EVAL */
// #define GPIO_CS                  GPIOB
// #define RCC_APB2Periph_GPIO_CS   RCC_APB2Periph_GPIOB
// #define GPIO_Pin_CS              GPIO_Pin_2 
//#endif

//=========================================================================
//GT32L32S0140	   4M  flash  ,512K 可自由读写
#define CMD_READ            0x03  // Read Data Bytes
#define CMD_FASTREAD        0x0B  // Read Data Bytes at Higher Speed
#define CMD_WREN            0x06  // Write Enalbe 
#define CMD_WRDI            0x04  // Write Disable 
#define CMD_PP              0x02  // Page Program  (1 to256)
#define CMD_SE              0x20  // Sector Erase
#define CMD_BE              0xD8  // Block Erase(64K)
//#define CMD_CE              0x60  // Chip Erase 
#define CMD_CE              0xC7   // Chip Erase 
#define CMD_STATE_READ      0x05  // read state regist

#define SR_WIP              0X01<<0
#define SR_WSL              0X01<<1
#define SR_SP0              0X01<<2
#define SR_SP1              0X01<<3
#define SR_SP2              0X01<<4
#define SR_SP3              0X01<<5
#define SR_SP4              0X01<<6
#define SR_BP0              0X01<<7

//计算扇区和块的地址
#define BLOCK_SIZE          (64*1024)
#define SECTOR_SIZE         (4*1024)

#define BLOCK_ADDR(BSN)     (BSN*BLOCK_SIZE)
#define SECTOR_ADDR(BSN)    (BSN*SECTOR_SIZE)

#define FONT_FLASH_SECTOR_ADDR(n) 1024*4*n  //扇区n的首地址

 /* 定义点阵字库的基地址*/
 #define  ASCII_5_7_BASE_ADDR                    0x080000    //5*7点阵ASC标准字符			96
 #define  ASCII_7_8_BASE_ADDR                    0x080300    //7*8点阵ASC标准字符			96
 #define  ASCII_7_8_BLOD_BASE_ADDR               0x080600    //7*8点阵ASC粗体字符			96
 #define  ASCII_6_12_BASE_ADDR                   0x080900    //6*12点阵ASC字符				96
 #define  ASCII_8_16_BASE_ADDR                   0x080D80    //8*16点阵ASC标准字符			128
 #define  ASCII_8_16_BOLD_BASE_ADDR              0x081580    //8*16点阵ASC粗体字符			96
 #define  ASCII_12_24_BASE_ADDR                  0x081B80    //12*24点阵ASC标准字符			96
 #define  ASCII_12_24_PRINT_ADDR                 0x082D80    //12*24点阵ASC打印机字符		224

 #define  ASCII_12ND_BASE_ADDR                   0x085780    //12点阵不等宽方头(Arial)字符				  96
 #define  ASCII_16ND_BASE_ADDR                   0x086140    //16点阵不等宽方头(Arial)字符				  96
 #define  ASCII_24ND_BASE_ADDR                   0x086E00    //24点阵不等宽方头(Arial)字符				  96
 #define  ASCII_32ND_BASE_ADDR                   0x0889C0    //32点阵不等宽方头(Arial)字符				  96
 #define  ASCII_12NDW_BASE_ADDR                  0x08BA80    //12点阵不等宽白正(Times New Roman)字符	  96
 #define  ASCII_16NDW_BASE_ADDR                  0x08C450    //16点阵不等宽白正(Times New Roman)字符	  96
 #define  ASCII_24NDW_BASE_ADDR                  0x08D140    //24点阵不等宽白正(Times New Roman)字符	  96
 #define  ASCII_32NDW_BASE_ADDR                  0x08ED40    //32点阵不等宽白正(Times New Roman)字符	  96

 #define  ASCII_16_32_BASE_ADDR                  0x091E00    //16*32点阵ASC标准字符						  96
 #define  ASCII_16_32_BOLD_BASE_ADDR             0x093600    //16*32点阵ASC粗体字符						  96

 #define  ASCII_14_28_NUM_BASE_ADDR              0x094E00    //14*28数字符号字符						  15
 #define  ASCII_20_40_NUM_BASE_ADDR              0x095148    //20*40数字符号字符						  12
 #define  ASCII_28_ND_NUM_BASE_ADDR              0x0956E8    //28点阵不等宽数字符号字符					  15
 #define  ASCII_40_ND_NUM_BASE_ADDR              0x095D96    //40点阵不等宽数字符号字符					  12

 #define  ASCII_12_12_GBK_BASE_ADDR          	 0x09670E    //12*12点阵GBK汉字	 GB2312汉字(宋体)		 6763
 #define  ASCII_16_16_GBK_BASE_ADDR          	 0x0C30DE    //16*16点阵GBK汉字  GB2312汉字(宋体)		 6763
 #define  ASCII_24_24_GBK_BASE_ADDR          	 0x0FE89E    //24*24点阵GBK汉字  GB2312汉字(宋体)		 6763
 #define  ASCII_24_24_GBK_BLA_ADDR          	 0x18460E    //24*24点阵GBK汉字  GB2312汉字(黑体)		 6763
 #define  ASCII_32_32_GBK_BASE_ADDR          	 0x20A37E    //32*32点阵GBK汉字  GB2312汉字(宋体)		 6763
 #define  ASCII_32_32_GBK_BLA_ADDR          	 0x2F828B    //32*32点阵GBK汉字  GB2312汉字(黑体)		 6763

 #define  ASCII_12_12_GBK_BASE1_ADDR         	 0x09670E    //12*12点阵GBK字符	 GB2312字符(宋体)		 846
 #define  ASCII_16_16_GBK_BASE1_ADDR         	 0x0C30DE    //16*16点阵GBK字符	 GB2312字符(宋体)		 846
 #define  ASCII_24_24_GBK_BASE1_ADDR         	 0x0FE89E    //24*24点阵GBK字符	 GB2312字符(宋体)		 846
 #define  ASCII_24_24_GBK_BLA1_ADDR         	 0x18460E    //24*24点阵GBK字符	 GB2312字符(黑体)		 846
 #define  ASCII_32_32_GBK_BASE1_ADDR         	 0x20A37E    //32*32点阵GBK字符	 GB2312字符(宋体)		 846
 #define  ASCII_32_32_GBK_BLA1_ADDR         	 0x2F828B    //32*32点阵GBK字符	 GB2312字符(黑体)		 846

 #define  UNICODE_TO_GBK_TAB_BASE_ADDR28         0x3E618B    //unicode->GB2312转码表					 6763+846
 #define  CODEBAR_12_27_EAN13_BASE_ADDR          0x3F222B    //12*27条码字符 EAN13						 60
 #define  CODEBAR_16_20_CODE128_BASE_ADDR        0x3F2ED3    //16*20条码字符 CODE138					 107



 #define  TX_12_12_BASE_ADDR        0x3F3F8B 	 //12*12天线符号
 #define  DC_12_12_BASE_ADDR        0x3F4003 	 //12*12电池符号


/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIO_CS, GPIO_Pin_CS)
/* Deselect SPI FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIO_CS, GPIO_Pin_CS)



/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
//void SPI_FLASH_Init(void);
//void SPI_FLASH_SectorErase(u32 SectorAddr);
//void SPI_FLASH_BulkErase(void);
//void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
//void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
//void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

//CPU_EXT	u32 SPI_FLASH_ReadID(void);
//CPU_EXT	void SPI_FLASH_StartReadSequence(u32 ReadAddr);

/*----- Low layer function -----*/
u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
