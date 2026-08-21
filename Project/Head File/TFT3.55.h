//#include  "../Head file/Typedef.h"
//#include  "../Head file/Glob.h"
//#include  "../Head file/FSMC_NORFlash.h"






// 颜色值转成16位的RGB565的值

#define mGRAY1           	RGB565CONVERT(36,  36,      36)			// 纯红	  0xF800
#define mGRAY2         		RGB565CONVERT(73,  73,      73)			// 纯绿	  0x07E0
#define mGRAY3          	RGB565CONVERT(109, 109,     109)		// 纯蓝	  0x001F
#define mGRAY4        		RGB565CONVERT(146, 146,     146)		// 纯黄	  0xFFE0
#define mGRAY5       		RGB565CONVERT(182, 182,     182)		// 纯粉	  0xF81F
#define mGRAY6          	RGB565CONVERT(219, 219,     219)		// 纯青	  0x07FF



#define aBLACK              RGB565CONVERT(0,    0,      0)			// 纯白	  0x0000
#define aRED           		RGB565CONVERT(255,  0,      0)			// 纯红	  0xF800
#define aGREEN         		RGB565CONVERT(0,    255,    0)			// 纯绿	  0x07E0
#define aBLUE          		RGB565CONVERT(0,    0,      255)		// 纯蓝	  0x001F
#define aYELLOW        		RGB565CONVERT(255,  255,    0)			// 纯黄	  0xFFE0
#define aMAGENTA       		RGB565CONVERT(255,  0,      255)		// 纯粉	  0xF81F
#define aCYAN          		RGB565CONVERT(0,    255,    255)		// 纯青	  0x07FF
#define aWHITE              RGB565CONVERT(255,  255,    255)		// 纯白	  0xFFFF
#define aGRAY               RGB565CONVERT(217,  217,    217)		// 浅灰

#define bRED                RGB565CONVERT(128,  0,      0)			// 深红
#define bGREEN              RGB565CONVERT(0,    128,    0)			// 深绿
#define bBLUE               RGB565CONVERT(0,    0,      128)		// 深蓝
#define bYELLOW        		RGB565CONVERT(128,  128,    0)			// 深黄
#define bMAGENTA       		RGB565CONVERT(128,  0,      128)		// 深粉
#define bCYAN               RGB565CONVERT(0,    128,    128)		// 深青
#define bGRAY               RGB565CONVERT(128,  128,    128)		// 深灰

#define cRED                RGB565CONVERT(192,  0,      0)			// 红
#define cGREEN              RGB565CONVERT(0,    192,    0)			// 绿
#define cBLUE               RGB565CONVERT(0,    0,      192)		// 蓝
#define cYELLOW        		RGB565CONVERT(192,  192,    0)			// 黄
#define cMAGENTA       		RGB565CONVERT(192,  0,      192)		// 粉
#define cCYAN               RGB565CONVERT(0,    192,    192)		// 青
#define cGRAY               RGB565CONVERT(192,  192,    192)		// 灰

#define dRED                RGB565CONVERT(64,   0,      0)			// 暗红
#define dGREEN              RGB565CONVERT(0,    64,     0)			// 暗绿
#define dBLUE               RGB565CONVERT(0,    0,      64)			// 暗蓝
#define dYELLOW        		RGB565CONVERT(64,   64,     0)			// 暗黄
#define dMAGENTA       		RGB565CONVERT(64,   0,      64)			// 暗粉
#define dCYAN               RGB565CONVERT(0,    64,     64)			// 暗青
#define dGRAY               RGB565CONVERT(64,   64,     64)			// 暗灰

#define eRED                RGB565CONVERT(96,   0,      0)			// 暗红
#define eGREEN              RGB565CONVERT(0,    96,     0)			// 暗绿
#define eBLUE               RGB565CONVERT(0,    0,      96)			// 暗蓝
#define eYELLOW        		RGB565CONVERT(96,   96,     0)			// 暗黄
#define eMAGENTA       		RGB565CONVERT(96,   0,      96)			// 暗粉
#define eCYAN               RGB565CONVERT(0,    96,     96)			// 暗青
#define eGRAY               RGB565CONVERT(96,   96,     96)			// 暗灰

#define gRED                RGB565CONVERT(32,   0,      0)			// 暗红
#define gGREEN              RGB565CONVERT(0,    32,     0)			// 暗绿
#define gBLUE               RGB565CONVERT(0,    0,      32)			// 暗蓝
#define gYELLOW        		RGB565CONVERT(32,   32,     0)			// 暗黄
#define gMAGENTA       		RGB565CONVERT(32,   0,      64)			// 暗粉
#define gCYAN               RGB565CONVERT(0,    32,     32)			// 暗青
#define gGRAY               RGB565CONVERT(32,   32,     32)			// 暗灰

//#define bCYAN1              RGB565CONVERT(217,  217,    217)

//#define fRED                RGB565CONVERT(153,   0,      0)			// 暗红
//#define fGREEN              RGB565CONVERT(0,    153,     0)			// 暗绿
//#define fBLUE               RGB565CONVERT(0,    0,      153)		// 暗蓝
//#define fYELLOW        		RGB565CONVERT(153,  153,     0)			// 暗黄
//#define fMAGENTA       		RGB565CONVERT(153,   0,     153)		// 暗粉
//#define fCYAN               RGB565CONVERT(0,    153,    153)		// 暗青
//#define fGRAY               RGB565CONVERT(153,  153,    153)		// 暗灰

