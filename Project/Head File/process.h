#ifndef _dnet_h_ 
#define _dnet_h_






#define DISPLAY_MAX_NUM         65535
#define DISPLAY_MAX_BIT         5

//COMMAND
#define INIT_ID_COMMAND 		1
#define INIT_NETWORK_COMMAND 	2
#define TO_RUN_COMMAND		3
#define INIT_PARAM_COMMAND	0x20
#define NETWORK_IO				5
#define NETWORK_IO_STATUS		6
#define PARAM_READ_RESP		0x8e
#define PARAM_WRITE_RESP		0x90

#define ONE_CMD_MIN_LENGTH         6

#define READ_MULTI_BYTE			0x03
#define WRITE_SINGLE_BYTE		0x06
#define WRITE_MULTI_BYTE     	0x10

#define ACK_DATA_LEGTH  2

//下传的命令码
#define DISPMODE_NONE	0X00		// 清空所有数据，无提示,led无显示
#define DISPMODE_ASII	0X02		// 字符串显示
#define DISPMODE_TASK	0X03		// 作业命令

#define DISPMODE_INFO	0X10		// 600U信息


#define	ALL_UNLOCK	00
#define	ALL_LOCK		01
#define	FUNC_LOCK		02
#define	OK_LOCK		  03



//BYTE0   命令号
//BYTE1   货位号 
//BYTE2   数量高位
//BYTE3   数量低位
//BYTE4   保留
//BYTE5   保留

//协议如下
//  0      1    2   3   4   5   6   7   8     
//命令字  长度 【2 - 7字节协议如上述】  和校验 
//        6+1     
//长度：从第一个数据字节起到最后一个字节，包括校验码。
//校验：校验码的产生方式—累加和。从命令号开始计算，到数据结束。

#define  MULCAST_ADDR		 248

#endif
