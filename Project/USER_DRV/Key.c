/******************************************************************************
* 文件名: Key.c
* 作者  : 黎昌其
* 版本  : V1.00
* 日期  : 2014-08-13
* 描述  : 按键扫描以及按键处理.
********************************************************************************/

#include "../Head file/includes.h" 

float last_humi_set = 0;
/*******************************************************************************
	 			  设置回显
	input:   none    
	output:  none					
*******************************************************************************/  
void  BackDispSet(void) 
{

	KeyMode.f_BackDisp = ON;	  // 置回显标志
	//BackTask.index     = 1;		  // 当前回显索引
	BackTask.index     = BackTask.num ;		// 回显最后一个
	BackTask.page      = 1;		  // 当前回显页清0
	SetLcdLay( PAGE_BACKDISPLAY );// 其他页面进入回显页面，页面更新，需要更新背景图
}  
/*******************************************************************************
	 			  根据F1高4位参数触发回显
	input:   none    
	output:  none					
*******************************************************************************/
void  F1_BackDispSet(void) 
{
  if ( para.str.FinishedLED & 0xF0 )
  { 
    KeyMode.f_F1BackDisp  = ON;
//    BackDispSet(); 
	SetLcdLay( PAGE_RUN );// 其他页面进入回显页面，页面更新，需要更新背景图
  }  
}

/*******************************************************************************
	 			  任务保存共用函数
	input:   ptr1为保存区的地址，ptr2 为数据区的地址     
	output:  none					
*******************************************************************************/
void	PutFinishedToStack_Com(TASK_INFO XDATA *ptr1,TASK_INFO XDATA *ptr2)
{
	UINT8 i ,len;
		  
	len = (*ptr1).srcode_len = (*ptr2).srcode_len;  

	if(len > MAX_SRCODE_NUM) { len = MAX_SRCODE_NUM; }
	// 保存批次信息
	for(i = 0; i < len; i++)
	{  
        (*ptr1).srcode[i] = (*ptr2).srcode[i]; 
    }

	(*ptr1).sku   = (*ptr2).sku;					// 库位号
	(*ptr1).count = (*ptr2).count;					// 数量

	(*ptr1).dir   = (*ptr2).dir;					// 方向
	(*ptr1).uni[0] = (*ptr2).uni[0];				// 单位
	(*ptr1).uni[1] = (*ptr2).uni[1];				// 单位

	// 保存4个库位信息
	for(i = 0; i < 4; i++)
	{ (*ptr1).sku_ascii[i]   = (*ptr2).sku_ascii[i]; }
//	(*ptr1).full  = (*ptr2).full;					// 满箱操作标志
}	

/*******************************************************************************
	 			一个拣货任务完成后，存入任务历史堆栈，以供回显使用
	input:   none     
	output:  none					
*******************************************************************************/
void PutFinishedToStack(void)
{
	UINT8 i ;

	//堆栈未满，直接压入
	if (BackTask.num < MAX_BACK_NUM)
	{
		// 第一个指针是要保存区的地址，第二个指针是数据区的地址
		PutFinishedToStack_Com(&BackTask.info[BackTask.num],&UnfinishTask.info[0]);
		BackTask.num++;
	}
	//堆栈已满，先移动再压入
	else
	{
		for (i = 0; i < MAX_BACK_NUM-1; i++)
		{
			// 第一个指针是要保存区的地址，第二个指针是数据区的地址
			PutFinishedToStack_Com(&BackTask.info[i],&BackTask.info[i+1]);
		}
		// 第一个指针是要保存区的地址，第二个指针是数据区的地址
		PutFinishedToStack_Com(&BackTask.info[MAX_BACK_NUM-1],&UnfinishTask.info[0]);
		BackTask.num = MAX_BACK_NUM;
	}
}
/*******************************************************************************
 正常模式下，改变对应的值，并保证在范围内    
 sts = 0则自减，非0自加        
*******************************************************************************/