#define fRED                RGB565CONVERT(160,   0,      0)			// 暗红
#define fGREEN              RGB565CONVERT(0,    160,     0)			// 暗绿
#define fBLUE               RGB565CONVERT(0,    0,      160)		// 暗蓝
#define fYELLOW        		RGB565CONVERT(160,  160,     0)			// 暗黄
#define fMAGENTA       		RGB565CONVERT(160,   0,     160)		// 暗粉
#define fCYAN               RGB565CONVERT(0,    160,    160)		// 暗青
#define fGRAY               RGB565CONVERT(160,  160,    160)		// 暗灰

//#define MAGENTA             RGB565CONVERT(128,  0,      128)
#define BROWN               RGB565CONVERT(165,  42,    42)
#define ORANGE              RGB565CONVERT(255,  165,    0)
#define bORANGE             RGB565CONVERT(255,  140,    0)
#define cORANGE             RGB565CONVERT(255,  128,    0)
#define dORANGE             RGB565CONVERT(192,  96,    0)
#define PURPLE              RGB565CONVERT(155,  48,    255)
#define PINK                RGB565CONVERT(255,  192,   203)

//#define LIGHTGRAY           RGB565CONVERT(128,  128,    128)
//#define DARKGRAY            RGB565CONVERT(64,   64,     64)
//#define LIGHTBLUE           RGB565CONVERT(128,  128,    255)
//#define LIGHTBLUE2          RGB565CONVERT(  0,  200,    255)
//#define LIGHTGREEN          RGB565CONVERT(128,  255,    128)
//#define LIGHTCYAN           RGB565CONVERT(128,  255,    255)
//#define LIGHTRED            RGB565CONVERT(255,  128,    128)
//#define LIGHTMAGENTA        RGB565CONVERT(255,  128,    255)
//#define YELLOW              RGB565CONVERT(255,  255,    128)
//#define WHITE2              RGB565CONVERT(150,  150,    150)

// 饶勇定义
#define rRED                RGB565CONVERT(192,   0,      0)			// 暗红
#define rGREEN              RGB565CONVERT(0,   176,     80)			// 暗绿
#define rBLUE               RGB565CONVERT(65,   77,    177)			// 暗蓝
#define rYELLOW        		RGB565CONVERT(237, 207,     24)			// 暗黄
#define rMAGENTA       		RGB565CONVERT(185,   0,    185)			// 暗粉
#define rCYAN               RGB565CONVERT(0,    185,   185)			// 暗青
#define rGRAY               RGB565CONVERT(217, 217,    217)			// 暗灰

// 背景色
#define BK_BLACK   aBLACK
#define BK_RED     cRED//aRED		 //定义颜色常量 
#define BK_GREEN   cGREEN//aGREEN
#define BK_BLUE    cBLUE//aBLUE
#define BK_YELLOW  cYELLOW//aYELLOW
#define BK_MAGENTA aMAGENTA
#define BK_CYAN    aCYAN
#define BK_WHITE   aWHITE
#define BK_BROWN   BROWN		 //定义颜色常量 
#define BK_GRAY    bGRAY
#define BK_ORANGE  ORANGE
//#define BK_YELLOW  aYELLOW
//#define BK_MAGENTA aMAGENTA
//#define BK_CYAN    aCYAN
//#define BK_WHITE   aWHITE

// 文本色
#define TXT_BLACK   aBLACK
#define TXT_RED     cRED//aRED		 //定义颜色常量 
#define TXT_GREEN   cGREEN//aGREEN
#define TXT_BLUE    cBLUE//aBLUE
#define TXT_YELLOW  cYELLOW//aYELLOW
#define TXT_MAGENTA aMAGENTA
#define TXT_CYAN    aCYAN
#define TXT_WHITE   aWHITE
#define TXT_BROWN   BROWN		 //定义颜色常量 
#define TXT_GRAY    bGRAY
#define TXT_ORANGE  ORANGE
//#define BK_YELLOW  aYELLOW
//#define BK_MAGENTA aMAGENTA
//#define BK_CYAN    aCYAN
//#define BK_WHITE   aWHITE

// 描述文本色
#define DSC_BLACK   aBLACK
#define DSC_RED     cRED//aRED		 //定义颜色常量 
#define DSC_GREEN   cGREEN//aGREEN
#define DSC_BLUE    cBLUE//aBLUE
#define DSC_YELLOW  cYELLOW//aYELLOW
#define DSC_MAGENTA aMAGENTA
#define DSC_CYAN    aCYAN
#define DSC_WHITE   aWHITE
#define DSC_BROWN   BROWN		 //定义颜色常量 
#define DSC_GRAY    aGRAY
#define DSC_ORANGE  ORANGE
//#define BK_YELLOW  aYELLOW
//#define BK_MAGENTA aMAGENTA
//#define BK_CYAN    aCYAN
//#define BK_WHITE   aWHITE


// Nor Flash 和 LCD 共用FSMC总线，地址不同自动选不同的区域
//TFT3.5 相关子函数
#define    Bank1_NOR2_ADDR   (SPIM_TEST_ADDR-0x100000)//((u32)0x64000000)		//Nor Flash 的地址 0x64xxxx = NE2

#define    DMAMAXNUM         0xFFFF	  //DMA一次传输最大次数
//#define    TFT_COM_ADDR      ((u32)0x6C000000)  //TFT 指令REG地址
//#define    TFT_DAT_ADDR      ((u32)0x6C020000)   //TFT 数据RAM地址

//TFT3.0 尺寸大小
#define    TFT_XSIZE          240		  
#define    TFT_YSIZE          320
#define    TFTPIXELNUM       (TFT_XSIZE*TFT_YSIZE)        //TFT像素点总数 96000=240*400

