

#include  "../Head file/includes.h" 

#include "QR_Encode.h"

//#define DATA_LEN     		7
//#define DEVICE_NAME_LEN		23

extern INT16U CRC16 ( INT8U * puchMsg, INT16U usDataLen );


/*******************************************************************************
	 					系统参数复位
*******************************************************************************/
void sys_data_reset(void)
{
    INT8U i;
	
    UnfinishTask.num    = 0; 			 // 未完成任务清0
	UnfinishTask.remain = 0;    		 // 剩余任务清0
    FinishTask.num      = 0;			 // 已完成任务清0
	UnfinishTask.total  = 0;			 // 一次作业总任务数
	UnfinishTask.order	= 0;			 // 已完成任务序号
	OsCmdSet.beSettingAddr 	    = 0;	 // 地址设定清0
	OsCmdSet.LastSoftGetOkCount = 0;	 // 上位机已采集到OK按键次数清0
	UnfinishTask.repeat	 = FALSE;		 // 清重复接收到任务标志

    for(i=0; i< MAX_TASK_NUM; i++)
    {	
		//未完成任务块
		UnfinishTask.info[i].sku = 0;
		UnfinishTask.info[i].count = 0;
        //已完成任务块
		FinishTask.info[i].sku = 0;
		FinishTask.info[i].count = 0;
		FinishTask.info[i].full = 0;
    }
	Disp.f_Loading = LOAD_MODE_OFF;	     	  // 进度条等待显示标志清除
	OsCmdSet.CurDispMode = DISPMODE_NONE;	  // 当前任务状态设为非运行状态
}