void NormolChgVal(INT8U sts)
{
	INT8U	ucFocus,rev,val[DISPLAY_MAX_BIT];
	INT8U	max;
	INT16U	usValue;
	INT16U  tem16_q,tem16_b,tem16_s,tem16_g;
	INT32U  tem32;
	INT16VU *ptr;

	// 无数量值修改标志则返回
	if( (KeyMode.f_ChangeValue1 == OFF)&&(KeyMode.f_ChangeValue2 == OFF) )
	{ return; }

	if(KeyMode.f_ChangeValue1 == ON)
		{ ptr = &UnfinishTask.info[0].count; }
	else{ ptr = &UnfinishTask.info[1].count; }

	//其它原先没有长按下Fn键的情况下，并且当前显示数值<DISPLAY_MAX_NUM，并且任务显示时
	usValue = *ptr;//UnfinishTask.info[0].count;			   // 取出当前数量值
	ucFocus = Disp.Focus ;							   // 取出当前焦点位
//	if( (DispMode == 0X00) && (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) && (task_unfinish_num <= MAX_TASK_NUM) )
//	if( (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) && (task_unfinish_num <= MAX_TASK_NUM) )
	if( (UnfinishTask.num  > 0) && (usValue <= DISPLAY_MAX_NUM) && (UnfinishTask.num  <= MAX_TASK_NUM) )
	{
		// 高位存在0位
		tem32  = usValue;
		val[0] = usValue / 10000;   tem16_q = usValue = (usValue % 10000);		 
		val[1] = usValue / 1000;	tem16_b = usValue = (usValue % 1000); 
		val[2] = usValue / 100 ; 	tem16_s = usValue = (usValue % 100);  
		val[3] = usValue / 10 ; 	tem16_g = usValue = (usValue % 10);			;
		val[4] = usValue  ;

		//---------------------------------------------
		// 5位显示有超过65535问题
		max = 9;	
		if(ucFocus == 0)
		{// 万位
			max = DISPLAY_MAX_NUM / 10000; //6;
			tem32 = tem16_q + (DISPLAY_MAX_NUM - DISPLAY_MAX_NUM % 10000);//(60000 + tem16_q);
		}
		else if(val[0] >= (DISPLAY_MAX_NUM / 10000))//6)
		{ //万位为6，60000
			if(ucFocus == 1)	
		  	{// 调整千位
			 	max = (DISPLAY_MAX_NUM % 10000) / 1000;//5;
				tem32 = tem16_b + (DISPLAY_MAX_NUM - DISPLAY_MAX_NUM % 1000);//(65000 + tem16_b);
			}
			else if(val[1] >= ((DISPLAY_MAX_NUM % 10000) / 1000))//5)
		  	{// 千位为5	，65000
				if(ucFocus == 2)
				{ // 调整百位
			 		max = (DISPLAY_MAX_NUM % 1000) / 100;//5;
					tem32 = tem16_s + (DISPLAY_MAX_NUM - DISPLAY_MAX_NUM % 100);//(65500 + tem16_s);
				}
				else if(val[2] >= ((DISPLAY_MAX_NUM % 1000) / 100))//5)
				{ // 百位为5，65500
					if(ucFocus == 3)
					{ // 调整十位
			 			max = (DISPLAY_MAX_NUM % 100) / 10;//3;
						tem32 = tem16_g + (DISPLAY_MAX_NUM - DISPLAY_MAX_NUM % 10);//(65530 + tem16_g);
					}
					else if(val[3] >= ((DISPLAY_MAX_NUM % 100) / 10))//3)
					{  // 十位为3 ，65530
					   max = (DISPLAY_MAX_NUM % 10);//5;
					   tem32 = DISPLAY_MAX_NUM;//65535;
					}
				}
			}
		}

		if( tem32 > DISPLAY_MAX_NUM )
		{ max --; }
		//---------------------------------------------

		ucFocus = ucFocus % DISPLAY_MAX_BIT;		
		rev = val[ucFocus];

		if(sts != 0){ rev++; }
		else		{ rev--; }
		
//		if(rev > 9)
		if(rev > max)
		{
			if(sts != 0){ rev = 0; }
//			else		{ rev = 9; }
			else		{ rev = max; }
		}
		val[ucFocus] = rev;


		tem32 = (val[0]*10000 + val[1]*1000 + val[2]*100 + val[3]*10 + val[4]);
		if( tem32 > DISPLAY_MAX_NUM )
			{ usValue = DISPLAY_MAX_NUM; }
		else{ usValue = (INT16U)tem32; }

//		UnfinishTask.info[0].count = usValue ;		// 把值写回存储器
		*ptr = usValue ;		// 把值写回存储器
		Disp.Focus				   = ucFocus ;		// 存储对应焦点位
		Disp.TatolTimeRefresh8	= TIMER_MODIFY;		// 超时退出
		LCD.f_LcdEnable         = WAIT_REFRESH;		// 等待刷新总标，不更新背景
	}
}