//TFT3.0 相关子函数
#define    TFT1_COM_ADDR      ((u32)0x60000000)//((u32)0x68000000)  //TFT 指令REG地址 0x68xxxx = NE3, 0x60xxx = NE1
#define    TFT1_DAT_ADDR      ((u32)0x60020000)   //TFT 数据RAM地址

/* The address of write data & command (XMC_A16) */
#define  XMC_LCD_COMMAND    0x6001FFFE
#define  XMC_LCD_DATA       0x60020000
//======================================================================================================

#define    TFT1		   1
//#define    TFT2		   2

//点阵类型
//英文
#define    CHAR1324      0		 // 必须和中文的CHINA2424 对应上
#define    CHAR2240      1		 // 必须和中文的CHINA4040 对应上
#define    CHAR1016		 2		 // 必须和中文的CHINA1616 对应上
#define    CHAR1632		 3		 // 必须和中文的CHINA3232 对应上
#define    CHAR1324L     4		 // 必须和中文的CHINA2424 对应上

#define    CHAR4080B     5		 // 库位和数量，单屏显示时 ，等宽
#define    CHAR2040B     6		 // 库位和数量，双屏显示时 ，等宽
#define    CHAR3264B     7		 // 库位和数量，双屏显示时 ，等宽， 没用

//#define    CHAR54112     3

//中文
#define    CHINA2424     0		  // 和英文的CHAR1324 对应上
#define    CHINA4040     1		  // 和英文的CHAR2240 对应上
#define    CHINA1616     2		  // 和英文的CHAR1016 对应上
#define    CHINA3232     3		  // 和英文的CHAR1632 对应上
#define    CHINA2424L    4		  // 和英文的CHAR1324L 对应上




//显示界面模式
#define		PAGE_OFF			0			// 灭页面

#define		PAGE_RUN			1			// 运行页面
#define		PAGE_WAIT			2			// 等待（进度条）页面
#define		PAGE_BACKDISPLAY	3			// 回显页面
#define		PAGE_CFGADDR		4			// 配置地址页面
#define		PAGE_CFGFUNC1	  	5			// 湿度设置界面
#define		PAGE_CFGFUNC2		6			// 湿度校准值设置界面
#define		PAGE_CFGFUNC3  		7			// 温度校准值设置界面
#define		PAGE_CFGFUNC4  		8			// 中英文切换界面
#define		PAGE_START  		9			// 开机版本
#define		PAGE_NULL			10			// NU****
#define		PAGE_CFGFUNC5  		11			// 配置功能5页面
#define		PAGE_INFO			12			// 显示信息 600U

#define		PAGE_CFGPWD			13          // 配置密码页面


//刷新模式
#define		NONE_REFRESH		0			//无刷新页面
#define		WAIT_REFRESH		1			//等待刷新页面
#define		RUN_REFRESH			2			//正在刷新页面
#define		RUNIRQ_REFRESH		3			//正在刷新时页面被更改

//====================================================
// 滚屏显示模式
#define ROLL_MODE_OFF			0		   	// 无滚屏模式
// 任务1滚屏
#define ROLL_MODE_NAME	    	0x01	   	// 滚屏品名信息标志
#define ROLL_MODE_SRC1	    	0x02		// 滚屏描述行1刷新标志
#define ROLL_MODE_SRC2	    	0x04		// 滚屏描述行2刷新标志
#define ROLL_MODE_SRC3	    	0x08		// 滚屏描述行3刷新标志
#define ROLL_MODE_SNAME	    	0x10	   	// 滚屏品名信息标志
// 任务2滚屏
//#define ROLL2_MODE_NAME	    	0x10	   	// 滚屏品名信息标志
//#define ROLL2_MODE_SRC1	    	0x20		// 滚屏描述行1刷新标志
//#define ROLL2_MODE_SRC2	    	0x40		// 滚屏描述行2刷新标志
//#define ROLL2_MODE_SRC3	    	0x80		// 滚屏描述行3刷新标志

#define ROLL_MODE_NAME_BAKE1	0x01		// 滚屏回显区品名1刷新标志
#define ROLL_MODE_NAME_BAKE2	0x02		// 滚屏回显区品名2刷新标志
#define ROLL_MODE_NAME_BAKE3	0x04		// 滚屏回显区品名3刷新标志
#define ROLL_MODE_NAME_BAKE4	0x08		// 滚屏回显区品名4刷新标志
#define ROLL_MODE_NAME_BAKE5	0x10		// 滚屏回显区品名5刷新标志


#define DISP_ROLL_SPEED			20			// 滚屏时间

// 进度条显示模式
#define LOAD_MODE_OFF	0		   // 进度模式关
#define LOAD_MODE_SHORT	1		   // 进度模式短时间
#define LOAD_MODE_LONG	2		   // 进度模式长时间
// 字符串结束符号
#define		NULL		0

// 背景颜色窗口有
#define ROW  320			//显示的列数  
#define COL  240			//显示的行数

//#define ROW  240			//显示的列数
//#define COL  400			//显示的行数

//----------------------------------------------------------------
//-------------------------------------------------------------------------
// 横屏运行显示位置
// 单任务屏
#define		H_RUN_SKU_ROW		8//8			// 库位和数量的起始行
#define		H_RUN_SKU_UL		(H_RUN_SKU_ROW+76)//80+2)			 // 上线
//#define		H_RUN_NAME_DL		(H_RUN_NAME_ROW-13)			 // 下线
#define		H_RUN_COUNT_COL		155//155		 	// 数量的起始列

