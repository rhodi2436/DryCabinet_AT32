/**********************************************************************
* TFT Control                                                 
*---------------------------------------------------------
*备注：TFT控制模块


//说明：
1. 图片和汉字库存在NorFlash里，ASCII码存在芯片内部FLASH里
2. NorFlash 烧录文件

***********************************************************************/

#include "../Head file/includes.h" 				  // 主头文件

#include "qrencode.h"
#include "qr_encode.h"
#include <math.h> // 嵌入式编译器需开启数学库（如GCC加-lm参数）
//#include  "../Head file/Image_Const.h"
//#include  "../Head file/China_Dot_Lib.h"
//#include  "../Head file/Dot_Lib.h"

#define		TFT_HJ30	0x7793
#define		TFT_HJ28	0x9326
#define		TFT_HJ28_9327	0x0000
//#define		TFT_TYPE	TFT_HJ30	// 鸿佳3.0寸屏
//#define		TFT_TYPE	TFT_HJ28	// 鸿佳2.8寸屏,ILI9326
#define		TFT_TYPE	TFT_HJ28_9327	// 鸿佳2.8寸屏,ILI9327

#define	 HARD_VER	"T-ES1 V1.0"
#if TFT_TYPE == TFT_HJ28
	#define	 DEV_NEME	"T-ES1"
	#define	 HARS_SOFT_VER	"V1.00    2020-09-10"
	
#elif TFT_TYPE == TFT_HJ30
	#define	 DEV_NEME	"T-S1"
	#define	 HARS_SOFT_VER	"V3.00    2020-09-10"
	
#elif TFT_TYPE == TFT_HJ28_9327
	#define	 DEV_NEME	"T-ES1"
	#define	 HARS_SOFT_VER	"V1.20    2021-02-25"
#endif

//-----------------------------------------------------------------------

const  INT16U DispBackNameRow[2][PAGE_MAX_BACK_NUM] = { { H_BACK_TASK1_ROW, H_BACK_TASK2_ROW, H_BACK_TASK3_ROW, H_BACK_TASK4_ROW, H_BACK_TASK5_ROW },	
                                                   { S_BACK_TASK1_ROW, S_BACK_TASK2_ROW, S_BACK_TASK3_ROW, S_BACK_TASK4_ROW, S_BACK_TASK5_ROW } };		  // 品名区
const  INT16U DispBackSkuRow[2][PAGE_MAX_BACK_NUM]  = { { H_BACK_SKU1_ROW,  H_BACK_SKU2_ROW,  H_BACK_SKU3_ROW,  H_BACK_SKU4_ROW,  H_BACK_SKU5_ROW },	
                                                   { S_BACK_SKU1_ROW,  S_BACK_SKU2_ROW,  S_BACK_SKU3_ROW,  S_BACK_SKU4_ROW,  S_BACK_SKU5_ROW } };		  // 库位数量区
const  INT16U DispBackNameCol[2]    = {         COL_S1 ,COL_S1 	 };		  // 品名 列
const  INT16U DispBackSkuCol[2]     = { H_BACK_SKU_COL ,COL_S1 	 };		  // 品名 列
const  INT16U DispBackUniCol[2]     = { ROW,COL };		  // 品名 列
const  INT16U DispBackNameEndCol[2] = { H_BACK_SKU_COL , COL };		  // 品名 列
//-------------------------------------------------------------------------


u16   Min_flag_zeng = 0;				// 条形图更新显示，一分钟加1;
u16   qie_flang = 0;    				// 切换标置;
float Last_SHT30_Humidity = 0 ;       	// 最后1次的湿度值，初始化，避免随机值
float Cur_SHT30_Humidity = 0 ;       	// 当前显示湿度值
//-------------------------------------------------------------------------


#define	BK_FLASHPIC	0			// 取图像背景色
#define	BK_SALFDEF	1			// 自定义背景色

INT8U	TFT_Data[1500];			// 备份数据
//INT8U	TFT_CharBuff[20];

#define	 NAME_MAX	37		       // 品名
#define	 SRC_INFO_MAX	61		   // 品名

// 品名，描述信息
union
{
	struct
	{	
		INT8U	TaskNum[8];						// 任务框显示任务个数
		INT8U	Name[NAME_MAX];					// 品名信息，一行最多10个汉字，20个字符，最后一个作为结束标志
		INT8U	SrcInfo[3][SRC_INFO_MAX];		// 描述信息，一行最多10个汉字，20个字符，最后一个作为结束标志
	}run;

	struct
	{	
		INT8U	TaskNum[12];
		INT8U	Name[NAME_MAX];
	}wait;

	struct
	{	
		INT8U	NameM[NAME_MAX];				// 主标题，一行最多10个汉字，20个字符，最后一个作为结束标志
		INT8U	NameS[NAME_MAX];				// 付标题，一行最多10个汉字，20个字符，最后一个作为结束标志
		INT8U	SrcInfo[3][SRC_INFO_MAX];		// 描述信息，一行最多10个汉字，20个字符，最后一个作为结束标志
	}info;

}strTFT1[2];

// 库位，数量，方向，单位
#define		SKU_DISP_MAX	4			// 库位最大显示位数
#define		CNT_DISP_MAX	5			// 数量最大显示位数
#define		DIR_DISP_MAX	1			// 方向最大显示位数
#define		UNI_DISP_MAX	2			// 单位最大显示位数
union
{
	struct
	{	
		INT8U	CurSku[SKU_DISP_MAX];	   // 库位号
		INT8U	CurCnt[CNT_DISP_MAX];	   // 数量
		INT8U	CurDir[DIR_DISP_MAX];	   // 方向
		INT8U	CurUni[UNI_DISP_MAX];	   // 单位
	}run;

	struct
	{	
		INT8U	CfgValue;
		INT8U	Addr[3];
	}Cfg;

	struct
	{	
		INT8U	picture;	 	// 图片
	}info;

	INT8U	Byte[30];
}strTFT2[2];



INT8U  bk_colour = 0;	 // 背景图 ，查表用
INT16U  bk_colour_true = 0;	 // 背景图 ，真正颜色

//                              0        1        2       3        4         5         6        7        8       9       A	     B       C       D      E       F
//INT16U bk_colour_tab[16]   = {aBLACK,  eRED,    eGREEN,  eBLUE,   eYELLOW, eMAGENTA,  eCYAN,   eGRAY,   aBLACK,  aGRAY,  ORANGE, BROWN,  PURPLE, PINK              };	// 背景颜色
INT16U bk_colour_tab[16]   = {aBLACK,  fRED,    eGREEN,  fBLUE,   dORANGE, fMAGENTA,   bCYAN,   eGRAY,   aBLACK,  aGRAY,  ORANGE, BROWN,  PURPLE, PINK              };	// 背景颜色
//INT16U bk_colour_tab[16]   = {aBLACK,  rRED,    rGREEN,  rBLUE,   rYELLOW, rMAGENTA,  rCYAN,   rGRAY,   aBLACK, aGRAY,  ORANGE, BROWN,  PURPLE, PINK              };	// 背景颜色
INT16U dsc_colour_tab[16]  = {aGRAY,   aGRAY,   aGRAY,   aGRAY,   aWHITE,   aGRAY,     aGRAY,   aGRAY,   aGRAY ,  aGRAY,  aGRAY,  aGRAY,  aGRAY,  aGRAY, aGRAY, aGRAY  };// 描述信息文字颜色
INT16U txt_colour_tab[16]  = {aWHITE,  aWHITE,  aWHITE,  aWHITE,  aWHITE,  aWHITE,    aWHITE,  aWHITE,  aWHITE,  aWHITE, aWHITE, aWHITE, aWHITE, aWHITE,aWHITE,aWHITE };// 品名文字颜色
INT16U btxt_colour_tab[16] = {bGRAY,   bGRAY,   bGRAY,   bGRAY,   bGRAY,   bGRAY,     bGRAY ,  bGRAY ,  bGRAY ,  bGRAY,  bGRAY,  bGRAY,  bGRAY,  bGRAY ,bGRAY ,bGRAY  };// 品名未选中颜色
//INT16U sku_colour_tab[16]  = {aGREEN,  aGREEN,  aMAGENTA,aMAGENTA,aMAGENTA,aGREEN,    aMAGENTA,aMAGENTA,aMAGENTA,aMAGENTA,aGREEN,aGREEN,                               };	// 库位颜色
//INT16U sku_colour_tab[16]  = {aGREEN,  aGREEN,  aRED,    aRED,    aRED,    aGREEN,    aRED,    aRED,    aRED,    aRED,   aGREEN, aGREEN,                               };	// 库位颜色
INT16U sku_colour_tab[16]  = {aGREEN,  aWHITE,  aWHITE,  aWHITE,  aWHITE,  aWHITE,    aWHITE,  aWHITE,  aWHITE,  aWHITE, aWHITE, aWHITE, aWHITE,aWHITE, aWHITE,aWHITE };// 库位颜色
INT16U bsku_colour_tab[16] = {bGREEN,  bGRAY,   bGRAY,   bGRAY,   bGRAY,   bGRAY,     bGRAY,   bGRAY,   bGRAY,   bGRAY,  bGRAY,  bGRAY,  bGRAY, bGRAY,  bGRAY, bGRAY, };// 库位未选中颜色
//INT16U cnt_colour_tab[16]  = {aRED,    aCYAN,   aRED,    aRED,    aRED,    aCYAN,     aRED,    aRED,    aRED,    aRED,   aCYAN,  aCYAN,                               };	// 数量颜色
//INT16U cnt_colour_tab[16]  = {aRED,    bGREEN,  bRED,    bRED,    bRED,    bGREEN,    bRED,    bRED,    bRED,    bRED,   bGREEN, bGREEN,                               };	// 数量颜色
//INT16U cnt_colour_tab[16]  = {aRED,    fGREEN,  fRED,    fRED,    fRED,    fGREEN,    fRED,    fRED,    fRED,    fRED,   fGREEN, fGREEN,                               };	// 数量颜色
INT16U cnt_colour_tab[16]  = {aRED,    aWHITE,  aWHITE,  aWHITE,  aWHITE,  aWHITE,    aWHITE,  aWHITE,  aWHITE,  aWHITE, aWHITE, aWHITE, aWHITE,aWHITE, aWHITE,aWHITE };	// 数量颜色
INT16U bcnt_colour_tab[16] = {bRED,    bGRAY,   bGRAY,   bGRAY,   bGRAY,   bGRAY,     bGRAY,   bGRAY,   bGRAY,   bGRAY,  bGRAY,  bGRAY,  bGRAY, bGRAY,  bGRAY, bGRAY, };	// 数量未颜色
//INT16U bk_colour_tab[16]   = {BK_BLACK,BK_RED,  BK_GREEN,BK_BLUE, BK_YELLOW,BK_MAGENTA,BK_CYAN, BK_WHITE,BK_BROWN,BK_GRAY,BK_ORANGE};
//INT16U txt_colour_tab[16]  = {TXT_WHITE,TXT_WHITE,TXT_BLACK,TXT_WHITE,TXT_BLACK, TXT_WHITE,  TXT_BLACK,TXT_BLACK,TXT_WHITE,TXT_WHITE,TXT_WHITE};
//INT16U dsc_colour_tab[16]  = {DSC_GRAY,DSC_GRAY,DSC_BLACK,DSC_GRAY,BK_BLACK, DSC_GRAY,  BK_BLACK,BK_BLACK,DSC_GRAY,DSC_GRAY,DSC_GRAY};
//INT16U btxt_colour_tab[16] = {,DSC_GRAY,BK_BLACK,DSC_GRAY,BK_BLACK, DSC_GRAY,  BK_BLACK,BK_BLACK,DSC_GRAY,DSC_GRAY,DSC_GRAY};


//---------------------------------------------------------------------
/**********************************************************************
**函数名:  TFT_WD_COM
**功能:    写命令
***********************************************************************/
void TFT1_WD_COM(INT16U command)
{	*(INT16U *) (TFT1_COM_ADDR) = (command);  	  }

/**********************************************************************
**函数名:  TFT_WD_DAT
**功能:    写数据
***********************************************************************/
void TFT1_WD_DAT(INT16U dat)
{	*(INT16U *) (TFT1_DAT_ADDR) = dat;    }

/**********************************************************************
**函数名:  TFT_RD_DAT
**功能:    读数据
***********************************************************************/
INT16U TFT1_RD_DAT(void)
{	return(*(INT16U *) (TFT1_DAT_ADDR));    }

/**********************************************************************
**函数名:  TFT_WD_REG
**功能:    写寄存器
***********************************************************************/

void TFT1_WD_RE(INT16U reg,INT16U dat)						
{	
	TFT1_WD_COM(reg);
	TFT1_WD_DAT(dat);
}

/**********************************************************************
**函数名:  TFT_RD_REG
**功能:    读寄存器
***********************************************************************/
INT16U TFT1_RD_REG(INT16U reg)
{	
	TFT1_WD_COM(reg);
	return(TFT1_RD_DAT());
}
/**********************************************************************
显示模式设置，横屏竖屏 ，竖屏从32开始到431，横屏从0到400
***********************************************************************/
const TFT_DIR_SET	conLcdDirSet_7793[]=
{	
//ST7793 和 ILI9326一致	
 {H_DISPLAY_MODE,ONLY_DISPLAY_MODE,  0x10B0,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，任务单屏
 {S_DISPLAY_MODE,ONLY_DISPLAY_MODE,  0x1098,0x00,0xEF, 0x00,0x18F,0x00},			//竖屏，任务单屏

 {H_DISPLAY_MODE,DOUBLE_DISPLAY_MODE,0x10B0,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，任务双屏
 {S_DISPLAY_MODE,DOUBLE_DISPLAY_MODE,0x1098,0x00,0xEF, 0x00,0x18F,0x00},			//竖屏，任务双屏

 {H_DISPLAY_MODE,INFO_DISPLAY_MODE,  0x10B0,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，信息屏
 {S_DISPLAY_MODE,INFO_DISPLAY_MODE,  0x1098,0x00,0xEF, 0x00,0x18F,0x00},			//竖屏，信息屏	

};

const TFT_DIR_SET	conLcdDirSet_9327[]=
{
	
 {H_DISPLAY_MODE,ONLY_DISPLAY_MODE,  0x48,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，任务单屏
 {S_DISPLAY_MODE,ONLY_DISPLAY_MODE,  0xE8,0x20,0x1AF,0x00,0xEF, 0x20},			//竖屏，任务单屏

 {H_DISPLAY_MODE,DOUBLE_DISPLAY_MODE,0x48,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，任务双屏
 {S_DISPLAY_MODE,DOUBLE_DISPLAY_MODE,0xE8,0x20,0x1AF,0x00,0xEF, 0x20},			//竖屏，任务双屏

 {H_DISPLAY_MODE,INFO_DISPLAY_MODE,  0x48,0x00,0xEF, 0x00,0x18F,0x00},			//横屏，信息屏
 {S_DISPLAY_MODE,INFO_DISPLAY_MODE,  0xE8,0x20,0x1AF,0x00,0xEF, 0x20},			//竖屏，信息屏
};

void  LCD_Dsip_Mode(void)
{
	INT8U mode;
	const TFT_DIR_SET *ptr; 
	mode = para.str.DispMode;//LcdDispMode; 
	if(mode >= 5)	{ mode = 5; }
	
	// Entry Mode (R003h)
	if( (LCD.TFT1_ID == TFT_HJ28)||(LCD.TFT1_ID == TFT_HJ30) )
	{
		
		ptr = &conLcdDirSet_7793[mode];

		DispDir.dir  =  ptr->dir;
		DispDir.only =  ptr->only;
		DispDir.dval =  ptr->dval;
		DispDir.col0 =  ptr->col0; //+ 0x28;
		DispDir.col1 =  ptr->col1; //+ 0x28;;
		DispDir.row0 =  ptr->row0; //+ 0x28;;
		DispDir.row1 =  ptr->row1; //+ 0x28;;
		DispDir.dif  =  ptr->dif;		
		
		TFT1_WD_COM(0x0003);  //Set_address_mode 显示方向，1个数据
		TFT1_WD_DAT(DispDir.dval);

		//  (R210h, R211h, R212h, R213h) 水平和垂直窗口地址位置	
		TFT1_WD_COM(0x0210); TFT1_WD_DAT(DispDir.col0);
		TFT1_WD_COM(0x0211); TFT1_WD_DAT(DispDir.col1);
		TFT1_WD_COM(0x0212); TFT1_WD_DAT(DispDir.row0);
		TFT1_WD_COM(0x0213); TFT1_WD_DAT(DispDir.row1);
	}
	else
	{
		ptr = &conLcdDirSet_9327[mode];

		DispDir.dir  =  ptr->dir;
		DispDir.only =  ptr->only;
		DispDir.dval =  ptr->dval;
		DispDir.col0 =  ptr->col0; //+ 0x28;
		DispDir.col1 =  ptr->col1; //+ 0x28;;
		DispDir.row0 =  ptr->row0; //+ 0x28;;
		DispDir.row1 =  ptr->row1; //+ 0x28;;
		DispDir.dif  =  ptr->dif;

		TFT1_WD_COM(0x36);  //Set_address_mode 显示方向，1个数据
		TFT1_WD_DAT(DispDir.dval);

		//Set_column_address,带4个数据
		TFT1_WD_COM(0x2A);  //Set_column_address
		TFT1_WD_DAT(DispDir.col0>>8);
		TFT1_WD_DAT(DispDir.col0&0xff); 
		TFT1_WD_DAT(DispDir.col1>>8);
		TFT1_WD_DAT(DispDir.col1&0xff); 

		//Set_page_address,带4个数据
		TFT1_WD_COM(0x2B);  //Set_page_address
		

		TFT1_WD_DAT(DispDir.row0>>8);
		TFT1_WD_DAT(DispDir.row0&0xff);
		TFT1_WD_DAT(DispDir.row1>>8);
		TFT1_WD_DAT(DispDir.row1&0xff); 	
	}
}

/**********************************************************************
**函数名:  TFT1_JBT6K85_INIT
**功能:    TFT IC JBT6K85初始化
***********************************************************************/

void TFT1_ILI9327_INIT(void)
{

	// 硬件复位	
	TFT1_REST_ON();     	//Reset = 1
	Delay_ms(10);		  	//10 Delay3.15ms    
	TFT1_REST_OFF();	  	//Reset = 0
	Delay_ms(100);      	//Delay 500 16Ms //This delay time is necessary
	TFT1_REST_ON();	  		//Reset = 1
	Delay_ms(500);	  		//Delay 3000 60Ms

	

	
//#if TFT_TYPE != TFT_HJ28_9327	
  /* Read ID*/
//	TFT1_WD_COM(0x0000);
//	Delay_us(5);
//	LCD.TFT1_ID = TFT1_RD_DAT();
//-----------------Display Control Setting---------------------//
 if( LCD.TFT1_ID ==	TFT_HJ30)
 {	
	//--------------------------------------------------------------------------------
	//鸿佳3.0寸屏初始化  ST7793
	//-----------------Display Control Setting---------------------//
	// R001h Driver Output Control  ,有效位SM-D10 和 SS-D8
	TFT1_WD_COM(0x0001);  TFT1_WD_DAT(0x0100);		//SS(D8)=0  SM(D10)=0 
	//TFT1_WD_DAT(0x0100);	//SS(D8)=1  SM(D10)=0	
	// R003h Entry Mode  显示翻转  ,有效位AM-D3,I/D-[D4:5],BGR-D12,ORG-D7,DFM-D14,TRI-D15
	// TFT1_WD_COM(0x0003);
	// TFT1_WD_DAT(0x1020);	// AM=0,I/D=10,BGR=1,ORG=0,TRI=0,DFM=0		
	//  TFT1_WD_DAT(0x10B0);	// AM=0,I/D=10,BGR=1,ORG=1,TRI=0,DFM=0
	LCD_Dsip_Mode();		// 显示模式

	// R008h Display Control2 ,  FP-[D8:15]  BP-[D0:7]
	// TFT1_WD_COM(0x0008); TFT1_WD_DAT(0x0000);
	 
	// R090h Frame Marker Contro 帧标记控制
	TFT1_WD_COM(0x0090);  TFT1_WD_DAT(0x8000);	

	// Base Image Display Control (R400h,R401h, R404h)
	// R400h   Base Image Number of Line 
	// GS= 0 指定栅极驱动器开始扫描的栅极线, 设置以8行为间隔驱动LCD的行数
	TFT1_WD_COM(0x0400);  TFT1_WD_DAT(0x6200);		// GS= 0 	，从0开始扫描
	//TFT1_WD_DAT(0x6200);		// GS= 0 	

	// R401h   Base Image Display Control 
	TFT1_WD_COM(0x0401);  TFT1_WD_DAT(0x0000);			
	// R404h   Base Image Vertical Scroll Control  
	TFT1_WD_COM(0x0404);  TFT1_WD_DAT(0x0000);		
	//----------------End Display Control setting-------------------//
	//------------- Power Control Registers Initial ----------------//
	TFT1_WD_COM(0x00ff); TFT1_WD_DAT(0x0001);			// Ext Register Control 设置为1以启用对扩展寄存器的访问,包括R380h~R389h,R702h,R708h,R710h,R712h,R713h,R724h,R7E1h,

	TFT1_WD_COM(0x0102); TFT1_WD_DAT(0x01b0);			// Power Control 3  打开电源。必须写入PON和PSON才能开始内部电源操作。
	TFT1_WD_COM(0x0710); TFT1_WD_DAT(0x0010);			// 18h 4.489 V
	TFT1_WD_COM(0x0712); TFT1_WD_DAT(0x000f);			// 0Fh 0 mV
	TFT1_WD_COM(0x0752); TFT1_WD_DAT(0x002f);			// 2FH  Medium High 
	//TFT1_WD_COM(0x0759);TFT1_WD_DAT(0x0070);
	//--------------End Power Control Registers Initial -----------//
	//Delay_ms(100);
	//---------------Display Windows 240 X 400------------------ --//
	// Horizontal and Vertical Window Address Position (R210h, R211h, R212h, R213h)
	// TFT1_WD_COM(0x0210);TFT1_WD_DAT(0x0000);			// R210h  Horizontal 起始位置
	// TFT1_WD_COM(0x0211);TFT1_WD_DAT(0x00ef);			// R211h  Horizontal 结束位置
	// TFT1_WD_COM(0x0212);TFT1_WD_DAT(0x0000);			// R212h  Vertical   起始位置
	// TFT1_WD_COM(0x0213);TFT1_WD_DAT(0x018F);			// R213h  Vertical   结束位置
	//---------------End Display Windows 240 X 400-----------   --//
	Delay_ms(2);
	//------------------Gamma Cluster Setting----------------------//
	// Gamma Control (R380h~R389h)
	// TFT1_WD_COM(0x0380);TFT1_WD_DAT(0x0600);			// Gamma Control
	// TFT1_WD_COM(0x0381);TFT1_WD_DAT(0xcc13);
	// TFT1_WD_COM(0x0382);TFT1_WD_DAT(0x0c0a);
	// TFT1_WD_COM(0x0383);TFT1_WD_DAT(0x0a00);
	// TFT1_WD_COM(0x0384);TFT1_WD_DAT(0x1022);
	// TFT1_WD_COM(0x0385);TFT1_WD_DAT(0x0600);
	// TFT1_WD_COM(0x0386);TFT1_WD_DAT(0xcd11);
	// TFT1_WD_COM(0x0387);TFT1_WD_DAT(0x0e07);
	// TFT1_WD_COM(0x0388);TFT1_WD_DAT(0x0a00);
	// TFT1_WD_COM(0x0389);TFT1_WD_DAT(0x1022);

	// Gamma Control (R380h~R389h)
	TFT1_WD_COM(0x0380);TFT1_WD_DAT(0x0103);			// Gamma Control
	TFT1_WD_COM(0x0381);TFT1_WD_DAT(0x5c1b);
	TFT1_WD_COM(0x0382);TFT1_WD_DAT(0x0b01);
	TFT1_WD_COM(0x0383);TFT1_WD_DAT(0x0615);
	TFT1_WD_COM(0x0384);TFT1_WD_DAT(0x0111);
	TFT1_WD_COM(0x0385);TFT1_WD_DAT(0x0103);
	TFT1_WD_COM(0x0386);TFT1_WD_DAT(0x9c17);
	TFT1_WD_COM(0x0387);TFT1_WD_DAT(0x0607);
	TFT1_WD_COM(0x0388);TFT1_WD_DAT(0x0614);
	TFT1_WD_COM(0x0389);TFT1_WD_DAT(0x0111);

	//--------------------End Gamma Setting----------------------//
	//--------------------Vcom Setting-----------------------  --//
	TFT1_WD_COM(0x0702);TFT1_WD_DAT(0x0057);
	TFT1_WD_COM(0x00ff);TFT1_WD_DAT(0x0000);			// Ext Register Control 设置为0，关闭对外部寄存器的访问
	//--------------------End Vcom Setting-----------------------//
	// R007h Display Control 1  PTDE-D12,BASEE-D8
	TFT1_WD_COM(0x0007); TFT1_WD_DAT(0x0100);		// BASEE=1,PTDE=0; 使能基本图像显示，关闭部分图像显示

	Delay_ms(6); //Delay 6ms
	// R200h DRAM水平地址设置 
	TFT1_WD_COM(0x0200); TFT1_WD_DAT(0x0000);		
	// R201h DRAM垂直地址设置 
	TFT1_WD_COM(0x0201); TFT1_WD_DAT(0x0000);		

	// TFT1_WD_COM(0x0202); //Read/Write GRAM
	// TFT1_WD_COM(0x0029);     //Display on
	//--------------------------------------------------------------------------------
 }
//#elif TFT_TYPE == TFT_HJ28
 else if( LCD.TFT1_ID == TFT_HJ28)
 {	 
	//--------------------------------------------------------------------------------
	// 鸿佳2.8寸初始化, ILI9326
	//    //************* Start Initial Sequence **********//
	TFT1_WD_COM(0x0702);  TFT1_WD_DAT(0x3008);		//  LCD_CtrlWrite_ILI9326(0x0702, 0x3008);// Set internal timing, don’t change this value
	TFT1_WD_COM(0x0705);  TFT1_WD_DAT(0x0036);		//    LCD_CtrlWrite_ILI9326(0x0705, 0x0036);// Set internal timing, don’t change this value
	TFT1_WD_COM(0x070B);  TFT1_WD_DAT(0x1213);		//    LCD_CtrlWrite_ILI9326(0x070B, 0x1213);// Set internal timing, don’t change this value

	TFT1_WD_COM(0x0001);  TFT1_WD_DAT(0x0100);		//    TFT1_WD_RE(0x0001, 0x0100); // set SS and SM bit
	TFT1_WD_COM(0x0002);  TFT1_WD_DAT(0x0100);		//   LCD_CtrlWrite_ILI9326(0x0002, 0x0100); // set 1 line inversion
	//  TFT1_WD_COM(0x0003);  TFT1_WD_DAT(0x10A8);	    //    TFT1_WD_RE(0x0003, 0x1030); // set GRAM write direction and BGR=1.
	LCD_Dsip_Mode();		// 显示模式	

	// R090h Frame Marker Contro 帧标记控制
	TFT1_WD_COM(0x0090);  TFT1_WD_DAT(0x8000);

	TFT1_WD_COM(0x0008);  TFT1_WD_DAT(0x0202);	//    LCD_CtrlWrite_ILI9326(0x0008, 0x0202); // set the back porch and front porch
	TFT1_WD_COM(0x0009);  TFT1_WD_DAT(0x0000);	//    LCD_CtrlWrite_ILI9326(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	TFT1_WD_COM(0x000C);  TFT1_WD_DAT(0x0000);	//    LCD_CtrlWrite_ILI9326(0x000C, 0x0000); // RGB interface setting
	TFT1_WD_COM(0x000F);  TFT1_WD_DAT(0x0000);	//    LCD_CtrlWrite_ILI9326(0x000F, 0x0000); // RGB interface polarity
	//    //*************Power On sequence ****************//
	TFT1_WD_COM(0x0100);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	TFT1_WD_COM(0x0102);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0102, 0x0000); // VREG1OUT voltage
	TFT1_WD_COM(0x0103);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude	
	Delay_ms(20); // Dis-charge capacitor power voltage	
	TFT1_WD_COM(0x0100);  TFT1_WD_DAT(0x1190);//    TFT1_WD_RE(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
	TFT1_WD_COM(0x0101);  TFT1_WD_DAT(0x0227);//    TFT1_WD_RE(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	Delay_ms(5); // Delay 50ms
	TFT1_WD_COM(0x0102);  TFT1_WD_DAT(0x009F);//    TFT1_WD_RE(0x0102, 0x01BD); // VREG1OUT voltage, VREG1R(D7)=1, 内部参考电压2.5V，倍率1.6~2.4
	Delay_ms(5); // Delay 50ms
	TFT1_WD_COM(0x0103);  TFT1_WD_DAT(0x2D00);//    TFT1_WD_RE(0x0103, 0x3800); // VDV[4:0] for VCOM amplitude
	TFT1_WD_COM(0x0281);  TFT1_WD_DAT(0x000E);//    TFT1_WD_RE(0x0281, 0x0013); // VCM[5:0] for Vcom High Voltage
	Delay_ms(5);
	TFT1_WD_COM(0x0200);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0200, 0x0000); // GRAM horizontal Address
	TFT1_WD_COM(0x0201);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0201, 0x0000); // GRAM Vertical Address
	//    // ----------- Adjust the Gamma Curve ----------//
	TFT1_WD_COM(0x0300);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0300, 0x0000);
	TFT1_WD_COM(0x0301);  TFT1_WD_DAT(0x0707);//    TFT1_WD_RE(0x0301, 0x0707);
	TFT1_WD_COM(0x0302);  TFT1_WD_DAT(0x0606);//    TFT1_WD_RE(0x0302, 0x0606);
	TFT1_WD_COM(0x0305);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0305, 0x0000);
	TFT1_WD_COM(0x0306);  TFT1_WD_DAT(0x0D00);//    TFT1_WD_RE(0x0306, 0x0E00);
	TFT1_WD_COM(0x0307);  TFT1_WD_DAT(0x0706);//    TFT1_WD_RE(0x0307, 0x0606);
	TFT1_WD_COM(0x0308);  TFT1_WD_DAT(0x0005);//    TFT1_WD_RE(0x0308, 0x0005);
	TFT1_WD_COM(0x0309);  TFT1_WD_DAT(0x0007);//    TFT1_WD_RE(0x0309, 0x0007);
	TFT1_WD_COM(0x030C);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x030C, 0x0002);
	TFT1_WD_COM(0x030D);  TFT1_WD_DAT(0x000A);//    TFT1_WD_RE(0x030D, 0x000E);
	//    //------------------ Set GRAM area ---------------//
	//  TFT1_WD_COM(0x0210);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0210, 0x0000); // Horizontal GRAM Start Address
	//  TFT1_WD_COM(0x0211);  TFT1_WD_DAT(0x00EF);//    TFT1_WD_RE(0x0211, 0x00EF); // Horizontal GRAM End Address
	//  TFT1_WD_COM(0x0212);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0212, 0x0000); // Vertical GRAM Start Address
	//  TFT1_WD_COM(0x0213);  TFT1_WD_DAT(0x018F);//    TFT1_WD_RE(0x0213, 0x018F); // Vertical GRAM Start Address
	TFT1_WD_COM(0x0400);  TFT1_WD_DAT(0x3100);//    TFT1_WD_RE(0x0400, 0x3100); // Gate Scan Line GS=0，限定行400
	TFT1_WD_COM(0x0401);  TFT1_WD_DAT(0x0001);//    TFT1_WD_RE(0x0401, 0x0001); // NDL,VLE, REV
	TFT1_WD_COM(0x0404);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0404, 0x0000); // set scrolling line
	//    //-------------- Partial Display Control ---------//
	TFT1_WD_COM(0x0500);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0500, 0x0000);//Partial Image 1 Display Position
	TFT1_WD_COM(0x0501);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0501, 0x0000); //Partial Image 1 RAM Start/End Address
	TFT1_WD_COM(0x0502);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0502, 0x0000); //Partial Image 1 RAM Start/End Address
	TFT1_WD_COM(0x0503);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0503, 0x0000); //Partial Image 2 Display Position
	TFT1_WD_COM(0x0504);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
	TFT1_WD_COM(0x0505);  TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0505, 0x0000); //Partial Image 2 RAM Start/End Address
	//    //-------------- Panel Control -------------------//
	TFT1_WD_COM(0x0010);  TFT1_WD_DAT(0x0010);//    TFT1_WD_RE(0x0010, 0x0010); //DIVI[1:0];RTNI[4:0]
	TFT1_WD_COM(0x0011);  TFT1_WD_DAT(0x0600);//    TFT1_WD_RE(0x0011, 0x0600); //NOWI[2:0];SDTI[2:0]
	TFT1_WD_COM(0x0020);  TFT1_WD_DAT(0x0002);//    TFT1_WD_RE(0x0020, 0x0002); //DIVE[1:0];RTNE[5:0]
	TFT1_WD_COM(0x0007);  TFT1_WD_DAT(0x0173);//    TFT1_WD_RE(0x0007, 0x0173); // 262K color and display ON
	//  TFT1_WD_COM(0x202);  //    TFT1_WD_COM(0x202);
  }
  else