/*******************************************************************************
 正常模式下，改变对应的焦点位，并保证在范围内   
  sts = 0则焦点位后移，非0焦点位前移            
*******************************************************************************/
void NormolChgFoc(INT8U sts)
{
	INT8U	ucFocus = Disp.Focus;
	INT16U	usValue = UnfinishTask.info[0].count;	
	INT8U   flag = 0;
	
	if( KeyMode.f_ChangeValue2 == ON )
	{ usValue = UnfinishTask.info[1].count; }			
	// 有任务，且数量小于最大可显示数值，且任务数量小于最大数量
	//其它原先没有长按下Fn键的情况下，并且当前显示数值<DISPLAY_MAX_NUM，并且任务显示时
//	if (DispMode == 0X00 && task_unfinish_num > 0 && usValue <=DISPLAY_MAX_NUM)
//	if( (task_unfinish_num > 0) && (usValue <=DISPLAY_MAX_NUM) )
	if( (UnfinishTask.num  > 0) && (usValue <= DISPLAY_MAX_NUM) && (UnfinishTask.num  <= MAX_TASK_NUM) )
	{
		if(sts != 0)
		{ 
			if( (KeyMode.f_ChangeValue1 == OFF)&&(KeyMode.f_ChangeValue2 == OFF) )
			{ ucFocus = DISPLAY_MAX_BIT-1; KeyMode.f_ChangeValue1  = ON; }				 // 把设置值设在最低位
			else if( ++ucFocus >= DISPLAY_MAX_BIT)			// 焦点位前移
			{ 
				ucFocus = 0; 								// 焦点位前移
				flag    = 1;
			}
		}
		else
		{
			if( (KeyMode.f_ChangeValue1 == OFF)&&(KeyMode.f_ChangeValue2 == OFF) )
			{ ucFocus = DISPLAY_MAX_BIT-1; KeyMode.f_ChangeValue1  = ON; }				// 把设置值设在最高位
			else if( ucFocus-- == 0 )
			{ 
				ucFocus = DISPLAY_MAX_BIT-1; 				// 焦点位后移
				flag    = 1;
			}		   			
		}	
		// 双屏，且2个任务时，循环到头则调到下个位置
		if( (flag != 0)&&(ONLY_DISPLAY_MODE != DispDir.only)&&(UnfinishTask.num  > 1) )
		{
			if( KeyMode.f_ChangeValue1 == ON )
			{
				KeyMode.f_ChangeValue1  = OFF; 
				KeyMode.f_ChangeValue2  = ON; 
			}
			else
			{
				KeyMode.f_ChangeValue2  = OFF; 
				KeyMode.f_ChangeValue1  = ON; 
			}
		}
					   
		Disp.Focus             = ucFocus;			// 得到焦点位
//		KeyMode.f_ChangeValue1  = ON; 				// 设置值修改标志
		Disp.TatolTimeRefresh8 = TIMER_MODIFY;		// 超时退出
		LCD.f_LcdEnable        = WAIT_REFRESH;		// 等待刷新总标，不更新背景
	}

}
/************************************************************************
* 按键正常功能,非配置状态
* 输入按键值
************************************************************************/
void	KeyNormolFun(INT8U keyvalue)
{
	 INT8U	i;

	// 如进入配置参数密码状态，非"后退"按键，则清除配置及显示，退出
	// 其他按键清除该配置状态
	if((Key.f_LongFunc == TRUE)&&( keyvalue != KEYFUN_BACK ))
	{  Display_Clear(); return; }

	// 提取出共用的部分
	// 原先按下Fn做满箱操作的情况下,则非OK键，清除该标志并刷新显示
	// 满箱操作，数量区显示“------”  ，清楚该显示后恢复原来的数量
	if( (keyvalue != KEYFUN_OK)&&((Key.f_IsFnKeyPressed1 == ON)||(Key.f_IsFnKeyPressed2 == ON)) )
	{
		Key.f_IsFnKeyPressed1 = OFF;  				// 其他按键清满箱标志
		Key.f_IsFnKeyPressed2 = OFF;  				// 其他按键清满箱标志
		LCD.f_LcdEnable      = WAIT_REFRESH;		// 等待刷新总标，不更新背景
		return;
	}

	//==================================================================			
	switch( keyvalue )
	{	//--------------------------------------------------------------
		// 功能键  长按完成退出功能,短按完成参数页面切换功能
		case KEYFUN_FUNC:
		{ 			
			if(Key.f_LongOnce == TRUE)
			{  // 长按
				Key.f_LongOnce = FALSE;		// 只执行一次
				
				// 无作业任务且完成缓冲区空，长按进入参数查看及配置界面.
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		
				{  // 未完成为0，剩余为0，则认为任务为空

					Key.f_LongFunc = TRUE;						// 进入配置状态的密码显示状态
					Key.CfgPwdCnt  = 0;							// 密码核对次数
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/3;		// 超时退出
					SetLcdLay( PAGE_CFGPWD );				   	// 进入地址配置密码显示页面

					// 把进入配置状态改到移位键
//					KeyMode.f_Config = ON;						   // 进入配置状态
//					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 超时退出					
//					Disp.Focus		 = 2;						   // 焦点位设置,地址的最低位
//					Disp.MenuPage	 = 0;						   // 菜单页面设置 ，进入地址设置界面
// 					// 取出参数放入备份区
//					for (i = 0; i < MAX_CFGPARA; i++)
//					{ 			
//						para_bk.Byte[i] = para.Byte[i]; 		   // 保存入备份区，设置和显示用的
//					} 
//					SetLcdLay( PAGE_CFGADDR );				 	   // 其他页面进入配置界面，页面更新，需要更新背景图
				}
//				else if(UnfinishTask.num > 0) 
				else if( (UnfinishTask.num > 0) )//&& ((KeyMode.f_ChangeValue1 == ON) || (KeyMode.f_ChangeValue2 == ON)) )
				{	// 在拣货过程中(有任务显示的时候)，长按Fn键，对数量清零（以支持零拣）或置满箱操作标志
					// 把数量清零（以支持零拣）的功能转给长按‘-’键了
					switch(para.str.FnDefine)
					{
//						case 0:
//						if(DispMode != 0X01)
//						{
//							task_un_goods_numH[0] =  0;
//							task_un_goods_numL[0] =  0;
//							LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标志
//						}
//						break;
						case 1:
//						if( KeyMode.f_ChangeValue1 == ON )
							{ Key.f_IsFnKeyPressed1 = ON; }					// 置满箱操作标志
//						else
							{ Key.f_IsFnKeyPressed2 = ON; }	
						LCD.f_LcdEnable      = WAIT_REFRESH;		// 等待刷新总标，不更新背景
						break;
					}
				}                    				
			}
			else
			{	//非配置状态下,短按Fn

				// 拣货过程中(有任务显示的时候)，短按Fn键，恢复下发时的初始数据
				if(UnfinishTask.num > 0)
				{	// 可快速恢复到数值

					if( ( KeyMode.f_ChangeValue1 == ON ) )
					{
						UnfinishTask.info[0].count = OsCmdSet.TaskBakCount[0];	// 恢复当前显示任务
						LCD.f_LcdEnable  = WAIT_REFRESH;					// 等待刷新总标，不更新背景
					}
					else if( ( KeyMode.f_ChangeValue2 == ON ) )
					{
						UnfinishTask.info[1].count = OsCmdSet.TaskBakCount[1];	// 恢复当前显示任务
						LCD.f_LcdEnable  = WAIT_REFRESH;					// 等待刷新总标，不更新背景
					}
				}
				// 只有全部拣货任务完成后才能进入回显模式
				// 开始回翻数据，每按一次FN键往前回翻一个，此时SYS灯亮，并且指示灯颜色不变，快闪显示，按OK键或者全部回翻完毕后结束恢复至原来状态
				else if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) ) 
				{
					// 有回显任务才进入
					if(BackTask.num > 0)
					{
						//进入回显模式
						if(KeyMode.f_BackDisp == OFF)
						{	// 非回显模式则置回显模式标志，并进入回显页面

//							KeyMode.f_BackDisp = ON;	  // 置回显标志
//							//BackTask.index     = 1;		  // 当前回显索引
//							BackTask.index     = BackTask.num ;		// 回显最后一个
//							BackTask.page      = 1;		  // 当前回显页清0
//							SetLcdLay( PAGE_BACKDISPLAY );// 其他页面进入回显页面，页面更新，需要更新背景图

                          	KeyMode.f_F1BackDisp = OFF;       // 清F1触发的回显标志
                          	BackDispSet();                    // 设置回显页面
						}
						else
						{
							if(BackTask.index == 1)
							{	// 回显模式且回显到第一个则退出回显页面
								Display_Clear();		  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
							}
							else
							{
								BackTask.index--;							// 索引++
//								BackTask.page = BackTask.index/5 + 1;		// 判断是否换页
								LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标，不更新背景
							}

//							if(BackTask.page*4 >= BackTask.num)
//							{ Display_Clear(); }		   // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
//							else
//							{  
//
//								BackTask.index = BackTask.page*4+1;			// 索引位置对应变
//								BackTask.page ++; 							// 页增加
//								LCD.f_LcdEnable  = WAIT_REFRESH; 			// 等待刷新总标，不更新背景
//							}					
						}
					}
				}									 
			}
		}break;					
		//--------------------------------------------------------------
		// ‘-’ 减一键		
		case KEYFUN_SUB :
		{						
			if(Key.f_LongOnce == TRUE)
			{  // 长按
				Key.f_LongOnce = FALSE;		// 只执行一次

				if( (UnfinishTask.num > 0) ) 	
				{  // 拣货过程中(有任务显示的时候)，长按-1键，清零（以支持零拣）
//					if( ((Key.SubLongCnt & 0x01) == 0x01)&&( ONLY_DISPLAY_MODE != DispDir.only)&&(UnfinishTask.num > 1) )
					if( ( KeyMode.f_ChangeValue2 == ON ) )
					{
					UnfinishTask.info[1].count = 0;		 // 清0
					LCD.f_LcdEnable  = WAIT_REFRESH;	 // 等待刷新总标，不更新背景
					}
					else if( ( KeyMode.f_ChangeValue1 == ON ) )
					{
					UnfinishTask.info[0].count = 0;		 // 清0
					LCD.f_LcdEnable  = WAIT_REFRESH;	 // 等待刷新总标，不更新背景
					}
//					Key.SubLongCnt++;
				}
			}
			else
			{  // 短按
				NormolChgVal(0);		// 判断条件符合则设置数量
			}			 	
		}break;		
		//--------------------------------------------------------------
		// ‘+’ 加一键
		case KEYFUN_ADD   :
		{								
			{
				NormolChgVal(1);		// 判断条件符合则设置数量
			}
		}break;							
		//--------------------------------------------------------------
		// 向前键 
		case KEYFUN_FORWARD :
		{								

			if(KeyMode.f_BackDisp == ON)
			{  // 回显界面
				if((BackTask.index >= BackTask.num)||(BackTask.index >= MAX_BACK_NUM))
				{	// 回显模式且回显到第一个则退出回显页面
					Display_Clear();		  		// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
				}
				else
				{
					BackTask.index++;							// 索引++
					BackTask.page = BackTask.index/5 + 1;		// 判断是否换页
					LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标，不更新背景
				}
			}
			else
			{	// 非回显界面，有任务则进入该拣货数量界面
				NormolChgFoc(1);						// 焦点位前移
			}	
		}break;
		
		//--------------------------------------------------------------
		// 向后键 	
	    case KEYFUN_BACK:
	    {			
//			if(KeyMode.f_BackDisp == ON)
//			{// 回显界面
//
//				if(BackTask.index == 1)
//				{	// 回显模式且回显到第一个则退出回显页面
//					Display_Clear();		  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
//				}
//				else
//				{
//					BackTask.index--;							// 索引++
//					BackTask.page = BackTask.index/5 + 1;		// 判断是否换页
//					LCD.f_LcdEnable  = WAIT_REFRESH;			// 等待刷新总标，不更新背景
//				}
//			}
			if(Key.f_LongFunc == TRUE)
			{
 				if( Key.CfgPwdCnt++ >= 3 )
				{
					Key.f_LongFunc =  FALSE;

					// 进入配置状态
					KeyMode.f_Config = ON;						   // 进入配置状态
					Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 超时退出					
					Disp.Focus		 = 2;						   // 焦点位设置,地址的最低位
					Disp.MenuPage	 = 0;						   // 菜单页面设置 ，进入地址设置界面
 					// 取出参数放入备份区
					for (i = 0; i < MAX_CFGPARA; i++)
					{ 			
						para_bk.Byte[i] = para.Byte[i]; 		   // 保存入备份区，设置和显示用的
					} 
					SetLcdLay( PAGE_CFGADDR );				 	   // 其他页面进入配置界面，页面更新，需要更新背景图
				}
				else 
				{ 
					Disp.TatolTimeRefresh8 = TIMER_CONFIG/6;		// 超时退出
					LCD.f_LcdEnable  = WAIT_REFRESH;				// 等待刷新总标，不更新背景
				}
			}
			else
			{	// 非回显界面，有任务则进入该拣货数量界面
				NormolChgFoc(0);						// 焦点位后移
			}	
		}break;	
		
		//--------------------------------------------------------------
		// 确认键 		
	    case KEYFUN_OK:
	    {		    			
			//修改焦点位数值状态关闭，不再闪烁
			KeyMode.f_ChangeValue1  = OFF; 				// 清修改标志
			KeyMode.f_ChangeValue2  = OFF; 				// 清修改标志

			// 无任务时 置OK键是否按下  给900U命令用  必须在前面设置，后面的处理会把它清除
			if(UnfinishTask.num == 0)
		    { Key.f_IsOkPressed1 = ON;  }

			//只有在未完成任务不空，并且非显示进度条时，才响应OK键。
			if( (UnfinishTask.num > 0)&&(Disp.f_Loading == LOAD_MODE_OFF) )
			{   //刷新数据
				//检查完成情况
				if( FinishTask.num < MAX_TASK_NUM )	//最大任务数2
				{
					//把数据存入已完成的数据区
					// 第一个指针是要保存区的地址，第二个指针是数据区的地址
					PutFinishedToStack_Com(&FinishTask.info[FinishTask.num],&UnfinishTask.info[0]);					
					// 判断是否有满箱标志
//					if(Key.f_IsFnKeyPressed == ON)	
//						{ FinishTask.info[FinishTask.num].full = 1; }						
//					else{ FinishTask.info[FinishTask.num].full = 0; }
					if(Key.f_IsFnKeyPressed1 == ON)	
						{ FinishTask.info[0].full = 1; }						
					else{ FinishTask.info[0].full = 0; }
					FinishTask.num++;					// 完成任务数+1

					//完成的数据做历史存储以供回显使用
					PutFinishedToStack();
					//原先有2个任务队列，则未完成块中作处理
					if(UnfinishTask.num == 2)
					{
						//当未完成块的数据=2时，数据前移 ，后往前挪                                           					
						// 第一个指针是要保存区的地址，第二个指针是数据区的地址
						PutFinishedToStack_Com(&UnfinishTask.info[0],&UnfinishTask.info[1]);
						OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;		// 保存当前数量
					}
					UnfinishTask.num--;

					// 双屏显示
					if( (DOUBLE_DISPLAY_MODE == DispDir.only)&&(UnfinishTask.num > 0) ) 
				    {
					
						if( FinishTask.num >= MAX_TASK_NUM )
						{ FinishTask.num = 0; }

						//把数据存入已完成的数据区
						// 第一个指针是要保存区的地址，第二个指针是数据区的地址
						PutFinishedToStack_Com(&FinishTask.info[FinishTask.num],&UnfinishTask.info[0]);					
						// 判断是否有满箱标志
//						if(Key.f_IsFnKeyPressed2 == ON)	
						if( (Key.f_IsFnKeyPressed1 == ON)||(Key.f_IsFnKeyPressed2 == ON)	)
							{ FinishTask.info[1].full = 1; }						
						else{ FinishTask.info[1].full = 0; }
						FinishTask.num++;					// 完成任务数+1

						//完成的数据做历史存储以供回显使用
						PutFinishedToStack();
						//原先有2个任务队列，则未完成块中作处理
						if(UnfinishTask.num == 2)
						{
							//当未完成块的数据=2时，数据前移 ，后往前挪                                           					
							// 第一个指针是要保存区的地址，第二个指针是数据区的地址
							PutFinishedToStack_Com(&UnfinishTask.info[0],&UnfinishTask.info[1]);
							OsCmdSet.TaskCurCount = UnfinishTask.info[0].count ;		// 保存当前数量
						}
						UnfinishTask.num--;					
					}
				}

				//该标志下，按下OK则后面任务全部清除
				if( (Key.f_IsFnKeyPressed1 == ON)||(Key.f_IsFnKeyPressed2 == ON) )
				{

					UnfinishTask.num     = 0;		   // 未完成任务清0
					UnfinishTask.remain  = 0;		   // 剩余任务清0
					Key.f_IsFnKeyPressed1 = OFF; 	   // 清除Fn长按标志
					Key.f_IsFnKeyPressed2 = OFF; 	   // 清除Fn长按标志				
				}
				//本次作业一全部完成任务，灯灭，数据清零 
				if( (UnfinishTask.num == 0) && (UnfinishTask.remain == 0) )		// 未完成为0，剩余为0，则认为任务为空	
				{					
					Display_Clear();							  // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
					led.light_type = (para.str.FinishedLED & 0x0f);		  // 任务完成根据参数F1亮对应的灯色
					led.flash_mode = LED_FLASH_MODE_ALWAYS;		  // 常亮模式

					F1_BackDispSet();          // 任务完成，根据F1 参数回显
				}
				//本次作业还有待拾取任务，进行拉条显示
				else											
				{	//任务缓冲区空或者主机还有剩余任务,显示-------
					if(UnfinishTask.num > 0)
					{	
						Disp.f_Loading    	    = LOAD_MODE_SHORT;	  // 置短时间载入标志
//						Disp.TatolTimeRefresh32 = (para.str.WtTime+1)*100;	 // LOADING短时间
						Disp.TatolTimeRefresh16 = (para.str.WtTime+1); // 短时间
						
					}
					else //if(task_unupdate_num > 0)				//本次作业还有任务，但是任务还没下发到本设备中，显示------
					{
						Disp.f_Loading    	    = LOAD_MODE_LONG;			 // 置长时间载入标志
//						Disp.TatolTimeRefresh32 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING长时间
						Disp.TatolTimeRefresh16 = DIGITAL_LED_LOAD_TIME_LONG;//LOADING长时间						
					}

					os_time.Base_100ms= TIMER_BASE_100MS;			// 对100ms基准值重设定
//					Disp.TimeRefresh  = DIGITAL_LED_LOAD_CNT_PER;	// 进度条时间赋值;
					//先保存当前哪组灯亮，在设置拉条时没有灯亮
					led.light_type_BK = led.light_type;				// 备份灯颜色
					led.light_type    = LED_NONE;					// 关灯
					SetLcdLay( PAGE_WAIT );							// 其他页面进入等待页面，页面更新，需要更新背景图
				}
			}
			else if(KeyMode.f_BackDisp == ON)		 // 回显状态下，按OK键取消回显
//			else if( (KeyMode.f_BackDisp == ON) && (KeyMode.f_F1BackDisp == OFF) )  // 回显状态下且非F1任务完成触发的回显，按OK键取消回显
			{
				Display_Clear(); 					 // 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志					
			} 
			Key.f_IsOkPressed = ON;      	
		}break;			 							
		default:   	break;		 							
	}					
}

