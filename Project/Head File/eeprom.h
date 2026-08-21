#ifndef _EEPROM_H_
#define _EEPROM_H_

/************************************************************************************/
#ifdef  AP_EPROM_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_EPROM_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_EPROM_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

/* Exported constants --------------------------------------------------------*/
#define PAGE_SIZE  (uint16_t)0x200  /* Page size = 1KByte */
//#define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */

	
/* EEPROM start address in Flash  */
// 每页长度为1K，共64页，页0~页63，其中页0 = 0x08000000~0x080003FF，	
#define EEPROM_START_ADDRESS    ((uint32_t)0x0803f800) /* EEPROM emulation start address:after 50 KByte of used Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
	
/* Used Flash pages for EEPROM emulation */
	
// 扇区数量
#define PAGE_0       0
#define PAGE_1       1 
#define PAGE_2       2
#define PAGE_NUM     10	

// 每个扇区里分成3块数据
#define SECTOR_0_ADDR       2
#define SECTOR_1_ADDR       (SECTOR_0_ADDR + 32) //20230116更改 eeproom擦写
#define SECTOR_2_ADDR       (SECTOR_1_ADDR + 32) 



#define EE_ADDRESS		0
#define EE_FINISHEDLED 	( EE_ADDRESS + 1 )      // F1
#define EE_WTTIME		( EE_ADDRESS + 2 )      // F2
#define EE_FNDEFINE		( EE_ADDRESS + 3 )      // F3
#define EE_DISPMODE		( EE_ADDRESS + 4 )      // F4
#define EE_KEYPROCLED	( EE_ADDRESS + 5 )      // F5
#define EE_TOUCHSET	    ( EE_ADDRESS + 6 )      // F6
#define EE_IRDISTANCESET ( EE_ADDRESS + 7 )      // F7
#define EE_KEYOKTASK	( EE_ADDRESS + 8 )      // F8
#define EE_IRTIMESET	( EE_ADDRESS + 9 )      // F9


//#define MAX_ADDR        	    247//255      //1~255	  地址
#define MAX_FINISHED_LED_DISP   0x17     //17  F1 任务完成后亮灯和回显2个参数，低4位为亮灯状态，高4位为回显模式
#define MAX_FINISHEDLED         7    	 //7       MAX_FINISHED_LED_DISP 亮灯状态最大值
#define MAX_FINISHEDBACKDISP    1        //1       MAX_FINISHED_LED_DISP 回显最大值
#define MAX_WTTIME        	    5    	 //5   F2
#define MAX_FNDEFINE     	    1    	 //1   F3
//#define MAX_DISPMODE            2    	 //1   F4
#define MAX_KEYPROCLED          8    	 //1   F5
#define MAX_TOUCHSET            48    	 //1   F6
#define MAX_IRDISTANCESET       9        //1   F7
#define MAX_KEYOKTASK           1    	 //1   F8
#define MAX_IRTIMESET           0x99     //1   F9   红外遮挡进入和退出确认时间（去抖用），低4位为无遮挡有效时间，高4位为遮挡有效时间，合并2个参数
#define MAX_IRTIMESET_IN        9        //1         高4位
#define MAX_IRTIMESET_OUT       9        //1         低4位

#define DEF_ADDR        	    1     	 //1	  默认值，设备地址1~255
#define DEF_FINISHED_LED_DISP  0x00    //       合并2个参数的值
#define DEF_FINISHEDLED         0    	 //0      任务完成后亮灯状态，0==灭，1=红，2=绿，3=蓝，4=黄，5=粉，6=青，7=白
#define DEF_FINISHEDBACKDISP    0    	 //0      任务完成后是否回显，0=不回显，1=回显
#define DEF_WTTIME        	    5    	 //5
#define DEF_FNDEFINE    	    0    	 //0  

#define DEF_KEYPROCLED          0    	 //0
#define DEF_TOUCHSET            10    	 //0
#define DEF_IRDISTANCESET       4    	 //1
#define DEF_KEYOKTASK           00    	 //0，
#define DEF_IRTIMESET           0x11     //1
#define DEF_IRTIMESET_IN        1
#define DEF_IRTIMESET_OUT       1
//================================================================================
// 函数声明
AP_EPROM_EXT void SaveParaToBlock(void);		   // 保存参数，保存前判断参数是否超出范围，如超出则恢复到默认值
AP_EPROM_EXT void LoadParaInEeprom(void);		   // 开机读取参数，如超出则恢复到默认值

#endif