#define		H_RUN_NAME_ROW		200			// 品名起始行
#define		H_RUN_SRC1_ROW		(H_RUN_NAME_ROW-24-10)	 // 描述区起始行1
#define		H_RUN_SRC2_ROW		(H_RUN_SRC1_ROW-24-5)	 // 描述区起始行2
#define		H_RUN_SRC3_ROW		(H_RUN_SRC2_ROW-24-5)	 // 描述区起始行3

// 双任务屏
#define		H_RUN_SKU_ROW2		20			// 库位和数量的起始行
#define		H_RUN_SKU_UL2	   (H_RUN_SKU_ROW2+40+20)			// 上线
#define		H_RUN_COUNT_COL2	95		//70	// 数量的起始列
#define		H_RUN_COUNT_COL3	175			// 单位的起始列		4行列，5数字则无单位

#define		H_RUN_NAME_ROW2		185			// 品名起始行
#define		H_RUN_SRC1_ROW2		(H_RUN_NAME_ROW2-24-10)	 // 描述区起始行1
#define		H_RUN_SRC2_ROW2		(H_RUN_SRC1_ROW2-24-5)	 // 描述区起始行2
#define		H_RUN_SRC3_ROW2		(H_RUN_SRC2_ROW2-24-5)	 // 描述区起始行3


// 竖屏运行显示位置
// 单屏显示对应位置
#if TFT_TYPE != TFT_HJ28_9327
#define		S_RUN_COUNT_ROW		312//42//34
#define		S_RUN_UNI_ROW		(S_RUN_COUNT_ROW+35)
#define		S_RUN_SKU_ROW		(S_RUN_COUNT_ROW-72)////(S_RUN_COUNT_ROW+72)//80
#define		S_RUN_SKU_UL		(S_RUN_SKU_ROW-15)//(S_RUN_SKU_ROW+85)//85)			 // 上线

#define		S_RUN_NAME1_ROW		25//370
#define		S_RUN_NAME2_ROW		(S_RUN_NAME1_ROW+40+5)//(S_RUN_NAME1_ROW-40-5)
#define		S_RUN_SRC1_ROW		(S_RUN_NAME2_ROW+24+10)//(S_RUN_NAME2_ROW-24-10)
#define		S_RUN_SRC2_ROW		(S_RUN_SRC1_ROW+24+5)//(S_RUN_SRC1_ROW-24-5)
#define		S_RUN_SRC3_ROW		(S_RUN_SRC2_ROW+24+5)//(S_RUN_SRC2_ROW-24-5)
#define		S_RUN_SRC4_ROW		(S_RUN_SRC3_ROW+24+5)//(S_RUN_SRC3_ROW-24-5)
#define		S_RUN_SRC5_ROW		(S_RUN_SRC4_ROW+24+5)//(S_RUN_SRC4_ROW-24-5)
#define		S_RUN_SRC6_ROW		(S_RUN_SRC5_ROW+24+5)//(S_RUN_SRC5_ROW-24-5)

// 任务1
#define		SS1_RUN_NAME1_ROW		18//390								// 品名区对应行
#define		SS1_RUN_SRC1_ROW		(SS1_RUN_NAME1_ROW+32+10)	//(SS1_RUN_NAME1_ROW-24-10)		// 描述区对应行1
#define		SS1_RUN_SRC2_ROW		(SS1_RUN_SRC1_ROW+24+5)//(SS1_RUN_SRC1_ROW-24-5)			// 描述区对应行2
#define		SS1_RUN_SRC3_ROW		(SS1_RUN_SRC2_ROW+24+5)//(SS1_RUN_SRC2_ROW-24-5)			// 描述区对应行3
#define		SS1_RUN_SKU_UL			(SS1_RUN_SRC3_ROW+24+0)//(SS1_RUN_SRC3_ROW-8)			 // 上线
#define		SS1_RUN_SKU_ROW			(SS1_RUN_SKU_UL+11)//(SS1_RUN_SKU_UL-50)
#define		SS1_RUN_UNI_ROW			(SS1_RUN_SKU_ROW+16)
//#define		SS1_RUN_COUNT_ROW		(SS1_RUN_SKU_ROW-40)
#define		SS1_RUN_COUNT_COL2	100		//90						   // 数量对应列
#define		SS1_RUN_COUNT_COL3	205		//195						   // 单位对应列
// 任务2
#define		SS2_RUN_NAME1_ROW		206//195								// 品名区对应行
#define		SS2_RUN_SRC1_ROW		(SS2_RUN_NAME1_ROW+32+10)//(SS2_RUN_NAME1_ROW-24-10)		// 描述区对应行1
#define		SS2_RUN_SRC2_ROW		(SS2_RUN_SRC1_ROW+24+5)//(SS2_RUN_SRC1_ROW-24-5)			// 描述区对应行2
#define		SS2_RUN_SRC3_ROW		(SS2_RUN_SRC2_ROW+24+5)//(SS2_RUN_SRC2_ROW-24-5)			// 描述区对应行3
#define		SS2_RUN_SKU_UL			(SS2_RUN_SRC3_ROW+24+0)//(SS2_RUN_SRC3_ROW-8)			 // 上线
#define		SS2_RUN_SKU_ROW			(SS2_RUN_SKU_UL+11)//(SS2_RUN_SKU_UL-50)
#define		SS2_RUN_UNI_ROW			(SS2_RUN_SKU_ROW+16)
//#define		SS2_RUN_COUNT_ROW		(SS2_RUN_SKU_ROW-40)
#define		SS2_RUN_COUNT_COL2	100		//90							// 数量对应列
#define		SS2_RUN_COUNT_COL3	205		//195							// 单位对应列