/************************************************************************
* 退出配置模式
************************************************************************/
void	Exit_Cfg_Mode(void)	
{
	INT8U i;
	lcd_background.TIMER_SHT_ENABLED = 1;	
	lcd_background.bar_interrupt_recover = 1;
	lcd_background.Set_Mode = 0;
	Display_Clear(); 					 		// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	OsCmdSet.set_addr = 0;
	if (Disp.TatolTimeRefresh8 == 0) return;    // 10s超时退出不保存

	// 备份区的数据写入主存区
	for (i = 0; i < MAX_CFGPARA; i++)
	{ para.Byte[i] = para_bk.Byte[i]; }			// 备份区的数据写入主存区

	SaveParaToBlock();					 		// 判断参数是否在范围内(如不再则恢复)，保存参数
}	

/************************************************************************
* 退出配置模式(不存储数据)
************************************************************************/
void	Quite_Cfg_Mode(void)	
{
//	INT8U i;
	lcd_background.TIMER_SHT_ENABLED = 1;	
	lcd_background.bar_interrupt_recover = 1;
	lcd_background.Set_Mode = 0;
	Display_Clear(); 					 		// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
	OsCmdSet.set_addr = 0;
	if (Disp.TatolTimeRefresh8 == 0) return;    // 10s超时退出不保存

}
/*******************************************************************************
	                    //改变对应的值，并保证在范围内            
*******************************************************************************/
//const	INT8U	CfgPageDB[MAX_CFGPARA]={ PAGE_CFGADDR,PAGE_CFGFUNC1,PAGE_CFGFUNC2,PAGE_CFGFUNC3,PAGE_CFGFUNC4,PAGE_CFGFUNC5 };
const	INT8U	CfgPageDB[MAX_CFGPARA] = { PAGE_CFGADDR,PAGE_CFGFUNC1,PAGE_CFGFUNC2,PAGE_CFGFUNC3,PAGE_CFGFUNC4};//,PAGE_CFGFUNC5 };