//#else  
 {	 
	//--------------------------------------------------------------------------------
	// 鸿佳2.8寸初始化, ILI9327
	//    //************* Start Initial Sequence **********//
//  柯达科初始化	

TFT1_WD_COM(0x11);  //存储器数据访问
Delay_ms(120);	  		//Delay 3000 60Ms

	 
TFT1_WD_COM(0x36);  //存储器数据访问
TFT1_WD_DAT(0xC0); 
 
TFT1_WD_COM(0x3A);	//	接口像素格式  
TFT1_WD_DAT(0x55); //65K 16bit
 
TFT1_WD_COM(0xB2);  //porch setting 门廊设置
TFT1_WD_DAT (0x0C); 
TFT1_WD_DAT (0x0C); 
TFT1_WD_DAT (0x00);
TFT1_WD_DAT (0x33); 
TFT1_WD_DAT (0x33);  
	 
//TFT1_WD_COM(0x2A);
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0xef); 
//	 
//TFT1_WD_COM(0x2B);
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0x01); 
//TFT1_WD_DAT (0x3f); 	

TFT1_WD_COM(0xB7);  //Gate Control
TFT1_WD_DAT (0x46); 
 
TFT1_WD_COM(0xBB);  
TFT1_WD_DAT (0x1B); 
 
TFT1_WD_COM(0xC0);  
TFT1_WD_DAT (0x2C);

TFT1_WD_COM(0xC2);    //Power control 
TFT1_WD_DAT (0x01);   //VRH[5:0] 
 
TFT1_WD_COM(0xC3);    //Power control 
TFT1_WD_DAT (0x0F);   //SAP[2:0];BT[3:0] 
 
TFT1_WD_COM(0xC4);    //VCM control 
TFT1_WD_DAT (0x20);  
 
TFT1_WD_COM(0xC6);    //VCM control2 
TFT1_WD_DAT(0X0F); 
 
TFT1_WD_COM(0xD0);    // Memory Access Control 
TFT1_WD_DAT(0xA4);
TFT1_WD_DAT(0xA1);  
 
TFT1_WD_COM(0xD6);    // Memory Access Control 
TFT1_WD_DAT(0xA1);

TFT1_WD_COM(0xE0);    //Set Gamma 
TFT1_WD_DAT(0xF0); 
TFT1_WD_DAT (0x00); 
TFT1_WD_DAT (0x06); 
TFT1_WD_DAT (0x04); 
TFT1_WD_DAT (0x05); 
TFT1_WD_DAT (0x05); 
TFT1_WD_DAT (0x31); 
TFT1_WD_DAT (0X44); 
TFT1_WD_DAT (0x48); 
TFT1_WD_DAT (0x36); 
TFT1_WD_DAT (0x12); 
TFT1_WD_DAT (0x12); 
TFT1_WD_DAT (0x2B); 
TFT1_WD_DAT (0x34); 
 
 
TFT1_WD_COM(0XE1);    //Set Gamma 
TFT1_WD_DAT (0xF0); 
TFT1_WD_DAT (0x0B); 
TFT1_WD_DAT (0x0F); 
TFT1_WD_DAT (0x0F); 
TFT1_WD_DAT (0x0D); 
TFT1_WD_DAT (0x26); 
TFT1_WD_DAT (0x31); 
TFT1_WD_DAT (0x43); 
TFT1_WD_DAT (0x47); 
TFT1_WD_DAT (0x38); 
TFT1_WD_DAT (0x14); 
TFT1_WD_DAT (0x14); 
TFT1_WD_DAT (0x2C); 
TFT1_WD_DAT (0x32); 

TFT1_WD_COM(0X21);    //Set Gamma 
TFT1_WD_COM(0X29);    //Set Gamma 
TFT1_WD_COM(0X2C);    //Set Gamma 

//TFT1_WD_COM(0x11);    //Exit Sleep 
//Delay_ms(120); 
//TFT1_WD_COM(0x29); 



//TFT1_WD_COM(0xE9);  
//TFT1_WD_DAT (0x11); 
//TFT1_WD_DAT (0x11); 
//TFT1_WD_DAT (0x08);

//TFT1_WD_COM(0xD8);  
//TFT1_WD_DAT (0x25); 
//TFT1_WD_DAT (0x15); 
//TFT1_WD_DAT (0x10);

//TFT1_WD_COM(0xB1);  
//TFT1_WD_DAT (0x40); 
//TFT1_WD_DAT (0x07); 
//TFT1_WD_DAT (0x08);

//TFT1_WD_COM(0xB0);  
//TFT1_WD_DAT (0x00); 
//TFT1_WD_DAT (0xF0); 

//TFT1_WD_COM(0xB0); 
//TFT1_WD_COM(0x21); 

////TFT1_WD_COM(0xC5); 
////TFT1_WD_COM(0x20);


//TFT1_WD_COM(0x51);  //BL
//TFT1_WD_DAT(0xf0); //	BL
//TFT1_WD_COM(0x53);  // BL
//TFT1_WD_DAT(0x2c); //	BL
//TFT1_WD_COM(0x55);  // BL
//TFT1_WD_DAT(0x02); //BL
  }  
//#endif
}

//---------------------------------------------------------------------


/**********************************************************************
**函数名:  TFT_SetWindows
**功能:    设置窗口区域
***********************************************************************/

void TFT1_SetWindows(INT16U Xstart,INT16U Xend,INT16U Ystart,INT16U Yend) 
{
//#if TFT_TYPE == TFT_HJ28_9327 

//#else
	if( (LCD.TFT1_ID == TFT_HJ28)||(LCD.TFT1_ID == TFT_HJ30) )
	{
		TFT1_WD_COM(0x0210); TFT1_WD_DAT(Xstart);		// col  最大240
		TFT1_WD_COM(0x0211); TFT1_WD_DAT(Xend);			// col  最大240
		TFT1_WD_COM(0x0212); TFT1_WD_DAT(Ystart);		// row  最大400
		TFT1_WD_COM(0x0213); TFT1_WD_DAT(Yend);			// row  最大400
		
		TFT1_WD_COM(0x0200); TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0200, 0x0000); // GRAM horizontal Address
		TFT1_WD_COM(0x0201); TFT1_WD_DAT(0x0000);//    TFT1_WD_RE(0x0201, 0x0000); // GRAM Vertical Address	
		TFT1_WD_COM(0x0202); //Read/Write GRAM
	}
	else
	{		
		if( H_DISPLAY_MODE != DispDir.dir) 
		{ 	// 竖屏	
			TFT1_WD_COM(0x002A);		   //Set_column_address
			//   TFT1_WD_COM(comd1);
			TFT1_WD_DAT(Ystart >> 8);
			TFT1_WD_DAT(Ystart & 0xff);
			TFT1_WD_DAT(Yend >> 8);
			TFT1_WD_DAT(Yend & 0xff);
			
			TFT1_WD_COM(0x002B);		  //Set_page_address
			//   TFT1_WD_COM(comd2);
			TFT1_WD_DAT(Xstart >> 8);
			TFT1_WD_DAT(Xstart & 0xff);
			TFT1_WD_DAT(Xend >> 8);
			TFT1_WD_DAT(Xend & 0xff);
		}			
		else
		{ 	// 横屏	
			TFT1_WD_COM(0x002A);		   //Set_column_address
			//   TFT1_WD_COM(comd1);
			TFT1_WD_DAT(Xstart >> 8);
			TFT1_WD_DAT(Xstart & 0xff);
			TFT1_WD_DAT(Xend >> 8);
			TFT1_WD_DAT(Xend & 0xff);

			TFT1_WD_COM(0x002B);		  //Set_page_address
			//   TFT1_WD_COM(comd2);
			TFT1_WD_DAT(Ystart >> 8);
			TFT1_WD_DAT(Ystart & 0xff);
			TFT1_WD_DAT(Yend >> 8);
			TFT1_WD_DAT(Yend & 0xff);
		}
		
		TFT1_WD_COM(0x002C);
	}			
//#endif
}
   

/**********************************************************************
** 设置液晶GRAM的扫描方向 
** 当设置成不同的扫描模式时, page(即x) 跟 column(即y) 的值是会改变的

If set_address_mode (36h) B5 = 0:
    The column and page registers are reset to the Start Column (SC) and Start Page (SP), respectively. Pixel Data 1 is
  stored in frame memory at (SC, SP). The column register is then incremented and pixels are written to the frame
  memory until the column register equals the End Column (EC) value. The column register is then reset to SC and the
  page register is incremented. Pixels are written to the frame memory until the page register equals the End Page (EP)
  value or the host processor sends another command. If the number of pixels exceeds (EC – SC + 1) * (EP – SP + 1) the
  extra pixels are ignored.
If set_address_mode (36h) B5 = 1:
    The column and page registers are reset to the Start Column (SC) and Start Page (SP), respectively. Pixel Data 1 is
  stored in frame memory at (SC, SP). The page register is then incremented and pixels are written to the frame memory
  until the page register equals the End Page (EP) value. The page register is then reset to SP and the column register is
  incremented. Pixels are written to the frame memory until the column register equals the End column (EC) value or the
  host processor sends another command. If the number of pixels exceeds (EC – SC + 1) * (EP – SP + 1) the extra pixels
  are ignored.

Set_address_mode (36h)  带一个8位的参数	 This command defines read/write scanning direction of frame memory.
 Bit B7 – Page Address Order
‘0’ = Top to Bottom
‘1’ = Bottom to Top
 Bit B6 – Column Address Order
‘0’ = Left to Right
‘1’ = Right to Left
 Bit B5 – Page/Column Order
‘0’ = Normal Mode
‘1’ = Reverse Mode
 Bit B4 –Line Address Order
‘0’ = LCD Refresh Top to Bottom
‘1’ = LCD Refresh Bottom to Top
 Bit B3 – RGB/BGR Order
‘0’ = Pixels sent in RGB order
‘1’ = Pixels sent in BGR order
 Bit B2 –Display Data Latch Data Order
This bit is not applicable for this project, so it is set to ‘0’. (Not supported)
 Bit B1 – Horizontal Flip
‘0’ = Normal display
‘1’ = Flipped display
 Bit B0 – Vertical Flip
‘0’ = Normal display
‘1’ = Flipped display
***********************************************************************/

//void Lcd_GramScan( INT16U option )
//{	
//	switch(option)
//	{
//		case 1:
//		{/* 左上角->右下脚      横屏  反字体*/
//			TFT1_WD_COM(0x36); 
//			TFT1_WD_DAT(0xC8);  
//			 
//			TFT1_WD_COM(0X2A); 
//			TFT1_WD_DAT(0x00);	/* x start */	
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);  /* x end */	
//			TFT1_WD_DAT(0xEF);  //239
//
//			TFT1_WD_COM(0X2B); 
//			TFT1_WD_DAT(0x00);	/* y start */  
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x01);	/* y end */   
//			TFT1_WD_DAT(0x8F);	//399
//					
//		}break;
//		case 2:
//		{/* 左下角->右上角      竖屏  */	
//			TFT1_WD_COM(0x36); 
//			TFT1_WD_DAT(0x68);	
//
//			TFT1_WD_COM(0X2A); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x01);
//		  	TFT1_WD_DAT(0x8F);	
//
//			TFT1_WD_COM(0X2B); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0xEF);			
//		}break;
//		case 3:
//		{/* 右下角->左上角    横屏  */
//			TFT1_WD_COM(0x36); 
//			TFT1_WD_DAT(0x28);
//				
//			TFT1_WD_COM(0X2A); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x01);
//		  	TFT1_WD_DAT(0x8F);	
//
//			TFT1_WD_COM(0X2B); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0xEF);			
//		}break;
//		case 4:
//		{/* 左下角->右上角      横屏 */
//			TFT1_WD_COM(0x36); 
//			TFT1_WD_DAT(0x48);	
//
//			TFT1_WD_COM(0X2A); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0xEF);	
//
//			TFT1_WD_COM(0X2B); 
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0x00);
//			TFT1_WD_DAT(0xEF);			
//		}break;
//	}
//	
//	/* write gram start */
//	TFT1_WD_COM(0x2C);
//}


/**********************************************************************
**函数名:  TFT_INIT
**功能:    TFT初始化
***********************************************************************/
void TFTLCD_Init(void)
{

	Delay_ms(50);			//1000 31.5ms

	TFT1_ILI9327_INIT();	//IC初始化

	// 加送黑和延时就不花屏了
	DispColor1(aBLACK);		//显示黑色	
	Delay_ms(160);			//5000 160ms		

	TFT1_BL_ON(); 			//开背光
}

/**********************************************************************
**函数名:  TFT1_Display_NORFlash_Picture
**功能:    显示NORFLASH图片
**参数：   imagep    ：结构体里包含16位的行，列起始位置
           imageinfo ：图像的16位的高和宽，32位的字库首地址
***********************************************************************/

void TFT1_Display_NORFlash_Picture(_ImageConstStr imagep,_ImageInfo imageinfo)
{  // DMA传送一副图片时间32ms , 非DMA传送46ms
    INT32U  dat32,tmp,PictureAddr;
    INT16U	i,k;
    INT16U	col0,col1,row0,row1,high; 

    if( H_DISPLAY_MODE != DispDir.dir)                      // 竖屏
    {  
        col0 = imagep.YS + DispDir.dif; 					// 纵坐标开始位置
        col1 = imagep.YS + DispDir.dif + (imageinfo.Width);	// 纵坐标介绍位置
        row0 = imagep.XS;					                // 横坐标开始位置
        row1 = imagep.XS+(imageinfo.High);	                // 横坐标结束位置
        high = imageinfo.High;

        TFT1_SetWindows(row0,row1-1,col0,col1-1);           
    }
    else                                    // 横屏
    { 	
        col0 = imagep.XS; 					// 纵坐标开始位置
        col1 = imagep.XS+(imageinfo.High);	// 纵坐标结束位置
        row0 = imagep.YS;					// 横坐标开始位置
        row1 = imagep.YS+(imageinfo.Width);	// 横坐标结束位置
        high = imageinfo.High;	

        TFT1_SetWindows(col0,col1-1,row0,row1-1);
    }	
    //坐标设置
    //	TFT1_SetWindows(col0,col1-1,row0,row1-1);
    // 图片存储的起始地址
    PictureAddr = imageinfo.ImageAddr + Bank1_NOR2_ADDR; 	
    // 写入数据
    for(k = 0;k < (row1-row0);k++)
    {
        tmp =  k * high;//imageinfo.High;				//imageinfo.High;
        for(i = 0; i < (col1-col0);i++)
        { 
            if( (i  % 0x02) == 0x00  )
            { 
                dat32 = *(INT32U *) (PictureAddr+(tmp+i)*2);  
            }	
            TFT1_WD_DAT( (INT16U ) dat32); 	
            dat32 = dat32 >> 16;
        }			
    }
}

/**********************************************************************
**函数名:  TFT1_Display_NORFlash_Picture
**功能:    显示MCU里的图片
**参数：   imagep    ：结构体里包含16位的行，列起始位置

		   pic_num
***********************************************************************/
const	INT16U   MCUPicture_color[] = { aWHITE, mGRAY6, mGRAY5, mGRAY4, mGRAY3, mGRAY2, mGRAY1, aBLACK };

void TFT1_Display_MCUFlash_Picture(_ImageConstStr imagep,INT8U pic_num)
{  // DMA传送一副图片时间32ms , 非DMA传送46ms
	INT32U  tmp,cnt;//,PictureAddr;
	const INT8U *  flash_addr ;
	INT16U	i,k;
	INT16U	col0,col1,row0,row1,high; 
	INT8U	dat;
	INT32U	dat32,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;
//LED_WRN_ON();	

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		col0 = imagep.YS + DispDir.dif; 					// 纵坐标开始位置
		col1 = imagep.YS + DispDir.dif + COL2;	// 纵坐标介绍位置
		row0 = imagep.XS;					// 横坐标开始位置
		row1 = imagep.XS + ROW2;	// 横坐标结束位置
		high = COL2;
		
		TFT1_SetWindows(row0,row1-1,col0,col1-1);
	}
	else
	{ 	// 横屏
		col0 = imagep.XS; 					// 纵坐标开始位置
		col1 = imagep.XS + ROW2;	// 纵坐标介绍位置
		row0 = imagep.YS;					// 横坐标开始位置
		row1 = imagep.YS + COL2;	// 横坐标结束位置
		high = COL2;	
		
		TFT1_SetWindows(col0,col1-1,row0,row1-1);
	}	
	//坐标设置
//	TFT1_SetWindows(col0,col1-1,row0,row1-1);
	// 图片存储的起始地址
	flash_addr = &gImage_1_Information_8bit[pic_num-1][0] ; 	
	// 写入数据
	for(k = 0;k < (row1-row0);k++)
	{
		tmp =  k * high;//imageinfo.High;				//imageinfo.High;
		cnt =  (row0+k)*COL+col0;	   // 对应背景图位置
		for(i = 0; i < (col1-col0);i++)
		{ 
		   dat = *(flash_addr +(tmp+i));

		   if(dat > 0xF8)
		   { 	
		   		if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色}// 透明
//				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息
				else 
				{ 	// 取上副背景图信息
										
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (i  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 
				}													
		   }
		   else
		   { TFT1_WD_DAT(MCUPicture_color[dat/0x23]); }
		   
		   if( (i  & 0x01) == 0x01  )
		   { cnt++; }			   
//		   cnt++;
		}			
	}
	
//LED_WRN_OFF();
}

/**********************************************************************
**函数名: 	  void DispColor(INT16U color)
**功能:    	  显示颜色
**参数：   	  color 16位颜色，65535色
***********************************************************************/
//INT16U  time;
void DispColor1(INT16U color)
{
	INT16U i,j;
//	INT32U  tim;
//	if( H_DISPLAY_MODE != DispDir.dir) 
//	{ TFT1_SetWindows(0+DispDir.dif,DispDir.dif+ROW-1,0,COL-1);	}	   // 竖屏模式
//	else
//	{ TFT1_SetWindows(0,COL-1,0,ROW-1);	}							   // 横屏模式
	
//	tim = os_time.Systime_Count;
	
	{ TFT1_SetWindows(0,COL-1,0+DispDir.dif,DispDir.dif+ROW-1);	}							   // 横屏模式
	for(i = 0;i < ROW;i ++)
	{
	    for(j = 0;j < COL;j ++)
		{ TFT1_WD_DAT(color); }
	}
	
//	time = os_time.Systime_Count - tim;
	
//	{ TFT1_SetWindows(0,COL-1,0,ROW-1);	}							   // 横屏模式
//	for(i = 0;i < ROW;i ++)
//	{
//	    for(j = 0;j < COL;j ++)
//		{ TFT1_WD_DAT(aBLACK); }
//	}	
	
//	{ TFT1_SetWindows(0,COL-31,30,ROW-1);	}		
//	{ color = aRED; }
//	for(i = 30;i < ROW;i ++)
//	{
//		if(i == 100)
//		{ color = aGREEN; }		
//	  else if(i == 230)
//		{ color = aBLUE; }			
//		
//		for(j = 30;j < COL;j ++)
//		{ TFT1_WD_DAT(color); }
//	}	
}


/************************************************************************
* 名    称：void TFT_PutChar(INT16U row,INT16U row,INT8U c,INT8U chartype,INT16U charColor,INT16U bkColor，INT8U defBKCflag)
* 功    能：在指定座标显示一个自定义点阵的ascii字符，自定义前字体颜色以及背景
* 入口参数：row         行座标
*           col         列座标
*           C           字符 
*           chartype    字符点阵大小
*           charColor   字符的颜色
*           bkColor     字符背景颜色
* 出口参数：无
* 说    明：显示范围可根据点阵大小显示，但必须为标准ASIIC点阵表，点阵表存在MCU的Flash中
* 调用方法：TFT_PutChar(10,10,'c',CHAR0816,0x0000,0xffff,0);
************************************************************************/

INT16U TFT1_PutChar(
					INT16U row,					// 显示起始行
					INT16U col,					// 显示起始列
					INT8U c,					// 待显示 字符
					INT8U chartype,				// 字体参数
					INT16U charColor,			// 字符颜色
					INT16U bkColor				// 背景颜色
					)//,INT8U  bkSel)
{
	INT16U  i,z,j,col1,row1,row8,Xend,Yend;
	const INT8U *  flash_addr ;
	INT16U	tmp,tmp_char = 0;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;

//	col1 = PicCharStr[chartype].Col; 			// 字宽
	row1 = PicCharStr[chartype].Row;  			// 字高
	flash_addr = PicCharStr[chartype].P;	   	// 对应字库的地址

	
	// 空格之前的字符默认用空格
	if(c < ' '){ c = ' '; }
	// 字库默认从空格开始
	c -= ' ';
	//获得字符对应宽度，和对应偏移地址
	switch(chartype)
	{
	 	case  CHAR1324L:
		case  CHAR1324:	   	col1 = Char1324_Indexes[c].Col; 
							cnt  = Char1324_Indexes[c].DifAddr;
							break;
	 	case  CHAR2240:	   	col1 = Char2240_Indexes[c].Col; 
							cnt  = Char2240_Indexes[c].DifAddr;
							break;
	 	case  CHAR1016:	   	col1 = Char1016_Indexes[c].Col; 
							cnt  = Char1016_Indexes[c].DifAddr;
							break;
	 	case  CHAR1632:	   	col1 = Char1632_Indexes[c].Col; 
							cnt  = Char1632_Indexes[c].DifAddr;
							break;
	 	case  CHAR4080B:	col1 = Char4080B_Indexes[c].Col; 
							cnt  = Char4080B_Indexes[c].DifAddr;
							break;
	 	case  CHAR2040B:	col1 = Char2040B_Indexes[c].Col; 
							cnt  = Char2040B_Indexes[c].DifAddr;
							break;
	 	case  CHAR3264B:	col1 = Char3264B_Indexes[c].Col; 
							cnt  = Char3264B_Indexes[c].DifAddr;
							break;
		default	:			col1 = Char1324_Indexes[c].Col; 
							cnt  = Char1324_Indexes[c].DifAddr;
							break;
	}

	
	// 算出偏移地址
	row8 = (row1/8);			// 字高占的字节数
	j = col1*row8;				// 该字符总字节数
	flash_addr += (cnt*row8);	// 偏移地址*字高字节数 + 字库首地址
	
	for(i = 0; i < j;i++)
	{  // 读取8位数据放在dat中
		// 假设8*16字符，实现数据 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15	 =>	8,0,9,1,10,2,11,3,12,4,13,5,14,6,15,7
		// 实现逐行Z到逐列N的变换，并反转高低字节                                                              
		tmp = row8 + ((i%col1)*row8) - (i/col1+1);
		TFT_Data[tmp] = * (flash_addr+i);
	}  //读取字符显示数据		不懂

	//设定显示区域
	Xend = row+row1;	  Yend = col+col1;

//	if(Xend >= COL){Xend = COL;}
// 	if(Yend >= ROW){Yend = ROW;}
	if( H_DISPLAY_MODE != DispDir.dir) 
	{ 	
		if(Xend >= (ROW+DispDir.dif)){Xend = ROW+DispDir.dif;}
	 	if(Yend >= COL){Yend = COL;}
		TFT1_SetWindows(col,Yend-1,row,Xend-1);
	}
	else
	{	
		if(Xend >= COL){Xend = COL;}
	 	if(Yend >= ROW){Yend = ROW;}
		TFT1_SetWindows(row,Xend-1,col,Yend-1);
	}
	//坐标窗口全屏 	
//	TFT1_SetWindows(row,Xend-1,col,Yend-1);   	
	//兼容普通3100字符点阵 纵向倒序取模
//	for (z = 0; z < col1; z++)
	for (z = 0; z < Yend-col; z++)
	{

		tmp	= z*row8;
		cnt = (col+z )*COL+row;

		for (i = 0; i < row8; i++)
		{
			tmp_char = TFT_Data[tmp+i];
			for (j = 0;j < 8;j++)
			{
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ 
					TFT1_WD_DAT(charColor);   
				}// 字符颜色
//				else  if( (bkSel != BK_FLASHPIC)||((chartype == CHAR54112)&&(z >= 3)&&((i+1) < row8)
//					  &&((TFT_Data[(z-3)*row8+i+((j+3)/8)] >> ((j+3)%8) & 0x01) == 0x01)) )
//				{ TFT1_WD_DAT(bkColor);	 }		//自定义背景色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bkColor);	 }		//自定义背景色
				else 
				{ 	// 取上副背景图信息
//					TFT1_WD_DAT(*(INT16U *) (PictureAddr+cnt*2)); 					

					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					
//					if( (j  & 0x01) == 0x01  )
//					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 

				}		
				
//				if( (j  & 0x01) == 0x01  )
//				{ cnt++; }					
				
				cnt++;
			}
		}
	}
	return(col1);
}
/**********************************************************************
**函数名: 	  void TFT2_FillBackPic(INT16U col0,INT16U col1,INT16U row0,INT16U row1)
**功能:    	  填充背景色
***********************************************************************/

void TFT1_FillBackPic(INT16U col0,INT16U col1,INT16U row0,INT16U row1)
{
	INT16U i,j;

	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;   

	
	if((col0 >= col1)||(row0 >= row1)) { return; }

	if( H_DISPLAY_MODE != DispDir.dir) // 竖屏
	{ TFT1_SetWindows(row0,row1-1,col0,col1-1); } //坐标设置
	else
	{ TFT1_SetWindows(col0,col1-1,row0,row1-1); } //坐标设置

	for(i = row0;i < row1; i++)
	{

		cnt = i*COL;
		cnt = i*COL+col0;
//		for(j = col0;j < col1;j++)
		for(j = 0;j < (col1-col0);j++)
		{	

			if(bk_colour == BK_FLASHPIC)
			{ // 取上副背景图信息	
//				TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt+j)*2)); 
				if( (j % 2) == 0 )
				{ dat32 = *(INT32U *) (PictureAddr+(cnt+j)*2); }
				
				TFT1_WD_DAT( (INT16U)dat32 ); 
				dat32 = dat32>>16;
//				TFT1_WD_DAT( (INT16U)(dat32>>16) );
//				j++;
			}		
			else
			{ TFT1_WD_DAT(bk_colour_tab[bk_colour]); }
	
		}		
	}
}

/************************************************************************
* 名    称：INT32U GB2312BaseAddr(INT8U blk,INT8U bit,INT16U ChinaSize)
* 功    能：取GB2312字库的对应地址 
* input：   blk为汉字编码高字节，bit为汉字编码低字节，chinaSize 一个汉字占多少字节
* 出口参数：偏移地址
*************************************************************************/
INT32U GB2312BaseAddr(INT8U blk,INT8U bit,INT16U ChinaSize)
{

	if(bit >= 0xA1)
	{	// 字符区间
		if	((blk >= 0xA1)&&(blk <= 0xA9))
		{ return( ((blk-0xA1)*94+(bit-0xA1))*ChinaSize ); }
		// 汉字编码
		if((blk >= 0xB0)&&(blk <= 0xF7))
		{ return( ((blk-0xB0)*94+(bit-0xA1)+846)*ChinaSize ); }
	}	
	return(0);
}


/************************************************************************
* 名    称：INT16U TFT1_PutHanZi(INT16U row0,INT16U col0,INT8U *ptr,INT8U chartype,INT16U charColor,INT16U bkColor)
* 功    能：在指定座标显示一个GBK2312汉字，自定义前字体颜色以及背景
* 入口参数：row0         行座标
*           col0         列座标
*           ptr           字符 
*           chartype    字符点阵大小
*           charColor   字符的颜色
*           bkColor     字符背景颜色
* 出口参数：返回显示到最后的列数
* 说    明：显示范围可根据点阵大小显示，但必须为标准GBK2312字库
* 调用方法：TFT_PutChar(10,10,'c',CHAR0816,0x0000,0xffff,0);
************************************************************************/

INT16U TFT1_PutHanZi(
					INT16U row0,			   // 显示起始行
					INT16U col0,			   // 显示起始列
					INT8U *ptr,				   // 待显示 “汉字码”
					INT8U chartype,			   // 字体参数
					INT16U charColor,		   // 字符颜色
					INT16U bkColor			   // 背景颜色
					)//,INT8U  bkSel)
{	
	INT16U  i,z,j,tmp,tmp_char,col1,row1,row8,Xend,Yend;
	INT32U  norflash_addr,cnt,dat32 ;
	INT32U	PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;

	col1 		  = GB2312Str[chartype].High; 		// 字高
	row1          = GB2312Str[chartype].Width;  	// 字宽
	norflash_addr = GB2312Str[chartype].ImageAddr;	// 字库首地址
	row8 = row1/8;											   // 字高占的字节数
	j = col1*row8;											   // 一个字占用的字节数
	norflash_addr += GB2312BaseAddr(*ptr,*(ptr+1),j) ;		   // 得到字对应的首地址	
	norflash_addr += Bank1_NOR2_ADDR;						   // 加上NorFlash基本地址

	for(i = 0; i < j; i++)
	{  // 读取16位数据放在dat中
		// 假设8*16字符，实现数据 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15	 =>	8,0,9,1,10,2,11,3,12,4,13,5,14,6,15,7
		// 实现逐行Z到逐列N的变换，并反转高低字节   
//		tmp_char = *(INT16U *) (norflash_addr+i); 

//		tmp = row8 + ((i%col1)*row8) - (i/col1+1);			   // 得到要存放的位置
//		TFT_Data[tmp] = (INT8U)tmp_char;//tmp_char>>8;

//		i++; 
//		tmp = row8 + ((i%col1)*row8) - (i/col1+1);			   // 得到要存放的位置
//		TFT_Data[tmp] = tmp_char>>8;//(INT8U)tmp_char;
		
		// 一次取4个数据
		if(i % 4 == 0)
		{ dat32 = *(INT32U *) (norflash_addr+i); }
		
		tmp = row8 + ((i%col1)*row8) - (i/col1+1);			   // 得到要存放的位置
		TFT_Data[tmp] = (INT8U)dat32;//tmp_char>>8;	

		dat32 = dat32>>8;
	}  //读取字符显示数据

	//设定显示区域
	Xend = row0+row1;	  Yend = col0+col1;
//	if(Xend >= COL){Xend = COL;}
//	if(Yend >= ROW){Yend = ROW;}	
	if( H_DISPLAY_MODE != DispDir.dir) 
	{ 	// 竖屏
		if(Xend >= (ROW+DispDir.dif)){Xend = ROW+DispDir.dif;}
	 	if(Yend >= COL){Yend = COL;}
		
		TFT1_SetWindows(col0,Yend-1,row0,Xend-1);
	}
	else
	{	// 横屏
		if(Xend >= COL){Xend = COL;}
	 	if(Yend >= ROW){Yend = ROW;}
		TFT1_SetWindows(row0,Xend-1,col0,Yend-1);
	}		
	// 设定对应写入数据的区域
//	TFT1_SetWindows(row0,Xend-1,col0,Yend-1);				

	// 写数据到TFT屏 ，逐列写过去
	for (z = 0; z < (Yend-col0); z++)
	{
		tmp	= z*row8;				   // 对应第几列 * 字高
		cnt = (col0+z)*COL+row0;	   // 对应背景图位置
		// 写一列字高的数据
		for (i = 0; i < row8; i++)
		{
			//计算数据位置
			tmp_char = TFT_Data[tmp+i];
			// 写一字节的数据，有显示的填对应字体颜色，没有则填背景颜色
			for (j = 0;j < 8;j++)	   
			{
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ 
					TFT1_WD_DAT(charColor);  
				}	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bkColor);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息					
				{ 
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		// 取上副背景图信息
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }					

//				cnt++;
			}
		}
	}
	return(col1);
}
/************************************************************************
* 名    称：void TFT1_PutString_flash(INT16U row,INT16U col,INT8U *ptr,INT8U charnum,INT8U chartype,INT16U charColor,INT16U bkColor)
* 功    能：
* 入口参数：row         行座标
*           col         列座标
*           *ptr        字符串指针 
*			charnum		字符个数
*           chartype    字符点阵大小
*           charColor   字符的颜色
*           bkColor     字符背景颜色
* 出口参数：无
* 说    明：显示范围可根据点阵大小显示，但必须为标准ASIIC点阵表
* 调用方法：TFT_PutString(0,0,"I love u..",CHAR0816,0x0000,0xffff);
			TFT_PutString(0,0,"I love u..",CHAR0816,0x0000,0xffff);
*************************************************************************/