#else
// 竖屏运行显示位置
// 单屏显示对应位置
#define		S_RUN_COUNT_ROW		32//34
#define		S_RUN_UNI_ROW		(S_RUN_COUNT_ROW+14)
#define		S_RUN_SKU_ROW		(S_RUN_COUNT_ROW+72)//80
#define		S_RUN_SKU_UL		(S_RUN_SKU_ROW+85)//85)			 // 上线

#define		S_RUN_NAME1_ROW		370
#define		S_RUN_NAME2_ROW		(S_RUN_NAME1_ROW-40-5)
#define		S_RUN_SRC1_ROW		(S_RUN_NAME2_ROW-24-10)
#define		S_RUN_SRC2_ROW		(S_RUN_SRC1_ROW-24-5)
#define		S_RUN_SRC3_ROW		(S_RUN_SRC2_ROW-24-5)
#define		S_RUN_SRC4_ROW		(S_RUN_SRC3_ROW-24-5)
#define		S_RUN_SRC5_ROW		(S_RUN_SRC4_ROW-24-5)
#define		S_RUN_SRC6_ROW		(S_RUN_SRC5_ROW-24-5)

// 任务1
#define		SS1_RUN_NAME1_ROW		390								// 品名区对应行
#define		SS1_RUN_SRC1_ROW		(SS1_RUN_NAME1_ROW-24-10)		// 描述区对应行1
#define		SS1_RUN_SRC2_ROW		(SS1_RUN_SRC1_ROW-24-5)			// 描述区对应行2
#define		SS1_RUN_SRC3_ROW		(SS1_RUN_SRC2_ROW-24-5)			// 描述区对应行3
#define		SS1_RUN_SKU_UL			(SS1_RUN_SRC3_ROW-8)			 // 上线
#define		SS1_RUN_SKU_ROW			(SS1_RUN_SKU_UL-50)
#define		SS1_RUN_UNI_ROW			(SS1_RUN_SKU_ROW+8)
//#define		SS1_RUN_COUNT_ROW		(SS1_RUN_SKU_ROW-40)
#define		SS1_RUN_COUNT_COL2	100		//90						   // 数量对应列
#define		SS1_RUN_COUNT_COL3	205		//195						   // 单位对应列
// 任务2
#define		SS2_RUN_NAME1_ROW		195								// 品名区对应行
#define		SS2_RUN_SRC1_ROW		(SS2_RUN_NAME1_ROW-24-10)		// 描述区对应行1
#define		SS2_RUN_SRC2_ROW		(SS2_RUN_SRC1_ROW-24-5)			// 描述区对应行2
#define		SS2_RUN_SRC3_ROW		(SS2_RUN_SRC2_ROW-24-5)			// 描述区对应行3
#define		SS2_RUN_SKU_UL			(SS2_RUN_SRC3_ROW-8)			 // 上线
#define		SS2_RUN_SKU_ROW			(SS2_RUN_SKU_UL-50)
#define		SS2_RUN_UNI_ROW			(SS2_RUN_SKU_ROW+8)
//#define		SS2_RUN_COUNT_ROW		(SS2_RUN_SKU_ROW-40)
#define		SS2_RUN_COUNT_COL2	100		//90							// 数量对应列
#define		SS2_RUN_COUNT_COL3	205		//195							// 单位对应列
#endif

//-------------------------------------------------------------------------
// 回显区对应的横竖屏行列定义
// 横屏
#define		H_BACK_TASK5_ROW		17						// 任务品名1行
#define		H_BACK_TASK4_ROW		(H_BACK_TASK5_ROW+45)	// 任务品名2行
#define		H_BACK_TASK3_ROW		(H_BACK_TASK4_ROW+45)	// 任务品名3行
#define		H_BACK_TASK2_ROW		(H_BACK_TASK3_ROW+45)	// 任务品名4行
#define		H_BACK_TASK1_ROW		(H_BACK_TASK2_ROW+45)	// 任务品名4行

#define		H_BACK_SKU5_ROW			17						// 任务库位1行
#define		H_BACK_SKU4_ROW			(H_BACK_SKU5_ROW+45)	// 任务库位2行
#define		H_BACK_SKU3_ROW			(H_BACK_SKU4_ROW+45)	// 任务库位3行
#define		H_BACK_SKU2_ROW			(H_BACK_SKU3_ROW+45)	// 任务库位4行
#define		H_BACK_SKU1_ROW			(H_BACK_SKU2_ROW+45)	// 任务库位4行

#define		H_BACK_LINE1_ROW		(H_BACK_TASK1_ROW-10)			// 线1
#define		H_BACK_LINE2_ROW		(H_BACK_TASK2_ROW-10)			// 线2
#define		H_BACK_LINE3_ROW		(H_BACK_TASK3_ROW-10)			// 线3
#define		H_BACK_LINE4_ROW		(H_BACK_TASK4_ROW-10)			// 线4
#define		H_BACK_LINE5_ROW		(H_BACK_TASK5_ROW-10)			// 线4

#define		H_BACK_SKU_COL			225						// 回显库位位置

// 竖屏
#if TFT_TYPE != TFT_HJ28_9327
#define		S_BACK_TASK1_ROW		28//370						// 任务1行
#define		S_BACK_TASK2_ROW		(S_BACK_TASK1_ROW+70)//(S_BACK_TASK1_ROW-70)	// 任务2行
#define		S_BACK_TASK3_ROW		(S_BACK_TASK2_ROW+70)//(S_BACK_TASK2_ROW-70)			// 任务1行
#define		S_BACK_TASK4_ROW		(S_BACK_TASK3_ROW+70)//(S_BACK_TASK3_ROW-70)			// 任务2行
#define		S_BACK_TASK5_ROW		(S_BACK_TASK4_ROW+70)//(S_BACK_TASK4_ROW-70)			// 任务2行