void CfgParaValChg(INT8U sts)
{
	INT8U	rev,max,addr,humility_data,tem_data;//,ValMax;
	INT8U   min;
	INT8U	tem8_g,tem8_s;//,hum8_g;
	INT16U	tmp16,hum16;
	INT8VU	Buffer[3];	
	INT8VU   *ptr;//,*ParaPtr;

	INT8U  page = CfgPageDB[Disp.MenuPage];


	switch(page)
	{
		case PAGE_CFGADDR:   // 地址设定
			max = 9;	 		   // 个位和十位最大值都是9
			tmp16 = addr = para_bk.str.address;
			Buffer[2] = addr /100;	tem8_s = addr = (addr % 100);
			Buffer[1] = addr /10;	tem8_g = addr = (addr % 10);
			Buffer[0] = addr ;	   
			if(Disp.Focus > 2){ Disp.Focus = 2; }
			if(Disp.Focus == 2)
			{  
				max = MAX_ADDR/100;//4;
				tmp16 = (MAX_ADDR - MAX_ADDR % 100) + tem8_s ;//240
			}
			else if( Buffer[2] >= MAX_ADDR/100)
			{  // 百位为2，200
				if(Disp.Focus == 1)
				{	// 设置十位
					max = (MAX_ADDR % 100) /10;//4;
					tmp16 = tem8_g + (MAX_ADDR - MAX_ADDR % 10) ;//240
				}
				else if( Buffer[1] >= (MAX_ADDR % 100) /10)
				{	// 十位为4，240	  , 设置个位
					max = MAX_ADDR % 10;//7;
					tmp16 = MAX_ADDR;
				}
			}
			min = 0;
			if(tmp16 > MAX_ADDR)
			{ max --; }						
			ptr = &Buffer[Disp.Focus]; 		   
		break;
		
		case PAGE_CFGFUNC1:   // 湿度值设定		 		   
			max = 9;
			hum16 = humility_data = para_bk.str.SHT30_Humidity_SET;


			Buffer[0] = humility_data%10;	//个位
			Buffer[1] = humility_data/10;	//十位
			if(Disp.Focus > 1)
			{ 
				Disp.Focus = 1; 
			}    
			min = 0;  
			ptr = &Buffer[Disp.Focus]; 		   	
        break;	

		case PAGE_CFGFUNC2:   // 湿度校准值设定		 		  
			max = 9;
			hum16 = humility_data = para_bk.str.SHT30_Hum_Cal_SET;                   
			Buffer[0] = humility_data%10;
			Buffer[1] = humility_data/10;
			if(Disp.Focus > 1){ Disp.Focus = 1; }  
			if(Disp.Focus == 1)
			{  
				max = MAX_HUM_CAL_SET/10;	//1;
			} 
			min = 0;  
			ptr = &Buffer[Disp.Focus]; 		   
			alarm_scan();
		break;		

		case PAGE_CFGFUNC3:   // 温度校准值设定		 		  
			max = 9;
			hum16 = tem_data = para_bk.str.SHT30_Tem_Cal_SET;                   
			Buffer[0] = tem_data%10;
			Buffer[1] = tem_data/10;
			if(Disp.Focus > 1){ Disp.Focus = 1; }  
			if(Disp.Focus == 1)
			{  
				max = MAX_TEM_CAL_SET/10;	//1;
			} 
			min = 0;  
			ptr = &Buffer[Disp.Focus]; 		   
			alarm_scan();
		break;	

		case PAGE_CFGFUNC4:   // 中英文切换界面		 		  
			max = 1;
			hum16 = tem_data = para_bk.str.Language_switch;                   
			Buffer[0] = para_bk.str.Language_switch;
			if(Disp.Focus > 1){ Disp.Focus = 1; }  
			if(Disp.Focus == 1)
			{  
				max = 1;	//1;
			} 
			min = 0;  
			ptr = &Buffer[Disp.Focus]; 		   
			alarm_scan();
		break;	

		default: 	
		break;
	}
	
	rev =   *ptr;
	if(sts != 0)	//按下+
	{ rev++; }
	else        
	{ rev--;}		//按下-
	
	if(rev > max)
	{
		if(sts != 0)rev = 0;
		else		rev = max;
	}

	*ptr = rev;

	// 地址对应写入存储器
	if(page == PAGE_CFGADDR)
	{ 
		addr  = Buffer[2]*100 + Buffer[1]*10 + Buffer[0]; 
		if(addr > MAX_ADDR)
		{ addr = MAX_ADDR;}
		para_bk.str.address = addr;
	}

	// F1湿度设置
    if( page == PAGE_CFGFUNC1 )
	{ 
//		sht30_par.SHT30_Humidity_SET = Buffer[0];
		last_humi_set = para_bk.str.SHT30_Humidity_SET;
		para_bk.str.SHT30_Humidity_SET	= Buffer[0] + Buffer[1]*10;
		
		
	}   

	// F2湿度校准值设置
	if( page == PAGE_CFGFUNC2 )
	{ 
//		sht30_par.SHT30_Humidity_SET = Buffer[0];
		para_bk.str.SHT30_Hum_Cal_SET = Buffer[0] + Buffer[1]*10;
	}
	
	// F3温度校准值设置
	if( page == PAGE_CFGFUNC3 )
	{ 
//		sht30_par.SHT30_Humidity_SET = Buffer[0];
		para_bk.str.SHT30_Tem_Cal_SET = Buffer[0] + Buffer[1]*10;
	}

	// F4中英文切换界面
	if( page == PAGE_CFGFUNC4 )
	{ 
//		sht30_par.SHT30_Humidity_SET = Buffer[0];
		para_bk.str.Language_switch = Buffer[0];
	}
	
	Disp.f_OffDisp 		   = OFF;				   // 先显示数值
	Disp.TatolTimeRefresh8 = TIMER_CONFIG;		   // 设置超时时间	
	LCD.f_LcdEnable        = WAIT_REFRESH;		   // 等待刷新总标，不更新背景	
}
/*******************************************************************************
	                    //配置页面变换           
*******************************************************************************/
//const	INT8U	  CfgParaFocusDb[MAX_CFGPARA] = {3,1,1,1,1,1} ;
//const	INT8U	  CfgParaFocusDb[] = {3,1,1,1,1,1} ;
const	INT8U	  CfgParaFocusDb[MAX_CFGPARA] = {3,2,2,2,2};//,1} ;//焦点位