INT16U TFT1_PutString_flash(
					INT16U 	row,			//显示起始行
					INT16U 	col,			//显示起始列
					INT8U 	*ptr,			//字符指针									
					INT8U 	charnum,	    //显示字符数
					INT8U 	chartype,		//字符类型
					INT16U 	charColor,	//字符颜色
					INT16U 	bkColor		//背景颜色
//					INT8U  bkSel		//背景类型
					)		
{

	INT16U	cnt,col0,row0;
	col0 = col; row0 = row;
	for (cnt = 0;cnt < charnum; cnt++)
	{		
		if((*ptr) == NULL)	{ break; }	 	//结束符
		else if ((*ptr) < 0x80)			// 汉字第一个B0A1
		{  //英文字符,显示一个字符	
			col0 += TFT1_PutChar(row0, col0, *ptr, chartype, charColor, bkColor);//,bkSel);
			if(col0 >= ROW){ break; }	 
		}	  		
		else
		{	//中文汉字,显示一个汉字					
			col0 +=	TFT1_PutHanZi(row0,col0, ptr,chartype,charColor,bkColor);//,bkSel);
			if(col0 >= ROW){ break; }
			ptr++;
		}
		ptr++; 
	}
	return(col0);
}



/************************************************************************
** 功能描述：设置LCD显示页面和显示层
** 输入 :   LcdPage 显示的页面 , 低7位代表页面，高位代表该页面可否覆盖
************************************************************************/
void SetLcdLay(INT8U LcdPage)
{	
    if( LCD.f_LcdEnable == RUN_REFRESH )                                                                              
    { 
        LCD.f_LcdEnable = RUNIRQ_REFRESH;                           // 正在刷新过程中被打断		
    }		
    else
    { 
        LCD.f_LcdEnable = WAIT_REFRESH;                             // 等待刷新总标志
    }		
    LCD.f_Update = TRUE;					                        // 使能背景更新标志
    DispRoll.f_Rolling[0] = DispRoll.f_Rolling[1] = ROLL_MODE_OFF;  
    DispRoll.f_Rolling1   = DispRoll.f_Rolling2   = ROLL_MODE_OFF;				
    if( (LcdLay.LayCnt++) >= LCDLAYMAX-1 )                          // 设置的层和当前层不一致	
    { 
        LcdLay.LayCnt = 0; 
    }	
    LCD.Type = LcdPage&0x7f;				                        // 当前层显示页面
    LcdLay.Lay[LcdLay.LayCnt] = LcdPage;	                        // 保存当前层页面	
    LcdLay.LayTime[LcdLay.LayCnt] = os_time.Systime_Count;	        // 保存当前层页面	
}

/************************************************************************
* 画框 
************************************************************************/

void	Draw_Kuang(INT16U col0,INT16U col1,INT16U row0,INT16U row1,	INT16U charColor)//, INT8U	BackColor)	
{
 
	INT16U j;
//#if TFT_TYPE == TFT_HJ28_9327	
//	if( H_DISPLAY_MODE != DispDir.dir) 
//	{ 	// 竖屏
//	// 画左边竖线
//		TFT1_SetWindows(row0,row0,col0,col1);  //坐标设置
//		for(j = col0;j < col1;j ++)
//		{ TFT1_WD_DAT(charColor); }
//		// 画右边竖线
//		TFT1_SetWindows(row1,row1,col0,col1);  //坐标设置
//		for(j = col0;j < col1;j ++)
//		{ TFT1_WD_DAT(charColor); }
//		// 画下边横线
//		TFT1_SetWindows(row0,row1,col0,col0);  //坐标设置
//		for(j = row0;j <= row1;j ++)
//		{ TFT1_WD_DAT(charColor); }
//		// 画上边横线
//		TFT1_SetWindows(row0,row1,col1,col1);  //坐标设置
//		for(j = row0;j < row1;j ++)
//		{ TFT1_WD_DAT(charColor); }	
//	}
//	else
//#endif
	{
		// 画左边竖线
		TFT1_SetWindows(col0,col1,row0,row0);  //坐标设置
		for(j = col0;j < col1;j ++)
		{ TFT1_WD_DAT(charColor); }
		// 画右边竖线
		TFT1_SetWindows(col0,col1,row1,row1);  //坐标设置
		for(j = col0;j < col1;j ++)
		{ TFT1_WD_DAT(charColor); }
		// 画下边横线
		TFT1_SetWindows(col0,col0,row0,row1);  //坐标设置
		for(j = row0;j <= row1;j ++)
		{ TFT1_WD_DAT(charColor); }
		// 画上边横线
		TFT1_SetWindows(col1,col1,row0,row1);  //坐标设置
		for(j = row0;j < row1;j ++)
		{ TFT1_WD_DAT(charColor); }
	}
}
/************************************************************************
* 画线
************************************************************************/

void TFT1_Draw_Line(INT16U col0,INT16U col1,INT16U row0,INT16U row1,	INT16U charColor)//, INT8U	BackColor)	
{
 
    INT16U j,end;

    if(col0 == col1)	// 画横线
    { 
        TFT1_SetWindows(col0,col0,row0,row1); //坐标设置 
        j = row0;
        end = row1;
    }  
    else				// 画竖线
    { 
        TFT1_SetWindows(col0,col1-1,row0,row0);  //坐标设置
        j = col0;
        end = col1; 
    }  

    for(;j < end;j ++)
    { 
        TFT1_WD_DAT(charColor); 
    }

}

/************************************************************************
* 画虚线
************************************************************************/

void	TFT1_Draw_XLine(INT16U col0,INT16U col1,INT16U row0,INT16U row1,INT16U charColor)//, INT8U	BackColor)	
{
 	INT16U  flag = 0;
	INT16U j,end;
	INT32U	dat32,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR; 

    if(col0 == col1)	// 画横线
	{ 
		TFT1_SetWindows(col0,col0,row0,row1); //坐标设置 
		j = row0;
		end = row1;
	}  
	else				// 画竖线
	{ 
		TFT1_SetWindows(col0,col1-1,row0,row0);  //坐标设置
		j = col0;
		end = col1; 
	}  

	for(;j < end;j ++)
	{ 
		if(flag & 0x02)
	  	{	TFT1_WD_DAT(charColor); }
	  	else
	  	{	
			if(bk_colour == BK_FLASHPIC)
//			{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(j)*2)); }		// 取上副背景图信息	
			{// 取上副背景图信息	
				if( (j % 2) == 0)
				{ dat32 = *(INT32U *) (PictureAddr+(j)*2); }
				else
				{ 
					dat32 = *(INT32U *) (PictureAddr+(j-1)*2); 
					dat32 = dat32 >> 16;
				}
				
				TFT1_WD_DAT( (INT16U)dat32 );
			}
			else
			{ TFT1_WD_DAT(bk_colour_tab[bk_colour]); }	  
	  	}
	  	flag++;
	}
}

/************************************************************************
* 画框 
************************************************************************/

void	Clr_Kuang(INT16U col0,INT16U col1,INT16U row0,INT16U row1)//, INT8U	BackColor)	
{ 
	TFT1_FillBackPic(col0-1,col1+1,row0-1,row0+1);				// 后面填充背景
	TFT1_FillBackPic(col0-1,col1+1,row1-1,row1+1);				// 后面填充背景
	TFT1_FillBackPic(col0-1,col0+1,row0-1,row1+1);				// 后面填充背景
	TFT1_FillBackPic(col1-1,col1+1,row0-1,row1+1);				// 后面填充背景
}


/************************************************************************
* 载入背景颜色  
************************************************************************/
//void	Load_BK_Colour(INT8U	colour)
//{
//	if(colour > 16) { colour = 0; }
//
//	// TFT1显示
//	if(LCD.f_Update == TRUE)
//	{
//		// TFT1显示的背景颜色	
//		DispColor1(bk_colour_tab[colour]);
//	}
//}
/************************************************************************
* 载入背景图  
************************************************************************/
//INT16U  time2;
void Load_Picture(INT8U	PicNum1)
{
//	INT32U tim;
	if(PicNum1 > 16) 
    { 
        PicNum1 = 0; 
    }
	if(LCD.f_Update == TRUE)            // TFT1显示
	{
		if(bk_colour == BK_FLASHPIC )  // 背景为0 则载入图片
		{  	
			LCD.TFT1_PicNum	= PicNum1;				// TFT1显示的图片序号	
			TFT1_Display_NORFlash_Picture(FullImageConst[0],FullImageInfo1[LCD.TFT1_PicNum]);
		}
		else        // TFT1显示的背景颜色
		{  
			PicNum1 = bk_colour;
			if(PicNum1 > 16) 
            { 
                PicNum1 = 0; 
            }
				
			DispColor1(bk_colour_tab[PicNum1]);         //显示颜色
		}
	}
}

/************************************************************************
*  	8位库位转换成2位显示
************************************************************************/
void	Ku_Hex16ToASCII2(TASK_INFO XDATA *ptr1,INT8U ord)//(INT8U dat,INT8U ord)	  
{
	INT8U dat;

	dat = (*ptr1).sku;

	if( ((*ptr1).sku_ascii[0]) != NULL	)   	// 优先取4位的SKU
	{
		strTFT2[ord].run.CurSku[0] =  (*ptr1).sku_ascii[0]; 
		strTFT2[ord].run.CurSku[1] =  (*ptr1).sku_ascii[1];
		strTFT2[ord].run.CurSku[2] =  (*ptr1).sku_ascii[2]; 
		strTFT2[ord].run.CurSku[3] =  (*ptr1).sku_ascii[3];
	}
	else
	{
		if( dat == 0xff)    // 2位SKU不显示ff
		{  
			strTFT2[ord].run.CurSku[0] = ' ';
			strTFT2[ord].run.CurSku[1] = ' ';
		}
		else        	// 显示2位的SKU
		{
			strTFT2[ord].run.CurSku[0] = ((dat>>4)&0X0F) ; 
			strTFT2[ord].run.CurSku[1] = (dat&0X0F) ;

			if(strTFT2[ord].run.CurSku[0] < 0x0A) 
				{ strTFT2[ord].run.CurSku[0] += '0'; }
			else{ strTFT2[ord].run.CurSku[0] += ('A' - 10); }

			if( strTFT2[ord].run.CurSku[1] < 0x0A) 
				{ strTFT2[ord].run.CurSku[1] += '0'; }
			else{ strTFT2[ord].run.CurSku[1] += ('A' - 10); }
		}
	    strTFT2[ord].run.CurSku[2]	 = NULL;	//介绍
	}
}


/************************************************************************
* 	16位数量转换成5位显示，前0灭掉
************************************************************************/
void	Count_WordToASCII5(INT16U	Count,INT8U ord)
{

	if(Count > DISPLAY_MAX_NUM)
	{
		strTFT2[ord].run.CurCnt[0] = 'F'; strTFT2[ord].run.CurCnt[1] = 'U'; 
		strTFT2[ord].run.CurCnt[2] = 'L'; strTFT2[ord].run.CurCnt[3] = 'L';
		strTFT2[ord].run.CurCnt[4] = ' ';
	}
	else
	{
		strTFT2[ord].run.CurCnt[0] = strTFT2[ord].run.CurCnt[1] = strTFT2[ord].run.CurCnt[2] = strTFT2[ord].run.CurCnt[3] = ' ';
		strTFT2[ord].run.CurCnt[4] = (UINT8)( Count % 10) + '0';
		if(Count >= 10)  { strTFT2[ord].run.CurCnt[3] = (UINT8)((Count % 100) / 10)   + '0'; }
		if(Count >= 100) { strTFT2[ord].run.CurCnt[2] = (UINT8)((Count % 1000) / 100) + '0'; }
		if(Count >= 1000){ strTFT2[ord].run.CurCnt[1] = (UINT8)((Count % 10000)/ 1000)+ '0'; }
		if(Count >= 10000){ strTFT2[ord].run.CurCnt[0] = (UINT8)( Count / 10000)      + '0'; }
	}
}

/************************************************************************
* TFT1 单屏描述显示区域数据转变及更新 					
************************************************************************/
const	INT8U	Roll_Src_Flag[]={ROLL_MODE_SRC1,ROLL_MODE_SRC2,ROLL_MODE_SRC3};

#define		JIANGE_SRC	60*(24/8)

void	TFT1_DispSrcDataChgSave( INT8U task )//(void)
{
	INT8U	i,k,row8,dat;
	INT16U	j,col,row,save_cnt[3],num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16U	col_max	;

	// 只能是任务0和1
	if(task > MAX_TASK_MODE || task != ONLY_TASK_MODE){ return; }

	save_cnt[0] =  0;						// 存储的列数清0
	row8 = (24/8);
	// 清描述信息滚屏标志
	DispRoll.f_Rolling[0] &= ~(ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3);

	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col_max = COL; }			  	// 竖屏
	else
	{ col_max = ROW1_P-8; } 		// 横屏

	// 把显示数据存入滚屏缓存区中
	for(k = 0; k < 3;k++)
	{	// 最多3行

		if(k > 0){save_cnt[k] =  save_cnt[k-1] + JIANGE_SRC; }

		DispRoll.uni.str1.SrcInfo_Start[k] = save_cnt[k]/row8;	 // 存起始
//	for(save_cnt = i = 0;i < SRC_INFO_MAX;i++)
		for(i = 0;i < SRC_INFO_MAX;i++)
		{		   
		   dat = strTFT1[0].run.SrcInfo[k][i];
		   if( (NULL == dat)||(save_cnt[k] >= ROLL_SRCINFO_MAX) )
		   { break; } 
		   else if (dat < 0x80)
		   {	// ASCII码字符					
				if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
				dat -= ' ';							// 字库从‘ ’开始
	
				row = PicCharStr[CHAR1324].Row;  		// 行,等高
				col = Char1324_Indexes[dat].Col; 		// 列,不等宽			
				cnt = Char1324_Indexes[dat].DifAddr;	// 偏移地址
				flash_addr = PicCharStr[CHAR1324].P;	// 字库首地址
					
				row8 = row/8;
				num  = col*row8;
				cnt  *= row8;
				flash_addr +=  cnt;
	
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
	//				tmp = ((col - 1 - j%col)*(row8)) + (j/col);
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = * (flash_addr+j);
					DispRoll.uni.str1.SrcInfo[save_cnt[k]+tmp] = * (flash_addr+j);
				}  //读取字符显示数?	  
				save_cnt[k] += num; 
		   }	
		   else
		   {	// 中文字库
				col = GB2312Str[CHINA2424L].High; 		// 行,等高
				row = GB2312Str[CHINA2424L].Width;  		// 列,等高
				norflash_addr = GB2312Str[CHINA2424L].ImageAddr;
	
				row8 = row/8;
				num  = col*row8;
				norflash_addr += GB2312BaseAddr(dat,strTFT1[0].run.SrcInfo[k][i+1],num) ;	// 得到NorFlash地址	
				norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
				
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
					tmp_char = *(INT16U *) (norflash_addr+j); 
			
	//				tmp = ((col - 1 - j%col)*row8) + (j/col);

					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = (INT8U)tmp_char;//tmp_char>>8;
					DispRoll.uni.str1.SrcInfo[save_cnt[k]+tmp] = (INT8U)tmp_char;
			
					j++; 
	//				tmp = ((col - 1 - j%col)*row8) + (j/col);
					
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = tmp_char>>8;//(INT8U)tmp_char;
					DispRoll.uni.str1.SrcInfo[save_cnt[k]+tmp] = tmp_char>>8;//(INT8U)tmp_char;
				}  //读取字符显示数?			 
				i++;
				save_cnt[k] += num;
		   }
		}
	
		DispRoll.uni.str1.SrcInfo_End[k] = (save_cnt[k])/row8;		 // 存结束
		if( (DispRoll.uni.str1.SrcInfo_End[k]-DispRoll.uni.str1.SrcInfo_Start[k]) > (col_max-COL_S1*2) )
		{
			DispRoll.f_Rolling[0] |= Roll_Src_Flag[k];					// 置滚屏标志
			for(j = 0; j < JIANGE_SRC;j++)
			{ DispRoll.uni.str1.SrcInfo[save_cnt[k]+j] = 0x00; }
			DispRoll.uni.str1.SrcInfo_End[k] += JIANGE_SRC/row8;
			DispRoll.uni.str1.SrcInfo_Index[k] = DispRoll.uni.str1.SrcInfo_Start[k];
			DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;				//滚屏定时值赋值
		}
	}
}

/************************************************************************
* TFT1 信息描述显示区域数据转变及更新 
************************************************************************/

void	TFT1_DispInfoSrcDataChgSave(void)
{
	INT8U	i,k,row8,dat;
	INT16U	j,col,row,save_cnt[3],num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16U	col_max	;


	save_cnt[0] =  0;						// 存储的列数清0
	row8 = (24/8);
	// 清描述信息滚屏标志
	DispRoll.f_Rolling1 &= ~(ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3);


	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col_max = COL; }			  	// 竖屏
	else
	{ col_max = ROW; } 				// 横屏

	// 把显示数据存入滚屏缓存区中
	for(k = 0; k < 3;k++)
	{	// 最多3行

		if(k > 0){save_cnt[k] =  save_cnt[k-1] + JIANGE_SRC; }

		DispRoll.uni.str4.SrcInfo_Start[k] = save_cnt[k]/row8;	 // 存起始
//	for(save_cnt = i = 0;i < SRC_INFO_MAX;i++)
		for(i = 0;i < SRC_INFO_MAX;i++)
		{		   
		   dat = strTFT1[0].info.SrcInfo[k][i];
		   if( (NULL == dat)||(save_cnt[k] >= ROLL_SRCINFO_MAX) )
		   { break; } 
		   else if (dat < 0x80)
		   {	// ASCII码字符					
				if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
				dat -= ' ';							// 字库从‘ ’开始
	
				row = PicCharStr[CHAR1324].Row;  		// 行,等高
				col = Char1324_Indexes[dat].Col; 		// 列,不等宽			
				cnt = Char1324_Indexes[dat].DifAddr;	// 偏移地址
				flash_addr = PicCharStr[CHAR1324].P;	// 字库首地址
					
				row8 = row/8;
				num  = col*row8;
				cnt  *= row8;
				flash_addr +=  cnt;
	
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
	//				tmp = ((col - 1 - j%col)*(row8)) + (j/col);
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = * (flash_addr+j);
					DispRoll.uni.str4.SrcInfo[save_cnt[k]+tmp] = * (flash_addr+j);
				}  //读取字符显示数?	  
				save_cnt[k] += num; 
		   }	
		   else
		   {	// 中文字库
				col = GB2312Str[CHINA2424L].High; 		// 行,等高
				row = GB2312Str[CHINA2424L].Width;  		// 列,等高
				norflash_addr = GB2312Str[CHINA2424L].ImageAddr;
	
				row8 = row/8;
				num  = col*row8;
				norflash_addr += GB2312BaseAddr(dat,strTFT1[0].info.SrcInfo[k][i+1],num) ;	// 得到NorFlash地址	
				norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
				
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
					tmp_char = *(INT16U *) (norflash_addr+j); 
			
	//				tmp = ((col - 1 - j%col)*row8) + (j/col);

					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = (INT8U)tmp_char;//tmp_char>>8;
					DispRoll.uni.str4.SrcInfo[save_cnt[k]+tmp] = (INT8U)tmp_char;
			
					j++; 
	//				tmp = ((col - 1 - j%col)*row8) + (j/col);
					
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
	//				DispRoll.Roll_SrcInfo[save_cnt+tmp] = tmp_char>>8;//(INT8U)tmp_char;
					DispRoll.uni.str4.SrcInfo[save_cnt[k]+tmp] = tmp_char>>8;//(INT8U)tmp_char;
				}  //读取字符显示数?			 
				i++;
				save_cnt[k] += num;
		   }
		}
	
		DispRoll.uni.str4.SrcInfo_End[k] = (save_cnt[k])/row8;		 // 存结束
		if( (DispRoll.uni.str4.SrcInfo_End[k]-DispRoll.uni.str4.SrcInfo_Start[k]) > (col_max-COL_S1*2) )
		{
			DispRoll.f_Rolling1 |= Roll_Src_Flag[k];					// 置滚屏标志
			for(j = 0; j < JIANGE_SRC;j++)
			{ DispRoll.uni.str4.SrcInfo[save_cnt[k]+j] = 0x00; }
			DispRoll.uni.str4.SrcInfo_End[k] += JIANGE_SRC/row8;
			DispRoll.uni.str4.SrcInfo_Index[k] = DispRoll.uni.str4.SrcInfo_Start[k];
			DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;				//滚屏定时值赋值
		}
	}
}
/************************************************************************
* TFT1 双屏描述显示区域数据转变及更新 
************************************************************************/
#define		JIANGE_SRC2	40*(16/8)

void	TFT1_DispSrcDataChgSave2( INT8U task )//(void)
{
	INT8U	i,k,row8,dat;
	INT16U	j,col,row,save_cnt[3],num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16U	col_max	;

	// 只能是任务0和1
	if(task > MAX_TASK_MODE || task == ONLY_TASK_MODE){ return; }
	task--;

	save_cnt[0] =  0;						// 存储的列数清0
	row8 = 	 (16/8);
	// 清描述信息滚屏标志
	DispRoll.f_Rolling[task] &= ~(ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3);

	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col_max = COL; }			  	// 竖屏
	else
	{ col_max = ROW/2; } 		// 横屏

	// 把显示数据存入滚屏缓存区中
	for(k = 0; k < 3;k++)
	{	// 最多3行

		if(k > 0){save_cnt[k] =  save_cnt[k-1] + JIANGE_SRC2; }

		DispRoll.uni.str3[task].SrcInfo_Start[k] = save_cnt[k]/row8;	 // 存起始
//	for(save_cnt = i = 0;i < SRC_INFO_MAX;i++)
		for(i = 0;i < SRC_INFO_MAX;i++)
		{		   
		   dat = strTFT1[task].run.SrcInfo[k][i];
		   if( (NULL == dat)||(save_cnt[k] >= ROLL_SRCINFO_MAX/2) )
		   { break; } 
		   else if (dat < 0x80)
		   {	// ASCII码字符					
				if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
				dat -= ' ';							// 字库从‘ ’开始
	
				row = PicCharStr[CHAR1016].Row;  		//行,等高
				col = Char1016_Indexes[dat].Col; 		//列,不等宽			
				cnt = Char1016_Indexes[dat].DifAddr;	//偏移地址
				flash_addr = PicCharStr[CHAR1016].P;	// 字库首地址
					
				row8 = row/8;
				num  = col*row8;
				cnt  *= row8;
				flash_addr +=  cnt;
	
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中

					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str3[task].SrcInfo[save_cnt[k]+tmp] = * (flash_addr+j);
				}  //读取字符显示数?	  
				save_cnt[k] += num; 
		   }	
		   else
		   {	// 中文字库
				col = GB2312Str[CHINA1616].High; 		//行,等高
				row = GB2312Str[CHINA1616].Width;  		//列,等高
				norflash_addr = GB2312Str[CHINA1616].ImageAddr;
	
				row8 = row/8;
				num  = col*row8;
				norflash_addr += GB2312BaseAddr(dat,strTFT1[task].run.SrcInfo[k][i+1],num) ;	// 得到NorFlash地址	
				norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
				
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
					tmp_char = *(INT16U *) (norflash_addr+j); 
			
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str3[task].SrcInfo[save_cnt[k]+tmp] = (INT8U)tmp_char;
			
					j++; 					
					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str3[task].SrcInfo[save_cnt[k]+tmp] = tmp_char>>8;//(INT8U)tmp_char;
				}  //读取字符显示数?			 
				i++;
				save_cnt[k] += num;
		   }
		}
	
		DispRoll.uni.str3[task].SrcInfo_End[k] = (save_cnt[k])/row8;		 // 存结束
		if( (DispRoll.uni.str3[task].SrcInfo_End[k]-DispRoll.uni.str3[task].SrcInfo_Start[k]) > (col_max-COL_S1*2) )
		{
			DispRoll.f_Rolling[task] |= Roll_Src_Flag[k];					// 置滚屏标志
			for(j = 0; j < JIANGE_SRC2;j++)
			{ DispRoll.uni.str3[task].SrcInfo[save_cnt[k]+j] = 0x00; }
			DispRoll.uni.str3[task].SrcInfo_End[k] += JIANGE_SRC2/row8;
			DispRoll.uni.str3[task].SrcInfo_Index[k] = DispRoll.uni.str3[task].SrcInfo_Start[k];
			DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;				//滚屏定时值赋值
		}

	}
}
/**
**********************************************************************
* TFT1 回显显示区域数据转变及更新 
************************************************************************/
const	INT8U	Roll_Back_Flag[PAGE_MAX_BACK_NUM] ={ ROLL_MODE_NAME_BAKE1, ROLL_MODE_NAME_BAKE2, ROLL_MODE_NAME_BAKE3, ROLL_MODE_NAME_BAKE4, ROLL_MODE_NAME_BAKE5 };
#define		JIANGE_BACK	60*(24/8)

void	TFT1_DispBackDataChgSave(void)
{
	INT8U	i,k,row8,dat,ord;
	INT16U	j,col,row,save_cnt[ PAGE_MAX_BACK_NUM ],num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16U	col_max;

	// 初始化数值
	for(i = 0;i < PAGE_MAX_BACK_NUM;i++)
	{
		save_cnt[i] = 0;
		DispRoll.uni.str2.BackName_Start[i] =  0;
   	}
	
	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col_max = COL; }			  	// 竖屏
	else
	{ col_max = H_BACK_SKU_COL-5; } 		// 横屏	
	 
	row8 = (24/8);
	// 清滚屏标志
	DispRoll.f_Rolling2 &= ~(ROLL_MODE_NAME_BAKE1 | ROLL_MODE_NAME_BAKE2 | ROLL_MODE_NAME_BAKE3 | ROLL_MODE_NAME_BAKE4 | ROLL_MODE_NAME_BAKE5);

	ord = BackTask.index - 1;
	if(ord >= MAX_BACK_NUM){ ord = MAX_BACK_NUM-1; }
		
	if(ord >= PAGE_MAX_BACK_NUM)
		{ord = PAGE_MAX_BACK_NUM;}
	else{ord = 0;}

	for(k = 0; k < PAGE_MAX_BACK_NUM;k++,ord++)
	{
		// 序号大于最大备份数,或大于最大回显任务则退出
		if( (ord >= MAX_BACK_NUM)||(ord >= BackTask.num) )
		{ break; } 

		for(i = 0;i < NAME_MAX;i++)
		{		   
		   dat = BackTask.info[ord].srcode[i];
		   // 数据为结束符或者超过数组最大存储范围则退出
		   if( (NULL == dat)||(save_cnt[k] >= ROLL_BACKNAME_MAX) )
		   { break; } 
		   else if (dat < 0x80)
		   {	// ASCII码字符					
				if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
				dat -= ' ';							// 字库从‘ ’开始
	
				row = PicCharStr[CHAR1324].Row;  		//行,等高
				col = Char1324_Indexes[dat].Col; 		//列,不等宽			
				cnt = Char1324_Indexes[dat].DifAddr;	//偏移地址
	
				row8 = row/8;
				num  = col*row8;
				cnt  *= row8;
				flash_addr = PicCharStr[CHAR1324].P + cnt;
	
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中

//					tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str2.BackName[k][save_cnt[k]+tmp] = * (flash_addr+j);
				}  //读取字符显示数?	  
				save_cnt[k] += num; 
		   }	
		   else
		   {	// 回显的品名区
				col = GB2312Str[CHINA2424].High; 		//行,等高
				row = GB2312Str[CHINA2424].Width;  		//列,等高
				norflash_addr = GB2312Str[CHINA2424].ImageAddr;
	
				row8 = row/8;
				num  = col*row8;
				norflash_addr += GB2312BaseAddr(dat,BackTask.info[ord].srcode[i+1],num) ;	// 得到NorFlash地址	
				norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
				
				for(j = 0; j < num;j++)
				{  // 读取16位数据放在dat中
					tmp_char = *(INT16U *) (norflash_addr+j); 
			
 //					tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);			   // 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str2.BackName[k][save_cnt[k]+tmp] = (INT8U)tmp_char;
			
					j++; 
  //				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
					tmp = row8 + ((j%col)*row8) - (j/col+1);			   // 实现逐行Z到逐列N的变化，并高低字节换位
					DispRoll.uni.str2.BackName[k][save_cnt[k]+tmp] = tmp_char>>8;
				}  //读取字符显示数?			 
				i++;
				save_cnt[k] += num;
		   }
		}
	
		DispRoll.uni.str2.BackName_End[k] = (save_cnt[k])/row8;		 // 存结束
		if( (DispRoll.uni.str2.BackName_End[k]-DispRoll.uni.str2.BackName_Start[k]) > (col_max - COL_S1*2) )
		{			
			for(j = 0; j < JIANGE_BACK;j++)
			{ DispRoll.uni.str2.BackName[k][save_cnt[k]+j] = 0x00; }
			DispRoll.uni.str2.BackName_Index[k] = DispRoll.uni.str2.BackName_Start[k];
			DispRoll.uni.str2.BackName_End[k] += JIANGE_BACK/row8;
			DispRoll.f_Rolling2 |= Roll_Back_Flag[k];				// 置滚屏标志
			DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;				// 滚屏定时值赋值
		}

	}
}

/************************************************************************
* TFT1 品名显示区域数据转变及更新 
* 显示数据超过最大显示范围则滚屏
* 单屏显示/双屏均可用
************************************************************************/
#define	NAME_HIGH1 		(24/8)
#define	NAME_HIGH2 		(24/8) 
#define	JIANGE_NAME		(80*NAME_HIGH1)
#define	JIANGE_NAME2	(60*NAME_HIGH2)
void	TFT1_DispNameDataChgSave( INT8U task )//(void)
{
	INT8U	i,row8,dat;
	INT16U	j,col,row,save_cnt,num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16VU *index,*start,*end;
	INT8VU  *roll,*name;
	INT16U	roll_max;

	if( task > MAX_TASK_MODE ){ task = ONLY_TASK_MODE; }

	save_cnt = 0;							// 存储的列数
	if( task == ONLY_TASK_MODE )
	{	// 单屏显示
		index = &DispRoll.uni.str1.Name_Index;
		start = &DispRoll.uni.str1.Name_Start;
		end   = &DispRoll.uni.str1.Name_End;
		roll  = &DispRoll.uni.str1.Name[0];
		name  = &strTFT1[task].run.Name[0];
		row8 = 	NAME_HIGH1;
		roll_max = ROLL_NAME_MAX;
	}
	else
	{
		task --;
		index = &DispRoll.uni.str3[task].Name_Index;
		start = &DispRoll.uni.str3[task].Name_Start;
		end   = &DispRoll.uni.str3[task].Name_End;
		roll  = &DispRoll.uni.str3[task].Name[0];
		name  = &strTFT1[task].run.Name[0];
		row8 = 	NAME_HIGH2;		
		roll_max = ROLL_NAME_MAX/2;
	}


	*start = 0;		// 滚屏起始位置	
	DispRoll.f_Rolling[task] &= ~ROLL_MODE_NAME;	// 清品名区滚屏标志

	// 把显示数据存入滚屏缓存区中
	for(i = 0;i < NAME_MAX;i++)
	{
	   dat = *(name+i); //strTFT1[0].run.Name[i];			// 取任务1字符数据	   
	   // 如果字符为空或者大于最大数据空间，则退出
	   if( (NULL == dat)||(save_cnt > roll_max) )
	   { break; } 
	   else if (dat < 0x80)
	   {	// ASCII码字符					
			if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
			dat -= ' ';							// 字库从‘ ’开始

	   		if( DOUBLE_DISPLAY_MODE == DispDir.only)
	   		{ // 双屏
				row = PicCharStr[CHAR1324].Row;  		// 行,等高
				col = Char1324_Indexes[dat].Col; 		// 列,不等宽，字符对应的宽度		
				cnt = Char1324_Indexes[dat].DifAddr;	// 偏移地址	，字符对应的位置
				flash_addr =  PicCharStr[CHAR1324].P;	// 字库首地址
			}
	   		else
	   		{
				row = PicCharStr[CHAR1632].Row;  		// 行,等高
				col = Char1632_Indexes[dat].Col; 		// 列,不等宽，字符对应的宽度		
				cnt = Char1632_Indexes[dat].DifAddr;	// 偏移地址	，字符对应的位置
				flash_addr = PicCharStr[CHAR1632].P;	// 字库首地址
			}

			row8 = row/8;							// 字高除以8得到字高占的字节数
			num  = col*row8;						// 字符占有的数据字节数
			cnt  *= row8;							// 字符总的偏移字节数
			flash_addr +=  cnt;	  					// 得到字符的真实地址

			for(j = 0; j < num;j++)
			{  // 读取字符的16位数据放在dat中
//				tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = * (flash_addr+j);
				*(roll+save_cnt+tmp) = * (flash_addr+j);
			}  //读取字符显示数?	  
			save_cnt += num; 
	   }	
	   else
	   {
	   		if( DOUBLE_DISPLAY_MODE == DispDir.only)
	   		{	// 双屏的品名区
				col = GB2312Str[CHINA2424].High; 		//行,等高
				row = GB2312Str[CHINA2424].Width;  		//列,等高
				norflash_addr = GB2312Str[CHINA2424].ImageAddr;		// 字库的首地址
			}
	   		else
	   		{
				col = GB2312Str[CHINA3232].High; 		//行,等高
				row = GB2312Str[CHINA3232].Width;  		//列,等高
				norflash_addr = GB2312Str[CHINA3232].ImageAddr;		// 字库的首地址
			}
			row8 = row/8;
			num  = col*row8;
			norflash_addr += GB2312BaseAddr( dat,*(name+i+1),num) ;//GB2312BaseAddr(strTFT1[0].run.Name[i],strTFT1[0].run.Name[i+1],num) ;	// 得到NorFlash地址	
			norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
			
			for(j = 0; j < num;j++)
			{  // 读取16位数据放在dat中
				tmp_char = *(INT16U *) (norflash_addr+j); 
		
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			   // 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = (INT8U)tmp_char;
				*(roll+save_cnt+tmp) = 	(INT8U)tmp_char;
				j++; 
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			  // 实现逐行Z到逐列N的变化
//				DispRoll.uni.str1.Name[save_cnt+tmp] = tmp_char>>8;
				*(roll+save_cnt+tmp) = (tmp_char>>8);
			}  // 读取字符显示数		 
			i++;
			save_cnt += num;
	   }
	}

	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col = COL; }			  // 竖屏
	else
	{ col = ROW; } 			  // 横屏

	// 根据单双屏确认间隔
	if( DOUBLE_DISPLAY_MODE == DispDir.only)
	{ 
		row = JIANGE_NAME2; 
		if( H_DISPLAY_MODE == DispDir.dir)
		{ col /= 2; }				 // 横双屏对半
	}
	else
	{ row = JIANGE_NAME; }

	// 判断显示的列数是否超过显示范围
	if(save_cnt > (col-COL_S1*2)*row8)
	{																   
		DispRoll.f_Rolling[task] |= ROLL_MODE_NAME;						    // 置滚屏标志
		// 存入头尾间隔的空白
		for(j = 0; j < row;j++)
//		{ DispRoll.uni.str1.Name[save_cnt+j] = 0x00; }
		{ *(roll+save_cnt+j) = 0x00; }

		*end = (save_cnt+row)/row8;				// 滚屏数据结束位置
		*index = *start;						// 取数据索引在开始位置
		DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;						// 滚屏定时值赋值
	}
}


