//#include  "../Head file/Typedef.h"
/****************************************************
**全副图像存储地址		  1扇区64K，地址10000H
****************************************************/
// flash  地址 最大0x1000000
//#define ROW  320			//显示的列数
//#define COL  240			//显示的行数
//#define ROW1  65			//显示的列数
//#define COL1  65			//显示的行数

#define		DIR_DIF		0x2200
#define		FLASH_DIR_ADDR0		0x514000
#define		FLASH_DIR_ADDR1		(FLASH_DIR_ADDR0 +DIR_DIF)
#define		FLASH_DIR_ADDR2		(FLASH_DIR_ADDR1 +DIR_DIF)
#define		FLASH_DIR_ADDR3		(FLASH_DIR_ADDR2 +DIR_DIF)
#define		FLASH_DIR_ADDR4		(FLASH_DIR_ADDR3 +DIR_DIF)
#define		FLASH_DIR_ADDR5		(FLASH_DIR_ADDR4 +DIR_DIF)
#define		FLASH_DIR_ADDR6		(FLASH_DIR_ADDR5 +DIR_DIF)
#define		FLASH_DIR_ADDR7		(FLASH_DIR_ADDR6 +DIR_DIF)
#define		FLASH_DIR_ADDR8		(FLASH_DIR_ADDR7 +DIR_DIF)
#define		FLASH_DIR_ADDR9		(FLASH_DIR_ADDR8 +DIR_DIF)
#define		FLASH_DIR_ADDR10	(FLASH_DIR_ADDR9 +DIR_DIF)
#define		FLASH_DIR_ADDR11	(FLASH_DIR_ADDR10+DIR_DIF)
#define		FLASH_DIR_ADDR12	(FLASH_DIR_ADDR11+DIR_DIF)
#define		FLASH_DIR_ADDR13	(FLASH_DIR_ADDR12 +DIR_DIF)
#define		FLASH_DIR_ADDR14	(FLASH_DIR_ADDR13 +DIR_DIF)
#define		FLASH_DIR_ADDR15	(FLASH_DIR_ADDR14 +DIR_DIF)
#define		FLASH_DIR_ADDR16	(FLASH_DIR_ADDR15 +DIR_DIF)
#define		FLASH_DIR_ADDR17	(FLASH_DIR_ADDR16 +DIR_DIF)
#define		FLASH_DIR_ADDR18	(FLASH_DIR_ADDR17 +DIR_DIF)
#define		FLASH_DIR_ADDR19	(FLASH_DIR_ADDR18 +DIR_DIF)
#define		FLASH_DIR_ADDR20	(FLASH_DIR_ADDR19 +DIR_DIF)
#define		FLASH_DIR_ADDR21	(FLASH_DIR_ADDR20+DIR_DIF)
#define		FLASH_DIR_ADDR22	(FLASH_DIR_ADDR21+DIR_DIF)
#define		FLASH_DIR_ADDR23	(FLASH_DIR_ADDR22 +DIR_DIF)
#define		FLASH_DIR_ADDR24	(FLASH_DIR_ADDR23 +DIR_DIF)
#define		FLASH_DIR_ADDR25	(FLASH_DIR_ADDR24 +DIR_DIF)
#define		FLASH_DIR_ADDR26	(FLASH_DIR_ADDR25 +DIR_DIF)
#define		FLASH_DIR_ADDR27	(FLASH_DIR_ADDR26 +DIR_DIF)
#define		FLASH_DIR_ADDR28	(FLASH_DIR_ADDR27 +DIR_DIF)
#define		FLASH_DIR_ADDR29	(FLASH_DIR_ADDR28 +DIR_DIF)