void CfgPageChg(INT8U sts)
{
	INT8U	ucCfgStatus = Disp.MenuPage;

	if( ++ucCfgStatus >= MAX_CFGPARA )
	{ ucCfgStatus = 0; }			 	// 功能参数页面切换   
	KeyMode.f_Config = ON;
	Disp.MenuPage = ucCfgStatus;				// 参数页面
	Disp.TatolTimeRefresh8	= TIMER_CONFIG;		// 超时计数器初始化
	SetLcdLay( CfgPageDB[ucCfgStatus] );		// 参数页面更新，页面更新，需要更新背景图
	LCD.f_Update     = TRUE;					// 不更新背景
	lcd_background.TIMER_SHT_ENABLED = 0;
}

/*******************************************************************************
//配置焦点位变换           
*******************************************************************************/

void CfgFocusChg(INT8U sts)
{
	INT8U	ucFocus     = Disp.Focus;
	INT8U	ucCfgStatus = Disp.MenuPage; 
		
	if(sts != 0)
	{ 
		if( ++ucFocus >= CfgParaFocusDb[ucCfgStatus] )
		{ ucFocus = 0; }  
	}
	else
	{
		if( ucFocus-- == 0 )
		{ ucFocus = CfgParaFocusDb[ucCfgStatus]-1; }     
	}
			   
	Disp.Focus = ucFocus;					// 得到焦点位
	Disp.TatolTimeRefresh8	= TIMER_CONFIG;	// 超时计数器初始化
	LCD.f_LcdEnable         = WAIT_REFRESH; // 等待刷新总标，不更新背景
}
/************************************************************************
* 按键配置功能
* 输入按键值
************************************************************************/
void	KeyConfigFun(INT8U keyvalue)
{
	//==================================================================			
	switch( keyvalue )
	{	
		//--------------------------------------------------------------
		// 功能键  长按完成退出功能,短按完成参数页面切换功能
		case KEYFUN_FUNC:
		{ 			
			CfgPageChg(1); 			
		}break;						
		//--------------------------------------------------------------
		// -1		
		case KEYFUN_SUB :
		{						
			CfgParaValChg(0);			  //改变对应的值，并保证在范围内		 	
		}break;		
		//--------------------------------------------------------------
		// +1
		case KEYFUN_ADD   :
		{								
			CfgParaValChg(1);			 //改变对应的值，并保证在范围内
		}break;							
		//--------------------------------------------------------------
		// 向前键 
		case KEYFUN_BACK :
		{								
			CfgFocusChg(1);				//改变对应的焦点位，并保证在范围内
		}break;				
		//--------------------------------------------------------------
		// 确认键 		
	    case KEYFUN_OK:
	    {		    			
			 Exit_Cfg_Mode();		// 退出配置模式
		}break;	
		//--------------------------------------------------------------
		// 取消键 				
		case KEYFUN_QUITE:
		{
			Quite_Cfg_Mode();		//取消配置模式
		}
		// 其他键 		 							
		default:	break;					 							
	}					
}