#define		S_BACK_SKU1_ROW			63//335						// 任务库位1行
#define		S_BACK_SKU2_ROW			(S_BACK_SKU1_ROW+70)//(S_BACK_SKU1_ROW-70)	// 任务库位2行
#define		S_BACK_SKU3_ROW			(S_BACK_SKU2_ROW+70)//(S_BACK_SKU2_ROW-70)	// 任务库位3行
#define		S_BACK_SKU4_ROW			(S_BACK_SKU3_ROW+70)//(S_BACK_SKU3_ROW-70)	// 任务库位4行
#define		S_BACK_SKU5_ROW			(S_BACK_SKU4_ROW+70)//(S_BACK_SKU4_ROW-70)	// 任务库位4行

#define		S_BACK_LINE1_ROW		(S_BACK_SKU1_ROW+28)//(S_BACK_SKU1_ROW-5)			// 线1
#define		S_BACK_LINE2_ROW		(S_BACK_SKU2_ROW+28)//(S_BACK_SKU2_ROW-5)			// 线2
#define		S_BACK_LINE3_ROW		(S_BACK_SKU3_ROW+28)//(S_BACK_SKU3_ROW-5)			// 线3
#define		S_BACK_LINE4_ROW		(S_BACK_SKU4_ROW+28)//(S_BACK_SKU4_ROW-5)			// 线4
#define		S_BACK_LINE5_ROW		(S_BACK_SKU5_ROW+28)//(S_BACK_SKU5_ROW-5)			// 线4

#else
#define		S_BACK_TASK1_ROW		370						// 任务1行
#define		S_BACK_TASK2_ROW		(S_BACK_TASK1_ROW-70)	// 任务2行
#define		S_BACK_TASK3_ROW		(S_BACK_TASK2_ROW-70)			// 任务1行
#define		S_BACK_TASK4_ROW		(S_BACK_TASK3_ROW-70)			// 任务2行
#define		S_BACK_TASK5_ROW		(S_BACK_TASK4_ROW-70)			// 任务2行

#define		S_BACK_SKU1_ROW			335						// 任务库位1行
#define		S_BACK_SKU2_ROW			(S_BACK_SKU1_ROW-70)	// 任务库位2行
#define		S_BACK_SKU3_ROW			(S_BACK_SKU2_ROW-70)	// 任务库位3行
#define		S_BACK_SKU4_ROW			(S_BACK_SKU3_ROW-70)	// 任务库位4行
#define		S_BACK_SKU5_ROW			(S_BACK_SKU4_ROW-70)	// 任务库位4行

#define		S_BACK_LINE1_ROW		(S_BACK_SKU1_ROW-5)			// 线1
#define		S_BACK_LINE2_ROW		(S_BACK_SKU2_ROW-5)			// 线2
#define		S_BACK_LINE3_ROW		(S_BACK_SKU3_ROW-5)			// 线3
#define		S_BACK_LINE4_ROW		(S_BACK_SKU4_ROW-5)			// 线4
#define		S_BACK_LINE5_ROW		(S_BACK_SKU5_ROW-5)			// 线4
#endif
//-------------------------------------------------------------------
// 参数配置
//横屏
#define		H_FN_SEL		195						// FN选择项	行
#define		H_FN_SEL_UL		(H_FN_SEL+24+8)			// FN选择项上横线
#define		H_FN_SEL_DL	    (H_FN_SEL-8)			// FN选择项下横线

#define		H_FN_SEL_AD		25						// AD选择项
#define		H_FN_SEL_AD_L	(H_FN_SEL_AD-8)			// AD选择项左竖线
#define		H_FN_SEL_AD_R	(H_FN_SEL_AD+24*2+8)	// AD选择项右竖线

#define		H_FN_SEL_F1		107						// F1选择项
#define		H_FN_SEL_F1_L	(H_FN_SEL_F1-10)		// F1选择项左竖线
#define		H_FN_SEL_F1_R	(H_FN_SEL_F1+13*2+10)	// F1选择项右竖线

#define		H_FN_SEL_F2		(H_FN_SEL_F1+50)		// F2选择项
#define		H_FN_SEL_F2_L	(H_FN_SEL_F2-10)		// F2选择项左竖线
#define		H_FN_SEL_F2_R	(H_FN_SEL_F2+13*2+10)	// F2选择项右竖线

#define		H_FN_SEL_F3		(H_FN_SEL_F2+50)		// F3选择项
#define		H_FN_SEL_F3_L	(H_FN_SEL_F3-10)		// F3选择项左竖线
#define		H_FN_SEL_F3_R	(H_FN_SEL_F3+13*2+10)	// F3选择项右竖线

#define		H_FN_SEL_F4		(H_FN_SEL_F3+50)		// F4选择项
#define		H_FN_SEL_F4_L	(H_FN_SEL_F4-10)		// F4选择项左竖线
#define		H_FN_SEL_F4_R	(H_FN_SEL_F4+13*2+10)	// F4选择项右竖线

// 竖屏
#if TFT_TYPE != TFT_HJ28_9327
#define		S_FN_SEL1		  24//382						// FN选择项
#define		S_FN_SEL1_UL	(S_FN_SEL1-9)//(S_FN_SEL1+24+8)		// FN选择项上横线
#define		S_FN_SEL1_DL	(S_FN_SEL1+24+7)//(S_FN_SEL1-8)			// FN选择项下横线

