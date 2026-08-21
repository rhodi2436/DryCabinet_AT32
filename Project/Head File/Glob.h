
#ifndef GLOB_H
#define GLOB_H

// ************************************************************************* //
#ifdef  GLOB_DEF			//是否声明，该源文件定义，其他模块引用		
	#define CPU_EXT			//没有声明 ，则强制为空，此时为变量定义
#else
	#define CPU_EXT  extern	//已声明，则强制为 extern，此时为变量声明
#endif
// ************************************************************************* //

// ************************全局内存变量定义开始************************* //
//==================================================================================
// 任务存储区
//#define MAX_SRCODE_NUM 	20	//最大接收的显示信息

typedef enum
{
	Dehumidify = 0x00,	//除湿
	humidify   = 0x01,
}Movement_status_indication;

CPU_EXT 	Movement_status_indication	movement_status_indication;

typedef enum
{
	Tasking = 0x00,
	Tasked  = 0x01,
}Lcd_background_type;
CPU_EXT	Lcd_background_type lcd_background_type;
typedef  struct 
{
	Lcd_background_type	lcd_back;				// 层状态，保存还是覆盖
 	Movement_status_indication	lcd_state;				// 层

} LCD_TYPE_SEL;
	
CPU_EXT 	LCD_TYPE_SEL	lcd_type_sel;
	
// 一个任务完整信息
#define MAX_SRCODE_NUM 	128	//最大接收的显示信息
typedef struct 
{
	INT8VU	  sku_ascii[4];				// 库位—ascii 
 	INT16VU   count;		  			// 数量
	INT8VU 	  sku;			  			// 库位—hex
	INT8VU 	  full;			  			// 满箱操作
//	INT8VU	  Dummy1;					// 哑元
	INT8VU	  uni[2];					// 单位
	INT8VU	  dir;						// 方向
	INT8VU 	  srcode_len;	  			// 批次数据长度
	INT8U 	  srcode[MAX_SRCODE_NUM]; 	// 批次信息
}TASK_INFO;

#define MAX_TASK_NUM 	2	//最大的作业任务数量
typedef struct 
{
	INT8VU 	   num;			  		// 计数
	INT8VU	   total;				// 总任务个数
	INT8VU	   order;				// 完成到第几个
	INT8VU	   remain;				// 剩余任务个数
	INT8VU	   repeat;				// 重复接收到任务标志
	INT8VU	   Dummy1;				// 哑元
	TASK_INFO  info[MAX_TASK_NUM];	// 任务信息
}TASK_STR;

#define MAX_BACK_NUM	5	//最大回显信息支持
typedef struct 
{
	INT8VU 	   num;			  		// 计数
	INT8VU	   index;				// 索引
	INT8VU	   page;				// 页
	INT8VU	   Dummy1;				// 哑元
	TASK_INFO  info[MAX_BACK_NUM];	// 任务信息
}BACK_TASK_STR;

typedef struct 
{

	INT8VU 	  srcode_len;	  				// 批次数据长度
	INT8U 	  srcode[MAX_SRCODE_NUM*2]; 	// 批次信息
}INFO_600U;


CPU_EXT 	TASK_STR      XDATA FinishTask;			// 已完成任务信息保存数组
CPU_EXT 	TASK_STR      XDATA UnfinishTask;		// 未完成任务信息保存数组
CPU_EXT 	INFO_600U     XDATA InfoDisp;			// 信息显示

CPU_EXT 	BACK_TASK_STR XDATA BackTask;			// 已完成任务信息备份回显数组

typedef struct 
{
	INT16VU TaskCurCount;				// 当前任务数量备份，
	INT16VU TaskBakCount[MAX_TASK_NUM];	// 任务数量备份，

	INT8VU	LastSoftGetOkCount;			// 上位机软件下发此次命令之前，由0X03号命令采集到的已经OK响应的组数
	INT8VU  CurDispMode ;				// 对应于3种显示模式：0-无内容；1-显示字符串；3-显示分拣任务(1,3实现)
										// 00为非运行状态,01,03为运行状态
	INT8VU  beSettingAddr;				// 0:非设置站址		1:设置站址		2：站址设定已经按下OK键
	INT8VU  SystemLocked;				// 锁定系统、解锁系统
	INT8VU  set_addr;
}OS_TASK_STR;

CPU_EXT 	OS_TASK_STR  DATA OsCmdSet;			// 命令设置系统重要参数