/************************************************************************
* 按键功能执行子程序
* 输入按键值，使能LCD刷新标志
************************************************************************/
void Key_Fun(INT8U keyvalue)
{		
//	if((FALSE == Key.f_Long)&&(KEYFUN_OK != keyvalue))
//	{ AlarmSet(ALMKEY); } 		//蜂鸣器叫声
		
	// 判断是否主机设置站址
	if(OsCmdSet.beSettingAddr == 1)
	{ 
		//任意键，都记下地址	
		para.str.address = para_bk.str.address;	// 保存设定的地址
		SaveParaToBlock();						// 判断参数是否在范围内(如不再则恢复)，保存参数

//		led.light_type = LED_BLUE;				// 接收前亮第一组蓝色灯，接收后亮第二组灯
		OsCmdSet.beSettingAddr  = 2;			// 说明站址设定已经按下按键确认
	} 

	KeyConfigFun(keyvalue);
}

/************************************************************************
* 获取按键值
* 返回键值		 
************************************************************************/
INT16U	Scan_Val(void)
{

	INT8U	KeyOK_val, KeyFunc_val ;
	//-------------------------------------------
	//检测列1           延时1us 	
//	SETKEYOKBIT();						// 置IO口为1

	KeyOK_val    = Key.OkBackup;
	KeyFunc_val  = Key.FuncBackup;

//	if(TRUE == Key.f_KeyOK_En)
//	{
//		KeyOK_val  = KEYFUN_NOKEY;
//		if(READKEYOKBIT() == Bit_RESET)	  							// 读OK键
//		{ KeyOK_val = KEYFUN_OK; }													
//	}

//	if(TRUE == Key.f_KeyFunc_En)
//	{
////		SETKEYCFGBIT();
////		SETKEYSHIFTBIT();
////		SETKEYADDBIT();

//		KeyFunc_val  = KEYFUN_NOKEY;

//		if(READKEYCFGBIT() == Bit_RESET)	  							// 读CFG键
//		{ KeyFunc_val = KEYFUN_FUNC; }						
//		else if(READKEYADDBIT() == Bit_RESET)
//		{ KeyFunc_val = KEYFUN_SUB; }
//		else if(READKEYSHIFTBIT() == Bit_RESET)	
//		{ KeyFunc_val = KEYFUN_BACK; }
//	}

	if(TRUE == Key.f_KeyOK_En)
	{
		KeyOK_val  = KEYFUN_NOKEY;
		if(iic_key.iic_key_type == 0x08)	  							// 读OK键
		{ 
			KeyOK_val = KEYFUN_OK; 
		}													
	}

	if(TRUE == Key.f_KeyFunc_En)
	{
//		SETKEYCFGBIT();
//		SETKEYSHIFTBIT();
//		SETKEYADDBIT();

		KeyFunc_val  = KEYFUN_NOKEY;

		if(iic_key.iic_key_type == 0x10)	  							
		{ KeyFunc_val = KEYFUN_FUNC; }						
		else if(iic_key.iic_key_type == 0x02)	  							
		{ KeyFunc_val = KEYFUN_SUB; }
		else if(iic_key.iic_key_type == 0x01)	  							
		{ KeyFunc_val = KEYFUN_ADD; }
		else if(iic_key.iic_key_type == 0x20)	  							
		{KeyFunc_val = KEYFUN_BACK;}			//按键从上往下，依次为功能键，移位键，ok键，加键，减键
		else if(iic_key.iic_key_type == 0x04)	  							
		{KeyFunc_val = KEYFUN_QUITE;}
	}
	
	//-------------------------------------------		
	return( KeyFunc_val*256+KeyOK_val );
}