#define		S_FN_SEL2		72//334						// FN选择项
#define		S_FN_SEL2_UL	(S_FN_SEL2-9)	//(S_FN_SEL2+24+8)		// FN选择项上横线
#define		S_FN_SEL2_DL	(S_FN_SEL2+24+7)//(S_FN_SEL2-8)			// FN选择项下横线

#define		S_FN_SEL1_AD	20				// FN选择项
#define		S_FN_SEL1_AD_L	(S_FN_SEL1_AD-9)				// 功能名字上横线
#define		S_FN_SEL1_AD_R	(S_FN_SEL1_AD+24*2+7)		// 功能名字下横线

#define		S_FN_SEL1_F1	97				// FN选择项
#define		S_FN_SEL1_F1_L	(S_FN_SEL1_F1-10)				// 功能名字上横线
#define		S_FN_SEL1_F1_R	(S_FN_SEL1_F1+13*2+8)		// 功能名字下横线

#define		S_FN_SEL1_F2	(S_FN_SEL1_F1+50)				// FN选择项
#define		S_FN_SEL1_F2_L	(S_FN_SEL1_F2-10)				// 功能名字上横线
#define		S_FN_SEL1_F2_R	(S_FN_SEL1_F2+13*2+8)		// 功能名字下横线

#define		S_FN_SEL1_F3	(S_FN_SEL1_F2+50)				// FN选择项
#define		S_FN_SEL1_F3_L	(S_FN_SEL1_F3-10)				// 功能名字上横线
#define		S_FN_SEL1_F3_R	(S_FN_SEL1_F3+13*2+8)		// 功能名字下横线

#define		S_FN_SEL2_F4	20				// FN选择项
#define		S_FN_SEL2_F4_L	(S_FN_SEL2_F4-10)				// 功能名字上横线
#define		S_FN_SEL2_F4_R	(S_FN_SEL2_F4+13*2+8)		// 功能名字下横线

#else
#define		S_FN_SEL1		382						// FN选择项
#define		S_FN_SEL1_UL	(S_FN_SEL1+24+8)		// FN选择项上横线
#define		S_FN_SEL1_DL	(S_FN_SEL1-8)			// FN选择项下横线

#define		S_FN_SEL2		334						// FN选择项
#define		S_FN_SEL2_UL	(S_FN_SEL2+24+8)		// FN选择项上横线
#define		S_FN_SEL2_DL	(S_FN_SEL2-8)			// FN选择项下横线

#define		S_FN_SEL1_AD	20				// FN选择项
#define		S_FN_SEL1_AD_L	(S_FN_SEL1_AD-8)				// 功能名字上横线
#define		S_FN_SEL1_AD_R	(S_FN_SEL1_AD+24*2+8)		// 功能名字下横线

#define		S_FN_SEL1_F1	97				// FN选择项
#define		S_FN_SEL1_F1_L	(S_FN_SEL1_F1-10)				// 功能名字上横线
#define		S_FN_SEL1_F1_R	(S_FN_SEL1_F1+13*2+10)		// 功能名字下横线

#define		S_FN_SEL1_F2	(S_FN_SEL1_F1+50)				// FN选择项
#define		S_FN_SEL1_F2_L	(S_FN_SEL1_F2-10)				// 功能名字上横线
#define		S_FN_SEL1_F2_R	(S_FN_SEL1_F2+13*2+10)		// 功能名字下横线

#define		S_FN_SEL1_F3	(S_FN_SEL1_F2+50)				// FN选择项
#define		S_FN_SEL1_F3_L	(S_FN_SEL1_F3-10)				// 功能名字上横线
#define		S_FN_SEL1_F3_R	(S_FN_SEL1_F3+13*2+10)		// 功能名字下横线

#define		S_FN_SEL2_F4	20				// FN选择项
#define		S_FN_SEL2_F4_L	(S_FN_SEL2_F4-10)				// 功能名字上横线
#define		S_FN_SEL2_F4_R	(S_FN_SEL2_F4+13*2+10)		// 功能名字下横线
#endif
//-------------------------------------------------------------------
// 参数配置
//横屏
#define		H_FN_NAME_ROW	    123					 // 功能名称行
#define		H_FN_VAL_ROW	    123					 // 数值
#define		H_FN_NAME_UL_ROW	(H_FN_NAME_ROW+40+13)//180			// 功能名字上横线
#define		H_FN_NAME_DL_ROW	(H_FN_NAME_ROW-13)//106				// 功能名字下横线
#define		H_FN_VAL_COL	320					 // 数值
			// 
#define		H_FN_SRC1_ROW	   (H_FN_VAL_ROW - 48)		// 差48
#define		H_FN_SRC2_ROW	   (H_FN_SRC1_ROW - 28)		// 差32
#define		H_FN_SRC3_ROW	   (H_FN_SRC2_ROW - 28)		// 差32

// 竖屏
#if TFT_TYPE != TFT_HJ28_9327
#define		S_FN_NAME_ROW	   (140)					// 260
#define		S_FN_NAME_UL_ROW	(S_FN_NAME_ROW-13)//(S_FN_NAME_ROW+40+13)//305				// 功能名字上横线
#define		S_FN_VAL_ROW	   190					// 210
#define		S_FN_NAME_DL_ROW	(S_FN_VAL_ROW+40+13)//183				// 功能名字下横线