/************************************************************************
* TFT1 信息主标题显示区域数据转变及更新 
* 显示数据超过最大显示范围则滚屏
************************************************************************/

#define	JIANGE_MNAME		(70*(32/8))

void	TFT1_DispInfoMNameDataChgSave(void)
{
	INT8U	i,row8,dat;
	INT16U	j,col,row,save_cnt,num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16VU *index,*start,*end;
	INT8VU  *roll,*name;
	INT16U	roll_max;


	save_cnt = 0;							// 存储的列数
	// 单屏显示
	index = &DispRoll.uni.str4.MName_Index;
	start = &DispRoll.uni.str4.MName_Start;
	end   = &DispRoll.uni.str4.MName_End;
	roll  = &DispRoll.uni.str4.MName[0];
	name  = &strTFT1[0].info.NameM[0];
	row8 = 	(32/8);
	roll_max = ROLL_MNAME_MAX;



	*start = 0;		// 滚屏起始位置	
	DispRoll.f_Rolling1 &= ~ROLL_MODE_NAME;	// 清品名区滚屏标志

	// 把显示数据存入滚屏缓存区中
	for(i = 0;i < NAME_MAX;i++)
	{
	   dat = *(name+i); //strTFT1[0].run.Name[i];			// 取任务1字符数据	   
	   // 如果字符为空或者大于最大数据空间，则退出
	   if( (NULL == dat)||(save_cnt > roll_max) )
	   { break; } 
	   else if (dat < 0x80)
	   {	// ASCII码字符					
			if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
			dat -= ' ';							// 字库从‘ ’开始


			row = PicCharStr[CHAR1632].Row;  		// 行,等高
			col = Char1632_Indexes[dat].Col; 		// 列,不等宽，字符对应的宽度		
			cnt = Char1632_Indexes[dat].DifAddr;	// 偏移地址	，字符对应的位置
			flash_addr = PicCharStr[CHAR1632].P;	// 字库首地址
			

			row8 = row/8;							// 字高除以8得到字高占的字节数
			num  = col*row8;						// 字符占有的数据字节数
			cnt  *= row8;							// 字符总的偏移字节数
			flash_addr +=  cnt;	  					// 得到字符的真实地址

			for(j = 0; j < num;j++)
			{  // 读取字符的16位数据放在dat中
//				tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = * (flash_addr+j);
				*(roll+save_cnt+tmp) = * (flash_addr+j);
			}  //读取字符显示数?	  
			save_cnt += num; 
	   }	
	   else
	   {

			col = GB2312Str[CHINA3232].High; 		//行,等高
			row = GB2312Str[CHINA3232].Width;  		//列,等高
			norflash_addr = GB2312Str[CHINA3232].ImageAddr;		// 字库的首地址
			
			row8 = row/8;
			num  = col*row8;
			norflash_addr += GB2312BaseAddr( dat,*(name+i+1),num) ;//GB2312BaseAddr(strTFT1[0].run.Name[i],strTFT1[0].run.Name[i+1],num) ;	// 得到NorFlash地址	
			norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
			
			for(j = 0; j < num;j++)
			{  // 读取16位数据放在dat中
				tmp_char = *(INT16U *) (norflash_addr+j); 
		
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			   // 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = (INT8U)tmp_char;
				*(roll+save_cnt+tmp) = 	(INT8U)tmp_char;
				j++; 
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			  // 实现逐行Z到逐列N的变化
//				DispRoll.uni.str1.Name[save_cnt+tmp] = tmp_char>>8;
				*(roll+save_cnt+tmp) = (tmp_char>>8);
			}  // 读取字符显示数		 
			i++;
			save_cnt += num;
	   }
	}

	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col = COL; }			  // 竖屏
	else
	{ 
		col = ROW; 
		if( strTFT2[0].info.picture != 0)
		{ col -= H_INFO_COL; }
	} 			  // 横屏

	// 根据单双屏确认间隔
	row = JIANGE_MNAME; 

	// 判断显示的列数是否超过显示范围
	if(save_cnt > (col-COL_S1*2)*row8)
	{																   
		DispRoll.f_Rolling1 |= ROLL_MODE_NAME;						    // 置滚屏标志
		// 存入头尾间隔的空白
		for(j = 0; j < row;j++)
//		{ DispRoll.uni.str1.Name[save_cnt+j] = 0x00; }
		{ *(roll+save_cnt+j) = 0x00; }

		*end = (save_cnt+row)/row8;				// 滚屏数据结束位置
		*index = *start;						// 取数据索引在开始位置
		DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;						// 滚屏定时值赋值
	}
}

/************************************************************************
* TFT1 信息主标题显示区域数据转变及更新 
* 显示数据超过最大显示范围则滚屏
************************************************************************/

#define	JIANGE_SNAME		(55*(24/8))

void	TFT1_DispInfoSNameDataChgSave(void)
{
	INT8U	i,row8,dat;
	INT16U	j,col,row,save_cnt,num,tmp_char,tmp;
	INT32U  cnt,norflash_addr;
	const INT8U *  flash_addr ;
	INT16VU *index,*start,*end;
	INT8VU  *roll,*name;
	INT16U	roll_max;


	save_cnt = 0;							// 存储的列数
	// 单屏显示
	index = &DispRoll.uni.str4.SName_Index;
	start = &DispRoll.uni.str4.SName_Start;
	end   = &DispRoll.uni.str4.SName_End;
	roll  = &DispRoll.uni.str4.SName[0];
	name  = &strTFT1[0].info.NameS[0];
	row8 = 	(24/8);
	roll_max = ROLL_SNAME_MAX;



	*start = 0;		// 滚屏起始位置	
	DispRoll.f_Rolling1 &= ~ROLL_MODE_SNAME;	// 清品名区滚屏标志

	// 把显示数据存入滚屏缓存区中
	for(i = 0;i < NAME_MAX;i++)
	{
	   dat = *(name+i); //strTFT1[0].run.Name[i];			// 取任务1字符数据	   
	   // 如果字符为空或者大于最大数据空间，则退出
	   if( (NULL == dat)||(save_cnt > roll_max) )
	   { break; } 
	   else if (dat < 0x80)
	   {	// ASCII码字符					
			if(dat < ' '){ dat = ' '; }	 		// 不显示‘ ’之前的数据
			dat -= ' ';							// 字库从‘ ’开始


			row = PicCharStr[CHAR1324].Row;  		// 行,等高
			col = Char1324_Indexes[dat].Col; 		// 列,不等宽，字符对应的宽度		
			cnt = Char1324_Indexes[dat].DifAddr;	// 偏移地址	，字符对应的位置
			flash_addr = PicCharStr[CHAR1324].P;	// 字库首地址
			

			row8 = row/8;							// 字高除以8得到字高占的字节数
			num  = col*row8;						// 字符占有的数据字节数
			cnt  *= row8;							// 字符总的偏移字节数
			flash_addr +=  cnt;	  					// 得到字符的真实地址

			for(j = 0; j < num;j++)
			{  // 读取字符的16位数据放在dat中
//				tmp = ((j%col)*(row8)) + (j/col);				// 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);		// 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = * (flash_addr+j);
				*(roll+save_cnt+tmp) = * (flash_addr+j);
			}  //读取字符显示数?	  
			save_cnt += num; 
	   }	
	   else
	   {   // 信息屏的副标题
			col = GB2312Str[CHINA2424].High; 		//行,等高
			row = GB2312Str[CHINA2424].Width;  		//列,等高
			norflash_addr = GB2312Str[CHINA2424].ImageAddr;		// 字库的首地址
			
			row8 = row/8;
			num  = col*row8;
			norflash_addr += GB2312BaseAddr( dat,*(name+i+1),num) ;//GB2312BaseAddr(strTFT1[0].run.Name[i],strTFT1[0].run.Name[i+1],num) ;	// 得到NorFlash地址	
			norflash_addr += Bank1_NOR2_ADDR;						   							// 加上基本地址	  
			
			for(j = 0; j < num;j++)
			{  // 读取16位数据放在dat中
				tmp_char = *(INT16U *) (norflash_addr+j); 
		
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			   // 实现逐行Z到逐列N的变化，并高低字节换位
//				DispRoll.uni.str1.Name[save_cnt+tmp] = (INT8U)tmp_char;
				*(roll+save_cnt+tmp) = 	(INT8U)tmp_char;
				j++; 
//				tmp = ((j%col)*(row8)) + (j/col);					   // 实现逐行Z到逐列N的变化
				tmp = row8 + ((j%col)*row8) - (j/col+1);			  // 实现逐行Z到逐列N的变化
//				DispRoll.uni.str1.Name[save_cnt+tmp] = tmp_char>>8;
				*(roll+save_cnt+tmp) = (tmp_char>>8);
			}  // 读取字符显示数		 
			i++;
			save_cnt += num;
	   }
	}

	// 根据横竖屏确认最大宽度
	if( H_DISPLAY_MODE != DispDir.dir)
	{ col = COL; }			  // 竖屏
	else
	{ 
		col = ROW; 
		if( strTFT2[0].info.picture != 0)
		{ col -= H_INFO_COL; }
	} 			  // 横屏

	// 根据单双屏确认间隔
	row = JIANGE_SNAME; 

	// 判断显示的列数是否超过显示范围
	if(save_cnt > (col-COL_S1*2)*row8)
	{																   
		DispRoll.f_Rolling1 |= ROLL_MODE_SNAME;						    // 置滚屏标志
		// 存入头尾间隔的空白
		for(j = 0; j < row;j++)
//		{ DispRoll.uni.str1.Name[save_cnt+j] = 0x00; }
		{ *(roll+save_cnt+j) = 0x00; }

		*end = (save_cnt+row)/row8;				// 滚屏数据结束位置
		*index = *start;						// 取数据索引在开始位置
		DispRoll.RollDelayCnt 	 = DISP_ROLL_SPEED;						// 滚屏定时值赋值
	}
}

/************************************************************************
* 名    称：品名区滚屏
* 功    能：
* 调用方法：
************************************************************************/
const INT16U NameRoll_SD[] = { SS1_RUN_NAME1_ROW,	SS2_RUN_NAME1_ROW };
void TFT1_NameRoll(
					INT8U  task,		// 对应任务
					INT16U charColor	// 字体颜色
				  )//,INT8U  bkSel)		//背景类型
{	
	INT16U  i,z,j,tmp,tmp_char;
    INT16U	col0,col1,row0,row1;
	INT8U 	row8;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;
	INT16VU  *index,start,end;
	INT8VU	*ptr;	//指针

	// 只能是任务0和1
	if(task > MAX_TASK_MODE){ task = ONLY_TASK_MODE; }

	// 单双屏存储区不一致
	if(task == ONLY_TASK_MODE)
	{  // 单屏
		index = &DispRoll.uni.str1.Name_Index;
		start =	DispRoll.uni.str1.Name_Start;
		end   = DispRoll.uni.str1.Name_End;
		ptr   = &DispRoll.uni.str1.Name[0];
	}
	else
	{
		task--;
		index = &DispRoll.uni.str3[task].Name_Index;
		start =	DispRoll.uni.str3[task].Name_Start;
		end   = DispRoll.uni.str3[task].Name_End;
		ptr   = &DispRoll.uni.str3[task].Name[0];
	}


	// 设定滚屏的显示区域
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

		if( DOUBLE_DISPLAY_MODE == DispDir.only)
		{ // 双屏
			col0 = 	COL_S1;
			col1 = 	COL - COL_S1;			
			row0 = 	NameRoll_SD[task];
			row1 = 	row0+(NAME_HIGH2*8)-1;
			row8 = NAME_HIGH2;
		}
		else
		{  // 单屏
			col0 = 	COL_S1;
			col1 = 	COL - COL_S1;
			row0 = 	S_RUN_NAME1_ROW;
			row1 = 	S_RUN_NAME1_ROW+(NAME_HIGH1*8)-1;
			row8 =  NAME_HIGH1;		
		}
		TFT1_SetWindows(col0,col1,row0,row1);			// 设置窗口区域
	}
	else
	{	// 横屏
		if( DOUBLE_DISPLAY_MODE == DispDir.only)
		{  // 双屏
			col0 = 	(ROW/2)*task    + COL_S1;
			col1 = 	(ROW/2)*(task+1)- COL_S1;
			row0 = 	H_RUN_NAME_ROW2;
			row1 = 	H_RUN_NAME_ROW2+(NAME_HIGH2*8)-1;
			row8 = NAME_HIGH2;	  
		}
		else
		{
			col0 = 	100    ;
			col1 = 	ROW - COL_S1;
			row0 = 	H_RUN_NAME_ROW;
			row1 = 	H_RUN_NAME_ROW+(NAME_HIGH1*8)-1;
			row8 = 	NAME_HIGH1;
		}
		TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
	}
//	TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域

	
	// 滚屏索引+1 ，如果到达最后，则循环到头
	(*index) ++;
	if( (*index) >= end )
	{ (*index) = start; }

//	for (z = col1-col0 ; z > 0; z--)			// 从左向右移动
	for (z = 0; z < col1-col0; z++)			   // 从右向左移动
	{

		tmp	= ((*index)+z);		// 取到对应列
		// 如果超过最大位置则回到头位置
		if( tmp >= end )
		{ tmp = tmp - end; }
		tmp = tmp * row8;							// 一列的字节数（字高）
		cnt = (col0+z)*COL+row0;//(ROW-z)*COL+COL-123-40;			// 对应位置的背景图

		for (i = 0; i < row8; i++)
		{
			//计算数据位置 ，并取出数据
			tmp_char = *(ptr+tmp+i);//DispRoll.uni.str1.Name[tmp+i];

			for (j = 0;j < 8;j++)
			{	// 根据字节数据对每个点颜色进行写入
//				if ( ((tmp_char << j) & 0x80) == 0x80 )
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ TFT1_WD_DAT(charColor);   }	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息				
				{ // 取上副背景图信息

					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }						
							
//				cnt++;
			}
		}
	}
}


/************************************************************************
* 名    称：描述区滚屏
* 功    能：
* 调用方法：
************************************************************************/
const INT16U InfoNameRoll_SD[] = { SS1_RUN_NAME1_ROW,	SS2_RUN_NAME1_ROW };

void TFT1_InfoNameRoll(
					INT8U  task,		// 对应任务
					INT16U charColor	// 字体颜色
				  )//,INT8U  bkSel)		//背景类型
{	
	INT16U  i,z,j,tmp,tmp_char;
    INT16U	col0,col1,row0,row1;
	INT8U 	row8;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;
	INT16VU  *index,start,end;
	INT8VU	*ptr;	//指针

	// 只能是任务0和1
	if(task > MAX_TASK_MODE){ task = ONLY_TASK_MODE; }

	// 单双屏存储区不一致
	if(task == ONLY_TASK_MODE)
	{  // 主标题
		index = &DispRoll.uni.str4.MName_Index;
		start =	 DispRoll.uni.str4.MName_Start;
		end   =  DispRoll.uni.str4.MName_End;
		ptr   = &DispRoll.uni.str4.MName[0];
		row8 = (32/8);	

		if( H_DISPLAY_MODE != DispDir.dir) 
		{  // 竖屏
			col0 = 	COL_S1;
			col1 = 	COL ;
			row0 = 	S_INFO_NAMEM_ROW;
		}
		else
		{  // 横屏
			col0 = 	COL_S1;			//无用
			if( strTFT2[0].info.picture != 0)
			{ col0 = H_INFO_COL; }
 			
			col1 = 	100 ;
			row0 = 	H_INFO_NAMEM_ROW;
		}
		col1 -= COL_S1;
		row1 = 	row0+32-1;
	}
	else
	{  // 付标题
		index = &DispRoll.uni.str4.SName_Index;
		start =	 DispRoll.uni.str4.SName_Start;
		end   =  DispRoll.uni.str4.SName_End;
		ptr   = &DispRoll.uni.str4.SName[0];
		row8 = (24/8);	
		if( H_DISPLAY_MODE != DispDir.dir) 
		{  // 竖屏
			col0 = 	COL_S1;
			col1 = 	COL ;
			row0 = 	S_INFO_NAMES_ROW;
		}
		else
		{  // 横屏
			col0 = 	COL_S1;
			if( strTFT2[0].info.picture != 0)
			{ col0 = H_INFO_COL; }
			col1 = 	ROW ;
			row0 = 	H_INFO_NAMES_ROW;
		}
		col1 -= COL_S1;
		row1 = 	row0+24-1;
	}

	if( H_DISPLAY_MODE != DispDir.dir ) 
	{ TFT1_SetWindows(col0,col1,row0,row1); }
	else
	{ TFT1_SetWindows(row0,row1,col0,col1); }
//	TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域

	
	// 滚屏索引+1 ，如果到达最后，则循环到头
	(*index) ++;
	if( (*index) >= end )
	{ (*index) = start; }

//	for (z = col1-col0 ; z > 0; z--)			// 从左向右移动
	for (z = 0; z < col1-col0; z++)			   // 从右向左移动
	{

		tmp	= ((*index)+z);		// 取到对应列
		// 如果超过最大位置则回到头位置
		if( tmp >= end )
		{ tmp = tmp - end; }
		tmp = tmp * row8;							// 一列的字节数（字高）
		cnt = (col0+z)*COL+row0;//(ROW-z)*COL+COL-123-40;			// 对应位置的背景图

		for (i = 0; i < row8; i++)
		{
			//计算数据位置 ，并取出数据
			tmp_char = *(ptr+tmp+i);//DispRoll.uni.str1.Name[tmp+i];

			for (j = 0;j < 8;j++)
			{	// 根据字节数据对每个点颜色进行写入
//				if ( ((tmp_char << j) & 0x80) == 0x80 )
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ TFT1_WD_DAT(charColor);   }	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息
				{ // 取上副背景图信息
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }	
//				cnt++;	
			}
		}
	}
}

/************************************************************************
* 名    称：描述区滚屏，
* 功    能：
* 调用方法：
************************************************************************/
//              描述信息行1	 2	 3	回显行1  2  3  4
//const	INT8U	dagaha[]={75,43,11,      145,96,52,10};

const	INT16U	hd_SrcRollRow[]={ H_RUN_SRC1_ROW, H_RUN_SRC2_ROW ,H_RUN_SRC3_ROW ,H_RUN_SRC1_ROW, H_RUN_SRC2_ROW ,H_RUN_SRC3_ROW };		    // 横单屏
const	INT16U	hs_SrcRollRow[]={ H_RUN_SRC1_ROW2,H_RUN_SRC2_ROW2,H_RUN_SRC3_ROW2,H_RUN_SRC1_ROW2,H_RUN_SRC2_ROW2,H_RUN_SRC3_ROW2 };		// 横双屏
const	INT16U	sd_SrcRollRow[]={   S_RUN_SRC1_ROW,  S_RUN_SRC2_ROW,  S_RUN_SRC3_ROW,  S_RUN_SRC1_ROW,  S_RUN_SRC2_ROW,  S_RUN_SRC3_ROW };	// 竖单屏
const	INT16U	ss_SrcRollRow[]={ SS1_RUN_SRC1_ROW,SS1_RUN_SRC2_ROW,SS1_RUN_SRC3_ROW,SS2_RUN_SRC1_ROW,SS2_RUN_SRC2_ROW,SS2_RUN_SRC3_ROW };	// 竖双屏

void TFT1_SrcRoll(INT8U area,INT16U charColor)//,INT8U  bkSel)		//背景类型
{	
//	INT8U	row = dagaha[area];
	INT16U  i,z,j,tmp,tmp_char;//,row8,place;
	INT16U	index,start,end;
	INT8VU	*ptr;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;	  // 取图片背景的首地址
	INT16U	col0,col1,row0,row1;
	INT8U	row8;
	
	i = area;
	if(i > 5){ i = 0; }

	if( DOUBLE_DISPLAY_MODE == DispDir.only)
	{  // 双屏描述区滚屏

		index = DispRoll.uni.str3[i/3].SrcInfo_Index[i%3];
		end   = DispRoll.uni.str3[i/3].SrcInfo_End[i%3];
		start = DispRoll.uni.str3[i/3].SrcInfo_Start[i%3];
		
		index += 1;
		if(index >= end)
		{ index = start; }
		
		DispRoll.uni.str3[i/3].SrcInfo_Index[i%3] = index;
		ptr   = &DispRoll.uni.str3[i/3].SrcInfo[0];

	}
	else
	{   // 单屏描述区滚屏
		index = DispRoll.uni.str1.SrcInfo_Index[i%3];
		end   = DispRoll.uni.str1.SrcInfo_End[i%3];
		start = DispRoll.uni.str1.SrcInfo_Start[i%3];
		
		index += 1;
		if(index >= end)
		{ index = start; }
		
		DispRoll.uni.str1.SrcInfo_Index[i%3] = index;
		ptr   = &DispRoll.uni.str1.SrcInfo[0];
	}


	//设定显示区域
	// 设定滚屏的显示区域
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

		if( DOUBLE_DISPLAY_MODE == DispDir.only)
		{ // 双屏
			col0 = 	COL_S1;
			col1 = 	COL - COL_S1;			
			row0 = 	ss_SrcRollRow[i];
			row1 = 	row0+16-1;
			row8 = (16/8);
		}
		else
		{  // 单屏
			col0 = 	COL_S1;
			col1 = 	COL - COL_S1;
			row0 = 	sd_SrcRollRow[i];
			row1 = 	row0+24-1;
			row8 = (24/8);					
		}
		TFT1_SetWindows(col0,col1,row0,row1);			// 设置窗口区域
	}
	else
	{	// 横屏
		if( DOUBLE_DISPLAY_MODE == DispDir.only)
		{  // 双屏
			col0 = 	(ROW/2)*(i/3)    + COL_S1;
			col1 = 	(ROW/2)*(i/3+1)- COL_S1;
			row0 = 	hs_SrcRollRow[i];;
			row1 = 	row0+16-1;
			row8 = (16/8);	  
		}
		else
		{
			col0 = 	COL_S1;								//无用
			col1 = 	ROW1_P - 8 - COL_S1;
			row0 = 	hd_SrcRollRow[i];;
			row1 = 	row0+24-1;
			row8 = (24/8);
		}
		TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
	}
//	TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
//	TFT1_SetWindows(COL-row-24,COL-row-1,COL_S1,ROW-COL_S1);

//	for (z = 0; z < ROW-COL_S1*2; z++)
//	for (z = ROW-COL_S1*2 ; z > 0; z--)
//	for (z = col1-col0 ; z > 0; z--)			// 从左向右移动
	for (z = 0; z < col1-col0; z++)			   // 从右向左移动
	{
		//坐标窗口全屏
		tmp	= (index + z);
		if(tmp >= end){ tmp = tmp - end + start; }
		tmp = tmp*row8;
//		cnt = (ROW-z)*COL+COL-row-24;
		cnt = (col0+z)*COL+row0;//(ROW-z)*COL+COL-123-40;			// 对应位置的背景图
		for (i = 0; i < row8; i++)
		{
			//计算数据位置
			tmp_char = *(ptr+tmp+i);//DispRoll.Roll_SrcInfo[tmp+i];

			for (j = 0;j < 8;j++)
			{
//				if ( ((tmp_char << j) & 0x80) == 0x80 )
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ TFT1_WD_DAT(charColor);   }	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息
				{ // 取上副背景图信息
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }					
				
//				cnt++;
			}
		}
	}
}

/************************************************************************
* 名    称：信息屏描述区滚屏，
* 功    能：
* 调用方法：
************************************************************************/
const	INT16U	hd_InfoSrcRollRow[]={ H_INFO_SRC1_ROW, H_INFO_SRC2_ROW ,H_INFO_SRC3_ROW };		// 横单屏
const	INT16U	sd_InfoSrcRollRow[]={ S_INFO_SRC1_ROW, S_INFO_SRC2_ROW, S_INFO_SRC3_ROW };		// 竖单屏


void TFT1_InfoSrcRoll(INT8U area,INT16U charColor)//,INT8U  bkSel)		//背景类型
{	
//	INT8U	row = dagaha[area];
	INT16U  i,z,j,tmp,tmp_char;//,row8,place;
	INT16U	index,start,end;
	INT8VU	*ptr;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;	  // 取图片背景的首地址
	INT16U	col0,col1,row0,row1;
	INT8U	row8;
	
	i = area;
	if(i > 3){ i = 0; }

// 单屏描述区滚屏
	index = DispRoll.uni.str4.SrcInfo_Index[i];
	end   = DispRoll.uni.str4.SrcInfo_End[i];
	start = DispRoll.uni.str4.SrcInfo_Start[i];
		
	index += 1;
	if(index >= end)
	{ index = start; }
		
	DispRoll.uni.str4.SrcInfo_Index[i] = index;
	ptr   = &DispRoll.uni.str4.SrcInfo[0];
	

	//设定显示区域
	// 设定滚屏的显示区域
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏 单屏
		col0 = 	COL_S1;
		col1 = 	COL;
		row0 = 	sd_InfoSrcRollRow[i];		
		col1 -= COL_S1;
		row1 = 	row0+24-1;		
		TFT1_SetWindows(col0,col1,row0,row1);			// 设置窗口区域
		
	}
	else
	{	// 横屏
		col0 = 	100;
		col1 = 	ROW;
		row0 = 	hd_InfoSrcRollRow[i];		
		col1 -= COL_S1;
		row1 = 	row0+24-1;		
		TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
	}
//	col1 -= COL_S1;
//	row1 = 	row0+24-1;
	row8 = (24/8);
//	TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
//	TFT1_SetWindows(COL-row-24,COL-row-1,COL_S1,ROW-COL_S1);

//	for (z = 0; z < ROW-COL_S1*2; z++)
//	for (z = ROW-COL_S1*2 ; z > 0; z--)
//	for (z = col1-col0 ; z > 0; z--)			// 从左向右移动
	for (z = 0; z < col1-col0; z++)			   // 从右向左移动
	{
		//坐标窗口全屏
		tmp	= (index + z);
		if(tmp >= end){ tmp = tmp - end + start; }
		tmp = tmp*row8;
//		cnt = (ROW-z)*COL+COL-row-24;
		cnt = (col0+z)*COL+row0;//(ROW-z)*COL+COL-123-40;			// 对应位置的背景图
		for (i = 0; i < row8; i++)
		{
			//计算数据位置
			tmp_char = *(ptr+tmp+i);//DispRoll.Roll_SrcInfo[tmp+i];

			for (j = 0;j < 8;j++)
			{
//				if ( ((tmp_char << j) & 0x80) == 0x80 )
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ TFT1_WD_DAT(charColor);   }	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息
				{ // 取上副背景图信息
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }					
//				cnt++;
			}
		}
	}
}
/************************************************************************
* 名    称：描述区滚屏，
* 功    能：
* 调用方法：
************************************************************************/
void TFT1_DispBackRoll(INT8U area,INT16U charColor)//,INT8U  bkSel)		//背景类型
{	
	INT16U  i,z,j,tmp,tmp_char;//,row8,place;
	INT16U	index,start,end;
	INT8VU	*ptr;
	INT32U	dat32,cnt,PictureAddr = FullImageInfo1[LCD.TFT1_PicNum].ImageAddr + Bank1_NOR2_ADDR;	  // 取图片背景的首地址
	INT16U	col0,col1,row0,row1;
	INT8U	row8;
    
	area %= PAGE_MAX_BACK_NUM ;     // 确保不越界

	i = area;
    
 	// 回显品名区滚屏
	index = DispRoll.uni.str2.BackName_Index[i];
	end   = DispRoll.uni.str2.BackName_End[i];
	start = DispRoll.uni.str2.BackName_Start[i];

	index += 1;
	if(index >= end)
	{ index = start; }

	DispRoll.uni.str2.BackName_Index[i] = index;
	ptr   = &DispRoll.uni.str2.BackName[i][0];
	
	row8 = (24/8);
	// 设定显示区域
	// 设定滚屏的显示区域
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		col0 = 	COL_S1;
		col1 = 	COL - COL_S1;
		row0 =  DispBackNameRow[1][i]; 
		row1 = 	row0+24-1;		 
		
		TFT1_SetWindows(col0,col1,row0,row1);			// 设置窗口区域
	}
	else
	{	// 横屏
		col0 = 	COL_S1;									//无用参数
		col1 = 	H_BACK_SKU_COL - COL_S1 - 5;
		row0 = 	DispBackNameRow[0][i];
		row1 = 	row0+24-1;
		
		TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
	}
//	TFT1_SetWindows(row0,row1,col0,col1);			// 设置窗口区域
//	TFT1_SetWindows(COL-row-24,COL-row-1,COL_S1,ROW-COL_S1);

//	for (z = col1-col0 ; z > 0; z--)			// 从左向右移动
	for (z = 0; z < col1-col0; z++)			   // 从右向左移动
	{
		//坐标窗口全屏
		tmp	= (index + z);
		if(tmp >= end){ tmp = tmp - end + start; }
		tmp = tmp*row8;
//		cnt = (ROW-z)*COL+COL-row-24;
		cnt = (col0+z)*COL+row0;//(ROW-z)*COL+COL-123-40;			// 对应位置的背景图
		for (i = 0; i < row8; i++)
		{
			//计算数据位置
			tmp_char = *(ptr+tmp+i);//DispRoll.Roll_SrcInfo[tmp+i];

			for (j = 0;j < 8;j++)
			{
//				if ( ((tmp_char << j) & 0x80) == 0x80 )
				if ( ((tmp_char >> j) & 0x01) == 0x01 )
				{ TFT1_WD_DAT(charColor);   }	// 字符颜色
//				else  if(bkSel != BK_FLASHPIC)
				else  if(bk_colour != BK_FLASHPIC)		 // 背景颜色非背景图
				{ TFT1_WD_DAT(bk_colour_tab[bk_colour]);	 }		//自定义背景色
				else
//				{ TFT1_WD_DAT(*(INT16U *) (PictureAddr+(cnt)*2)); }		// 取上副背景图信息
				{ // 取上副背景图信息
					dat32 = *(INT32U *) (PictureAddr+cnt*2);
					if( (j  & 0x01) == 0x01  )
					{ dat32 = dat32>>16;  }	
					
					TFT1_WD_DAT( (INT16U ) dat32); 					
				}		
				
				if( (j  & 0x01) == 0x01  )
				{ cnt++; }				
//				cnt++;
			}
		}
	}
}
/************************************************************************
* LCD 界面 		运行横屏
************************************************************************/