/************************************************************************
** 功能描述： Key scan and Key Function module
************************************************************************/

void Key_Scan(void)
{		
	INT16U	keyval;
	INT8U	okval,funcval;
	/*------------------------------------------------------------------*/
	// 系统锁定或接收到数据，按键无效
	if( (OsCmdSet.SystemLocked == ALL_LOCK) )//||(uart.rIndex != 0) ){return;}
	{ return; }
	//扫描标志使能确认,定时器32ms使能一次
	if(Key.f_Enable == FALSE){ return; }
	Key.f_Enable = FALSE;		//扫描则清使能标志
		
	/*------------------------------------------------------------------*/
	//键值扫描 ，得到对应按键值
	// 先读OK键，OK键有效则不再检测功能键
	keyval = Scan_Val();		
	funcval = keyval/256;	okval = (INT8U)keyval;	 			
	/*------------------------------------------------------------------*/
	// 功能键检测时间长，只检测一次
	// 功能键30ms一次，检测2次
//	if(TRUE == Key.f_KeyFunc_En)
	// FUNC_LOCK 未锁定，且扫描时间到
	if( (OsCmdSet.SystemLocked != FUNC_LOCK)&&(TRUE == Key.f_KeyFunc_En) )
	{
		if( funcval != Key.FuncBackup ) {  }  // 2次不一致不处理
		//长按住不释放，某些键有效
		//第一次按下
		else if(KEYFUN_NOKEY == funcval)
		{ 	// 无功能键按下
			if(TRUE == Key.f_FuncShort)
			{  // 长按未够时间松开处理
			   if((Key.BackProssVal != KEYFUN_NOKEY)||(Key.BackProssVal != KEYFUN_OK))
			   	{
					if(Key.BackProssVal == KEYFUN_FUNC)
					{
						lcd_background.Set_Mode = 1;
						Key_Fun(Key.BackProssVal);
					}
					else if(lcd_background.Set_Mode == 1)
						Key_Fun(Key.BackProssVal);
				}	   		//执行短按功能键
			}
			
			Key.f_FuncShort  = FALSE; 			//短按释放
			Key.BackProssVal = funcval;			//备份键值
		}
		else if( FALSE == Key.f_FuncShort )
		{ 					
//			 Key_Fun(funcval); 				//执行按键功能
			Key.f_FuncShort = TRUE; 			//短按有效				
			Key.BackProssVal = funcval;			//备份键值		
		} 
	
		Key.FuncBackup	= funcval;				// 保存功能键值		
	}
		if( (OsCmdSet.SystemLocked != OK_LOCK)&&(TRUE == Key.f_KeyOK_En) )	
	{	
		if( okval != Key.OkBackup ) {  }	   // 2次不一致不处理
		else if(KEYFUN_NOKEY == okval)
		{ Key.f_OkShort = FALSE; }
		else if(( FALSE == Key.f_OkShort )&&(lcd_background.Set_Mode == 1))
		{ Key_Fun(okval); Key.f_OkShort = TRUE; } 

		Key.OkBackup = okval;					// 保存OK键值	
	}
   Key.f_KeyOK_En   = FALSE; 
   Key.f_KeyFunc_En = FALSE;
}