//--------------------------------------------------------------------------------------
// LED指示灯
typedef struct 
{
	INT8VU 	light_type;			// 亮灯类型
	INT8VU 	light_type_BK;		// 亮灯类型备份	 
	INT8VU 	flash_mode;			// 闪烁模式
	INT8VU 	flash_status;		// 闪烁状态
	INT16VU	flash_period;		// 闪烁周期基准，可调节
	INT16VU time_refresh;		// 定时刷新时间		系统时钟里自减
}LED_STR;

CPU_EXT 	 LED_STR DATA led;	   // 指示灯组结构体变量
//iic_key
typedef struct
{
	INT8VU iic_key_type;
}IIC_KEY;
CPU_EXT  IIC_KEY iic_key;
//--------------------------------------------------------------------------------------
// 参数存储
//#define MAX_CFGPARA       	6      	 //配置参数最大个数
#define MAX_CFGPARA       5	      	 //配置参数最大个数
typedef union 
{
	struct{
	 
		INT8VU 	  	address;				 //地址(1~255)
		INT8VU    	SHT30_Humidity_SET; 
		INT8VU    	SHT30_Hum_Cal_SET; 		//湿度校准设定值
		INT8VU    	SHT30_Tem_Cal_SET; 	 	//温度校准设定值
		INT8VU		Language_switch;		 // 语言切换：0-中文，1-英文
		INT8VU 	  	FinishedLED;			//完成后的LED亮灯情况(0,1,2,3,4,5,6,7-->N,R,G,B,Y,粉，青，白)
		INT8VU 	  	WtTime;				 	//进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  	FnDefine;				 //功能键定义(0,1->零拣,满箱操作)
		INT8VU 	  	DispMode;				 //显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
		INT8VU 	  	KeyProssLED;			 //OK键按下指示灯(0,1,2,3,4,5,6,7,8-->N,R,G,B,Y,粉，青，白,8前一种灯色+1)，（800U 触摸芯片）
		
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;


CPU_EXT	STORAGE_UNI  IDATA  para;		   // 参数联合
CPU_EXT	STORAGE_UNI  IDATA  para_bk;	   // 备份参数联合


/**************************************************
**中断模块 时间
**************************************************/
//CPU_EXT  INT32VU  Systime_Count;


/**************************************************
**910U_P6 移植过来的变量
**************************************************/
#define UART_BUF_LEN	50	//串口缓冲器大小
#define UART_REVBUF_LEN	256	//串口接收缓冲器大小
typedef struct
{
	INT8U wBuffer[UART_BUF_LEN];	//发送缓冲区
	INT8U wLen;						//发送数据个数
	INT8U rIndex;					//接收数据数量
	INT8U rBuffer[UART_REVBUF_LEN];	//接收缓冲区
	INT8U rTimer;					// 计时器
	INT8U rFlag;			 		// 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
	INT16U ErrCnt;
}OS_UART;

CPU_EXT  OS_UART  uart;

/**************************************************
**按键模块
**************************************************/
// 变量定义
CPU_EXT struct
{
	INT8VU	f_OkShort;			// 按键释放标志          0释放
	INT8VU	f_FuncShort;		// 功能键的释放标志      0释放
	INT8VU	f_Long;				// 按键长按标志          1长按 	
	INT8VU	f_LongOnce;			// 按键长按执行一次标志
	INT8VU	f_Enable;			// 按键扫描使能标志
	INT8VU	f_KeyOK_En;			// OK按键长扫描允许
	INT8VU	f_KeyFunc_En;		// 功能按键长扫描允许
 	INT8VU	f_IsOkPressed;		// OK键是否按下  给600U命令用
	INT8VU	f_IsOkPressed1;		// OK键是否按下  给900U命令用

	INT8VU  f_IsFnKeyPressed1;	// 功能键是否已经按下（再拍OK键之前）
	INT8VU  f_IsFnKeyPressed2;	// 功能键是否已经按下（再拍OK键之前）

	INT8VU  f_LongFunc;			// 功能键长按 
	INT8VU  CfgPwdCnt;			// 配置密码次数

	INT8VU	SysCnt;				// 对系统时钟计时
	INT8VU	OnCnt;				// 按下计数时间，单位为32ms
	INT8VU	OkBackup;			// 保存OK键上次的键值
	INT8VU	BackProssVal;		// 保存上次执行的键值
	INT8VU	FuncBackup;			// 保存功能键上次的键值

	INT8VU	ShiftLongCnt;		// 移动按键长按次数
	INT8VU	SubLongCnt;			// 减一按键长按次数
	INT8VU	FunLongCnt;			// 功能按键长按次数
//	INT8VU	ScanCnt;			// 扫描计数
} Key;


// 按键设置的模式
typedef struct
{
	INT8VU	f_Config;		    // 系统配置标志位,=OFF -> 不在配置状态，=ON ->在配置状态
	INT8VU	f_ChangeValue1;	    // 工作显示数值状态下，表示任务1是否正在修改焦点位，以区分（修改焦点位下，高位0需要显示，否则不需要显示）
	INT8VU	f_ChangeValue2;	    // 工作显示数值状态下，表示任务2是否正在修改焦点位，以区分（修改焦点位下，高位0需要显示，否则不需要显示）
	INT8VU	f_BackDisp;		    // 回显状态标志	   OFF=关

	INT8VU	f_F1BackDisp;	    // 最后一个任务完成，是否回显标志
}KEY_MODE_STR;

CPU_EXT	KEY_MODE_STR	 DATA KeyMode;		 // 按键设置的模式 的结构体
/**************************************************
**蜂鸣器模块
**************************************************/
CPU_EXT struct {               
	INT8VU	Type;		//鸣叫类型				                 
	INT8VU	Next;  		//鸣叫工作表偏移地址
	INT8VU	TimCnt;    	//计时器，系统时间为单位   
	INT8VU	BellON;		//蜂鸣开关状态     
	INT8VU	CtlSts;		//控制状态          		
} Sound;

//CPU_EXT	 INT8U Spi_flash_Rev[300];
/**************************************************
**主控通信结构体
**************************************************/


/**************************************************
**液晶显示模块
**************************************************/

//显示const定义
CPU_EXT  const  _ImageInfo      FullImageInfo[];
CPU_EXT  const  _ImageInfo      FullImageInfo1[];
CPU_EXT  const  _ImageConstStr  FullImageConst[];


CPU_EXT  const strPicCharStr  PicCharStr[];
CPU_EXT  const strPicChinaStr PicChinaStr[];


CPU_EXT  const INT8U  CharDot_1324[];
CPU_EXT  const INT8U  CharDot_2240[];
CPU_EXT  const INT8U  CharDot_1016[];
CPU_EXT  const INT8U  CharDot_1632[];

CPU_EXT  const INT8U  CharDot_4080B[];
CPU_EXT  const INT8U  CharDot_2040B[];
CPU_EXT  const INT8U  CharDot_3264B[];

//CPU_EXT  const INT8U  CharDot_4080[];
//CPU_EXT  const INT8U  CharDot_54112[];



//CPU_EXT  const INT8U  GB2312[6770*2];
//CPU_EXT  const INT8U  ASCII[130];
CPU_EXT  const  _ImageInfo  GB2312Str[];					// 对应字库的字高，字宽，首地址

CPU_EXT  const strCharStrColIndex Char1324_Indexes[];		// 不等宽字符索引
CPU_EXT  const strCharStrColIndex Char2240_Indexes[];		// 不等宽字符索引
CPU_EXT  const strCharStrColIndex Char1016_Indexes[];		// 不等宽字符索引
CPU_EXT  const strCharStrColIndex Char1632_Indexes[];		// 不等宽字符索引

CPU_EXT  const strCharStrColIndex Char4080B_Indexes[];		// 不等宽字符索引
CPU_EXT  const strCharStrColIndex Char2040B_Indexes[];		// 不等宽字符索引
CPU_EXT  const strCharStrColIndex Char3264B_Indexes[];		// 不等宽字符索引
//CPU_EXT  const strCharStrColIndex Char54112_Indexes[]; 		//不等宽字符索引

CPU_EXT  const INT8U gImage_1_Information_8bit[][4096];		// 信息屏图标字库 




#define		LCDLAYMAX	6
CPU_EXT  struct {
	
	INT8U	LaySts;				// 层状态，保存还是覆盖
 	INT8U	LayCnt;				// 层
 	INT8U	Lay[LCDLAYMAX];				// 暂定层
	
	INT16U	LayDispTime[LCDLAYMAX];		// 暂定层
	INT32U	LayTime[LCDLAYMAX];			// 暂定层
} LcdLay;

CPU_EXT  struct{
	
	INT8VU	TFT1_PicNum;		// TFT1显示的背景图
	INT8VU	f_LcdEnable;		// 显示使能标志
	INT8VU	f_Update;			// 背景数据更新标志
	INT8VU	f_LcdIni;	
	INT8VU	Type;				// 显示模式：
	INT8VU	TypeBke;			// 前一次显示模式：
//	INT16VU	TimCnt;				// 计数，单位1ms
	INT8VU	f_DispBack;			// 反显标志
//	INT8VU	Cfg_Addr;			// 配置界面要显示的地址位置，有3个

	INT16U	TFT1_TimCnt1ms;		// 计时1ms
	INT16U	TFT1_ID;			// ID
} LCD;


// 显示
typedef struct{
	
	INT8VU	f_Refresh;		// 数据刷新标志
	INT8VU	f_Loading;		// 正在载入标志
//	INT8VU	f_Rolling;		// 滚屏标志
	INT8VU	f_OffDisp;		// 灭显示标志

	INT8VU	Focus;			// 焦点位
	INT8VU	MenuPage;		// 菜单页
//	INT8VU	Shift;			// 移动

	INT8VU	Page;			// 显示页面
	INT8VU	PageBak;		// 显示页面
	
 	INT8VU  TatolTimeRefresh8;   // 总的定时刷新时间	系统时钟里自减(单位100ms)

	INT16VU Value;			// 计数值
	INT16VU TimeRefresh;		 // 定时刷新时间		系统时钟里自减
//	INT8VU  RollDelayCnt;		 // 滚屏定时时间		系统时钟里自减
//	INT16VU TatolTimeRefresh16;  // 总的定时刷新时间	系统时钟里自减

//	INT32VU TatolTimeRefresh32;  // 总的定时刷新时间	系统时钟里自减
	INT16VU TatolTimeRefresh16;  // 总的定时刷新时间	系统时钟里自减(单位100ms)
} DISPLAY_STR;

CPU_EXT 	 DISPLAY_STR DATA Disp;	   // 显示结构体变量

// 滚屏数据保存
//#define		NAME_MAX   		37
//#define		SRCINFO_MAX   	61
#define		ROLL_NAME_MAX   	7200
#define		ROLL_MNAME_MAX   	4800
#define		ROLL_SNAME_MAX   	3600
#define		ROLL_SRCINFO_MAX   	5000
#define		ROLL_BACKNAME_MAX   3000
#define		PAGE_MAX_BACK_NUM			5	 // 单页回显最大显示

#define		SHT30_SAMP_NUM	128
typedef struct{
	float SHT30_Temprature;
	float SHT30_Humidity;
	
	INT8VU SHT30_Humidity_SET;
	INT8U  SHT30_Humidity_SETTEMP;
	INT8U  Gled;
	INT8U  xxxx;
	
	// lcq 加入 采集数据
	INT16U SampCnt;	
	INT16U TemRawVal[SHT30_SAMP_NUM];
	INT16U HumRawVal[SHT30_SAMP_NUM];
	
}SHT30_PAR;
CPU_EXT	SHT30_PAR sht30_par;

typedef struct{
	INT8U current_background;
	INT8U current_statepic;
	INT8U TIMER_SHT_ENABLED;  // 定时器中断开关宏定义
	INT8U bar_interrupt_recover;
	INT8U Set_Mode;
}LCD_Background;
CPU_EXT	LCD_Background lcd_background;
typedef  struct{
	
	INT8VU	f_Rolling[2];					// 2个任务显示滚屏标志
	INT8VU	f_Rolling1;						// 信息滚屏标志
	INT8VU	f_Rolling2;						// 回显滚屏标志
	INT8VU	dammy;							// 亚元
	INT16VU	RollDelayCnt;					// 滚屏时间

	union
	{
		struct
		{	// 单任务屏的滚屏
			INT16VU	Name_Index;					// 品名区显示对应位置
			INT16VU	SrcInfo_Index[3];
			INT16VU	Name_Start;					// 品名区存放数据的起始位置
			INT16VU	SrcInfo_Start[3];
			INT16VU	Name_End;					// 品名区存放数据的结束位置
			INT16VU	SrcInfo_End[3];
			INT8VU	Name[ROLL_NAME_MAX];		//40*(40/8)*36	   品名区显示数据缓冲区
			INT8VU	SrcInfo[ROLL_SRCINFO_MAX];	//24*(24/8)*69
		}str1;
	
		struct
		{	// 回显任务的滚屏
			INT16VU	BackName_Index[ PAGE_MAX_BACK_NUM ];				// 回显5个任务
			INT16VU	BackName_Start[ PAGE_MAX_BACK_NUM ];				// 回显5个任务
			INT16VU	BackName_End[ PAGE_MAX_BACK_NUM ];				// 回显5个任务
			INT8VU	BackName[ PAGE_MAX_BACK_NUM ][ ROLL_BACKNAME_MAX ];	//24*(24/8)*36
		}str2;

		struct
		{	// 双任务屏的滚屏
			INT16VU	Name_Index;					// 品名区显示对应位置
			INT16VU	SrcInfo_Index[3];
			INT16VU	Name_Start;					// 品名区存放数据的起始位置
			INT16VU	SrcInfo_Start[3];
			INT16VU	Name_End;					// 品名区存放数据的结束位置
			INT16VU	SrcInfo_End[3];
			INT8VU	Name[ROLL_NAME_MAX/2];		//24*(24/8)*36	   品名区显示数据缓冲区
			INT8VU	SrcInfo[ROLL_SRCINFO_MAX/2];	//12*(24/8)*69
		}str3[2];

		struct
		{	// 信息屏的滚屏
			INT16VU	MName_Index;				// 标题区显示对应位置
			INT16VU	MName_Start;				// 标题区存放数据的起始位置
			INT16VU	MName_End;					// 标题区存放数据的结束位置

			INT16VU	SName_Index;				// 标题区显示对应位置
			INT16VU	SName_Start;				// 标题区存放数据的起始位置
			INT16VU	SName_End;					// 标题区存放数据的结束位置

			INT16VU	SrcInfo_Start[3];			// 描述区3行
			INT16VU	SrcInfo_Index[3];
			INT16VU	SrcInfo_End[3];

			INT8VU	MName[ROLL_MNAME_MAX];		//32*(32/8)*36	   品名区显示数据缓冲区
			INT8VU	SName[ROLL_SNAME_MAX];		//24*(24/8)*36	   品名区显示数据缓冲区
			INT8VU	SrcInfo[ROLL_SRCINFO_MAX];	//12*(24/8)*69
		}str4;
	}uni;


} DISP_TASK_SRC;
CPU_EXT 	DISP_TASK_SRC DispRoll;

typedef  struct{

	INT8U	dir;   		// 0=横屏，1=竖屏
	INT8U	only;		// 0=单屏，1=双屏
	INT16U	dval;		// 方向值  // INT8U
	INT16U	col0;		// 列
	INT16U	col1;		// 列
	INT16U  row0;		// 行
	INT16U  row1;		// 行  
	INT16U  dif;		// 偏差

}TFT_DIR_SET ;
CPU_EXT 	TFT_DIR_SET	 DispDir;


//CPU_EXT  INT8U	LcdDispMode;

extern INT8U  bk_colour ;
extern INT16U  bk_colour_true;

extern float Cur_SHT30_Humidity;	// lcq 加入



//相关子函数
CPU_EXT  void  LCD_Dsip_Mode(void);
//--------------------------------------------------------------------------------------
// 系统
typedef struct
{		
	INT32VU  Systime_Count;	 	 // 长计时
	INT16VU  Base_100ms;		 // 基准时间100ms
	INT32VU  Systime_s;
	INT16VU  Systime_Min;
	INT16VU  Systime_Min_flag;
} OS_TIME;

CPU_EXT 	OS_TIME  DATA os_time;

// ************************全局内存变量定义结束************************* //




// ***************************全局函数声明区**************************** //

/*-----------------------------------------------
				EEPROM
-----------------------------------------------*/
CPU_EXT  void EEPROM_Init(void);
CPU_EXT  void LoadParaInEeprom(void);

CPU_EXT  void SaveParaToBlock(void);
/*-----------------------------------------------
				BEEP_Module
-----------------------------------------------*/
//CPU_EXT  void  Beep_Operate(ALM_WORKTYPE wtype);
CPU_EXT  void  Beep_Init(void);
CPU_EXT  void  TurnOff_Beep(void);
CPU_EXT  void  TurnOn_Beep(void);

CPU_EXT  void	Almdo(void);					// 蜂鸣器执行程序
CPU_EXT  void	AlarmSet(INT8U Type);			// 设置蜂鸣器执行模式，并初始化

CPU_EXT  void 	alarm_scan(void);		// 红色指示灯根据湿度差值是否点亮
/*-----------------------------------------------
				FSMC_NORFlash
-----------------------------------------------*/
//相关子函数





/*-----------------------------------------------
				TFT_Module
-----------------------------------------------*/

CPU_EXT void   TFTLCD_Init(void); //TFT初始化
CPU_EXT void   DispColor1(INT16U color);
//CPU_EXT void   TFT1_Display_NORFlash_Picture(_ImageConstStr imagep,_ImageInfo imageinfo);
CPU_EXT  void Load_Picture(INT8U	PicNum1);


CPU_EXT void   TFT1_PutString(INT16U row,INT16U col,const INT8U *ptr,INT8U charnum,INT8U chartype,INT16U charColor,INT16U bkColor);

CPU_EXT void   TFT_GB2312(INT16U FontType,INT16U lcdsel);
CPU_EXT void   LCD_Scan(void);
CPU_EXT void   SetLcdLay(INT8U LcdPage);

CPU_EXT void display_point(unsigned int x,unsigned int y,unsigned int dat);

CPU_EXT void qr_code_display(unsigned int bgcolor,unsigned int color);



/*-----------------------------------------------
				KEY_Module
-----------------------------------------------*/
CPU_EXT  void  Key_Process(void); 
CPU_EXT  void  Key_Scan(void);  
CPU_EXT  void  Key_Init(void);
CPU_EXT  void  Exit_Cfg_Mode(void);	
CPU_EXT  void  PutFinishedToStack_Com( TASK_INFO XDATA *pt1,TASK_INFO XDATA *pt2 );	  // 备份数据
/*-----------------------------------------------
				LED_Module
-----------------------------------------------*/
CPU_EXT  void  LED_Init(void);
//CPU_EXT  void  led_light_ctrl(INT8U ucValue, INT8U ucStatus);
CPU_EXT  void  led_light_ctrl(INT8U ucValue);
CPU_EXT  void  LED_ALL_OFF(void);
CPU_EXT  void  LED_ALL_ON(void);
CPU_EXT  void  CMD_led_flash(void);
/*-----------------------------------------------
				RTC_Module
-----------------------------------------------*/
CPU_EXT  INT8U RTC_Configuration(void);
CPU_EXT  void  RTC_init(INT8U type,INT32U tolSec);
CPU_EXT  void  Get_ClockParameter(INT32U timedat);
CPU_EXT  void  Display_ClockOrDate(INT16U row,INT16U col,INT8U chartype,INT16U charColor,INT16U bkColor,ClockOrDate clkordate);

/*-----------------------------------------------
				UART_Module
-----------------------------------------------*/
CPU_EXT  void  UART_Init(void);
CPU_EXT  void  USART_SendByte(_UART uart,INT8U dat);
CPU_EXT  void  USART_SendBuff(_UART uart,INT8U *buff,INT16U num);
CPU_EXT  void SendSerialByte(UINT8 *Ram, UINT16 len);
/*-----------------------------------------------
				Communicate
-----------------------------------------------*/
//CPU_EXT  void  Communicate_Respond(void);
CPU_EXT  void  Cmd_Process(void);
CPU_EXT  void  sys_data_reset(void);
/*-----------------------------------------------
				Machine_Debug
-----------------------------------------------*/
CPU_EXT  void  Machine_Debug(void);
CPU_EXT  void  Server_SendState(void);

CPU_EXT  void  Timer_Configuration(void);
CPU_EXT  void  NVIC_Configuration(void);
CPU_EXT	 void  RCC_Configuration(void);
CPU_EXT	 void  SPI_FLASH_Init(void);

/*-----------------------------------------------
				SPI_Font_Flash
-----------------------------------------------*/
CPU_EXT	void SPI_FLASH_BufferFastRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
/*-----------------------------------------------
				Main
-----------------------------------------------*/
CPU_EXT  void  Board_Init(void);   		   //MCU初始化
CPU_EXT  void  System_Init(void);		   //系统初始化
CPU_EXT  void  Delay_ms(INT32U ms);		   //延时ms，实际1=32us
CPU_EXT  void  Delay_us(INT16U us);		   //延时us，实际1=1us
CPU_EXT  void  Display_Clear(void);

CPU_EXT   void Page_Run1(void);
CPU_EXT   void Set_Page_Run(void);



#endif