//const	INT8U	RUN_NAME[NAME_MAX] ={"苹果5代iPhone5S"}	;
//const	INT8U	RUN_SRC1[SRC_INFO_MAX] ={"美国苹果公司"}	;
//const	INT8U	RUN_SRC2[SRC_INFO_MAX] ={"富士康代工"};//{"2014022490380213"}	;
//const	INT8U	RUN_SRC3[SRC_INFO_MAX] ={"中国移动定制"}	;

#define		INFO_DIR_PIC_MAX	20			//18


void qr_code_display(unsigned int bgcolor,unsigned int color)
{
    u8 i = 0,j = 0,x = 0,y = 0;
 
    if(m_nSymbleSize==21)       //
    {
        for(j=0;j<(m_nSymbleSize*3);j++)
        {
            for(i=0;i<(m_nSymbleSize*3);i++)
            {
                display_point(313+i,106+j,bgcolor);     //清除二维码显示背景色   
            }
        }
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {
                    for(i=0;i<3;i++)
                    {
                        for(j=0;j<3;j++)
                        {
                            display_point(313+3*x+i,106+3*y+j,color); 
                        }
                    }
                }
            }
        }        
    }
    else if(m_nSymbleSize==25)
    {
        for(j=0;j<(m_nSymbleSize*2);j++)
        {
            for(i=0;i<(m_nSymbleSize*2);i++)
            {
                display_point(319+i,112+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {
                    for(i=0;i<2;i++)
                    {
                        for(j=0;j<2;j++)
                        {
                            display_point(319+2*x+i,112+2*y+j,color); 
                        }
                    }
                }
            }
        }        
    }
    else if(m_nSymbleSize==29)
    {
        for(j=0;j<(m_nSymbleSize*2);j++)
        {
            for(i=0;i<(m_nSymbleSize*2);i++)
            {
                display_point(315+i,108+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {
                    for(i=0;i<2;i++)
                    {
                        for(j=0;j<2;j++)
                        {
                            display_point(315+2*x+i,108+2*y+j,color); 
                        }
                    }
                }
            }
        }        
    }
    else if(m_nSymbleSize==33)
    {
        for(j=0;j<(m_nSymbleSize*2);j++)
        {
            for(i=0;i<(m_nSymbleSize*2);i++)
            {
                display_point(311+i,104+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {
                    for(i=0;i<2;i++)
                    {
                        for(j=0;j<2;j++)
                        {
                            display_point(311+2*x+i,104+2*y+j,color); 
                        }
                    }
                }
            }
        }        
    }
    else if(m_nSymbleSize==37)
    {
        for(j=0;j<m_nSymbleSize;j++)
        {
            for(i=0;i<m_nSymbleSize;i++)
            {
                display_point(325+i,118+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {

                    display_point(325+x,118+y,color); 
                }
            }
        }        
    }
    else if(m_nSymbleSize==41)
    {
        for(j=0;j<m_nSymbleSize;j++)
        {
            for(i=0;i<m_nSymbleSize;i++)
            {
                display_point(328+i,111+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {

                    display_point(328+x,111+y,color); 
                }
            }
        }        
    }
    else if(m_nSymbleSize==45)
    {
        for(j=0;j<m_nSymbleSize;j++)
        {
            for(i=0;i<m_nSymbleSize;i++)
            {
                display_point(321+i,114+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {

                    display_point(321+x,114+y,color); 
                }
            }
        }        
    } 
    else if(m_nSymbleSize==49)
    {
        for(j=0;j<m_nSymbleSize;j++)
        {
            for(i=0;i<m_nSymbleSize;i++)
            {
                display_point(319+i,112+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {

                    display_point(319+x,112+y,color); 
                }
            }
        }        
    }    
    else if(m_nSymbleSize==53)
    {
        for(j=0;j<m_nSymbleSize;j++)
        {
            for(i=0;i<m_nSymbleSize;i++)
            {
                display_point(317+i,110+j,bgcolor);     //清除二维码显示背景色   
            }
        } 
        for(x=0;x<m_nSymbleSize;x++)
        {
            for(y=0;y<m_nSymbleSize;y++)
            {
                if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
                {

                    display_point(317+x,110+y,color); 
                }
            }
        }        
    }    
}




#if 0
void qr_code_display(unsigned int x_axial,unsigned int y_axial,unsigned int bgcolor,unsigned int color)
{
    u8 i = 0,j = 0,x = 0,y = 0;
    
    if((x_axial>TFT_YSIZE-x_axial)&&(y_axial>TFT_XSIZE-y_axial))    //判断二维码是否会越界
        return;
    
	//EncodeData((char *)qr_code);//编码数据
  
    if(m_nSymbleSize > 21)  //生成二维码大小不能操作21个像素点位
        return;
    
    for(j=0;j<(m_nSymbleSize*3);j++)
    {
        for(i=0;i<(m_nSymbleSize*3);i++)
        {
            display_point(x_axial+i,y_axial+j,bgcolor);     //清除二维码显示背景色   
        }
    }
	for(x=0;x<m_nSymbleSize;x++)
	{
		for(y=0;y<m_nSymbleSize;y++)
		{
			if(m_byModuleData[x][y]==1)//判断该点位是否需要显示
			{
                for(i=0;i<3;i++)
                {
                    for(j=0;j<3;j++)
                    {
                        display_point(x_axial+3*x+i,y_axial+3*y+j,color); 
                    }
                }
			}
		}
	}
}
#endif

void Page_Run_HP(void)
{	
    //	INT8U	i,j,k,cnt;
    INT8U	j;
    INT16U	col0;

    // 滚屏模式加快速度,不再更新部分数据
    if( (DispRoll.f_Rolling[0] == ROLL_MODE_OFF)||(DispRoll.f_Rolling[1] == ROLL_MODE_OFF) )
    { 
        //====================================================================================
        // 载入背景，可能是图片也可能是颜色
        Load_Picture(0); 						// 载入横屏运行图片
        //====================================================================================
        // TFT1显示	
        if( ONLY_DISPLAY_MODE == DispDir.only) 
        { 	// 单屏显示
            // 品名区数据保存
            if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
            {  	// 无滚屏标志 
                TFT1_DispNameDataChgSave(ONLY_TASK_MODE);		 //品名区数据保存，并根据长度获得滚屏标志
            }
            // 品名区更新
            if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
            {  	// 无滚屏标志 					
                col0 = TFT1_PutString_flash( H_RUN_NAME_ROW,COL_S1,&strTFT1[0].run.Name[0]      ,NAME_MAX,CHINA3232,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 品名信息，数组最后的0作为结束字符
                TFT1_FillBackPic(H_RUN_NAME_ROW,H_RUN_NAME_ROW+40,col0,ROW);															// 后面填充背景	
            }	
            else
            { 
                TFT1_NameRoll(ONLY_TASK_MODE,txt_colour_tab[bk_colour] ); 
            }		// 有滚屏标志，滚屏显示
            if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)    // 描述区数据保存
            {	// 数据保存
                TFT1_DispSrcDataChgSave(ONLY_TASK_MODE);
            }
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)    // 描述行1区间更新或滚屏
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC1_ROW,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第1行描述信息
                TFT1_FillBackPic(H_RUN_SRC1_ROW,H_RUN_SRC1_ROW+26,col0,ROW1_P-8);															// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)    // 描述行2区间更新或滚屏
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC2_ROW,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第2行描述信息
                TFT1_FillBackPic(H_RUN_SRC2_ROW,H_RUN_SRC2_ROW+26,col0,ROW1_P-8);															// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)    // 描述行3区间更新或滚屏
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC3_ROW,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第3行描述信息
                TFT1_FillBackPic(H_RUN_SRC3_ROW,H_RUN_SRC3_ROW+26,col0,ROW1_P-8);															// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示	
        }
        else
        { // 双屏显示
            // 任务1
            // 品名区数据保存
            if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
            {  	// 品名区数据保存
                TFT1_DispNameDataChgSave(DOUBLE_TASK1_MODE);
            }
            // 品名区间更新或滚屏
            if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
            {
                col0 = TFT1_PutString_flash( H_RUN_NAME_ROW2,COL_S1,&strTFT1[0].run.Name[0]      ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 品名信息，数组最后的0作为结束字符
                TFT1_FillBackPic(H_RUN_NAME_ROW2,H_RUN_NAME_ROW2+26,col0,ROW/2-2);				
            }
            else
            { 
                TFT1_NameRoll(DOUBLE_TASK1_MODE,txt_colour_tab[bk_colour] ); 
            }					 // 滚屏显示
            // 描述区数据保存
            if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
            {	// 数据保存
                TFT1_DispSrcDataChgSave2(DOUBLE_TASK1_MODE);
            }
            // 描述行1区间更新或滚屏
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC1_ROW2,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第1行描述信息
                TFT1_FillBackPic(H_RUN_SRC1_ROW2,H_RUN_SRC1_ROW2+18,col0,ROW/2-2);																		// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示
            // 描述行2区间更新或滚屏
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC2_ROW2,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第2行描述信息
                TFT1_FillBackPic(H_RUN_SRC2_ROW2,H_RUN_SRC2_ROW2+18,col0,ROW/2-2);															// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示
            // 描述行3区间更新或滚屏
            if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)
            {
                col0 = TFT1_PutString_flash( H_RUN_SRC3_ROW2,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第3行描述信息
                TFT1_FillBackPic(H_RUN_SRC3_ROW2,H_RUN_SRC3_ROW2+18,col0,ROW/2-2);															// 后面填充背景
            }
            else
            { 
                TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  
            }				// 滚屏显示	

            if( UnfinishTask.num > 1 )
            {
                // 任务2
                // 品名区数据保存
                if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
                {  	// 品名区数据保存
                    TFT1_DispNameDataChgSave(DOUBLE_TASK2_MODE);
                }
                // 品名区间更新或滚屏
                if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
                {
                    col0 = TFT1_PutString_flash( H_RUN_NAME_ROW2,COL_S1+200,&strTFT1[1].run.Name[0]  ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 品名信息，数组最后的0作为结束字符
                    TFT1_FillBackPic(H_RUN_NAME_ROW2,H_RUN_NAME_ROW2+26,col0,ROW);														// 后面填充背景	
                }
                else
                { 
                    TFT1_NameRoll(DOUBLE_TASK2_MODE,txt_colour_tab[bk_colour] ); 
                }				// 滚屏显示
                if((DispRoll.f_Rolling[1] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)    // 描述区数据保存
                {	// 数据保存
                    TFT1_DispSrcDataChgSave2(DOUBLE_TASK2_MODE);
                }
                // 描述行1区间更新或滚屏
                if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC1) == 0x00)
                {
                    col0 = TFT1_PutString_flash( H_RUN_SRC1_ROW2,COL_S1+200,&strTFT1[1].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第1行描述信息
                    TFT1_FillBackPic(H_RUN_SRC1_ROW2,H_RUN_SRC1_ROW2+18,col0,ROW);														// 后面填充背景																	// 后面填充背景
                }
                else
                { 
                    TFT1_SrcRoll(3,dsc_colour_tab[bk_colour] );  
                }				// 滚屏显示
                if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC2) == 0x00)    // 描述行2区间更新或滚屏
                {
                    col0 = TFT1_PutString_flash( H_RUN_SRC2_ROW2,COL_S1+200,&strTFT1[1].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第2行描述信息
                    TFT1_FillBackPic(H_RUN_SRC2_ROW2,H_RUN_SRC2_ROW2+18,col0,ROW);															// 后面填充背景
                }
                else
                { 
                    TFT1_SrcRoll(4,dsc_colour_tab[bk_colour] );  
                }				// 滚屏显示
                if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC3) == 0x00)    // 描述行3区间更新或滚屏
                {
                    col0 = TFT1_PutString_flash( H_RUN_SRC3_ROW2,COL_S1+200,&strTFT1[1].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);	// 第3行描述信息
                    TFT1_FillBackPic(H_RUN_SRC3_ROW2,H_RUN_SRC3_ROW2+18,col0,ROW);															// 后面填充背景
                }
                else
                { 
                    TFT1_SrcRoll(5,dsc_colour_tab[bk_colour] );  
                }				// 滚屏显示	
            }
        }
    }
    // TFT2显示	
    // 方位
    if( (ONLY_DISPLAY_MODE == DispDir.only)&&(LCD.f_Update == TRUE) ) 
    {	// 单屏显示
        j = strTFT2[0].run.CurDir[0];
        if( (j != 0)&&((j <= 10)||(j >= 16)) )   //为0不显示，0~9显示箭头 0x0A显示二维码  大于0x10显示图标 2021年8月18日
        {
            if(j==0x0A) //定义显示二维码功能码为：0x0A   2021年8月18日
            {
                qr_code_display(aBLACK,aWHITE);     //显示二维码  2021年8月18日
            }
            else
            {
                j -= 1;                             //指向对应数组储存区位置   2021年8月18日
                if(j > INFO_DIR_PIC_MAX)            //限制最大储存范围          2021年8月18日
                { 
                    j = INFO_DIR_PIC_MAX; 
                }

                TFT1_Display_NORFlash_Picture(FullImageConst[1],FullImageInfo1[j+16]);      //显示64*64图片//箭头                
            }      
            if(j < 9)       //0~9为箭头指示，0x0A为后加显示二维码指令，都需要显示框 2021年8月18日
            {
                TFT1_SetWindows(COL1_P-8,COL1_P+COL1+7,ROW1_P-1,ROW1_P-1);  //坐标设置
                for(col0 = COL1_P-8;col0 < COL1_P+COL1+8;col0 ++)		
                { 
                    TFT1_WD_DAT(dsc_colour_tab[bk_colour]); 
                }
                TFT1_SetWindows(COL1_P-8,COL1_P+COL1+7,ROW1_P+ROW1+1,ROW1_P+ROW1+1);  //坐标设置
                for(col0 = COL1_P-8;col0 < COL1_P+COL1+8;col0 ++)
                { 
                    TFT1_WD_DAT(dsc_colour_tab[bk_colour]); 
                }
                TFT1_SetWindows(COL1_P-1,COL1_P-1,ROW1_P-8,ROW1_P+ROW1+7);  //坐标设置
                for(col0 = ROW1_P-8;col0 < ROW1_P+ROW1+8;col0 ++)
                { 
                    TFT1_WD_DAT(dsc_colour_tab[bk_colour]); 
                }
                TFT1_SetWindows(COL1_P+COL1+1,COL1_P+COL1+1,ROW1_P-8,ROW1_P+ROW1+7);  //坐标设置
                for(col0 = ROW1_P-8;col0 < ROW1_P+ROW1+7;col0 ++)
                { 
                    TFT1_WD_DAT(dsc_colour_tab[bk_colour]); 
                }
            }
        }
    }
    if( ONLY_DISPLAY_MODE == DispDir.only) 
    {// 单屏
        // 库位
        TFT1_Draw_Line(H_RUN_SKU_UL,H_RUN_SKU_UL,0,ROW,btxt_colour_tab[bk_colour]);
        col0 = TFT1_PutString_flash( H_RUN_SKU_ROW,COL_S1,&strTFT2[0].run.CurSku[0],SKU_DISP_MAX,CHAR4080B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示库位
        TFT1_FillBackPic(H_RUN_SKU_ROW,H_RUN_SKU_ROW+72,col0,H_RUN_COUNT_COL);															// 后面填充背景	
        // 物品数量  
        col0 = TFT1_PutString_flash(H_RUN_SKU_ROW, H_RUN_COUNT_COL,&strTFT2[0].run.CurCnt[0],CNT_DISP_MAX,CHAR4080B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
        TFT1_FillBackPic(H_RUN_SKU_ROW,H_RUN_SKU_ROW+72,col0,ROW);																// 后面填充背景		
        // 单位
        if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
        { 
            col0 = 260; 
        }
        else
        { 
            col0 = TFT1_PutString_flash( H_RUN_SKU_ROW+14,ROW-50,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	
        }		// 显示单位	
        TFT1_FillBackPic(H_RUN_SKU_ROW+14,H_RUN_SKU_ROW+14+24,col0,ROW);
    }
    else
    {// 双屏,4个库位，5个显示，加单位没法同步显示
        INT16U  col_sku = COL_S1/2;
        INT8U	i;
        i = SKU_DISP_MAX; j = CNT_DISP_MAX;
        if(strTFT2[0].run.CurSku[3] == ' ')
        { 
            i--; col_sku = COL_S1;
            if(strTFT2[0].run.CurSku[2] == ' ')
            { 
                i--; 
            }
        }
        TFT1_Draw_XLine(H_RUN_SKU_UL2,H_RUN_SKU_UL2,1,ROW/2-1,dsc_colour_tab[bk_colour]);   // 库位	
        col0 = TFT1_PutString_flash( H_RUN_SKU_ROW2,col_sku,&strTFT2[0].run.CurSku[0],i,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示库位														// 后面填充背景
        if(i > 2) { col_sku = col0+10; }
        else	  { col_sku = col0+20; }	
        TFT1_FillBackPic(H_RUN_SKU_ROW2,H_RUN_SKU_ROW2+40,col0,col_sku);															// 后面填充背景
        col0 = TFT1_PutString_flash(  H_RUN_SKU_ROW2, col_sku,&strTFT2[0].run.CurCnt[0],j,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
        TFT1_FillBackPic(H_RUN_SKU_ROW2,H_RUN_SKU_ROW2+40,col0,H_RUN_COUNT_COL3);																// 后面填充背景
        if(col0 < H_RUN_COUNT_COL3)     // 单位
        {
            if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
            { 
                col0 = H_RUN_COUNT_COL3; 
            }
        else
        {
            col0 = TFT1_PutString_flash( H_RUN_SKU_ROW2+8,H_RUN_COUNT_COL3,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
        }
        TFT1_FillBackPic(H_RUN_SKU_ROW2+8,H_RUN_SKU_ROW2+8+16,col0,ROW/2-2);															// 后面填充背景
        // 任务2
        TFT1_Draw_XLine(H_RUN_SKU_UL2,H_RUN_SKU_UL2,ROW/2+1,ROW-1,dsc_colour_tab[bk_colour]);
        if( UnfinishTask.num > 1 )
        {
            // 库位	
            col_sku = COL_S1/2+200;
            i = SKU_DISP_MAX; j = CNT_DISP_MAX;
            if(strTFT2[1].run.CurSku[3] == ' ')
            { 
                i--; col_sku = COL_S1+200;
                if(strTFT2[1].run.CurSku[2] == ' ')
                { i--; }
            }
            col0 = TFT1_PutString_flash( H_RUN_SKU_ROW2,col_sku,&strTFT2[1].run.CurSku[0],i,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	
            if(i > 2) { col_sku = col0+10; }
            else	  { col_sku = col0+20; } 
            TFT1_FillBackPic(H_RUN_SKU_ROW2,H_RUN_SKU_ROW2+40,col0,col_sku);
            col0 = TFT1_PutString_flash(  H_RUN_SKU_ROW2, col_sku,&strTFT2[1].run.CurCnt[0],CNT_DISP_MAX,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
            TFT1_FillBackPic(H_RUN_SKU_ROW2,H_RUN_SKU_ROW2+40,col0,H_RUN_COUNT_COL3+200);																// 后面填充背景
            // 单位
            if(col0 < H_RUN_COUNT_COL3+200)
            {
                if(strTFT2[1].run.CurCnt[0] == 'F' )	   // 满则不显示单位
                { 
                    col0 = H_RUN_COUNT_COL3+200; 
                }
                else
                {
                    col0 = TFT1_PutString_flash( H_RUN_SKU_ROW2+8,H_RUN_COUNT_COL3+200,&strTFT2[1].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
                }
                TFT1_FillBackPic(H_RUN_SKU_ROW2+8,H_RUN_SKU_ROW2+8+16,col0,ROW); 		
        }
        TFT1_Draw_Line(0,COL,ROW/2,ROW/2,btxt_colour_tab[bk_colour]);
    }
}

/************************************************************************
* LCD 界面 		运行竖屏
************************************************************************/
#if TFT_TYPE == TFT_HJ28_9327
void	Page_Run_SP(void)
{	
//	INT8U	i,j,k,cnt;
	INT8U	j;
	INT16U	col0;

	// 滚屏模式加快速度,不再更新部分数据
	if( (DispRoll.f_Rolling[0] == ROLL_MODE_OFF)||(DispRoll.f_Rolling[1] == ROLL_MODE_OFF) )
	{ 
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(0); 						// 载入横屏运行图片
		//====================================================================================
		// TFT1显示	
	if( ONLY_DISPLAY_MODE == DispDir.only) 
	{ // 单屏显示

		// 品名区数据保存
		if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
		{  	// 品名区数据保存
			TFT1_DispNameDataChgSave(ONLY_TASK_MODE);
		}
		// 品名区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
		{
			col0 = TFT1_PutString_flash( S_RUN_NAME1_ROW,COL_S1,&strTFT1[0].run.Name[0]      ,NAME_MAX,CHINA3232,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
			TFT1_FillBackPic(S_RUN_NAME1_ROW,S_RUN_NAME1_ROW+40,col0,COL);															// 后面填充背景		
		}
		else{ TFT1_NameRoll(0,txt_colour_tab[bk_colour] ); }					 // 滚屏显示
	
		// 描述区数据保存
		if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
		{	// 数据保存
			TFT1_DispSrcDataChgSave(ONLY_TASK_MODE);
		}
		// 描述行1区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)
		{
			col0 = TFT1_PutString_flash( S_RUN_SRC1_ROW,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
			TFT1_FillBackPic(S_RUN_SRC1_ROW,S_RUN_SRC1_ROW+26,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行2区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)
		{
			col0 = TFT1_PutString_flash( S_RUN_SRC2_ROW,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
			TFT1_FillBackPic(S_RUN_SRC2_ROW,S_RUN_SRC2_ROW+26,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行3区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)
		{
			col0 = TFT1_PutString_flash( S_RUN_SRC3_ROW,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
			TFT1_FillBackPic(S_RUN_SRC3_ROW,S_RUN_SRC3_ROW+26,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	
	}
	else
	{ // 双屏显示
		// 任务1
		// 品名区数据保存
		if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
		{  	// 品名区数据保存
			TFT1_DispNameDataChgSave(DOUBLE_TASK1_MODE);
		}
		// 品名区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS1_RUN_NAME1_ROW,COL_S1,&strTFT1[0].run.Name[0]  ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
			TFT1_FillBackPic(SS1_RUN_NAME1_ROW,SS1_RUN_NAME1_ROW+26,col0,COL);				
		}
		else{ TFT1_NameRoll(DOUBLE_TASK1_MODE,txt_colour_tab[bk_colour] ); }					 // 滚屏显示

		// 描述区数据保存
		if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
		{	// 数据保存
			TFT1_DispSrcDataChgSave2(DOUBLE_TASK1_MODE);
		}
		// 描述行1区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS1_RUN_SRC1_ROW,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
			TFT1_FillBackPic(SS1_RUN_SRC1_ROW,SS1_RUN_SRC1_ROW+18,col0,COL);																					// 后面填充背景
		}
		else{ TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行2区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS1_RUN_SRC2_ROW,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
			TFT1_FillBackPic(SS1_RUN_SRC2_ROW,SS1_RUN_SRC2_ROW+18,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行3区间更新或滚屏
		if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS1_RUN_SRC3_ROW,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
			TFT1_FillBackPic(SS1_RUN_SRC3_ROW,SS1_RUN_SRC3_ROW+18,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	


		// 任务2
		if( UnfinishTask.num > 1 )
		{
		// 品名区数据保存
		if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
		{  	// 品名区数据保存
			TFT1_DispNameDataChgSave(DOUBLE_TASK2_MODE);
		}
		// 品名区间更新或滚屏
		if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
		{			
			col0 = TFT1_PutString_flash( SS2_RUN_NAME1_ROW,COL_S1,&strTFT1[1].run.Name[0]  ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
			TFT1_FillBackPic(SS2_RUN_NAME1_ROW,SS2_RUN_NAME1_ROW+26,col0,COL);														// 后面填充背景		
		}
		else{ TFT1_NameRoll(DOUBLE_TASK2_MODE,txt_colour_tab[bk_colour] ); }					 // 滚屏显示

			
		// 描述区数据保存
		if((DispRoll.f_Rolling[1] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
		{	// 数据保存
			TFT1_DispSrcDataChgSave2(DOUBLE_TASK2_MODE);
		}
		// 描述行1区间更新或滚屏
		if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC1) == 0x00)
		{			
			col0 = TFT1_PutString_flash( SS2_RUN_SRC1_ROW,COL_S1,&strTFT1[1].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
			TFT1_FillBackPic(SS2_RUN_SRC1_ROW,SS2_RUN_SRC1_ROW+18,col0,COL);														// 后面填充背景																	// 后面填充背景
		}
		else{ TFT1_SrcRoll(3,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行2区间更新或滚屏
		if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC2) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS2_RUN_SRC2_ROW,COL_S1,&strTFT1[1].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
			TFT1_FillBackPic(SS2_RUN_SRC2_ROW,SS2_RUN_SRC2_ROW+18,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(4,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
		// 描述行3区间更新或滚屏
		if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC3) == 0x00)
		{
			col0 = TFT1_PutString_flash( SS2_RUN_SRC3_ROW,COL_S1,&strTFT1[1].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
			TFT1_FillBackPic(SS2_RUN_SRC3_ROW,SS2_RUN_SRC3_ROW+18,col0,COL);															// 后面填充背景
		}
		else{ TFT1_SrcRoll(5,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	
		}
	}
	}

	// TFT2显示
	
	// 方位
if( (ONLY_DISPLAY_MODE == DispDir.only)&&(LCD.f_Update == TRUE) )
{  // 单屏显示

	j = strTFT2[0].run.CurDir[0];			   // 方向信息
	if( (j != 0)&&((j < 10)||(j >= 16)) )
	{
		j -= 1;
		if(j > 18) { j = 18; }
		TFT1_Display_NORFlash_Picture(FullImageConst[2],FullImageInfo1[j+16]);
	
//		if(j < 9)
//		{
//			// 画＃
//			TFT1_SetWindows(ROW1_SP-1,ROW1_SP-1,COL1_SP-12,COL1_SP+COL1+11);  //坐标设置
//			for(col0 = COL1_SP-12;col0 < COL1_SP+COL1+12;col0 ++)		
//			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
//			
//			TFT1_SetWindows(ROW1_SP+ROW1+1,ROW1_SP+ROW1+1,COL1_SP-12,COL1_SP+COL1+11);  //坐标设置
//			for(col0 = COL1_SP-12;col0 < COL1_SP+COL1+12;col0 ++)
//			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
//			
//			TFT1_SetWindows(ROW1_SP-12,ROW1_SP+ROW1+11,COL1_SP-1,COL1_SP-1);  //坐标设置
//			for(col0 = ROW1_SP-12;col0 < ROW1_SP+ROW1+12;col0 ++)
//			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
//			
//			TFT1_SetWindows(ROW1_SP-12,ROW1_SP+ROW1+11,COL1_SP+COL1+1,COL1_SP+COL1+1);  //坐标设置
//			for(col0 = ROW1_SP-12;col0 < ROW1_SP+ROW1+12;col0 ++)
//			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
//		}

		if(j < 9)
		{
			// 画＃
			TFT1_SetWindows(COL1_SP-8,COL1_SP+COL1+7,DispDir.dif+ROW1_SP-1,DispDir.dif+ROW1_SP-1);  //坐标设置
			for(col0 = COL1_SP-8;col0 < COL1_SP+COL1+8;col0 ++)		
			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
			
			TFT1_SetWindows(COL1_SP-8,COL1_SP+COL1+7,DispDir.dif+ROW1_SP+ROW1+1,DispDir.dif+ROW1_SP+ROW1+1);  //坐标设置
			for(col0 = COL1_SP-8;col0 < COL1_SP+COL1+8;col0 ++)
			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
			
			TFT1_SetWindows(COL1_SP-1,COL1_SP-1,DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7);  //坐标设置
			for(col0 = ROW1_SP-8;col0 < ROW1_SP+ROW1+8;col0 ++)
			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
			
			TFT1_SetWindows(COL1_SP+COL1+1,COL1_SP+COL1+1,DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7);  //坐标设置
			for(col0 = ROW1_SP-8;col0 < ROW1_SP+ROW1+8;col0 ++)
			{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
		}
	}
}
if( ONLY_DISPLAY_MODE == DispDir.only) { // 单屏
	// 画线
	TFT1_Draw_Line(0,COL,S_RUN_SKU_UL,S_RUN_SKU_UL,btxt_colour_tab[bk_colour]);
	// 库位
	col0 = TFT1_PutString_flash( S_RUN_SKU_ROW,COL_S1,&strTFT2[0].run.CurSku[0]    ,SKU_DISP_MAX,CHAR4080B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
	TFT1_FillBackPic(S_RUN_SKU_ROW,S_RUN_SKU_ROW+72,col0,130);															// 后面填充背景
	// 物品数量  
	col0 = TFT1_PutString_flash(  S_RUN_COUNT_ROW, COL_S1,&strTFT2[0].run.CurCnt[0],CNT_DISP_MAX,CHAR4080B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
	TFT1_FillBackPic(S_RUN_COUNT_ROW,S_RUN_COUNT_ROW+72,col0,COL);																// 后面填充背景
 		
	// 单位
	if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
	{ col0 = 260; }
	else
	{ col0 = TFT1_PutString_flash( S_RUN_COUNT_ROW+14,COL-50,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位
	
	TFT1_FillBackPic(S_RUN_COUNT_ROW+14,S_RUN_COUNT_ROW+14+24+8,col0,COL);	
}
else{ // 双屏
	// 任务1
	// 库位	
	TFT1_Draw_XLine(0,COL,SS1_RUN_SKU_UL,SS1_RUN_SKU_UL,dsc_colour_tab[bk_colour]);	   // 画线1
	col0 = TFT1_PutString_flash( SS1_RUN_SKU_ROW,COL_S1,&strTFT2[0].run.CurSku[0],SKU_DISP_MAX,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
	TFT1_FillBackPic(SS1_RUN_SKU_ROW,SS1_RUN_SKU_ROW+40,col0,SS1_RUN_COUNT_COL2);															// 后面填充背景
	// 物品数量  
	col0 = TFT1_PutString_flash(  SS1_RUN_SKU_ROW, SS1_RUN_COUNT_COL2,&strTFT2[0].run.CurCnt[0],CNT_DISP_MAX,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
	TFT1_FillBackPic(SS1_RUN_SKU_ROW,SS1_RUN_SKU_ROW+40,col0,SS1_RUN_COUNT_COL3);																// 后面填充背景
	// 单位
	if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
	{ col0 = SS1_RUN_COUNT_COL3; }
	else
	{ col0 = TFT1_PutString_flash( SS1_RUN_SKU_ROW+8,SS1_RUN_COUNT_COL3,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
	TFT1_FillBackPic(SS1_RUN_SKU_ROW+8,SS1_RUN_SKU_ROW+8+18,col0,COL);															// 后面填充背景

	// 任务2
	TFT1_Draw_XLine(0,COL,SS2_RUN_SKU_UL,SS2_RUN_SKU_UL,dsc_colour_tab[bk_colour]);		  // 画线2
	if( UnfinishTask.num > 1 )
	{
		// 库位	
		col0 = TFT1_PutString_flash( SS2_RUN_SKU_ROW,COL_S1,&strTFT2[1].run.CurSku[0],SKU_DISP_MAX,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
		TFT1_FillBackPic(SS2_RUN_SKU_ROW,SS2_RUN_SKU_ROW+40,col0,SS2_RUN_COUNT_COL2);	
		// 物品数量
		col0 = TFT1_PutString_flash(  SS2_RUN_SKU_ROW, SS2_RUN_COUNT_COL2,&strTFT2[1].run.CurCnt[0],CNT_DISP_MAX,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
		TFT1_FillBackPic(SS2_RUN_SKU_ROW,SS2_RUN_SKU_ROW+40,col0,SS2_RUN_COUNT_COL3);																// 后面填充背景
		// 单位
		if(strTFT2[1].run.CurCnt[0] == 'F' )	   // 满则不显示单位
		{ col0 = SS2_RUN_COUNT_COL3; }
		else
		{ col0 = TFT1_PutString_flash( SS2_RUN_SKU_ROW+8,SS2_RUN_COUNT_COL3,&strTFT2[1].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
		TFT1_FillBackPic(SS2_RUN_SKU_ROW+8,SS2_RUN_SKU_ROW+8+18,col0,COL);															// 后面填充背景	
	}

	TFT1_Draw_Line(0,COL,232,232,btxt_colour_tab[bk_colour]);				 // 画双屏分界线
}
}
#else
void	Page_Run_SP(void)
{	
//	INT8U	i,j,k,cnt;
	INT8U	j;
	INT16U	col0;

	// 滚屏模式加快速度,不再更新部分数据
	if( (DispRoll.f_Rolling[0] == ROLL_MODE_OFF)||(DispRoll.f_Rolling[1] == ROLL_MODE_OFF) )
	{ 
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(5); 						// 载入横屏运行图片
		//====================================================================================
		// TFT1显示	
		if( ONLY_DISPLAY_MODE == DispDir.only) 
		{ // 单屏显示

			// 品名区数据保存
			if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
			{  	// 品名区数据保存
				TFT1_DispNameDataChgSave(ONLY_TASK_MODE);
			}
			// 品名区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
			{
				col0 = TFT1_PutString_flash( S_RUN_NAME1_ROW,COL_S1,&strTFT1[0].run.Name[0]      ,NAME_MAX,CHINA3232,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
				TFT1_FillBackPic(S_RUN_NAME1_ROW,S_RUN_NAME1_ROW+40,col0,COL);															// 后面填充背景		
			}
			else{ TFT1_NameRoll(0,txt_colour_tab[bk_colour] ); }					 // 滚屏显示
		
			// 描述区数据保存
			if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
			{	// 数据保存
				TFT1_DispSrcDataChgSave(ONLY_TASK_MODE);
			}
			// 描述行1区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)
			{
				col0 = TFT1_PutString_flash( S_RUN_SRC1_ROW,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
				TFT1_FillBackPic(S_RUN_SRC1_ROW,S_RUN_SRC1_ROW+26,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行2区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)
			{
				col0 = TFT1_PutString_flash( S_RUN_SRC2_ROW,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
				TFT1_FillBackPic(S_RUN_SRC2_ROW,S_RUN_SRC2_ROW+26,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行3区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)
			{
				col0 = TFT1_PutString_flash( S_RUN_SRC3_ROW,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
				TFT1_FillBackPic(S_RUN_SRC3_ROW,S_RUN_SRC3_ROW+26,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	
		}
		else
		{ // 双屏显示
			// 任务1
			// 品名区数据保存
			if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
			{  	// 品名区数据保存
				TFT1_DispNameDataChgSave(DOUBLE_TASK1_MODE);
			}
			// 品名区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS1_RUN_NAME1_ROW,COL_S1,&strTFT1[0].run.Name[0]  ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
				TFT1_FillBackPic(SS1_RUN_NAME1_ROW,SS1_RUN_NAME1_ROW+26,col0,COL);				
			}
			else{ TFT1_NameRoll(DOUBLE_TASK1_MODE,txt_colour_tab[bk_colour] ); }					 // 滚屏显示

			// 描述区数据保存
			if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
			{	// 数据保存
				TFT1_DispSrcDataChgSave2(DOUBLE_TASK1_MODE);
			}
			// 描述行1区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC1) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS1_RUN_SRC1_ROW,COL_S1,&strTFT1[0].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
				TFT1_FillBackPic(SS1_RUN_SRC1_ROW,SS1_RUN_SRC1_ROW+18,col0,COL);																					// 后面填充背景
			}
			else{ TFT1_SrcRoll(0,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行2区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC2) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS1_RUN_SRC2_ROW,COL_S1,&strTFT1[0].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
				TFT1_FillBackPic(SS1_RUN_SRC2_ROW,SS1_RUN_SRC2_ROW+18,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(1,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行3区间更新或滚屏
			if((DispRoll.f_Rolling[0] & ROLL_MODE_SRC3) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS1_RUN_SRC3_ROW,COL_S1,&strTFT1[0].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
				TFT1_FillBackPic(SS1_RUN_SRC3_ROW,SS1_RUN_SRC3_ROW+18,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(2,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	


			// 任务2
			if( UnfinishTask.num > 1 )
			{
			// 品名区数据保存
			if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
			{  	// 品名区数据保存
				TFT1_DispNameDataChgSave(DOUBLE_TASK2_MODE);
			}
			// 品名区间更新或滚屏
			if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) == 0x00)
			{			
				col0 = TFT1_PutString_flash( SS2_RUN_NAME1_ROW,COL_S1,&strTFT1[1].run.Name[0]  ,NAME_MAX,CHINA2424,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
				TFT1_FillBackPic(SS2_RUN_NAME1_ROW,SS2_RUN_NAME1_ROW+26,col0,COL);														// 后面填充背景		
			}
			else{ TFT1_NameRoll(DOUBLE_TASK2_MODE,txt_colour_tab[bk_colour] ); }					 // 滚屏显示

				
			// 描述区数据保存
			if((DispRoll.f_Rolling[1] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
			{	// 数据保存
				TFT1_DispSrcDataChgSave2(DOUBLE_TASK2_MODE);
			}
			// 描述行1区间更新或滚屏
			if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC1) == 0x00)
			{			
				col0 = TFT1_PutString_flash( SS2_RUN_SRC1_ROW,COL_S1,&strTFT1[1].run.SrcInfo[0][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第1行描述信息
				TFT1_FillBackPic(SS2_RUN_SRC1_ROW,SS2_RUN_SRC1_ROW+18,col0,COL);														// 后面填充背景																	// 后面填充背景
			}
			else{ TFT1_SrcRoll(3,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行2区间更新或滚屏
			if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC2) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS2_RUN_SRC2_ROW,COL_S1,&strTFT1[1].run.SrcInfo[1][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第2行描述信息
				TFT1_FillBackPic(SS2_RUN_SRC2_ROW,SS2_RUN_SRC2_ROW+18,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(4,dsc_colour_tab[bk_colour] );  }				// 滚屏显示
			// 描述行3区间更新或滚屏
			if((DispRoll.f_Rolling[1] & ROLL_MODE_SRC3) == 0x00)
			{
				col0 = TFT1_PutString_flash( SS2_RUN_SRC3_ROW,COL_S1,&strTFT1[1].run.SrcInfo[2][0],SRC_INFO_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	// 第3行描述信息
				TFT1_FillBackPic(SS2_RUN_SRC3_ROW,SS2_RUN_SRC3_ROW+18,col0,COL);															// 后面填充背景
			}
			else{ TFT1_SrcRoll(5,dsc_colour_tab[bk_colour] );  }				// 滚屏显示	
			}
		}
	}

	// TFT2显示
	
	// 方位
	if( (ONLY_DISPLAY_MODE == DispDir.only)&&(LCD.f_Update == TRUE) )
	{  // 单屏显示

		j = strTFT2[0].run.CurDir[0];			   // 方向信息
		if( (j != 0)&&((j < 10)||(j >= 16)) )
		{
			j -= 1;
	//		if(j > 18) { j = 18; }
			if(j > INFO_DIR_PIC_MAX) { j = INFO_DIR_PIC_MAX; }
			TFT1_Display_NORFlash_Picture(FullImageConst[2],FullImageInfo1[j+16]);
		


			if(j < 9)
			{
				// 画＃
//				TFT1_SetWindows(DispDir.dif+ROW1_SP-1,DispDir.dif+ROW1_SP-1,COL1_SP-8,COL1_SP+COL1+7);  //坐标设置
				TFT1_SetWindows(COL1_SP-8,COL1_SP+COL1+7,DispDir.dif+ROW1_SP-1,DispDir.dif+ROW1_SP-1);  //坐标设置
				for(col0 = COL1_SP-8;col0 < COL1_SP+COL1+8;col0 ++)		
				{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
				
//				TFT1_SetWindows(DispDir.dif+ROW1_SP+ROW1+1,DispDir.dif+ROW1_SP+ROW1+1,COL1_SP-8,COL1_SP+COL1+7);  //坐标设置
				TFT1_SetWindows(COL1_SP-8,COL1_SP+COL1+7,DispDir.dif+ROW1_SP+ROW1+1,DispDir.dif+ROW1_SP+ROW1+1);  //坐标设置
				for(col0 = COL1_SP-8;col0 < COL1_SP+COL1+8;col0 ++)
				{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
				
//				TFT1_SetWindows(DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7,COL1_SP-1,COL1_SP-1);  //坐标设置
				TFT1_SetWindows(COL1_SP-1,COL1_SP-1,DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7);  //坐标设置
				for(col0 = ROW1_SP-8;col0 < ROW1_SP+ROW1+8;col0 ++)
				{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
				
//				TFT1_SetWindows(DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7,COL1_SP+COL1+1,COL1_SP+COL1+1);  //坐标设置
				TFT1_SetWindows(COL1_SP+COL1+1,COL1_SP+COL1+1,DispDir.dif+ROW1_SP-8,DispDir.dif+ROW1_SP+ROW1+7);  //坐标设置
				for(col0 = ROW1_SP-8;col0 < ROW1_SP+ROW1+8;col0 ++)
				{ TFT1_WD_DAT(dsc_colour_tab[bk_colour]); }
			}
		}
	}
	
	if( ONLY_DISPLAY_MODE == DispDir.only) { // 单屏
		// 画线
		//TFT1_Draw_Line(S_RUN_SKU_UL,S_RUN_SKU_UL,0,COL,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_RUN_SKU_UL,S_RUN_SKU_UL,btxt_colour_tab[bk_colour]);
		// 库位
		col0 = TFT1_PutString_flash( S_RUN_SKU_ROW,COL_S1,&strTFT2[0].run.CurSku[0]    ,SKU_DISP_MAX,CHAR4080B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
		TFT1_FillBackPic(S_RUN_SKU_ROW,S_RUN_SKU_ROW+72,col0,130);															// 后面填充背景
		// 物品数量  
		col0 = TFT1_PutString_flash(  S_RUN_COUNT_ROW, COL_S1,&strTFT2[0].run.CurCnt[0],CNT_DISP_MAX,CHAR4080B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
		TFT1_FillBackPic(S_RUN_COUNT_ROW,S_RUN_COUNT_ROW+72,col0,COL);																// 后面填充背景
			
		// 单位
		if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
		{ col0 = 260; }
		else
//		{ col0 = TFT1_PutString_flash( S_RUN_COUNT_ROW+14,COL-50,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位
		{ col0 = TFT1_PutString_flash( S_RUN_UNI_ROW,COL-50,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位
		
		TFT1_FillBackPic(S_RUN_UNI_ROW,S_RUN_UNI_ROW+24+8,col0,COL);	
	}
	else{ // 双屏
		// 任务1
		// 库位	
//		TFT1_Draw_XLine(SS1_RUN_SKU_UL,SS1_RUN_SKU_UL,0,COL,dsc_colour_tab[bk_colour]);	   // 画线1
		TFT1_Draw_XLine(0,COL,SS1_RUN_SKU_UL,SS1_RUN_SKU_UL,dsc_colour_tab[bk_colour]);	   // 画线1
		
		col0 = TFT1_PutString_flash( SS1_RUN_SKU_ROW,COL_S1,&strTFT2[0].run.CurSku[0],SKU_DISP_MAX,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
		TFT1_FillBackPic(SS1_RUN_SKU_ROW,SS1_RUN_SKU_ROW+40,col0,SS1_RUN_COUNT_COL2);															// 后面填充背景
		// 物品数量  
		col0 = TFT1_PutString_flash(  SS1_RUN_SKU_ROW, SS1_RUN_COUNT_COL2,&strTFT2[0].run.CurCnt[0],CNT_DISP_MAX,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
		TFT1_FillBackPic(SS1_RUN_SKU_ROW,SS1_RUN_SKU_ROW+40,col0,SS1_RUN_COUNT_COL3);																// 后面填充背景
		// 单位
		if(strTFT2[0].run.CurCnt[0] == 'F' )	   // 满则不显示单位
		{ col0 = SS1_RUN_COUNT_COL3; }
		else
		{ col0 = TFT1_PutString_flash( SS1_RUN_UNI_ROW,SS1_RUN_COUNT_COL3,&strTFT2[0].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
		TFT1_FillBackPic(SS1_RUN_UNI_ROW,SS1_RUN_UNI_ROW+18,col0,COL);															// 后面填充背景

		// 任务2
		//TFT1_Draw_XLine(SS2_RUN_SKU_UL,SS2_RUN_SKU_UL,0,COL,dsc_colour_tab[bk_colour]);		  // 画线2
		TFT1_Draw_XLine(0,COL,SS2_RUN_SKU_UL,SS2_RUN_SKU_UL,dsc_colour_tab[bk_colour]);		  // 画线2
		
		if( UnfinishTask.num > 1 )
		{
			// 库位	
			col0 = TFT1_PutString_flash( SS2_RUN_SKU_ROW,COL_S1,&strTFT2[1].run.CurSku[0],SKU_DISP_MAX,CHAR2040B,sku_colour_tab[bk_colour],bk_colour_tab[bk_colour]  );			// 显示库位
			TFT1_FillBackPic(SS2_RUN_SKU_ROW,SS2_RUN_SKU_ROW+40,col0,SS2_RUN_COUNT_COL2);	
			// 物品数量
			col0 = TFT1_PutString_flash(  SS2_RUN_SKU_ROW, SS2_RUN_COUNT_COL2,&strTFT2[1].run.CurCnt[0],CNT_DISP_MAX,CHAR2040B,cnt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );			// 显示数量
			TFT1_FillBackPic(SS2_RUN_SKU_ROW,SS2_RUN_SKU_ROW+40,col0,SS2_RUN_COUNT_COL3);																// 后面填充背景
			// 单位
			if(strTFT2[1].run.CurCnt[0] == 'F' )	   // 满则不显示单位
			{ col0 = SS2_RUN_COUNT_COL3; }
			else
			{ col0 = TFT1_PutString_flash( SS2_RUN_UNI_ROW,SS2_RUN_COUNT_COL3,&strTFT2[1].run.CurUni[0],UNI_DISP_MAX,CHINA1616,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );	}		// 显示单位	
			TFT1_FillBackPic(SS2_RUN_UNI_ROW,SS2_RUN_UNI_ROW+18,col0,COL);															// 后面填充背景	
		}

		//TFT1_Draw_Line(232,232,0,COL,btxt_colour_tab[bk_colour]);				 // 画双屏分界线
		TFT1_Draw_Line(0,COL,200,200,btxt_colour_tab[bk_colour]);				 // 画双屏分界线
	}
}
#endif
/************************************************************************
* LCD 界面 	   运行显示界面
************************************************************************/
void Page_Run(void)
{
	INT8U	i,j,k,cnt;

	//====================================================================================
	// 取任务1信息
	// 取品名信息
	for(i = 0; i < NAME_MAX; i++)			   //20
	{ 
		strTFT1[0].run.Name[i] = UnfinishTask.info[0].srcode[i];
		
		if(UnfinishTask.info[0].srcode[i] == 0) break;		 		
	}
	cnt = i+1;	 							// 对应到描述信息存储的位置
		
	// 取描述信息
	strTFT1[0].run.SrcInfo[0][0] = strTFT1[0].run.SrcInfo[1][0] = strTFT1[0].run.SrcInfo[2][0] = 0;
	for(i = j = k =  0; i < SRC_INFO_MAX*3; i++)
	{ 	// j为不同的行
		strTFT1[0].run.SrcInfo[j][k] = UnfinishTask.info[0].srcode[cnt+i];//task_un_srcode[0][cnt+i];
		// 0结束
		if(UnfinishTask.info[0].srcode[cnt+i] == 0){	break;	}
		if((UnfinishTask.info[0].srcode[cnt+i] == 0x0D)&&(UnfinishTask.info[0].srcode[cnt+i+1] == 0x0A))    // 0D，0A 换行
		{ strTFT1[0].run.SrcInfo[j][k] = 0; k = 0; j++; i++; }
		else{k++;}	
	}

	cnt += i;							                        // 对应到方向信息存储的位置
	
	strTFT2[0].run.CurDir[0] = UnfinishTask.info[0].dir;        // 取方向信息			
	strTFT2[0].run.CurUni[0] = UnfinishTask.info[0].uni[0];     // 取单位信息
	strTFT2[0].run.CurUni[1] = UnfinishTask.info[0].uni[1];
	Ku_Hex16ToASCII2(&UnfinishTask.info[0],0);                  // 取库位
	Count_WordToASCII5(UnfinishTask.info[0].count,0);           // 取物品数量 

	if( Key.f_IsFnKeyPressed1 == ON )
	{	
		strTFT2[0].run.CurCnt[0] = strTFT2[0].run.CurCnt[1] = strTFT2[0].run.CurCnt[2] = strTFT2[0].run.CurCnt[3] = strTFT2[0].run.CurCnt[4] = '-';    //显示"----"     
	}
	else if(KeyMode.f_ChangeValue1 == ON)                       // 修改状态下且为灭
	{ 	
		if( strTFT2[0].run.CurCnt[0] == ' ' )	strTFT2[0].run.CurCnt[0] = '0';
		if( strTFT2[0].run.CurCnt[1] == ' ' )	strTFT2[0].run.CurCnt[1] = '0';
		if( strTFT2[0].run.CurCnt[2] == ' ' )	strTFT2[0].run.CurCnt[2] = '0';
		if( strTFT2[0].run.CurCnt[3] == ' ' )	strTFT2[0].run.CurCnt[3] = '0';

		if(LCD.f_DispBack == TRUE)
		{ 
            strTFT2[0].run.CurCnt[Disp.Focus] = ' '; 
        }
	}
	//----------------------------------------------------------------------------------------------------
	// 取任务2信息
	// 取品名信息
	for(i = 0; i < NAME_MAX; i++)			   //20
	{ 
		strTFT1[1].run.Name[i] = UnfinishTask.info[1].srcode[i];
		
		if(UnfinishTask.info[1].srcode[i] == 0)     // 0结束
            break;		 		
	}
	cnt = i+1;	 							// 对应到描述信息存储的位置

	strTFT1[1].run.SrcInfo[0][0] = strTFT1[1].run.SrcInfo[1][0] = strTFT1[1].run.SrcInfo[2][0] = 0;     // 取描述信息
	for(i = j = k =  0; i < SRC_INFO_MAX*3; i++)
	{ 	// j为不同的行
		strTFT1[1].run.SrcInfo[j][k] = UnfinishTask.info[1].srcode[cnt+i];//task_un_srcode[0][cnt+i];
		// 0结束
		if(UnfinishTask.info[1].srcode[cnt+i] == 0){	break;	}
		// 0D，0A 换行
		if((UnfinishTask.info[1].srcode[cnt+i] == 0x0D)&&(UnfinishTask.info[1].srcode[cnt+i+1] == 0x0A))
		{ strTFT1[1].run.SrcInfo[j][k] = 0; k = 0; j++; i++; }
		else{k++;}	
	}

	cnt += i;							   // 对应到方向信息存储的位置
	// 取方向信息
	strTFT2[1].run.CurDir[0] = UnfinishTask.info[1].dir;//UnfinishTask.info[1].srcode[cnt+1];//task_un_srcode[0][cnt+1];		
	// 取单位信息		
	strTFT2[1].run.CurUni[0] = UnfinishTask.info[1].uni[0];//UnfinishTask.info[1].srcode[cnt+2];//task_un_srcode[0][cnt+2];
	strTFT2[1].run.CurUni[1] = UnfinishTask.info[1].uni[1];//UnfinishTask.info[1].srcode[cnt+3];//task_un_srcode[0][cnt+3];
    
	Ku_Hex16ToASCII2(&UnfinishTask.info[1],1);      // 库位
 	
	Count_WordToASCII5(UnfinishTask.info[1].count,1);   // 物品数量  

	if( Key.f_IsFnKeyPressed2 == ON )       //显示"----"
	{	
		strTFT2[1].run.CurCnt[0] = strTFT2[1].run.CurCnt[1] = strTFT2[1].run.CurCnt[2] = strTFT2[1].run.CurCnt[3] = strTFT2[1].run.CurCnt[4] = '-';       
	}
	else if( (KeyMode.f_ChangeValue2 == ON) )   // 修改状态下且为灭
	{ 	
		if( strTFT2[1].run.CurCnt[0] == ' ' )	strTFT2[1].run.CurCnt[0] = '0';
		if( strTFT2[1].run.CurCnt[1] == ' ' )	strTFT2[1].run.CurCnt[1] = '0';
		if( strTFT2[1].run.CurCnt[2] == ' ' )	strTFT2[1].run.CurCnt[2] = '0';
		if( strTFT2[1].run.CurCnt[3] == ' ' )	strTFT2[1].run.CurCnt[3] = '0';

		if(LCD.f_DispBack == TRUE)
		{ strTFT2[1].run.CurCnt[Disp.Focus] = ' '; }
	}

	if( H_DISPLAY_MODE != DispDir.dir)  // 竖屏
	{  
		Page_Run_SP();	  // 竖屏显示效果
	}
	else     // 横屏
	{ 
		Page_Run_HP();	  // 横屏显示效果
	}
}

/************************************************************************
* LCD 界面 	   等待显示界面
************************************************************************/
#define		H_WAIT_NAME_ROW		123
#define		H_WAIT_NAME_UL		(H_WAIT_NAME_ROW+40+13)			 // 上线
#define		H_WAIT_NAME_DL		(H_WAIT_NAME_ROW-13)			 // 下线

#define		S_WAIT_NAME_ROW		S_FN_NAME_ROW//260
#define		S_WAIT_NAME_UL		S_FN_NAME_UL_ROW//(S_WAIT_NAME_ROW+40+13)			 // 上线
#define		S_WAIT_TASK_ROW		S_FN_VAL_ROW//210
#define		S_WAIT_NAME_DL		S_FN_NAME_DL_ROW//(S_WAIT_TASK_ROW-13)			 // 下线

void	Page_Wait(void)
{
	INT8U	i,order,tol;
	INT8U	*ptr;
	INT16U  col0;

	// 任务数/总任务数	
	UnfinishTask.order = UnfinishTask.total - UnfinishTask.remain - UnfinishTask.num;
	order = UnfinishTask.order+1;
	
  	if(order > 99)
	{ strTFT1[0].wait.TaskNum[0] = order/100 + '0'; strTFT1[0].wait.TaskNum[1] = order%100/10 + '0'; strTFT1[0].wait.TaskNum[2] = order%10 + '0'; i = 3; }
	else if(order > 9)
	{ strTFT1[0].wait.TaskNum[0] = order/10 + '0' ; strTFT1[0].wait.TaskNum[1] = order%10 + '0'; i = 2; }
	else 
	{ strTFT1[0].wait.TaskNum[0] = order + '0';  i = 1; }

	strTFT1[0].wait.TaskNum[i++] = '/';

	tol = UnfinishTask.total;	// 总任务数
	if(tol > 99)
	{ strTFT1[0].wait.TaskNum[i++] = tol/100 + '0'; strTFT1[0].wait.TaskNum[i++] = tol%100/10 + '0'; strTFT1[0].wait.TaskNum[i++] = tol%10 + '0'; }
	else if(tol > 9)
	{ strTFT1[0].wait.TaskNum[i++] = tol/10 + '0'; strTFT1[0].wait.TaskNum[i++] = tol%10 + '0';  }
	else 
	{ strTFT1[0].wait.TaskNum[i++] = tol + '0';   }

	ptr = &strTFT1[0].wait.TaskNum[0];			// 


	// 写数据进屏中
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(5); 						// 载入横屏运行图片
		//====================================================================================

//		TFT1_Draw_Line(S_WAIT_NAME_UL,S_WAIT_NAME_UL,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_WAIT_NAME_DL,S_WAIT_NAME_DL,0,COL,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_WAIT_NAME_UL,S_WAIT_NAME_UL,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_WAIT_NAME_DL,S_WAIT_NAME_DL,btxt_colour_tab[bk_colour]);

		// 提示等待中
//		col0 = TFT1_PutString_flash( S_WAIT_NAME_ROW,COL_S1,"等待中..."   ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_WAIT_NAME_ROW,COL_S1,"Waiting..."   ,11,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_WAIT_NAME_ROW,S_WAIT_NAME_ROW+42,col0,COL);						// 后面填充背景
		col0 = TFT1_PutString_flash( S_WAIT_TASK_ROW,COL_S1  ,ptr          ,i,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		// 显示任务数
		TFT1_FillBackPic(S_WAIT_TASK_ROW,S_WAIT_TASK_ROW+42,col0,COL);						// 后面填充背景

	}
	else
	{
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(0); 						// 载入横屏运行图片
		//====================================================================================

		TFT1_Draw_Line(H_WAIT_NAME_UL,H_WAIT_NAME_UL,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_WAIT_NAME_DL,H_WAIT_NAME_DL,0,ROW,btxt_colour_tab[bk_colour]);

		// 提示等待中
//		col0 = TFT1_PutString_flash( H_WAIT_NAME_ROW,COL_S1,"等待中...         "   ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_WAIT_NAME_ROW,COL_S1,"Waiting...         "   ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_WAIT_NAME_ROW,col0  ,ptr                     ,i,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		// 显示任务数
		TFT1_FillBackPic(H_WAIT_NAME_ROW,H_WAIT_NAME_ROW+42,col0,ROW);													// 后面填充背景
	}
}

/************************************************************************
* LCD 界面 		信息屏界面
************************************************************************/

void	Page_Info(void)
{
	INT8U i,cnt,j,k;
	INT16U  col0;
	INT8U	pic_num, info_num;
	INT16U	row_start[5],col_start[5],col_max;
	//====================================================================================
	// 取信息1
	// 取标题信息
	for(i = 0; i < NAME_MAX; i++)			   //20
	{ 
		strTFT1[0].info.NameM[i] = InfoDisp.srcode[i];
		// 0结束
		if(InfoDisp.srcode[i] == 0) break;		 		
	}
	cnt = i+1;	 							// 对应到描述信息存储的位置
		
	// 取副标题信息
	for(i = 0; i < NAME_MAX; i++)			   //20
	{ 
		strTFT1[0].info.NameS[i] = InfoDisp.srcode[cnt+i];
		// 0结束
		if(InfoDisp.srcode[cnt+i] == 0) break;		 		
	}
	cnt += (i+1);	 							

	// 取描述信息
	strTFT1[0].info.SrcInfo[0][0] = strTFT1[0].info.SrcInfo[1][0] = strTFT1[0].info.SrcInfo[2][0] = 0;
	for(i = j = k =  0; i < SRC_INFO_MAX*3; i++)
	{ 	// j为不同的行
		strTFT1[0].info.SrcInfo[j][k] = InfoDisp.srcode[cnt+i];		// 对应到描述信息存储的位置
		// 0结束
		if(InfoDisp.srcode[cnt+i] == 0){ break;	}
		// 0D，0A 换行
		if( (InfoDisp.srcode[cnt+i] == 0x0D) && (InfoDisp.srcode[cnt+i+1] == 0x0A) )
		{ strTFT1[0].info.SrcInfo[j][k] = 0; k = 0; j++; i++; }
		else{k++;}	
	}

	cnt += (i+1);							   						// 对应到方向信息存储的位置
	// 取图片信息
	strTFT2[0].info.picture = InfoDisp.srcode[cnt];				

	// 限定范围
	if( strTFT2[0].info.picture > 29 ){ strTFT2[0].info.picture = 0; }
	//====================================================================================
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		pic_num = 5;	  info_num = 4;
		row_start[0] = S_INFO_NAMEM_ROW; col_start[0] = COL_S1;
		row_start[1] = S_INFO_NAMES_ROW; col_start[1] = COL_S1;
		row_start[2] = S_INFO_SRC1_ROW;  col_start[2] = COL_S1;
		row_start[3] = S_INFO_SRC2_ROW;  col_start[3] = COL_S1;
		row_start[4] = S_INFO_SRC3_ROW;  col_start[4] = COL_S1;
		col_max = COL;
	}
	else
	{ 	// 横屏
		pic_num = 0;	  info_num = 3;
		row_start[0] = H_INFO_NAMEM_ROW; col_start[0] = COL_S1;
		row_start[1] = H_INFO_NAMES_ROW; col_start[1] = COL_S1;
		row_start[2] = H_INFO_SRC1_ROW;  col_start[2] = COL_S1;
		row_start[3] = H_INFO_SRC2_ROW;  col_start[3] = COL_S1;
		row_start[4] = H_INFO_SRC3_ROW;  col_start[4] = COL_S1;
		col_max = ROW;
		if( strTFT2[0].info.picture != 0)
		{ col_start[0] = col_start[1] = H_INFO_COL; }
	}

	// 滚屏模式加快速度,不再更新部分数据
	if( (DispRoll.f_Rolling1 == ROLL_MODE_OFF) )
	{
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(pic_num); 						// 载入横屏运行图片
		//====================================================================================

 		if( strTFT2[0].info.picture != 0 )
		{ TFT1_Display_MCUFlash_Picture(FullImageConst[info_num],strTFT2[0].info.picture); }
	}
	//====================================================================================
	// 信息存入滚屏区
	if((DispRoll.f_Rolling1 & ROLL_MODE_NAME) == 0x00)
	{  	// 无滚屏标志 
		TFT1_DispInfoMNameDataChgSave();		  // 主标题信息存储
	}
	if((DispRoll.f_Rolling1 & ROLL_MODE_SNAME) == 0x00)
	{  	// 无滚屏标志 
		TFT1_DispInfoSNameDataChgSave();		  // 副标题信息存储
	}
	if((DispRoll.f_Rolling[0] & (ROLL_MODE_SRC1|ROLL_MODE_SRC2|ROLL_MODE_SRC3)) == 0x00)
	{
		TFT1_DispInfoSrcDataChgSave();			  // 描述信息存储
	}
	//====================================================================================
	// 主标题
	if((DispRoll.f_Rolling1 & ROLL_MODE_NAME) == 0x00)
	{
		col0 = TFT1_PutString_flash( row_start[0], col_start[0], &strTFT1[0].info.NameM[0]  ,  NAME_MAX,      CHINA3232,  txt_colour_tab[bk_colour], bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
		TFT1_FillBackPic(row_start[0], row_start[0]+32, col0, col_max);															// 后面填充背景	
	}
	else { TFT1_InfoNameRoll(ONLY_TASK_MODE,txt_colour_tab[bk_colour] ); }	

	// 付标题
	if((DispRoll.f_Rolling1 & ROLL_MODE_SNAME) == 0x00)
	{  	// 无滚屏标志 
		col0 = TFT1_PutString_flash( row_start[1], col_start[1], &strTFT1[0].info.NameS[0]    , NAME_MAX,     CHINA2424,  dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
		TFT1_FillBackPic(row_start[1], row_start[1]+24, col0, col_max);														   // 后面填充背景
	}
	else { TFT1_InfoNameRoll(DOUBLE_TASK1_MODE,dsc_colour_tab[bk_colour] ); }
	
	// 描述区1
	if((DispRoll.f_Rolling1 & ROLL_MODE_SRC1) == 0x00)
	{  	// 无滚屏标志
		col0 = TFT1_PutString_flash( row_start[2], col_start[2], &strTFT1[0].info.SrcInfo[0][0], SRC_INFO_MAX, CHINA2424L, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
		TFT1_FillBackPic(row_start[2], row_start[2]+24, col0, col_max);
	}

	// 描述区2
	if((DispRoll.f_Rolling1 & ROLL_MODE_SRC2) == 0x00)
	{  	// 无滚屏标志
		col0 = TFT1_PutString_flash( row_start[3], col_start[3], &strTFT1[0].info.SrcInfo[1][0], SRC_INFO_MAX, CHINA2424L, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
		TFT1_FillBackPic(row_start[3], row_start[3]+24, col0, col_max);
	}

	// 描述区3
	if((DispRoll.f_Rolling1 & ROLL_MODE_SRC3) == 0x00)
	{  	// 无滚屏标志
		col0 = TFT1_PutString_flash( row_start[4], col_start[4] ,&strTFT1[0].info.SrcInfo[2][0], SRC_INFO_MAX, CHINA2424L, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour] );	// 品名信息，数组最后的0作为结束字符
		TFT1_FillBackPic(row_start[4],row_start[4]+24,col0,col_max);
	}
}
/************************************************************************
* LCD 界面 
************************************************************************/

void	Page_CFGPWD(void)
{
	INT16U	col0;

	strTFT2[0].Cfg.Addr[0] = Key.CfgPwdCnt+0x30;

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(7); 						// 载入横屏运行图片
		//====================================================================================
//		TFT1_Draw_Line(S_FN_NAME_UL_ROW,S_FN_NAME_UL_ROW,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_FN_NAME_DL_ROW,S_FN_NAME_DL_ROW,0,COL,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_FN_NAME_UL_ROW,S_FN_NAME_UL_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_FN_NAME_DL_ROW,S_FN_NAME_DL_ROW,btxt_colour_tab[bk_colour]);		

//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"请输入密码"            ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"Password"            ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"按4次\"<>\"键可进入"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"Press \"<>\" key 4 times,"   ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"参数设置界面"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"enter parameter setting"   ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);																	// 后面填充背景
		
		// 显示数值
	 	col0 = TFT1_PutString_flash( S_FN_VAL_ROW,COL_S1,strTFT2[0].Cfg.Addr,1,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );							// 显示数值
		TFT1_FillBackPic(S_FN_VAL_ROW, S_FN_VAL_ROW+40,col0,COL);		// 后面填充背景	
	}
	else
	{  // 横屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(2); 						// 载入横屏运行图片
		//====================================================================================
		TFT1_Draw_Line(H_FN_NAME_UL_ROW,H_FN_NAME_UL_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_FN_NAME_DL_ROW,H_FN_NAME_DL_ROW,0,ROW,btxt_colour_tab[bk_colour]);

//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW, COL_S1,"请输入密码"                         ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW, COL_S1,"Password"                         ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"按4次\"<>\"键可进入参数设置界面"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"Press \"<>\" key 4 times,"   ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);		// 后面填充背景

		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"enter parameter setting"    ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);		// 后面填充背景
		
		// 显示数值		
		col0 = TFT1_PutString_flash( H_FN_VAL_ROW,H_FN_VAL_COL,strTFT2[0].Cfg.Addr,1,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );							// 显示数值
		TFT1_FillBackPic(H_FN_VAL_ROW, H_FN_VAL_ROW+40,col0,ROW);		// 后面填充背景	
	}
}
/************************************************************************
* LCD 界面 
************************************************************************/

void	Page_BackDisplay(void)
{
	INT8U i,ord,dir;
	INT16U color[3][ PAGE_MAX_BACK_NUM ],row,col,col0;
	
	if(BackTask.index  == 0) return;

 	if( H_DISPLAY_MODE != DispDir.dir) 
	{ // 竖屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(5); 						// 载入横屏运行图片
		//====================================================================================

// 		TFT1_Draw_Line(S_BACK_LINE1_ROW,S_BACK_LINE1_ROW,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_BACK_LINE2_ROW,S_BACK_LINE2_ROW,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_BACK_LINE3_ROW,S_BACK_LINE3_ROW,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_BACK_LINE4_ROW,S_BACK_LINE4_ROW,0,COL,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(S_BACK_LINE5_ROW,S_BACK_LINE5_ROW,0,COL,btxt_colour_tab[bk_colour]);
		
		TFT1_Draw_Line(0,COL,S_BACK_LINE1_ROW,S_BACK_LINE1_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_BACK_LINE2_ROW,S_BACK_LINE2_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_BACK_LINE3_ROW,S_BACK_LINE3_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_BACK_LINE4_ROW,S_BACK_LINE4_ROW,btxt_colour_tab[bk_colour]);
		dir = 1;
	}
	else
	{ // 横屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(0); 						// 载入横屏运行图片
		//====================================================================================

 		TFT1_Draw_Line(H_BACK_LINE1_ROW,H_BACK_LINE1_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_BACK_LINE2_ROW,H_BACK_LINE2_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_BACK_LINE3_ROW,H_BACK_LINE3_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_BACK_LINE4_ROW,H_BACK_LINE4_ROW,0,ROW,btxt_colour_tab[bk_colour]);
//		TFT1_Draw_Line(H_BACK_LINE5_ROW,H_BACK_LINE5_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		dir = 0;
	}


	ord = BackTask.index - 1;
	if(ord >= MAX_BACK_NUM){ ord = MAX_BACK_NUM-1; }
	
	for(i = 0;i < PAGE_MAX_BACK_NUM;i++)	
	{
		if(i != ord)
		{
			color[0][i] = btxt_colour_tab[bk_colour];//bGRAY;		 // 深灰
			color[1][i] = bsku_colour_tab[bk_colour];//bGREEN;	// 深绿
			color[2][i] = bcnt_colour_tab[bk_colour];//bRED;		 // 深红
		}
		else
		{// 高亮显示
			color[0][i] = txt_colour_tab[bk_colour];//bGRAY;		 // 深灰
			color[1][i] = sku_colour_tab[bk_colour];//bGREEN;	// 深绿
			color[2][i] = cnt_colour_tab[bk_colour];//bRED;		 // 深红
		}
	}

//	换页用的
	if(ord >= 5){ord = 5;}
	else{ord = 0;}
	
	TFT1_DispBackDataChgSave();

	for(i = 0;i < PAGE_MAX_BACK_NUM;i++,ord++)
	{	
//		if(ord < DispContentStackCount)
		if(ord < BackTask.num)
	   	{
 			// 库位区
			col = DispBackSkuCol[dir];
			row = DispBackSkuRow[dir][i];
//			Ku_Hex16ToASCII2(BackTask.info[ord].sku,0);																				// 库位数据处理
			Ku_Hex16ToASCII2(&BackTask.info[ord],0);																				// 库位数据处理
			col0 = TFT1_PutString_flash( row, col  ,&strTFT2[0].run.CurSku[0]         ,SKU_DISP_MAX  ,CHINA2424,color[1][i],bk_colour_tab[bk_colour] );	// 库位区域更新	

			// 数量区	12*5
			col =  col0 + 24;//88;	 // 库位
			TFT1_FillBackPic(row,row+24,col0,col);																		// 后面填充背景
			Count_WordToASCII5(BackTask.info[ord].count,0);																			// 数量数据处理
			col0 = TFT1_PutString_flash( row, col,&strTFT2[0].run.CurCnt[0]           ,CNT_DISP_MAX  ,CHINA2424,color[2][i],bk_colour_tab[bk_colour] );	// 数量区域更新

			// 单位区
			col =  col0 + 4;//186;  // 数量宽度12*5 = 60
			TFT1_FillBackPic(row,row+24,col0,col);															// 后面填充背景
			strTFT2[0].run.CurUni[0] = BackTask.info[ord].uni[0];      
			strTFT2[0].run.CurUni[1] = BackTask.info[ord].uni[1];
//			strTFT2[0].run.CurUni[2] = NULL;
			col0 = TFT1_PutString_flash( row+5, col,&strTFT2[0].run.CurUni[0]         ,UNI_DISP_MAX  ,CHINA1616,color[0][i],bk_colour_tab[bk_colour] );	// 单位区域更新

			TFT1_FillBackPic(row,row+24,col0,DispBackUniCol[dir]);																	// 后面填充背景
			// 品名区
			if((DispRoll.f_Rolling2 & Roll_Back_Flag[i]) == 0x00)
			{

				row = DispBackNameRow[dir][i]; 
				col = DispBackNameCol[dir]; 

				col0 = TFT1_PutString_flash( row, col ,&BackTask.info[ord].srcode[0],30 ,CHINA2424,color[0][i],bk_colour_tab[bk_colour] );	// 品名
				TFT1_FillBackPic(row,row+26,col0,DispBackNameEndCol[dir]);																	// 后面填充背景
			}
		}
//		else
// 	   	{  // 填背景
//			row = TFT1BackDisplay[dir][i];
//		   	TFT1_FillBackPic(row,row+30,COL_S1-1,ROW-5);
//		}
	}	
}


/************************************************************************
* 配置选择共用  
************************************************************************/
#if TFT_TYPE == TFT_HJ28_9327
void	Page_Cfg_Common(INT8U  sel)
{

	INT8U	i,dat = 0;
	INT16U col0;

	// 找出对应的数值
	if(sel == PAGE_CFGADDR)
	{
		i = 3;			   // 显示的数组个数

		strTFT2[0].Cfg.Addr[0] = para_bk.str.address/100 + '0'; 
		strTFT2[0].Cfg.Addr[1] = para_bk.str.address%100/10 + '0';  
		strTFT2[0].Cfg.Addr[2] = para_bk.str.address%10 + '0';	  

		if( (LCD.f_DispBack == TRUE) )
		{ strTFT2[0].Cfg.Addr[Disp.Focus] = ' '; } // 修改状态下且为灭
	}
	else if(sel == PAGE_CFGFUNC1)
	{
		i = 2;			   // 显示的数组个数

		strTFT2[0].Cfg.Addr[0] = (para_bk.str.FinishedLED >> 4 ) + '0'; 
		strTFT2[0].Cfg.Addr[1] = (para_bk.str.FinishedLED & 0x0F) + '0';    

		if( (LCD.f_DispBack == TRUE) )
		{ strTFT2[0].Cfg.Addr[Disp.Focus] = ' '; } // 修改状态下且为灭
	}
	else
	{			
		i = 1;				  // 显示的数组个数
		if( LCD.f_DispBack == TRUE )
			{ strTFT2[0].Cfg.Addr[0] = ' '; }
		else
		{
//			if		(sel == PAGE_CFGFUNC1)
//			{ dat = para_bk.str.FinishedLED;	}
//			else 
			if	(sel == PAGE_CFGFUNC2)
			{ dat = para_bk.str.WtTime;			}
			else if	(sel == PAGE_CFGFUNC3)
			{ dat = para_bk.str.FnDefine;		}
			else if	(sel == PAGE_CFGFUNC4)
			{ dat = para_bk.str.DispMode;		}
			else
			{ dat = para_bk.str.KeyProssLED;	}

			strTFT2[0].Cfg.Addr[0] = dat + '0';			
		}
	}

	// 显示
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(7); 						// 载入横屏运行图片
		//====================================================================================

		TFT1_Draw_Line(0,COL,S_FN_NAME_UL_ROW,S_FN_NAME_UL_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_FN_NAME_DL_ROW,S_FN_NAME_DL_ROW,btxt_colour_tab[bk_colour]);

//		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_AD,"地址" ,2,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_AD,"ADR"   ,3,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F1,"F1"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F2,"F2"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F3,"F3"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL2, S_FN_SEL2_F4,"F4"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );

		if(sel == PAGE_CFGADDR)
			  //{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_AD_L, S_FN_SEL1_AD_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_AD_L,S_FN_SEL1_AD_R,S_FN_SEL1_DL,S_FN_SEL1_UL, dsc_colour_tab[bk_colour] ); }
		else  	{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_AD_L, S_FN_SEL1_AD_R ); }
			  //{  Clr_Kuang( S_FN_SEL1_AD_L,S_FN_SEL1_AD_R,S_FN_SEL1_DL,S_FN_SEL1_UL ); }

		if(sel == PAGE_CFGFUNC1)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_F1_L,S_FN_SEL1_F1_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F1_L,S_FN_SEL1_F1_R,S_FN_SEL1_DL,S_FN_SEL1_UL, dsc_colour_tab[bk_colour] ); }
		else	{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_F1_L,S_FN_SEL1_F1_R ); }
			//	{  Clr_Kuang( S_FN_SEL1_F1_L,S_FN_SEL1_F1_R,S_FN_SEL1_DL,S_FN_SEL1_UL ); }

		if(sel == PAGE_CFGFUNC2)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F2_L,S_FN_SEL1_F2_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F2_L,S_FN_SEL1_F2_R,S_FN_SEL1_DL,S_FN_SEL1_UL, dsc_colour_tab[bk_colour] ); }
		else	{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F2_L,S_FN_SEL1_F2_R); }
			//	{  Clr_Kuang( S_FN_SEL1_F2_L,S_FN_SEL1_F2_R,S_FN_SEL1_DL,S_FN_SEL1_UL ); }

		if(sel == PAGE_CFGFUNC3)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F3_L,S_FN_SEL1_F3_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F3_L,S_FN_SEL1_F3_R,S_FN_SEL1_DL,S_FN_SEL1_UL, dsc_colour_tab[bk_colour] ); }
		else	{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F3_L,S_FN_SEL1_F3_R ); }
			//	{  Clr_Kuang( S_FN_SEL1_F3_L,S_FN_SEL1_F3_R,S_FN_SEL1_DL,S_FN_SEL1_UL ); }

		if(sel == PAGE_CFGFUNC4)
			//{ Draw_Kuang( S_FN_SEL2_DL,S_FN_SEL2_UL,S_FN_SEL2_F4_L,S_FN_SEL2_F4_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL2_F4_L,S_FN_SEL2_F4_R,S_FN_SEL2_DL,S_FN_SEL2_UL, dsc_colour_tab[bk_colour] ); }
		else	{  Clr_Kuang( S_FN_SEL2_DL,S_FN_SEL2_UL,S_FN_SEL2_F4_L,S_FN_SEL2_F4_R ); }
			//	{  Clr_Kuang( S_FN_SEL2_F4_L,S_FN_SEL2_F4_R,S_FN_SEL2_DL,S_FN_SEL2_UL ); }

		// 显示数值
	 	col0 = TFT1_PutString_flash( S_FN_VAL_ROW,COL_S1,strTFT2[0].Cfg.Addr,i,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_VAL_ROW, S_FN_VAL_ROW+40,col0,COL);		// 后面填充背景		
//		TFT1_FillBackPic(col0,COL, S_FN_VAL_ROW, S_FN_VAL_ROW+40);		// 后面填充背景				
	
	}
	else
	{  // 横屏	
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(2); 						// 载入横屏运行图片
		//====================================================================================

		TFT1_Draw_Line(H_FN_NAME_UL_ROW,H_FN_NAME_UL_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_FN_NAME_DL_ROW,H_FN_NAME_DL_ROW,0,ROW,btxt_colour_tab[bk_colour]);

//		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_AD,"地址" ,2,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_AD,"ADR" ,3,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F1,"F1"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F2,"F2"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F3,"F3"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F4,"F4"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );

		if(sel == PAGE_CFGADDR)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_AD_L, H_FN_SEL_AD_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_AD_L, H_FN_SEL_AD_R ); }

		if(sel == PAGE_CFGFUNC1)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_F1_L,H_FN_SEL_F1_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_F1_L,H_FN_SEL_F1_R ); }

		if(sel == PAGE_CFGFUNC2)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F2_L,H_FN_SEL_F2_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F2_L,H_FN_SEL_F2_R); }

		if(sel == PAGE_CFGFUNC3)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F3_L,H_FN_SEL_F3_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F3_L,H_FN_SEL_F3_R ); }

		if(sel == PAGE_CFGFUNC4)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F4_L,H_FN_SEL_F4_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F4_L,H_FN_SEL_F4_R ); }

		// 显示数值
		col0 = TFT1_PutString_flash( H_FN_VAL_ROW,H_FN_VAL_COL,strTFT2[0].Cfg.Addr,i,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );							// 显示数值
		TFT1_FillBackPic(H_FN_VAL_ROW, H_FN_VAL_ROW+40,col0,ROW);		// 后面填充背景		
	}
}
#else
void	Page_Cfg_Common(INT8U  sel)
{

	INT8U	i,dat = 0;
	INT16U col0;

	// 找出对应的数值
	if(sel == PAGE_CFGADDR)
	{
		i = 3;			   // 显示的数组个数

		strTFT2[0].Cfg.Addr[0] = para_bk.str.address/100 + '0'; 
		strTFT2[0].Cfg.Addr[1] = para_bk.str.address%100/10 + '0';  
		strTFT2[0].Cfg.Addr[2] = para_bk.str.address%10 + '0';	  

		if( (LCD.f_DispBack == TRUE) )
		{ strTFT2[0].Cfg.Addr[Disp.Focus] = ' '; } // 修改状态下且为灭
	}
	else if(sel == PAGE_CFGFUNC1)
	{
		i = 2;			   // 显示的数组个数

		strTFT2[0].Cfg.Addr[0] = (para_bk.str.FinishedLED >> 4 ) + '0'; 
		strTFT2[0].Cfg.Addr[1] = (para_bk.str.FinishedLED & 0x0F) + '0';    

		if( (LCD.f_DispBack == TRUE) )
		{ strTFT2[0].Cfg.Addr[Disp.Focus] = ' '; } // 修改状态下且为灭
	}
	else
	{			
		i = 1;				  // 显示的数组个数
		if( LCD.f_DispBack == TRUE )
			{ strTFT2[0].Cfg.Addr[0] = ' '; }
		else
		{
//			if		(sel == PAGE_CFGFUNC1)
//			{ dat = para_bk.str.FinishedLED;	}
//			else 
			if	(sel == PAGE_CFGFUNC2)
			{ dat = para_bk.str.WtTime;			}
			else if	(sel == PAGE_CFGFUNC3)
			{ dat = para_bk.str.FnDefine;		}
			else if	(sel == PAGE_CFGFUNC4)
			{ dat = para_bk.str.DispMode;		}
			else
			{ dat = para_bk.str.KeyProssLED;	}

			strTFT2[0].Cfg.Addr[0] = dat + '0';			
		}
	}

	// 显示
	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(7); 						// 载入横屏运行图片
		//====================================================================================

		TFT1_Draw_Line(0,COL,S_FN_NAME_UL_ROW,S_FN_NAME_UL_ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(0,COL,S_FN_NAME_DL_ROW,S_FN_NAME_DL_ROW,btxt_colour_tab[bk_colour]);

//		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_AD,"地址" ,2,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_AD,"ADR"   ,3,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F1,"F1"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F2,"F2"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL1, S_FN_SEL1_F3,"F3"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( S_FN_SEL2, S_FN_SEL2_F4,"F4"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );

		if(sel == PAGE_CFGADDR)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_AD_L, S_FN_SEL1_AD_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_AD_L,S_FN_SEL1_AD_R,S_FN_SEL1_UL,S_FN_SEL1_DL, dsc_colour_tab[bk_colour] ); }
		else//{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_AD_L, S_FN_SEL1_AD_R ); }
				{  Clr_Kuang( S_FN_SEL1_UL,S_FN_SEL1_DL,S_FN_SEL1_AD_L,S_FN_SEL1_AD_R ); }

		if(sel == PAGE_CFGFUNC1)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_F1_L,S_FN_SEL1_F1_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F1_L,S_FN_SEL1_F1_R,S_FN_SEL1_UL,S_FN_SEL1_DL, dsc_colour_tab[bk_colour] ); }
		else//{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL, S_FN_SEL1_F1_L,S_FN_SEL1_F1_R ); }
				{  Clr_Kuang( S_FN_SEL1_UL,S_FN_SEL1_DL,S_FN_SEL1_F1_L,S_FN_SEL1_F1_R ); }

		if(sel == PAGE_CFGFUNC2)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F2_L,S_FN_SEL1_F2_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F2_L,S_FN_SEL1_F2_R,S_FN_SEL1_UL,S_FN_SEL1_DL, dsc_colour_tab[bk_colour] ); }
		else//{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F2_L,S_FN_SEL1_F2_R); }
				{  Clr_Kuang( S_FN_SEL1_UL,S_FN_SEL1_DL,S_FN_SEL1_F2_L,S_FN_SEL1_F2_R ); }

		if(sel == PAGE_CFGFUNC3)
			//{ Draw_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F3_L,S_FN_SEL1_F3_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL1_F3_L,S_FN_SEL1_F3_R,S_FN_SEL1_UL,S_FN_SEL1_DL, dsc_colour_tab[bk_colour] ); }
		else//{  Clr_Kuang( S_FN_SEL1_DL,S_FN_SEL1_UL,S_FN_SEL1_F3_L,S_FN_SEL1_F3_R ); }
				{  Clr_Kuang( S_FN_SEL1_UL,S_FN_SEL1_DL,S_FN_SEL1_F3_L,S_FN_SEL1_F3_R ); }

		if(sel == PAGE_CFGFUNC4)
			//{ Draw_Kuang( S_FN_SEL2_DL,S_FN_SEL2_UL,S_FN_SEL2_F4_L,S_FN_SEL2_F4_R,dsc_colour_tab[bk_colour] ); }
				{ Draw_Kuang( S_FN_SEL2_F4_L,S_FN_SEL2_F4_R,S_FN_SEL2_UL,S_FN_SEL2_DL, dsc_colour_tab[bk_colour] ); }
		else//{  Clr_Kuang( S_FN_SEL2_DL,S_FN_SEL2_UL,S_FN_SEL2_F4_L,S_FN_SEL2_F4_R ); }
				{  Clr_Kuang( S_FN_SEL2_UL,S_FN_SEL2_DL ,S_FN_SEL2_F4_L,S_FN_SEL2_F4_R ); }

		// 显示数值
	 	col0 = TFT1_PutString_flash( S_FN_VAL_ROW,COL_S1,strTFT2[0].Cfg.Addr,i,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_VAL_ROW, S_FN_VAL_ROW+40,col0,COL);		// 后面填充背景		
//		TFT1_FillBackPic(col0,COL, S_FN_VAL_ROW, S_FN_VAL_ROW+40);		// 后面填充背景				
	
	}
	else
	{  // 横屏	
		//====================================================================================
		// 载入背景，可能是图片也可能是颜色
		Load_Picture(2); 						// 载入横屏运行图片
		//====================================================================================

		TFT1_Draw_Line(H_FN_NAME_UL_ROW,H_FN_NAME_UL_ROW,0,ROW,btxt_colour_tab[bk_colour]);
		TFT1_Draw_Line(H_FN_NAME_DL_ROW,H_FN_NAME_DL_ROW,0,ROW,btxt_colour_tab[bk_colour]);

//		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_AD,"地址" ,2,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_AD,"ADR" ,3,CHINA2424,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F1,"F1"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F2,"F2"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F3,"F3"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_PutString_flash( H_FN_SEL, H_FN_SEL_F4,"F4"   ,2,CHAR1324 ,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );

		if(sel == PAGE_CFGADDR)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_AD_L, H_FN_SEL_AD_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_AD_L, H_FN_SEL_AD_R ); }

		if(sel == PAGE_CFGFUNC1)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_F1_L,H_FN_SEL_F1_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL, H_FN_SEL_F1_L,H_FN_SEL_F1_R ); }

		if(sel == PAGE_CFGFUNC2)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F2_L,H_FN_SEL_F2_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F2_L,H_FN_SEL_F2_R); }

		if(sel == PAGE_CFGFUNC3)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F3_L,H_FN_SEL_F3_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F3_L,H_FN_SEL_F3_R ); }

		if(sel == PAGE_CFGFUNC4)
			{ Draw_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F4_L,H_FN_SEL_F4_R,dsc_colour_tab[bk_colour] ); }
		else{  Clr_Kuang( H_FN_SEL_DL,H_FN_SEL_UL,H_FN_SEL_F4_L,H_FN_SEL_F4_R ); }

		// 显示数值
		col0 = TFT1_PutString_flash( H_FN_VAL_ROW,H_FN_VAL_COL,strTFT2[0].Cfg.Addr,i,CHAR2040B,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );							// 显示数值
		TFT1_FillBackPic(H_FN_VAL_ROW, H_FN_VAL_ROW+40,col0,ROW);		// 后面填充背景		
	}
}

#endif
/************************************************************************
* LCD 界面 
************************************************************************/

void	Page_CfgAddr(void)
{	
	INT16U	col0;
	

	// TFT1显示
	Page_Cfg_Common(PAGE_CFGADDR);

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"设备地址"           ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
			col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"Address"           ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"说明:"   			,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"Remark:"   			,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 地址范围,1~247"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"* Range,1~247"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: 禁止使用0"       ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"* Disable 0 address"       ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC3_ROW,S_FN_SRC3_ROW+26,col0,COL);																	// 后面填充背景


		TFT1_FillBackPic(S_FN_SRC4_ROW,S_FN_SRC4_ROW+26,0,COL);																		// 后面填充背景	
	}
	else
	{  // 横屏

//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"设备地址"           ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"Address"           ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);																	// 后面填充背景


//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"说明:"              ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"Remark:"              ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);																	// 后面填充背景


//		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: 地址范围,1~247"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"* Range,1~247"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);																	// 后面填充背景


//		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"2: 禁止使用0"       ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"* Disable 0 address"       ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC3_ROW,H_FN_SRC3_ROW+26,col0,ROW);																	// 后面填充背景
	}
	// TFT2显示	
//	Cfg_Value_Disp(PAGE_CFGADDR);
}
/************************************************************************
* LCD 界面 
************************************************************************/
void	Page_CfgFunc1(void)
{
	INT16U	col0;
	
	// TFT1显示
	Page_Cfg_Common(PAGE_CFGFUNC1);

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"完成指示灯"            ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"Finish Light"    ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: 灯灭     4: 黄色"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: Off        4: Yellow"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 红色     5: 紫色"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: Red      5: Magenta"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: 绿色     6: 青色"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: Green   6: Cyan"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC3_ROW,S_FN_SRC3_ROW+26,col0,COL);																	// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"3: 蓝色     7: 白色"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"3: Blue      7: White"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC4_ROW,S_FN_SRC4_ROW+26,col0,COL);																	// 后面填充背景	
	}
	else
	{  // 横屏

//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW, COL_S1,"完成指示灯"                         ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW, COL_S1,"Finish Light"                 ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: 灯灭　   3: 蓝色     6: 青色 "   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: Off　      3: Blue          6: Cyan",45,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: 红色　   4: 黄色     7: 白色"    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: Red　    4: Yellow       7: White" ,45,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"2: 绿色　   5: 紫色     "           ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"2: Green　 5: Magenta     "           ,45,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC3_ROW,H_FN_SRC3_ROW+26,col0,ROW);		// 后面填充背景	
	}
	// TFT2显示	
//	Cfg_Value_Disp(PAGE_CFGFUNC1);
}
/************************************************************************
* LCD 界面 
************************************************************************/
void	Page_CfgFunc2(void)
{
	INT16U	col0;

	// TFT1显示
	Page_Cfg_Common(PAGE_CFGFUNC2);				 // 功能区选择

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏
//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"等待时间"              ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_NAME_ROW, COL_S1,"Waiting Time"           ,15,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: 100毫秒  3: 400毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: 100ms   3: 400ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 200毫秒  4: 500毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 200ms   4: 500ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: 300毫秒  5: 600毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: 300ms   5: 600ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC3_ROW,S_FN_SRC3_ROW+26,col0,COL);		// 后面填充背景

		TFT1_FillBackPic(S_FN_SRC4_ROW,S_FN_SRC4_ROW+26,0,COL);		// 后面填充背景	
	}
	else
	{  // 横屏
//		col0 = TFT1_PutString_flash( 123,COL_S1,"任务等待时间"               ,10,CHINA4040,WHITE1,BLACK,BK_FLASHPIC);
//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"等待时间"                     ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"Waiting Time"            ,15,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: 100毫秒           3: 400毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: 100ms           3: 400ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: 200毫秒           4: 500毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: 200ms           4: 500ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
 		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);		// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"2: 300毫秒           5: 600毫秒"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"2: 300ms           5: 600ms"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC3_ROW,H_FN_SRC3_ROW+26,col0,ROW);		// 后面填充背景
	}
	// TFT2显示	
//	Cfg_Value_Disp(PAGE_CFGFUNC2);
}
/************************************************************************
* LCD 界面 
************************************************************************/
void	Page_CfgFunc3(void)
{
	INT16U	col0;
		
	// TFT1显示
	Page_Cfg_Common(PAGE_CFGFUNC3);			// 功能区选择

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW,COL_S1,"Fn键长按"              ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_NAME_ROW,COL_S1,"\"Fun\" press 3s"     ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: 预留"              ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: Reserve"              ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 满箱操作,按下OK"   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: Full box operation, "   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"   键,信息清除,后续"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"press \"OK\" key,clear "  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC3_ROW,S_FN_SRC3_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"   任务停止"          ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"display and tasks stop"          ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC4_ROW,S_FN_SRC4_ROW+26,col0,COL);			// 后面填充背景	
	}
	else
	{  // 横屏
//		col0 = TFT1_PutString_flash( 123,COL_S1,"Fn键长按定义"   		   ,10,CHINA4040,WHITE1,BLACK,BK_FLASHPIC);
//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"Fn键长按"   		        ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"\"Fun\" press 3s"   	,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( 75, COL_S1,"0: 拣货数量调整为0"   ,11,CHINA2424L,WHITE1,BLACK);
//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: 预留"   			     ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: Reserve"   			     ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);			// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: 满箱操作,按下OK键,信息清除"  ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"1: Full box operation,press \"OK\" key,"  ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
 		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);			// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"   ,后续任务停止" ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"clear display and tasks stop" ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		
		TFT1_FillBackPic(H_FN_SRC3_ROW,H_FN_SRC3_ROW+26,col0,ROW);			// 后面填充背景
	}
	// TFT2显示	
//	Cfg_Value_Disp(PAGE_CFGFUNC3);
}
/************************************************************************
* LCD 界面 
************************************************************************/
void Page_CfgFunc4(void)
{
	INT16U	col0;

	// TFT1显示
	Page_Cfg_Common(PAGE_CFGFUNC4);			// 功能区选择

	if( H_DISPLAY_MODE != DispDir.dir) 
	{  // 竖屏

//		col0 = TFT1_PutString_flash( S_FN_NAME_ROW,COL_S1,"屏显模式"        ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_NAME_ROW,COL_S1,"Display Mode"        ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		
		TFT1_FillBackPic(S_FN_NAME_ROW,S_FN_NAME_ROW+42,col0,COL);			// 后面填充背景
		
//		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: 单任务横屏   "   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC1_ROW, COL_S1,"0: Single, horizontal"   ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );		
		TFT1_FillBackPic(S_FN_SRC1_ROW,S_FN_SRC1_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: 单任务竖屏   "   ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC2_ROW, COL_S1,"1: Single, vertical"   ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC2_ROW,S_FN_SRC2_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: 双任务横屏   "    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC3_ROW, COL_S1,"2: Double, horizontal"    ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC3_ROW,S_FN_SRC3_ROW+26,col0,COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"3: 双任务竖屏   "    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( S_FN_SRC4_ROW, COL_S1,"3: Double, vertical"    ,40,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(S_FN_SRC4_ROW,S_FN_SRC4_ROW+26,col0,COL);			// 后面填充背景
	}
	else
	{  // 横屏

//		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"屏显模式"       ,10,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_NAME_ROW,COL_S1,"Display Mode"       ,20,CHINA4040,txt_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_NAME_ROW,H_FN_NAME_ROW+42,col0,H_FN_VAL_COL);			// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0: 单任务横屏   1: 单任务竖屏"    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC1_ROW, COL_S1,"0:Single,horizontal    1:Single,vertical"    ,60,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC1_ROW,H_FN_SRC1_ROW+26,col0,ROW);			// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"2: 双任务横屏   3: 双任务竖屏"    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		col0 = TFT1_PutString_flash( H_FN_SRC2_ROW, COL_S1,"2:Double,horizontal   3:Double,vertical"    ,60,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC2_ROW,H_FN_SRC2_ROW+26,col0,ROW);			// 后面填充背景

//		col0 = TFT1_PutString_flash( H_FN_SRC3_ROW, COL_S1,"4: 单信息横屏   5: 单信息竖屏"    ,30,CHINA2424L,dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour] );
		TFT1_FillBackPic(H_FN_SRC3_ROW,H_FN_SRC3_ROW+26,COL_S1,ROW);			// 后面填充背景
	}
	// TFT2显示	
//	Cfg_Value_Disp(PAGE_CFGFUNC4);
}
/************************************************************************
* LCD 界面 
************************************************************************/
void	Page_Off(void)
{
	DispColor1(aBLACK);		//显示黑色

	TFT1_BL_OFF(); 			//关背光
	lcd_background.current_background = 6;
}

/************************************************************************
* LCD 界面 
************************************************************************/

void	Page_Start(void)
{
	INT8U  BK_bk_colour ;
	INT16U  col0,row0,col1,row1;

//	DispColor1(aBLACK);		//显示黑色
	DispColor1(bk_colour_true);		//显示对应背景色
				  

	// TFT1显示	
	BK_bk_colour = bk_colour;
	bk_colour = BK_SALFDEF;		
	
	if( LCD.TFT1_ID ==	TFT_HJ30)
	{	
		if( H_DISPLAY_MODE != DispDir.dir) 
		{ 	// 竖屏	
			col0 = 145; row0 = 90;
			col1 = 196; row1 = 30;
		}
		else
		{	// 横屏
			col0 = 120; row0 = 170;
			col1 = 85;  row1 = 110;	
		}

		TFT1_PutString_flash( col0,row0, "HS_Cabinet     "                 ,6 ,CHINA4040, aWHITE,bk_colour_true );//aBLACK );//,BK_SALFDEF);			// 对应横屏
		TFT1_PutString_flash( col1,row1, "V1.00    2023-05-08"  ,30,CHINA2424 ,aGRAY, bk_colour_true );//aBLACK);//,BK_SALFDEF);			// 对应横屏
	}
	else if( LCD.TFT1_ID ==	TFT_HJ28)
	{	
		if( H_DISPLAY_MODE != DispDir.dir) 
		{ 	// 竖屏	
			col0 = 145; row0 = 90;
			col1 = 196; row1 = 30;
		}
		else
		{	// 横屏
			col0 = 120; row0 = 170;
			col1 = 85;  row1 = 110;	
		}

		TFT1_PutString_flash( col0,row0, "HS_Cabinet     "                 ,6 ,CHINA4040, aWHITE,bk_colour_true );//aBLACK );//,BK_SALFDEF);			// 对应横屏
		TFT1_PutString_flash( col1,row1, "V1.00    2023-05-08"  ,30,CHINA2424 ,aGRAY, bk_colour_true );//aBLACK);//,BK_SALFDEF);			// 对应横屏
	}		
	else
	{	
		if( H_DISPLAY_MODE != DispDir.dir) 
		{ 	// 竖屏	
			col0 = 220+15; row0 = 90;
			col1 = 185+15; row1 = 30;
		}
		else
		{	// 横屏
			col0 = 120; row0 = 80;//170
			col1 = 85;  row1 = 100;//110
		}  
		
		TFT1_PutString_flash( col0,row0, "SMD_DHC     "   ,10 ,CHINA4040, aWHITE,bk_colour_true );//aBLACK );//,BK_SALFDEF);			// 对应横屏
		TFT1_PutString_flash( col1,row1, "V2.62-260818"  ,30,CHINA2424 ,aGRAY, bk_colour_true );//aBLACK);//,BK_SALFDEF);			// 对应横屏
	}	
	
	bk_colour = BK_bk_colour ;
}

/*******************************************************************************
	 				滚屏显示	只有任务运行模式和回显模式才进入
    input:   none     
	output:  none
*******************************************************************************/
void Disp_Roll_Process(void)
{
	INT8U	i,ord;

	// 滚屏定时时间未到则返回
	if(DispRoll.RollDelayCnt != 0) { return; }
	DispRoll.RollDelayCnt 	= DISP_ROLL_SPEED;		// 滚屏定时值赋值
//	LCD.f_LcdEnable         = WAIT_REFRESH;			// 使能等待LCD刷新总标志

	if( LCD.TypeBke == PAGE_RUN )
   	{	// 运行页面
   		if((DispRoll.f_Rolling[0] & ROLL_MODE_NAME) != 0x00)
		{			
		   	if( DOUBLE_DISPLAY_MODE == DispDir.only)
			{ // 双屏
				TFT1_NameRoll(DOUBLE_TASK1_MODE,txt_colour_tab[bk_colour] );		// 品名区滚屏，字体颜色根据定义设置
			}
			else
			{
				TFT1_NameRoll(ONLY_TASK_MODE,txt_colour_tab[bk_colour] );		    // 品名区滚屏，字体颜色根据定义设置
			}
		}

   		if((DispRoll.f_Rolling[1] & ROLL_MODE_NAME) != 0x00)
		{
			TFT1_NameRoll(DOUBLE_TASK2_MODE,txt_colour_tab[bk_colour] );		// 品名区滚屏，字体颜色根据定义设置
		}

		for(i = 0; i < 3; i++)
		{
	   		if((DispRoll.f_Rolling[0] & Roll_Src_Flag[i]) != 0x00)
			{
				TFT1_SrcRoll(i,dsc_colour_tab[bk_colour] ); // 描述区滚屏，字体颜色根据定义设置
			}

	   		if((DispRoll.f_Rolling[1] & Roll_Src_Flag[i]) != 0x00)
			{
				TFT1_SrcRoll(3+i,dsc_colour_tab[bk_colour] ); // 描述区滚屏，字体颜色根据定义设置
			}
		}	
	}
	else if( LCD.TypeBke == PAGE_BACKDISPLAY )
	{	// 回显页面
		ord = (BackTask.index - 1) % PAGE_MAX_BACK_NUM;//(BackTask.index - 1)%4;
		for(i = 0; i < PAGE_MAX_BACK_NUM; i++)
		{
	   		if((DispRoll.f_Rolling2 & Roll_Back_Flag[i]) != 0x00)
			{
				if(ord == i)
				{ TFT1_DispBackRoll(i,txt_colour_tab[bk_colour] ); }
				else
				{ TFT1_DispBackRoll(i,btxt_colour_tab[bk_colour] ); }
			}
		}
	}
	else if( LCD.TypeBke == PAGE_INFO )
	{	// 信息屏页面
		ord = (BackTask.index - 1) % PAGE_MAX_BACK_NUM;//(BackTask.index - 1)%4;

   		if((DispRoll.f_Rolling1 & ROLL_MODE_NAME) != 0x00)
		{			
			TFT1_InfoNameRoll(ONLY_TASK_MODE,txt_colour_tab[bk_colour] );		// 品名区滚屏，字体颜色根据定义设置
		}

   		if((DispRoll.f_Rolling1 & ROLL_MODE_SNAME) != 0x00)
		{
			TFT1_InfoNameRoll(DOUBLE_TASK1_MODE,dsc_colour_tab[bk_colour] );		// 品名区滚屏，字体颜色根据定义设置
		}

		for(i = 0; i < 3; i++)
		{
	   		if((DispRoll.f_Rolling1 & Roll_Src_Flag[i]) != 0x00)
			{
				TFT1_InfoSrcRoll(i,dsc_colour_tab[bk_colour] ); // 描述区滚屏，字体颜色根据定义设置
			}
		}
	}
}



void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) //画矩形函数
{
	uint16_t i = 0;
	if(height > 30)
		height = 30;
    TFT1_SetWindows(y,y + height*2,x,x + width);
	for(i=0;i < (height*2 + 1)*(width + 1);i++)
		TFT1_WD_DAT(color);
}
#define DATA_SIZE 30
#define rectangle_roe_begin 23
int stored_data_count = 0;
uint16_t data[DATA_SIZE] = {0};
uint16_t insert_index = 0;
/*******************************************************************************
	 				矩形图绘制
    input:   none     
	output:  none
*******************************************************************************/
void bar_char(uint16_t new_data,INT8U Flag)
{
	uint16_t color;
	uint8_t i;
	uint16_t insert_index_copy = 0;
	uint16_t rectangle_col_begin = 15;
	if(Flag == 0)
	{
		data[insert_index] = new_data;
		insert_index = (insert_index + 1) % DATA_SIZE;
		if(stored_data_count < DATA_SIZE)
		{
			stored_data_count++;
		}		
	}

	insert_index_copy = insert_index;
	for(i = 0;i < stored_data_count;i++)
	{
		if(stored_data_count < DATA_SIZE)
		{
			if(data[i] <= para.str.SHT30_Humidity_SET)
				color = aGREEN;
			else
				color = aYELLOW;
			draw_rectangle(rectangle_col_begin,23,5,data[i], color);
		}
		else
		{
			if(data[insert_index_copy] <= para.str.SHT30_Humidity_SET)
				color = aGREEN;
			else
				color = aYELLOW;
			TFT1_FillBackPic(rectangle_roe_begin,rectangle_roe_begin + 61,rectangle_col_begin,rectangle_col_begin + 6);
			draw_rectangle(rectangle_col_begin,23,5,data[insert_index_copy], color);
			insert_index_copy++;
			insert_index_copy = insert_index_copy % DATA_SIZE;
		}
		if(rectangle_col_begin < 300)
			rectangle_col_begin += 9;
	}
}
/*******************************************************************************
	 				主页面背景显示
    input:   none     
	output:  none
*******************************************************************************/
void Page_Run_Background(void)
{
	int a = 0;
	
	//if(sht30_par.SHT30_Humidity <= para.str.SHT30_Humidity_SET+1)
	if(Cur_SHT30_Humidity <= para.str.SHT30_Humidity_SET+1)
	{
		lcd_type_sel.lcd_back = Tasked;		// 已完成的背景色
	}
	//else if(sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET+1)
	else if(Cur_SHT30_Humidity > para.str.SHT30_Humidity_SET+1)
	{
		lcd_type_sel.lcd_back = Tasking;	// 正在除湿中	
	}
	
	if((lcd_background.current_background != 0)&&(lcd_type_sel.lcd_back == Tasking))
	{
		if(para_bk.str.Language_switch == 0)
		{
			a = 0;
		}
		else
		{
			a = 0 + 8;
		}
		Load_Picture(a);
		bar_char(0,1);			//不增加新数据，仅随着背景图片刷新显示		20230808
		lcd_background.current_background = 0;
		
		lcd_background.current_statepic  = 2;
	}
	else if((lcd_background.current_background != 1)&&(lcd_type_sel.lcd_back == Tasked))
	{
		if(sht30_par.Gled == 1)
		{
			LED_G_ON();
			sht30_par.Gled = 0;		
		}
		if(para_bk.str.Language_switch == 0)
		{
			a = 1;
		}
		else
		{
			a = 1 + 8;
		}
		Load_Picture(a);
		bar_char(0,1);//不增加新数据，仅随着背景图片刷新显示		20230808
		lcd_background.current_background = 1;
		
		lcd_background.current_statepic  = 2;
	}

	if((lcd_type_sel.lcd_state == humidify)&&(lcd_background.current_statepic != 1))
	{
		if(para_bk.str.Language_switch == 0)
		{
			a = 5;
		}
		else
		{
			a = 5 + 8;
		}
		TFT1_Display_NORFlash_Picture(FullImageConst[1],FullImageInfo1[a]);		//吸湿中小图标
		lcd_background.current_statepic = 1;
	}
	else if((lcd_type_sel.lcd_state == Dehumidify)&&(lcd_background.current_statepic != 0))
	{
		if(para_bk.str.Language_switch == 0)
		{
			a = 4;
		}
		else
		{
			a = 4 + 8;
		}
		TFT1_Display_NORFlash_Picture(FullImageConst[1],FullImageInfo1[a]);		//烘干中小图标
		lcd_background.current_statepic = 0;
	}
}
/*******************************************************************************
	 				数字转化为ASCII码数组  20230808
    input:   none     
	output:  none
*******************************************************************************/
void Transform_arrays(INT32U data,INT8U *num)
{
	num[0] = (data % 1000) / 100 + 48;
	num[1] = (data % 100) /10 + 48;
	num[3] = (data % 10) + 48;
	num[2] = 46;
}



/*******************************************************************************
	 主页面显示
    input:   none     
	output:  none
*******************************************************************************/
// 新增全局/静态变量声明（根据实际工程调整）
//static int8_t Min_flag_zeng = 0; // 分钟计数器，初始化0

void Page_Run1(void)
{
	INT8U num_tem[4] = {0};      // 初始化数组，避免随机值
	INT8U num_hum[4] = {0};
	INT32U _Humidity1 = 0;
	float _Humidity = 0.0f;
	INT32U Temprature = 0;
	INT16U _Humidity2 = 0;
	
	
	// 变量定义移到函数开头（兼容嵌入式编译器）
	INT8U pic_humdity_num[2] = {0};

	
	_Humidity = Cur_SHT30_Humidity;	// 取当前值
//	if(qie_flang == 0)
//	{
//		// 显示实时湿度
//		_Humidity = sht30_par.SHT30_Humidity;
//	}
//	else
//	{
//		// 显示上次设置的湿度值
//	   _Humidity = (float)(int)Last_time_SHT30_Humidity + (sht30_par.SHT30_Humidity - (int)sht30_par.SHT30_Humidity);	//_Humidity = Last_time_SHT30_Humidity;//_Humidity = (float)(int)Last_time_SHT30_Humidity + (sht30_par.SHT30_Humidity - (int)sht30_par.SHT30_Humidity);
//	}
	
	_Humidity1 = (INT32U)(_Humidity * 10);  // 强制类型转换，避免浮点精度问题
	Temprature = (INT32U)(sht30_par.SHT30_Temprature * 10);
	_Humidity2 = (INT16U)_Humidity;
	
	// 设定湿度转ASCII（用于显示）
	pic_humdity_num[0] = para.str.SHT30_Humidity_SET / 10 + 48;	// 48=0x30
	pic_humdity_num[1] = para.str.SHT30_Humidity_SET % 10 + 48;
	
	// 绘制背景
	Page_Run_Background();

	// 转换数字为ASCII数组（用于显示）
	Transform_arrays(_Humidity1, num_hum);
	Transform_arrays(Temprature, num_tem);
	
	// 湿度显示逻辑优化（补全{}，修复显示漏洞）
	if(num_hum[0] == 48) // 十位为0（ASCII 0），隐藏十位
	{
		TFT1_PutChar(92, 132, num_hum[1], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
		TFT1_FillBackPic(92, 172, 92, 132); // 填充背景
		// 补充显示小数位，避免缺失
		TFT1_PutChar(92, 182, num_hum[3], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
	}
	else if(num_hum[0] != 0) // 补全{}，规范代码块
	{
		TFT1_PutString_flash(92, 92, num_hum, 2, CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
		TFT1_PutChar(92, 182, num_hum[3], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
	}
	
	draw_rectangle(168, 104, 6, 3, aWHITE); 
	// 显示温度
	TFT1_PutString_flash(180, 10, num_tem, 4, CHAR1324, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
	
	// 显示设定湿度
	TFT1_PutChar(118, 14, pic_humdity_num[0], CHAR1632, eGREEN, bk_colour_tab[bk_colour]);
	TFT1_PutChar(118, 30, pic_humdity_num[1], CHAR1632, eGREEN, bk_colour_tab[bk_colour]);
	
	// 每分钟刷新柱状图逻辑（修复核心控制逻辑）
	if((os_time.Systime_Min_flag == 1) || (lcd_background.bar_interrupt_recover == 1))
	{
//		Min_flag_zeng++; // 分钟计数+1
//		
////		Last_time_SHT30_Humidity = para_bk.str.SHT30_Humidity_SET; // 每分钟更新为最新设定值
//		
//		// 显式转换类型，避免隐式转换隐患
//		float current_set_hum = (float)para_bk.str.SHT30_Humidity_SET;
//		// 检测设定值是否被修改（浮点比较加容差，避免精度问题）
//		if(fabs(Last_time_SHT30_Humidity - current_set_hum) > 0.001f)
//		{
//			Last_time_SHT30_Humidity = para_bk.str.SHT30_Humidity_SET; // 更新为最新设定值
//			//Min_flag_zeng = 0; // 重置计数器，重新开始30分钟计时
//			qie_flang = 0;     // 设定值修改，立即切换为显示实时湿度
//		}		
//				
//		 if(Min_flag_zeng >= 30) // 三十分钟触发一次判断
//		{			
//			Min_flag_zeng = 0; // 重置计数器
////			Last_time_SHT30_Humidity = para_bk.str.SHT30_Humidity_SET; // 更新为最新设定值
//			
//			float hum_diff = sht30_par.SHT30_Humidity - Last_time_SHT30_Humidity;
//			if((hum_diff > 3.0f) || (hum_diff < -3.0f)) 
//			{
//				qie_flang = 1;
//			}else  
//			{
//				qie_flang = 0;
//			}
//		}
		
		// 绘制湿度柱状图+重置标志
		bar_char(_Humidity2, lcd_background.bar_interrupt_recover);
		os_time.Systime_Min_flag = 0;
		lcd_background.bar_interrupt_recover = 0;
	}
}


//void Page_Run1(void)
//{
//	INT8U num_tem[4] = {0};      // 初始化数组，避免随机值
//	INT8U num_hum[4] = {0};
//	INT32U _Humidity1 = 0;
//	float _Humidity = 0.0f;
//	INT32U Temprature = 0;
//	INT16U _Humidity2 = 0;
//	
//	
//	// 变量定义移到函数开头（兼容嵌入式编译器）
//	INT8U pic_humdity_num[2] = {0};

//	if(qie_flang == 0)
//	{
//		// 显示实时湿度
//		_Humidity = sht30_par.SHT30_Humidity;
//	}
//	else
//	{
//		
//		_Humidity = (float)(int)Last_time_SHT30_Humidity + (sht30_par.SHT30_Humidity - (int)sht30_par.SHT30_Humidity);
//	}
//	
//	_Humidity1 = (INT32U)(_Humidity * 10);  // 强制类型转换，避免浮点精度问题
//	Temprature = (INT32U)(sht30_par.SHT30_Temprature * 10);
//	_Humidity2 = (INT16U) _Humidity;
//	
//	// 设定湿度转ASCII（用于显示）
//	pic_humdity_num[0] = para.str.SHT30_Humidity_SET / 10 + 48;
//	pic_humdity_num[1] = para.str.SHT30_Humidity_SET % 10 + 48;
//	
//	// 绘制背景
//	Page_Run_Background();

//	// 转换数字为ASCII数组（用于显示）
//	Transform_arrays(_Humidity1, num_hum);
//	Transform_arrays(Temprature, num_tem);
//	
//	// 湿度显示逻辑优化（补全{}，修复显示漏洞）
//	if(num_hum[0] == 48) // 十位为0（ASCII 0），隐藏十位
//	{
//		TFT1_PutChar(92, 132, num_hum[1], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
//		TFT1_FillBackPic(92, 172, 92, 132); // 填充背景
//		// 补充显示小数位，避免缺失
//		TFT1_PutChar(92, 182, num_hum[3], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
//	}
//	else if(num_hum[0] != 0) // 补全{}，规范代码块
//	{
//		TFT1_PutString_flash(92, 92, num_hum, 2, CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
//		TFT1_PutChar(92, 182, num_hum[3], CHAR4080B, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
//	}
//	
//	draw_rectangle(168, 104, 6, 3, aWHITE); 
//	// 显示温度
//	TFT1_PutString_flash(180, 10, num_tem, 4, CHAR1324, dsc_colour_tab[bk_colour], bk_colour_tab[bk_colour]);
//	
//	// 显示设定湿度
//	TFT1_PutChar(118, 14, pic_humdity_num[0], CHAR1632, eGREEN, bk_colour_tab[bk_colour]);
//	TFT1_PutChar(118, 30, pic_humdity_num[1], CHAR1632, eGREEN, bk_colour_tab[bk_colour]);
//	
//	// 每分钟刷新柱状图逻辑（修复核心控制逻辑）
//	if((os_time.Systime_Min_flag == 1) || (lcd_background.bar_interrupt_recover == 1))
//	{
//		Min_flag_zeng++; // 分钟计数+1
//		if(Min_flag_zeng >= 30) // 三十分钟触发一次判断
//		{			
//			Min_flag_zeng = 0; // 重置计数器
//			Last_time_SHT30_Humidity = para_bk.str.SHT30_Humidity_SET; // 更新为最新设定值
//			
//			// 修复核心逻辑：判断湿度差值的绝对值是否大于3
//			if(sht30_par.SHT30_Humidity - Last_time_SHT30_Humidity >= 3.0f) // 差值超过3%RH，触发动作
//			{
//				qie_flang = 1;
//			}else if(sht30_par.SHT30_Humidity - Last_time_SHT30_Humidity <= 3.0f) // 差值超过3%RH，触发动作
//			{
//				qie_flang = 1;
//			}
//			else // 差值≤3%RH，停止动作
//			{
//				qie_flang = 0;
//			}
//		}
//		
//		// 绘制湿度柱状图+重置标志
//		bar_char(_Humidity2, lcd_background.bar_interrupt_recover);
//		os_time.Systime_Min_flag = 0;
//		lcd_background.bar_interrupt_recover = 0;
//	}
//}




//INT8U flag1 = 0;
/*******************************************************************************
	 				清除箭头
    input:   none     
	output:  none
*******************************************************************************/
void Clean_arrowhead(_ImageInfo picture_num,_ImageConstStr location)
{
	INT32U i;
	TFT1_SetWindows(location.XS,location.XS+(picture_num.High) - 1,location.YS,location.YS+(picture_num.Width) - 1);
	for(i = 0;i < 150;i++)
		TFT1_WD_DAT(aBLACK);
}

/*******************************************************************************
	 				箭头显示 20230808
    input:   none     
	output:  none
*******************************************************************************/
void Display_arrow(INT8U picture_num)
{
	INT8U picture_num_next = picture_num + 1;
	INT8U picture_num_previous = picture_num - 1;
	INT8U picture_num_min = 2;
	INT8U picture_num_max = 5;

	if(picture_num_next > picture_num_max)
		picture_num_next = picture_num_min;
	if(picture_num_previous < picture_num_min)
		picture_num_previous = picture_num_max;
	TFT1_Display_NORFlash_Picture(FullImageConst[picture_num],FullImageInfo1[6]);  
	Clean_arrowhead(FullImageInfo1[6],FullImageConst[picture_num_previous]); 
	Clean_arrowhead(FullImageInfo1[6],FullImageConst[picture_num_next]);

}

/*******************************************************************************
	 				设置湿度页面显示
    input:   none     
	output:  none
*******************************************************************************/
//void Set_Page_Run(void)
//{
//	if(lcd_background.current_background != 3)
//	{
//		Load_Picture(7);
//		lcd_background.current_background = 3;
//	}

//	
//	TFT1_PutChar(72, 220, para_bk.str.SHT30_Humidity_SET + 48, CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
//	switch(para_bk.str.SHT30_Humidity_SET)
//	{
//		case 9:    Display_arrow(2); break;               
//		case 8:    Display_arrow(3); break;   //98 183               
//		case 7:    Display_arrow(4); break;                 
//		case 6:    Display_arrow(5); break;                  
//	}
//	lcd_background.current_statepic = 2;

//}
/*******************************************************************************
	 				地址设置界面 数字外框绘制
    input:   none     
	output:  none
*******************************************************************************/
void Picture_frame_box(INT8U picture_num)
{
	INT8U i;
	INT16U color_set0,color_set1;
	if(picture_num == 0)
	{
		color_set0 = aWHITE;
		color_set1 = gGRAY;
	}
	else
	{
		color_set0 = gGRAY;
		color_set1 = aWHITE;
	}
	TFT1_SetWindows(44,44,180,299);
	for(i = 0;i < 120;i++)
		TFT1_WD_DAT(color_set0);
	TFT1_SetWindows(45,164,180,180);
	for(i = 0;i < 118;i++)
		TFT1_WD_DAT(color_set0);
	TFT1_SetWindows(164,164,180,299);
	for(i = 0;i < 120;i++)
		TFT1_WD_DAT(color_set0);	
	TFT1_SetWindows(45,164,299,299);
	for(i = 0;i < 118;i++)
		TFT1_WD_DAT(color_set0);
	
	
	TFT1_SetWindows(44,44,20,139);
	for(i = 0;i < 120;i++)
		TFT1_WD_DAT(color_set1);
	TFT1_SetWindows(45,164,20,20);
	for(i = 0;i < 118;i++)
		TFT1_WD_DAT(color_set1);
	TFT1_SetWindows(164,164,20,139);
	for(i = 0;i < 120;i++)
		TFT1_WD_DAT(color_set1);	
	TFT1_SetWindows(45,164,139,139);
	for(i = 0;i < 118;i++)
		TFT1_WD_DAT(color_set1);
}

void Addr_picture_frame_box(INT8U picture_num)
{
	INT8U i;
	INT16U color_set0,color_set1,color_set2;
	if(picture_num == 0)
	{
		color_set0 = aWHITE;
		color_set1 = gGRAY;
		color_set2 = gGRAY;
	}
	else if(picture_num == 1)
	{
		color_set0 = gGRAY;
		color_set1 = aWHITE;
		color_set2 = gGRAY;
	}
	else
	{
		color_set0 = gGRAY;
		color_set1 = gGRAY;
		color_set2 = aWHITE;	
	}
	TFT1_SetWindows(60,60,224,310);
	for(i = 0;i < 85;i++)
		TFT1_WD_DAT(color_set0);
	TFT1_SetWindows(60,148,224,224);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set0);
	TFT1_SetWindows(148,148,224,310);
	for(i = 0;i < 85;i++)
		TFT1_WD_DAT(color_set0);	
	TFT1_SetWindows(60,148,310,310);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set0);
	
	
	TFT1_SetWindows(60,60,117,204);
	for(i = 0;i < 87;i++)
		TFT1_WD_DAT(color_set1);
	TFT1_SetWindows(60,148,117,117);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set1);
	TFT1_SetWindows(148,148,117,204);
	for(i = 0;i < 87;i++)
		TFT1_WD_DAT(color_set1);	
	TFT1_SetWindows(60,148,204,204);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set1);
		
	TFT1_SetWindows(60,60,10,97);
	for(i = 0;i < 87;i++)
		TFT1_WD_DAT(color_set2);
	TFT1_SetWindows(60,148,10,10);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set2);
	TFT1_SetWindows(148,148,10,97);
	for(i = 0;i < 87;i++)
		TFT1_WD_DAT(color_set2);	
	TFT1_SetWindows(60,148,97,97);
	for(i = 0;i < 88;i++)
		TFT1_WD_DAT(color_set2);
}
/*******************************************************************************
	 				地址设置界面
    input:   none     
	output:  none
*******************************************************************************/
void Page_Addr_Set(void)
{
	INT8U pic_address = para_bk.str.address;
	INT8U pic_address_num[3] = {0};
	int a = 0;
	pic_address_num[0] = pic_address % 10 + 48;
	pic_address_num[1] = (pic_address %100) / 10 + 48;
	pic_address_num[2] = pic_address / 100 + 48;
	if(para_bk.str.Language_switch == 0)
	{
		a = 2;
	}
	else
	{
		a = 2 + 8;
	}
	if(lcd_background.current_background != a)
	{
		Load_Picture(a);
		lcd_background.current_background = a;
	}
	Addr_picture_frame_box(Disp.Focus);
	TFT1_PutChar(72, 243, pic_address_num[0], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	TFT1_PutChar(72, 137, pic_address_num[1], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	TFT1_PutChar(72, 30,  pic_address_num[2], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	lcd_background.current_statepic = 2;
}

/*******************************************************************************
	 				湿度设置界面
    input:   none     
	output:  none
*******************************************************************************/

void Set_Page_Run_0_99(void)
{
	INT8U Humdity_set = para_bk.str.SHT30_Humidity_SET;//para_bk.str.SHT30_Humidity_SET;//设置界面的值
	INT8U pic_humdity_num[2] = {0};
	pic_humdity_num[1] = Humdity_set / 10 + 48;
	pic_humdity_num[0] = Humdity_set % 10 + 48;
	int a = 0;
	if(para_bk.str.Language_switch == 0)
	{
		a = 3;
	}
	else
	{
		a = 3 + 8;
	}
	if(lcd_background.current_background != a)
	{
		Load_Picture(a);
		lcd_background.current_background = a;
	}
	Picture_frame_box(Disp.Focus);
	TFT1_PutChar(72, 220, pic_humdity_num[0], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	TFT1_PutChar(72, 60, pic_humdity_num[1], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	lcd_background.current_statepic = 2;
}

/*******************************************************************************
	 				湿度校准值设置界面
    input:   none     
	output:  none
*******************************************************************************/
void Set_Page_Hum_Cal(void)
{
	INT8U Hum_Cal_set = para_bk.str.SHT30_Hum_Cal_SET;
	INT8U pic_humdity_num[2] = {0};
	if (Hum_Cal_set < 10)
    {
        // 如果是负数，则将负号的ASCII码存入数组
        pic_humdity_num[1] = 45;
    }
    else
    {
        // 如果不是负数，则将正号的ASCII码并存入数组
        pic_humdity_num[1] = 43;
    }
	pic_humdity_num[0] = Hum_Cal_set % 10 + 48;
	int a = 0;
	if(para_bk.str.Language_switch == 0)
	{
		a = 6;
	}
	else
	{
		a = 6 + 8;
	}
	if(lcd_background.current_background != a)
	{
		Load_Picture(a);
		lcd_background.current_background = a;
	}
	Picture_frame_box(Disp.Focus);
	TFT1_PutChar(72, 220, pic_humdity_num[0], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	TFT1_PutChar(72, 60 , pic_humdity_num[1], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	lcd_background.current_statepic = 2;
}

/*******************************************************************************
	 				温度校准值设置界面
    input:   none     
	output:  none
*******************************************************************************/
void Set_Page_Tem_Cal(void)
{
	INT8U Tem_Cal_set = para_bk.str.SHT30_Tem_Cal_SET;
	INT8U pic_tem_num[2] = {0};
	if (Tem_Cal_set < 10)
    {
        // 如果是负数，则将负号的ASCII码存入数组
        pic_tem_num[1] = 45;
    }
    else
    {
        // 如果不是负数，则将正号的ASCII码并存入数组
        pic_tem_num[1] = 43;
    }
	pic_tem_num[0] = Tem_Cal_set % 10 + 48;

	int a = 0;
	if(para_bk.str.Language_switch == 0)
	{
		a = 7;
	}
	else
	{
		a = 7 + 8;
	}
	if(lcd_background.current_background != a)
	{
		Load_Picture(a);
		lcd_background.current_background = a;
	}
	Picture_frame_box(Disp.Focus);
	TFT1_PutChar(72, 220, pic_tem_num[0], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	TFT1_PutChar(72, 60 , pic_tem_num[1], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	lcd_background.current_statepic = 2;
}


/*******************************************************************************
	 				中英文切换界面
    input:   none     
	output:  none
*******************************************************************************/
void Set_Language_Switch(void)
{
	INT8U Language_flg = para_bk.str.Language_switch;
	INT8U pic_tem_num[1] = {0};
	int a = 16;
	pic_tem_num[0] = Language_flg % 10 + 48;
	
	if(lcd_background.current_background != a)
	{
		Load_Picture(a);
		lcd_background.current_background = a;
	}
	Picture_frame_box(Disp.Focus);
	TFT1_PutChar(72, 220, pic_tem_num[0], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	//TFT1_PutChar(72, 60 , pic_tem_num[1], CHAR4080B, dsc_colour_tab[bk_colour],bk_colour_tab[bk_colour]);
	lcd_background.current_statepic = 2;
}
/************************************************************************
* LCD 模块 没有刷新总标志f_LcdEnable，则返回。
* 刷新完成自动清刷新总标志
************************************************************************/
void LCD_Scan(void)
{	
	INT32U	TimCnt;
	//-----------------------------------------------------------------	
	if( NONE_REFRESH == LCD.f_LcdEnable )               // 非刷新标志，定时返回主界面
	{	
			if( Disp.TatolTimeRefresh8 == 0 )            // 特殊界面，完成超时退出功能
			{  
				if(KeyMode.f_Config == ON)
				{ 
                    Exit_Cfg_Mode();                    	// 如果在设置界面则，退出配置页面
                }	           
			}				
		if((Disp.f_Loading != LOAD_MODE_OFF))               // 等待状态   等待验证
		{
			if(Disp.TatolTimeRefresh16 == 0)                // 等待界面时间到
			{  
				Disp.f_Loading    = LOAD_MODE_OFF;			// 结束等待进度条
				led.light_type    = led.light_type_BK;		// 恢复原先使用的灯组     				
				SetLcdLay( PAGE_RUN );				 		// 其他页面进入运行界面，页面更新，需要更新背景图             
			}
		}

	}	
	// 如无页面刷新标志则返回
	if( NONE_REFRESH == LCD.f_LcdEnable )
    { 
        return; 
    }	
	//-----------------------------------------------------------------
	
	CLOSE_SYS_TMR_INT();					// 禁止系统时钟中断
	LCD.f_LcdEnable = RUN_REFRESH;			// 清刷新标志
	LCD.TypeBke 	= LCD.Type; 			// 保存当前页
	OPEN_SYS_TMR_INT();						// 使能系统时钟中断

	if(LCD.Type != PAGE_OFF)
	{
		TFT1_BL_ON(); 			//开背光
	}
	
//	if(TRUE == LCD.f_LcdIni)
//	{ 	//初始化显示
//		TFT1_ILI9327_INIT();	//IC初始化
//	}				
	TimCnt = os_time.Systime_Count;					// 初始值
	switch(LCD.TypeBke)
	{
		case	PAGE_START:			Page_Start();				break;
		case	PAGE_RUN:			Page_Run1();				break;			  // 运行页面
		case	PAGE_CFGADDR:		Page_Addr_Set();			break;			  // 配置地址页面
		case	PAGE_CFGFUNC1:		Set_Page_Run_0_99();		break;			  // 湿度设置界面
		case	PAGE_CFGFUNC2:		Set_Page_Hum_Cal(); 		break;			  // 湿度校准值设置界面
		case	PAGE_CFGFUNC3:		Set_Page_Tem_Cal(); 		break;			  // 温度校准值设置界面
		case	PAGE_CFGFUNC4:		Set_Language_Switch(); 		break;			  // 中英文切换界面
		default	:					Page_Run1();				break;			  //按键退出后直接显示主界面												
	}
	LCD.TFT1_TimCnt1ms = os_time.Systime_Count - TimCnt;
	//-----------------------------------------------------------------	
	if( LCD.f_LcdEnable == RUN_REFRESH )	//待验证
	{
		// 这地方是临界段，按键里设置的值，或通讯接收处理的显示设置值，可能再这里被清除
		LCD.f_LcdEnable = NONE_REFRESH;			// 清刷新标志
		LCD.f_Update	= FALSE;				// 清更新标志
		LCD.f_LcdIni	= FALSE;				// 清初始化标志
		if((LcdLay.LayCnt) >= LCDLAYMAX )
		{ 
            LcdLay.LayCnt = 0; 
        }
		LcdLay.LayDispTime[LcdLay.LayCnt] = LCD.TFT1_TimCnt1ms;
	}
	else
	{ 
		if(LcdLay.LayCnt == 0)
        { 
            LcdLay.LayCnt = LCDLAYMAX; 
        }
		else if( (LcdLay.LayCnt) >= LCDLAYMAX )
		{ 
            LcdLay.LayCnt = 1; 
        }
		LcdLay.LayDispTime[LcdLay.LayCnt-1] = LCD.TFT1_TimCnt1ms; 
	}
}

void display_point(unsigned int x,unsigned int y,unsigned int dat)   //打点函数
{
    TFT1_SetWindows(y,y,x,x);
    TFT1_WD_DAT(dat);          //红
} 

void clear_designated_area(unsigned int x,unsigned int y)   //清除指定区域
{
    TFT1_SetWindows(y,y,x,x);
    TFT1_WD_DAT(bRED);          //红
} 