/*******************************************************************************
	 					信息显示命令解析及处理
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Info600U_Process(void)
{
	INT8U  len0 = 0;			
	INT8U  i ;

	// 清除任务区任务，避免有回显
	UnfinishTask.num = 0;		   // 未完成任务个数清0
	FinishTask.num   = 0;		   // 已完成任务个数清0
	BackTask.num   = 0;			   // 回显备份区的保存总个数清0
	KeyMode.f_BackDisp = OFF;	   // 回显模式标志清除
			
	// 取显示数据长度
	len0 = uart.rBuffer[6] ;

	// 批次数据获取
	InfoDisp.srcode_len  = len0;
	for(i = 0; i < len0; i++)
	{
		InfoDisp.srcode[i] = uart.rBuffer[10+i];
	}

	//0、背景色
//	if(uart.rBuffer[3] >= 0x10)
	{ bk_colour = uart.rBuffer[3]>>4; }
//	else{ bk_colour = uart.rBuffer[3]>>4; }

	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (uart.rBuffer[8] >> 5)&0X07;	  // 保存在灯组备份区域中
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type    = led.light_type_BK; }			  // 非进度条模式才更新灯组显示值
    
	//2、闪烁时间间隔
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3、闪烁模式
    led.flash_mode   = (uart.rBuffer[8])&0X07;
	//4、闪烁用数据置位，停止闪烁
	led.time_refresh = 0;			// 让闪烁时间到
	led.flash_status = 0;			// 先亮


	// 设置显示
	{ SetLcdLay(PAGE_INFO); }
}

/*******************************************************************************
	 					组播命令，显示命令
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Info(INT8U XDATA * buf)
{
	UINT8  usReTaskNum,len0;			//本次下发的"电子标签显示内容"个数，

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	
	// 灯亮的状态、时间、模式 	  uart.rBuffer[8] 
	i = * (buf);

	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (i >> 5)&0X07;	  // 保存在灯组备份区域中
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type  = led.light_type_BK; }			  // 非进度条模式才更新灯组显示值
    
	//2、闪烁时间间隔
	switch((i >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3、闪烁模式
    led.flash_mode   = (i)&0X07;
	//4、闪烁用数据置位，停止闪烁
	led.time_refresh = 0;			// 让闪烁时间到
	led.flash_status = 0;			// 先亮

    // 上次采集到按钮任务数量(高4位)+本次任务数量(低4位) uart.rBuffer[9] 
	usReTaskNum = (* (buf+1))&0X0F;		//本次下发的"电子标签显示内容"个数， 
	// 剩余未下发任务数量 uart.rBuffer[10]			
	UnfinishTask.remain = * (buf+2);		 //本次作业中还未下发的个数

	UnfinishTask.num = 0;		   // 未完成任务个数清0
	FinishTask.num   = 0;		   // 已完成任务个数清0
	UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      //计算出一次任务总数量

	BackTask.num   = 0;			   // 回显备份区的保存总个数
	KeyMode.f_BackDisp = OFF;	   // 回显模式标志清除

	//根据本次下发显示的任务个数进行协议解析

	//来1组新数据，看当前剩余个数做处理			

	
	len0     = * (buf+3); //uart.rBuffer[11];	// 第一个的序列号长度                 

	// 对应的数组
	pt = &UnfinishTask.info[UnfinishTask.num];		
	// 批次数据
	(*pt).srcode_len  = len0;
	for(i = 0; i < len0; i++)
	{
		if(i >= MAX_SRCODE_NUM) break;
		(*pt).srcode[i] = * (buf + 4 + i);//uart.rBuffer[curIndex+i];
	}
	//库位号+数量
	curIndex = 4 + len0;					// 数据位置
//	curIndex += len0;			//uart.rBuffer[11]为第一个序列号长度 


	// 组播命令兼容4位库位码
 	// ascii码的Sku
	(*pt).sku_ascii[0]   = NULL;		// 库位号	 ，初始化为0，代表无
	(*pt).sku_ascii[1]   = NULL;		// 库位号	 ，初始化为0，代表无
 	(*pt).sku_ascii[2]   = NULL;		// 库位号	 ，初始化为0，代表无
	(*pt).sku_ascii[3]   = NULL;		// 库位号	 ，初始化为0，代表无

//	if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
	{
		// 判断是否旧协议，不包含后4个库位号
		if( * (buf + curIndex - 4) == NULL )
		{	// 旧协议
			(*pt).dir    = * (buf + curIndex - 3);		// 方向

			(*pt).uni[0] = * (buf + curIndex - 2);		// 单位
			(*pt).uni[1] = * (buf + curIndex - 1);		// 单位				
		}
		else
		{	// 新协议
			(*pt).dir    = * (buf + curIndex - 7);		// 方向

			(*pt).uni[0] = * (buf + curIndex - 6);		// 单位
			(*pt).uni[1] = * (buf + curIndex - 5);		// 单位

	 		(*pt).sku_ascii[0]   = * (buf + curIndex - 4);		// 库位号
			(*pt).sku_ascii[1]   = * (buf + curIndex - 3);		// 库位号
 			(*pt).sku_ascii[2]   = * (buf + curIndex - 2);		// 库位号
			(*pt).sku_ascii[3]   = * (buf + curIndex - 1);		// 库位号
		}
	}


	(*pt).sku	 = * (buf + curIndex);//uart.rBuffer[curIndex];	  //库位号
	(*pt).count  = (INT16U)((*(buf+curIndex+1))*256) + (INT16U)(*(buf+curIndex+2));//(INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量

//	if(UnfinishTask.num == 0)
	{ OsCmdSet.TaskCurCount = (*pt).count ; } 	  // 保存当前数量

	UnfinishTask.num++;

    { SetLcdLay(PAGE_RUN); }
}

/*******************************************************************************
	 					作业命令解析及处理
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Task_Process(void)
{
	UINT8  usReTaskNum,len0 = 0,len1 = 0;			

	UINT8  i = 0,curIndex = 0;
	TASK_INFO XDATA *  pt;
	
	// 本次下发的"电子标签显示内容"个数，     
	usReTaskNum = (uart.rBuffer[9])&0X0F;		 // 本次下发的"电子标签显示内容"个数，		
 	// 本次作业中还未下发的个数
	UnfinishTask.remain = uart.rBuffer[10];		 // 本次作业中还未下发的个数

	// 先初始化数据
	// 有2组待显示数据下来，则将原来的数据都冲掉，包括未完成缓存区与完成缓存区
	// 上位机上次采集到OK按钮的任务数量
	switch(OsCmdSet.LastSoftGetOkCount)
	{
		case 0:     // 作业启动的时候第一次发
		{
			// 回显数据处理区域，新来一次作业任务的数据,当前所有任务先清除(设备2此重复接收此命令时),回显数据清零
			UnfinishTask.num = 0;		   // 未完成任务个数清0
			FinishTask.num   = 0;		   // 已完成任务个数清0
			UnfinishTask.total = usReTaskNum + UnfinishTask.remain;      // 计算出一次任务总数量

			BackTask.num   = 0;			   // 回显备份区的保存总个数清0
			KeyMode.f_BackDisp = OFF;	   // 回显模式标志清除
			Key.f_IsOkPressed1 = OFF; 	   // 清除900U OK按键信息
			break;
		}
		case 1:    // 作业启动后，上位机发本次命令之前采集到1组完成的显示
		{
			FinishTask.num--;			   // 完成任务区减1
			if(FinishTask.num == 1)
			{	// 后往前挪
				// 第一个指针是要保存区的地址，第二个指针是数据区的地址
				PutFinishedToStack_Com(&FinishTask.info[0],&FinishTask.info[1]);
				FinishTask.info[0].full = FinishTask.info[1].full;				
			}
			else
			{
				FinishTask.num = 0;		  // 越界则回到0位置
			}
			break;
		}
//		case 2:    // 作业启动后，上位机发本次命令之前采集到2组完成的显示
//		{
//			FinishTask.num = 0;
//			break;
//		}
		default:   // 作业启动后，上位机发本次命令之前采集到2组或2组以上完成的显示
		{	
			FinishTask.num = 0;
			break;
		}
	}
	// 显示数据获取（货位号，数量，批次号）
	// 根据本次下发显示的任务个数进行协议解析
	switch(usReTaskNum)
	{
		case 1:		// 来1组新数据，看当前剩余个数做处理
		{			
			// 剩余0个或2个都保存在0组，剩余1个则保存在1组
			if(UnfinishTask.num >= MAX_TASK_NUM)
			{ UnfinishTask.num = 0; }

			// 取对应的任务缓冲区
			pt = &UnfinishTask.info[UnfinishTask.num];

			if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
			{	
				curIndex = 12;					// 数据位置
		 		len0     = uart.rBuffer[11];	// 第一个的序列号长度                 
			}
			else //if(uart.rBuffer[5] == 0X00)			//没有批次号的协议
			{
				curIndex = 11;					// 数据位置
				len0     = 0;                   // 批次长度为0
			}			
			// 批次数据
			(*pt).srcode_len  = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				(*pt).srcode[i] = uart.rBuffer[curIndex+i];
			}
			// 库位号+数量
			curIndex += len0;			// uart.rBuffer[11]为第一个序列号长度 

 			// ascii码的Sku
	 		(*pt).sku_ascii[0]   = NULL;		// 库位号	 ，初始化为0，代表无
			(*pt).sku_ascii[1]   = NULL;		// 库位号	 ，初始化为0，代表无
 			(*pt).sku_ascii[2]   = NULL;		// 库位号	 ，初始化为0，代表无
			(*pt).sku_ascii[3]   = NULL;		// 库位号	 ，初始化为0，代表无
			if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
			{
			 	// 判断是否旧协议，不包含后4个库位号
				if( uart.rBuffer[curIndex-4] == NULL )
				{	// 旧协议
					(*pt).dir    = uart.rBuffer[curIndex-3];		// 方向

					(*pt).uni[0] = uart.rBuffer[curIndex-2];		// 单位
					(*pt).uni[1] = uart.rBuffer[curIndex-1];		// 单位				
				}
				else
				{	// 新协议
					(*pt).dir    = uart.rBuffer[curIndex-7];		// 方向

					(*pt).uni[0] = uart.rBuffer[curIndex-6];		// 单位
					(*pt).uni[1] = uart.rBuffer[curIndex-5];		// 单位

	 				(*pt).sku_ascii[0]   = uart.rBuffer[curIndex-4];		// 库位号
					(*pt).sku_ascii[1]   = uart.rBuffer[curIndex-3];		// 库位号
 					(*pt).sku_ascii[2]   = uart.rBuffer[curIndex-2];		// 库位号
					(*pt).sku_ascii[3]   = uart.rBuffer[curIndex-1];		// 库位号
				}
			}

			(*pt).sku	 = uart.rBuffer[curIndex];	  				// 库位号 hex
			(*pt).count  = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 // 数量

//			if(UnfinishTask.num == 0)
			OsCmdSet.TaskCurCount = (*pt).count ; 	  // 保存当前数量
			OsCmdSet.TaskBakCount[UnfinishTask.num] = (*pt).count ; 	  // 保存当前数量

			UnfinishTask.num++;
			break;
		}
		case 2:		// 来2组新数据，原先的都冲掉，全部接收
		{

			if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
			{	
				curIndex = 12;							// 数据位置
		 		len0  	 = uart.rBuffer[11];			// 第1个的批次号长度  
				len1     = uart.rBuffer[6] - len0 - 11; // 第2个的批次号号长度 				              
			}
			else //if(uart.rBuffer[5] == 0X00)			// 没有批次号的协议
			{
				curIndex = 11;					// 数据位置
				len0     = 0;                   // 第1个批次长度为0
				len1     = 0;                   // 第2个批次长度为0
			}			
						
			// 保存第1个批次信息
			UnfinishTask.info[0].srcode_len = len0;
			for(i = 0; i < len0; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[0].srcode[i] = uart.rBuffer[curIndex+i];
			}

			// 库位号1+数量1
			curIndex += len0;			// 得到第1个库位号的位置
			// ascii码的Sku
	 		UnfinishTask.info[0].sku_ascii[0]   = NULL;		//库位号  ，初始化为0，代表无
			UnfinishTask.info[0].sku_ascii[1]   = NULL;		//库位号  ，初始化为0，代表无
 			UnfinishTask.info[0].sku_ascii[2]   = NULL;		//库位号  ，初始化为0，代表无
			UnfinishTask.info[0].sku_ascii[3]   = NULL;		//库位号  ，初始化为0，代表无
			if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
			{
			 	// 判断是否旧协议，不包含后4个库位号
				if( uart.rBuffer[curIndex-4] == NULL )
				{	// 旧协议
					UnfinishTask.info[0].dir    = uart.rBuffer[curIndex-3];		// 方向

					UnfinishTask.info[0].uni[0] = uart.rBuffer[curIndex-2];		// 单位
					UnfinishTask.info[0].uni[1] = uart.rBuffer[curIndex-1];		// 单位				
				}
				else
				{  // 新协议
					UnfinishTask.info[0].dir    = uart.rBuffer[curIndex-7];		// 方向

					UnfinishTask.info[0].uni[0] = uart.rBuffer[curIndex-6];		// 单位
					UnfinishTask.info[0].uni[1] = uart.rBuffer[curIndex-5];		// 单位

	 				UnfinishTask.info[0].sku_ascii[0]   = uart.rBuffer[curIndex-4];		//库位号
					UnfinishTask.info[0].sku_ascii[1]   = uart.rBuffer[curIndex-3];		//库位号
 					UnfinishTask.info[0].sku_ascii[2]   = uart.rBuffer[curIndex-2];		//库位号
					UnfinishTask.info[0].sku_ascii[3]   = uart.rBuffer[curIndex-1];		//库位号
				}
			}

			UnfinishTask.info[0].sku   = uart.rBuffer[curIndex];				// 库位号 hex		
			UnfinishTask.info[0].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量
            OsCmdSet.TaskBakCount[0] = UnfinishTask.info[0].count ;			   // 备份任务1数量，以备按键更改时快速恢复        
			// 保存第2个批次信息
			curIndex += 3;
			UnfinishTask.info[1].srcode_len = len1;
			for(i = 0; i < len1; i++)
			{
				if(i >= MAX_SRCODE_NUM) break;
				UnfinishTask.info[1].srcode[i] = uart.rBuffer[curIndex+i];
			}
			//库位号2+数量2
			curIndex += len1;			// 得到第2个库位号的位置
			// ascii码的Sku
	 		UnfinishTask.info[1].sku_ascii[0]   = NULL;		//库位号   ，初始化为0，代表无
			UnfinishTask.info[1].sku_ascii[1]   = NULL;		//库位号   ，初始化为0，代表无
 			UnfinishTask.info[1].sku_ascii[2]   = NULL;		//库位号   ，初始化为0，代表无
			UnfinishTask.info[1].sku_ascii[3]   = NULL;		//库位号   ，初始化为0，代表无
			if(uart.rBuffer[5] == 0X01)	// 带批次号的协议
			{
			 	// 判断是否旧协议，不包含后4个库位号
				if( uart.rBuffer[curIndex-4] == NULL )
				{	// 旧协议
					UnfinishTask.info[1].dir    = uart.rBuffer[curIndex-3];		// 方向

					UnfinishTask.info[1].uni[0] = uart.rBuffer[curIndex-2];		// 单位
					UnfinishTask.info[1].uni[1] = uart.rBuffer[curIndex-1];		// 单位
				}
				else
				{  // 新协议
					UnfinishTask.info[1].dir    = uart.rBuffer[curIndex-7];		// 方向

					UnfinishTask.info[1].uni[0] = uart.rBuffer[curIndex-6];		// 单位
					UnfinishTask.info[1].uni[1] = uart.rBuffer[curIndex-5];		// 单位

 					UnfinishTask.info[1].sku_ascii[0]   = uart.rBuffer[curIndex-4];		//库位号
					UnfinishTask.info[1].sku_ascii[1]   = uart.rBuffer[curIndex-3];		//库位号
 					UnfinishTask.info[1].sku_ascii[2]   = uart.rBuffer[curIndex-2];		//库位号
					UnfinishTask.info[1].sku_ascii[3]   = uart.rBuffer[curIndex-1];		//库位号
				}
			}

			UnfinishTask.info[1].sku   = uart.rBuffer[curIndex];				// 库位号 hex
			UnfinishTask.info[1].count = (INT16U)(uart.rBuffer[curIndex+1]*256) + (INT16U)uart.rBuffer[curIndex+2];	 //数量

			OsCmdSet.TaskBakCount[1] = UnfinishTask.info[1].count ;			   // 备份任务2数量，以备按键更改时快速恢复
			UnfinishTask.num = 2;
			break;
		}
		default: 		break;
	}

	//0、背景色
//	if(uart.rBuffer[3] >= 0x10)
	{ bk_colour = uart.rBuffer[3]>>4; }
//	else{ bk_colour = uart.rBuffer[3]>>4; }

	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	led.light_type_BK = (uart.rBuffer[8] >> 5)&0X07;	  // 保存在灯组备份区域中
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ led.light_type    = led.light_type_BK; }			  // 非进度条模式才更新灯组显示值
    
	//2、闪烁时间间隔
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
		case 0:		led.flash_period = 80;		break;
		case 1:		led.flash_period = 160;		break;
		case 2:		led.flash_period = 320;		break;
//		case 3:		led_flash_period = 640;		break;
		default:	led.flash_period = 640;		break;
	}
	//3、闪烁模式
    led.flash_mode   = (uart.rBuffer[8])&0X07;
	//4、闪烁用数据置位，停止闪烁
	led.time_refresh = 0;			// 让闪烁时间到
	led.flash_status = 0;			// 先亮

	//5、进度条模式下处理
	// 长等待则处理
	if(Disp.f_Loading == LOAD_MODE_LONG)
	{	
		INT16U	 LoadCount,MinWaitTime;

		//等待时间 > 最新等待时间,直接亮灯显示数据
		LoadCount   = DIGITAL_LED_LOAD_TIME_LONG - Disp.TatolTimeRefresh16;
//		MinWaitTime = (para.str.WtTime+1)*100;
		MinWaitTime = (para.str.WtTime+1);
		//等待时间 > 最新等待时间,直接亮灯显示数据
		if(LoadCount >= MinWaitTime)
		{
			Disp.f_Loading    = LOAD_MODE_OFF;			// 结束等待进度条
			led.light_type    = led.light_type_BK;		// 恢复原先使用的灯组
		}	
		//等待时间还没到最小等待时间，则继续等待到最小时间后才亮灯显示
		else
		{
			Disp.TatolTimeRefresh16  = MinWaitTime - LoadCount;
		}					
	}
	//非进度模式下条控制
	if(Disp.f_Loading == LOAD_MODE_OFF)
	{ SetLcdLay(PAGE_RUN); }
}

/*******************************************************************************
	 					对多字节读命令响应
	input:   none     
	output:  none					
*******************************************************************************/
// 只应答Modbus 0x03号采集按钮命令
void ACK_Multi_Read(void)
{
	UINT8   addr;
	UINT16  usCRC;
    
	UnfinishTask.repeat = FALSE;					// 清重复接收到任务标志    
	addr    = para.str.address;    					//	地址

	uart.wBuffer[0] = addr;                         // addr
	uart.wBuffer[1] = 0x03;                         // function code
    uart.wLen       = 2;    

	// uart.rBuffer[3]这个信息	 ，00为有任务采集，03为无任务采集
		
	if( (OsCmdSet.CurDispMode == DISPMODE_NONE)		// 无任务模式
	  ||((uart.rBuffer[3] == 0x03)&&(UnfinishTask.num == 0)&&(UnfinishTask.remain == 0)) ) // 无任务采集且任务已全部完成
	{  // 当前任务状态为非运行状态
		// 放入	 OK按键信息
		uart.wBuffer[2] = Key.f_IsOkPressed1;//0x00;                     // n byte	
	        
		usCRC = CRC16(uart.wBuffer, 3);
		uart.wBuffer[3] = (INT8U)usCRC;
		uart.wBuffer[4] = (INT8U)(usCRC>>8);
		uart.wLen       = 5; 	 
   
	// 为兼容未600U命令，允许先采集再发显示命令。
	}
	else if(OsCmdSet.CurDispMode == DISPMODE_TASK)
	{	// 当前任务状态设为运行状态
		//本次作业的最后一组数据，则需要所有都拾取完毕后再上传数据,原因如下：
		//最后一组数据，如果有2个待显示，如果第一个显示完毕并且已经按下OK键，此时标签显示第二个，此时上位机询问标签是否已经按下OK键
		//此时实际已经按下一个OK键，设备回答按下一个，上位机会继续询问是否按下OK键（因为设备还有下一个任务），此时标签该回答？？
		//但是任务过程中，上位机询问到标签已经按下OK键后，会补发下一道命令，在下一道命令到达标签之前，是不会有采集OK键命令到达的，
		//因此可以控制，但是，最后2组无法如此控制，因此必须一起上传
		if(FinishTask.num == 0)				 
		{  // 没有按下
			uart.wBuffer[2] = 0x02;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = 0X00;                    
	
			usCRC = CRC16(uart.wBuffer, 5);
			uart.wBuffer[5] = (INT8U)usCRC;
			uart.wBuffer[6] = (INT8U)(usCRC>>8);
			uart.wLen       = 7;                            
		}
		else  if(FinishTask.num == 1)
		{	// 1个按下
			uart.wBuffer[2] = 0x04;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = ((FinishTask.info[0].full<<4)|FinishTask.num);    
			uart.wBuffer[5] = FinishTask.info[0].count/256;                        
			uart.wBuffer[6] = (INT8U)FinishTask.info[0].count;    
	                
			usCRC = CRC16(uart.wBuffer, 7);
			uart.wBuffer[7] = (INT8U)usCRC;
			uart.wBuffer[8] = (INT8U)(usCRC>>8);
			uart.wLen       = 9;                            
		}
		else  if(FinishTask.num == 2)
		{	// 2个按下
			uart.wBuffer[2] = 0x06;                         //n byte    
	
			uart.wBuffer[3] = DISPMODE_TASK;                        
			uart.wBuffer[4] = ((FinishTask.info[1].full<<4)|FinishTask.num);   
			uart.wBuffer[5] = FinishTask.info[0].count/256;                         
			uart.wBuffer[6] = (INT8U)FinishTask.info[0].count;   
			uart.wBuffer[7] = FinishTask.info[1].count/256;                        
			uart.wBuffer[8] = (INT8U)FinishTask.info[1].count;   
	                
			usCRC = CRC16(uart.wBuffer, 9);
			uart.wBuffer[9] = (INT8U)usCRC;
			uart.wBuffer[10] = (INT8U)(usCRC>>8);
			uart.wLen       = 11;                            
		}
	} 
//	else if(OsCmdSet.CurDispMode == DISPMODE_INFO)
	else //if( (OsCmdSet.CurDispMode == DISPMODE_INFO)||(OsCmdSet.CurDispMode == DISPMODE_NONE) )
	{	// 非任务状态或600U模式下，回复按键
		// 当前为信息600U模式下，OK键回复
    
		uart.wBuffer[2] = 0x01;                         // 1 byte
		if(Key.f_IsOkPressed == ON)
			{ uart.wBuffer[3] = 0x01; }                 // Pressed
		else{ uart.wBuffer[3] = 0x00; }                 // UnPressed

		usCRC = CRC16(uart.wBuffer, 4);
		uart.wBuffer[4] = (INT8U)usCRC;
		uart.wBuffer[5] = (INT8U)(usCRC>>8);
		uart.wLen       = 6; 	
	}
		      
	USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);    // 串口发送
	
}
/*******************************************************************************
	 					对多字节地址设定是否成功响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_ReadTestAddr(void)
{
	INT8U addr;
	INT16U usCRC;
        
	addr    = para.str.address;  

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
	uart.wBuffer[2] = 0x00;                     	//Len
	usCRC = CRC16(uart.wBuffer, 3);
	uart.wBuffer[3] = (INT8U)usCRC;
	uart.wBuffer[4] = (INT8U)(usCRC>>8);
	uart.wLen   = 5;                            

	USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);    // 串口发送
}
/*******************************************************************************
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	USART_SendBuff(UART1,(INT8U *)uart.rBuffer, uart.rIndex);	// 串口发送
}
/*******************************************************************************
	 					对多字节写命令错误的响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write_RepeatError(void)
{
	UINT8   addr    = para.str.address;
	UINT16  usCRC;

	uart.wBuffer[0] = addr;                         // addr
	uart.wBuffer[1] = 0x90;                         // ERROR code
	uart.wBuffer[2] = OsCmdSet.LastSoftGetOkCount;                       

	usCRC = CRC16(uart.wBuffer, 3);

	uart.wBuffer[3] = (INT8U)usCRC;
	uart.wBuffer[4] = (INT8U)(usCRC>>8);

	uart.wLen        = 5; 
 
	USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);    // 串口发送
}
/*******************************************************************************
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write(void)
{
	UINT8   addr    = para.str.address;
	UINT16  usCRC;

	uart.wBuffer[0] = addr;                         // addr
	uart.wBuffer[1] = 0x10;                         // function code
	uart.wBuffer[2] = 0x00;                         // start addr
	uart.wBuffer[3] = 0x00;
	uart.wBuffer[4] = 0x00;
	uart.wBuffer[5] = 0x05;

	usCRC = CRC16(uart.wBuffer, 6);	                //数据校验  2021年8月17日				

	uart.wBuffer[6] = (INT8U)usCRC;
	uart.wBuffer[7] = (INT8U)(usCRC>>8);            

	uart.wLen       = 8; 

	USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);  // 串口发送
}
/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void ResetAddr(void)
{
	//显示清除,数据复位
	Display_Clear();			   // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	sys_data_reset();			   // 初始化任务相关变量
	//地址清零
	para.str.address = 0;
	SaveParaToBlock();			   // 保存参数
}
void ACK_Multi_Read_Humidity(void)
{
	UINT8   addr    = para.str.address;
	UINT16  usCRC;
	UINT16  usHumidity;
	UINT8   usHumidity_h;
	UINT8   usHumidity_l;
	UINT16  usTemprature;
	UINT8   usTemprature_h;
	UINT8   usTemprature_l;
	usHumidity = sht30_par.SHT30_Humidity*10;
	usHumidity_h = usHumidity/256;
	usHumidity_l = usHumidity%256;
	usTemprature = sht30_par.SHT30_Temprature*10;
	usTemprature_h = usTemprature/256;
	usTemprature_l = (usTemprature%256);
	
	
	
	uart.wBuffer[0] = addr;                         // addr
	uart.wBuffer[1] = 0x03;                         // function code
	uart.wBuffer[2] = 0x04;                         // start addr
	uart.wBuffer[3] = usHumidity_h;
	uart.wBuffer[4] = usHumidity_l;
	uart.wBuffer[5] = usTemprature_h;
	uart.wBuffer[6] = usTemprature_l;

	usCRC = CRC16(uart.wBuffer, 7);	                //数据校验  2021年8月17日				

	uart.wBuffer[7] = (INT8U)usCRC;
	uart.wBuffer[8] = (INT8U)(usCRC>>8);            

	uart.wLen       = 9; 

	USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);  // 串口发送

}




/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void DiscodeProtocalV1(void)
{
	UINT8   addr    = para.str.address;		// 取设备地址
	UINT16  usCRC, retCRC;					// 对接收的数据做CRC校验，并和数据的CRC对比
    
//    unsigned char qr_code[220];             //接收二维码缓存

	retCRC = CRC16(uart.rBuffer, uart.rIndex-2);		// CRC
	usCRC  = (UINT16)(uart.rBuffer [uart.rIndex-2]);	// CRC_L;
	usCRC += (UINT16)(uart.rBuffer [uart.rIndex-1]<<8);	// CRC_H

//	if (retCRC == usCRC) 						//crc校验成功 
//	{
		if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
		{
			if(uart.rBuffer[3] == 0X00)						//地址清零
			{ 
                ResetAddr();                                //地址清零并保存参数
            }
			else if(uart.rBuffer[3] == 0X01)				// 接收地址，并显示，进入等待按键确认
			{
				if(OsCmdSet.beSettingAddr != 0X02)			// 未回复地址设定
				{
					OsCmdSet.beSettingAddr = 1;				// 进入地址设定
					para_bk.str.address = uart.rBuffer[5];	// 把设定地址存入备份区

					//设置更新信息，显示地址设定提示
					// led.light_type = LED_GREEN;				// 接收前亮第一组灯，接收后亮第二组灯
					// led.flash_mode = LED_FLASH_MODE_ALWAYS;	// 常亮模式
					OsCmdSet.set_addr = 1;
					lcd_background.Set_Mode = 1;
					SetLcdLay(PAGE_CFGADDR);				// 设置显示页面为地址显示
				}
			}
		}
		else if( (uart.rBuffer[0] == addr)&&(addr != 0)	)			//地址校验
			{
				switch (uart.rBuffer[1])
				{
					case READ_MULTI_BYTE:	// 终端上传数据,0X03号modbus命令：通信时询问是否已经按完成键，设置地址时询问是否已经按完成键接收地址
					{
						if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			// 寄存器数量01 表示尝试地址是否已经设定的命令
						{ ACK_Multi_ReadTestAddr();	}
						if(uart.rBuffer[5] == 0x0E)
						{ ACK_Multi_Read_Humidity(); }
					}
					case WRITE_SINGLE_BYTE:		//06号命令
					{
						if(uart.rBuffer[3] == 0xC1)
						{
							ACK_Single_Write();
//							if((uart.rBuffer[4]/16 < 9) && (uart.rBuffer[4] % 16 < 9) && (uart.rBuffer[5] / 16 < 9) && (uart.rBuffer[5] % 16 < 9))
							para_bk.str.SHT30_Humidity_SET = ((float)(uart.rBuffer[4]*256 + uart.rBuffer[5]))/10;
//								para_bk.str.SHT30_Humidity_SET = uart.rBuffer[4]*10 + uart.rBuffer[5]/16; 
							if(para_bk.str.SHT30_Humidity_SET < 99) 
							{
								para.str.SHT30_Humidity_SET = para_bk.str.SHT30_Humidity_SET;
								SaveParaToBlock();					 	// 判断参数是否在范围内(如不再则恢复)，保存参数
								SetLcdLay(PAGE_CFGFUNC1);				// 设置显示页面为地址显示					
							}

						}
					}
				}
			}
//	}
}

/*******************************************************************************
	 					处理MODBUS总站下发的命令
每组命令共14个字节；
BYTE_ADD,BYTE_HEAD,BYTE1,---> BYTE10 , CRC_H , CRC_L

BYTE_ADD 	: 终端地址
BYTE_HEAD	:  下传命令码
BYTE1		:  下传指令号
BYTE2		:  LED指示灯显示方式
BYTE3		:  本次下发的"电子标签"个数
BYTE4		:  主机中的此地址剩余"电子标签"个数
BYTE5-BYTE7	:   第一个"电子标签"内容
BYTE8-BYTE10:   第二个"电子标签"内容	
BYTE11		: CRC_H
BYTE12		: CRC_L

下传命令号:
00-	  清除设备的所有显示信息即标签将不工作;
01-	  电子标签与商品条码进行绑定模式；此时3~10位数据无效，数据侦长度2字节
02-    指示灯显示方式单独控制，此时3~10位数据无效，数据侦长度2字节
03-	  下发显示数据以及指示灯的信息，即拾取、上货、盘点作业；
         数据侦长度7字节或者10字节
*******************************************************************************/
void Cmd_Process(void)
{
	if( (KeyMode.f_Config == ON)||(Key.f_LongFunc == TRUE) )    // 配置状态，或配置密码状态下退出 
	{ 
        return; 
    }	
	if (uart.rIndex < ONE_CMD_MIN_LENGTH)       // 数据包长度不够,不做Modbus应答
	{ 
        return; 
    }		
    // 先看地址、CRC校验、数据长度等判断
	// 再根据第二个字节的范围判断是采用新协议解析还是旧协议解析
	// 地址与CRC数据校验,下传指令存储,MODBUS应答,CRC_Check函数中包含地址设定命令
	DiscodeProtocalV1();

}
/*******************************************************************************
								END					
*******************************************************************************/