#define		S_FN_SRC1_ROW	   (S_FN_NAME_DL_ROW  + 11)		// 差48	-35
#define		S_FN_SRC2_ROW	   (S_FN_SRC1_ROW + 30)		// 差32
#define		S_FN_SRC3_ROW	   (S_FN_SRC2_ROW + 30)		// 差32
#define		S_FN_SRC4_ROW	   (S_FN_SRC3_ROW + 30)		// 差32

#else
#define		S_FN_NAME_ROW	   260					// 
#define		S_FN_NAME_UL_ROW	(S_FN_NAME_ROW+40+13)//305				// 功能名字上横线
#define		S_FN_VAL_ROW	   210
#define		S_FN_NAME_DL_ROW	(S_FN_VAL_ROW-13)//183				// 功能名字下横线

#define		S_FN_SRC1_ROW	   (S_FN_NAME_DL_ROW  - 35)		// 差48	
#define		S_FN_SRC2_ROW	   (S_FN_SRC1_ROW - 30)		// 差32
#define		S_FN_SRC3_ROW	   (S_FN_SRC2_ROW - 30)		// 差32
#define		S_FN_SRC4_ROW	   (S_FN_SRC3_ROW - 30)		// 差32
#endif


// 方向图标    重点
#define ROW1  36			//显示的行数  方向图标高
#define COL1  47			//显示的列数  方向图标宽
#define ROW2  16			//显示的行数  方向图标高
#define COL2  8			//显示的列数  方向图标宽
// 横屏
#define ROW1_P  270//312 223			//显示的行数  方向图标行起始位置
#define COL1_P  180//105 154			//显示的列数  方向图标列起始位置		调换  4.24

// 竖屏
#if TFT_TYPE != TFT_HJ28_9327
#define ROW1_SP  238//90//223				//显示的行数  方向图标行起始位置
#define COL1_SP  157//160//154			//显示的列数  方向图标列起始位置
#else
// 竖屏
#define ROW1_SP  98//223				//显示的行数  方向图标行起始位置
#define COL1_SP  178//154			//显示的列数  方向图标列起始位置
#endif
//----------------------------------------------------------------

// 横屏
#define ROW2_HP  24//312//223			//显示的行数  方向图标行起始位置
#define COL2_HP  150//105//154			//显示的列数  方向图标列起始位置

// 竖屏
#define ROW2_SP  295//90//223				//显示的行数  方向图标行起始位置
#define COL2_SP  24//160//154			//显示的列数  方向图标列起始位置

// 信息屏横屏，行列定义
#define		H_INFO_NAMEM_ROW		190
#define		H_INFO_NAMES_ROW		(H_INFO_NAMEM_ROW-40-5)	 // 上线
//#define		H_RUN_NAME_DL		(H_RUN_NAME_ROW-13)			 // 下线
#define		H_INFO_COL				120

#define		H_INFO_SRC1_ROW		(H_INFO_NAMES_ROW-24-20)
#define		H_INFO_SRC2_ROW		(H_INFO_SRC1_ROW-24-5)
#define		H_INFO_SRC3_ROW		(H_INFO_SRC2_ROW-24-5)
#define		H_INFO_SRC4_ROW		(H_INFO_SRC3_ROW-24-5)
//#define		H_INFO_SRC5_ROW		(H_INFO_SRC4_ROW-24-5)

// 竖屏，行列定义
#if TFT_TYPE != TFT_HJ28_9327
#define		S_INFO_NAMEM_ROW		280
#define		S_INFO_NAMES_ROW		(S_INFO_NAMEM_ROW-40-5)	 // 上线
//#define		H_RUN_NAME_DL		(H_RUN_NAME_ROW-13)			 // 下线
#define		S_INFO_COL			10

#define		S_INFO_SRC1_ROW		(S_INFO_NAMES_ROW-24-20)
#define		S_INFO_SRC2_ROW		(S_INFO_SRC1_ROW-24-5)
#define		S_INFO_SRC3_ROW		(S_INFO_SRC2_ROW-24-5)
#define		S_INFO_SRC4_ROW		(S_INFO_SRC3_ROW-24-5)
#else
#define		S_INFO_NAMEM_ROW		280
#define		S_INFO_NAMES_ROW		(S_INFO_NAMEM_ROW-40-5)	 // 上线
//#define		H_RUN_NAME_DL		(H_RUN_NAME_ROW-13)			 // 下线
#define		S_INFO_COL			10

#define		S_INFO_SRC1_ROW		(S_INFO_NAMES_ROW-24-20)
#define		S_INFO_SRC2_ROW		(S_INFO_SRC1_ROW-24-5)
#define		S_INFO_SRC3_ROW		(S_INFO_SRC2_ROW-24-5)
#define		S_INFO_SRC4_ROW		(S_INFO_SRC3_ROW-24-5)
#endif
//----------------------------------------------------------------

#define	COL_S1	12//12//8//15//12			// 留出左边间距
//-------------------------------------------------------------------
#define H_DISPLAY_MODE		0		// 横屏显示模式	
#define S_DISPLAY_MODE		1		// 竖屏显示模式	

#define ONLY_DISPLAY_MODE		0	// 单任务屏显示模式	
#define DOUBLE_DISPLAY_MODE		1	// 双任务屏显示模式	
#define INFO_DISPLAY_MODE		2	// 单信息屏显示模式	

#define ONLY_TASK_MODE			0	// 单任务屏显示模式	
#define DOUBLE_TASK1_MODE		1	// 双任务屏任务1显示模式	
#define DOUBLE_TASK2_MODE		2	// 双任务屏任务2显示模式	
#define MAX_TASK_MODE		2	// 双任务屏任务2显示模式
		