const 	_ImageInfo 	   FullImageInfo1[] = 
{
	//  高 ,宽, 图片在NOR FLASH 中的存储地址  
	// col = 240,row = 320,col1 = 47,row1 = 36
	{   COL,ROW,0x100000  }, //picture1	 	左TFT1，运行模式和等待模式	0	左上角温度计，左中间水滴，下方横纵坐标，中间橙色圆环和 湿度（%）
	{   COL,ROW,0x130000  }, //picture2	 	左TFT1，回显信息			1	左上角温度计，左中间水滴，下方横纵坐标，中间绿色圆环和 湿度（%）
	{   COL,ROW,0x160000  }, //picture3  	左TFT1，功能参数设置界面	2	地址设定界面
	{   COL,ROW,0x190000  }, //picture3  	左TFT1，功能参数设置界面	3	湿度设置界面
	{   45,37,0x1C0000  }, //picture3  		左TFT1，功能参数设置界面	4	烘干中小图标
	{   45,37,0x1C1000  }, //picture3  		左TFT1，功能参数设置界面	5	吸湿中小图标
	{   COL,ROW,0x1D0000  }, //picture3  	湿度校准值设置界面			6	湿度校准值设置界面
	{   COL,ROW,0x200000  },//				温度校准值设置界面			7	温度校准值设置界面

/************************************************************************************************************************************************************ */

	{   COL,ROW,0x230000  }, //picture1	 	左TFT1，运行模式和等待模式	8	左上角温度计，左中间水滴，下方横纵坐标，中间橙色圆环和 湿度（%）
	{   COL,ROW,0x260000  }, //picture2	 	左TFT1，回显信息			9	左上角温度计，左中间水滴，下方横纵坐标，中间绿色圆环和 湿度（%）
	{   COL,ROW,0x290000  }, //picture3  	左TFT1，功能参数设置界面	10	地址设定界面
	{   COL,ROW,0x2c0000  }, //picture3  	左TFT1，功能参数设置界面	11	湿度设置界面
	{   45,37,0x2f0000  }, //picture3  		左TFT1，功能参数设置界面	12	烘干中小图标
	{   45,37,0x2f1000  }, //picture3  		左TFT1，功能参数设置界面	13	吸湿中小图标
	{   COL,ROW,0x300000  }, //picture3  	湿度校准值设置界面			14	湿度校准值设置界面
	{   COL,ROW,0x330000  },//				温度校准值设置界面			15	温度校准值设置界面
	{   COL,ROW,0x360000  },//				温度校准值设置界面			16	中英文切换界面
	


	{   COL1,ROW1,FLASH_DIR_ADDR1  }, //picture17	1	   箭头_T
	{   COL1,ROW1,FLASH_DIR_ADDR2  }, //picture17	2	   箭头_TR
	{   COL1,ROW1,FLASH_DIR_ADDR3  }, //picture17	3	   箭头_R

	{   COL1,ROW1,FLASH_DIR_ADDR4  }, //picture17	4	   箭头_BR
	{   COL1,ROW1,FLASH_DIR_ADDR5  }, //picture17	5	   箭头_B
	{   COL1,ROW1,FLASH_DIR_ADDR6  }, //picture17	6	   箭头_BL
	{   COL1,ROW1,FLASH_DIR_ADDR7  }, //picture17	7	   箭头_L

	{   COL1,ROW1,FLASH_DIR_ADDR8  }, //picture17	8	   箭头_TL
	{   COL1,ROW1,FLASH_DIR_ADDR9  }, //picture17	9
	{   COL1,ROW1,FLASH_DIR_ADDR10  }, //picture17	10
	{   COL1,ROW1,FLASH_DIR_ADDR11  }, //picture17	11

	{   COL1,ROW1,FLASH_DIR_ADDR12  }, //picture17	12
	{   COL1,ROW1,FLASH_DIR_ADDR13  }, //picture17	13
	{   COL1,ROW1,FLASH_DIR_ADDR14  }, //picture17	14
//	{   COL1,ROW1,0x1BD800  }, //picture17	15		 

	{   COL1,ROW1,FLASH_DIR_ADDR15  }, //picture17	16		 //多规
	{   COL1,ROW1,FLASH_DIR_ADDR16  }, //picture17	17		 //看似
	{   COL1,ROW1,FLASH_DIR_ADDR17  }, //picture17	18		 //听似
	{   COL1,ROW1,FLASH_DIR_ADDR18  }, //picture17	19		 //看听似

	{   COL1,ROW1,FLASH_DIR_ADDR19  }, //picture17	20
	{   COL1,ROW1,FLASH_DIR_ADDR20  }, //picture17	21
	{   COL1,ROW1,FLASH_DIR_ADDR21  }, //picture17	22
	{   COL1,ROW1,FLASH_DIR_ADDR22  }, //picture17	23
	{   COL1,ROW1,FLASH_DIR_ADDR23  }, //picture17	24
};

// 
//const 	_ImageInfo 	   FullImageInfo2[] = 
//{
//	//  高 ,宽, 图片在NOR FLASH 中的存储地址  
//	{   COL2,ROW2,0x100000  }, //picture1	 	左TFT1，运行模式和等待模式
//	{   COL2,ROW2,0x100000  }, //picture2	 	左TFT1，回显信息
//	{   COL,ROW,0x160000  }, //picture3  	左TFT1，功能参数设置界面
//	{   COL,ROW,0x160000  }, //picture4	 	左TFT1，暂时不用
//	{   COL,ROW,0x160000  }, //picture5	 	左TFT1，暂时不用
//	{   COL,ROW,0x160000  }, //picture6  	左TFT1，暂时不用
//	{   COL,ROW,0x160000  }, //picture7	 	左TFT1，暂时不用
//	{   COL,ROW,0x160000  }, //picture8	 	左TFT1，暂时不用
//};


const 	_ImageConstStr 	FullImageConst[]= 
{
	//纵坐标开始位置,横坐标开始位置
	{ 0,			 0		}, 	// 全屏背景图
	{ COL1_P,		 ROW1_P }, 	// 方向，横屏
	{ COL1_SP,		 ROW1_SP }, 	// 方向，竖屏
	{ COL1_SP-40,	ROW1_SP }, 	// 方向，竖屏
	{ COL1_SP-80,		 ROW1_SP }, 	// 方向，竖屏
	{ COL1_SP-120,		 ROW1_SP }, 	// 方向，竖屏
	{ COL1_SP-150,		 ROW1_SP }, 	// 方向，竖屏

	{ COL2_HP,		 ROW2_HP }, 	// 信息，横屏
	{ COL2_SP,		 ROW2_SP }, 	// 信息，竖屏
};


// flash  地址 最大0x1000000
const  _ImageInfo  GB2312Str[] = 
{
	{  24,24,	0x1F0000},		//72*(846+6768)  = 60912  +487296   ,1FEDF0,275D70	 // 24*24 微软雅黑，作为品名
//	{  40,40,	0x280000},		//200*(846+6768) = 169200 +1353600	,3F3C70
	{  40,40,	0x276000},		//200*(846+6768) = 169200 +1353600	,29F4F0,3E9C70	 // 40*40 微软雅黑，作为品名
	{  16,16,	0x3EA000},		//32*(846+6768)  = 27072  +216576	,3F09C0,4257C0	 // 16*16 宋体，作为描述信息
	{  32,32,	0x426000},		//128*(846+6768) = 108288 +866304	,440700,513F00	 // 32*32 微软雅黑，作为品名

	{  24,24,	0x553B00},		//72*(846+6768)  = 60912  +487296	,5628F0,5D9870	 // 24*24 微软雅黑 Light，作为描述信息
};






