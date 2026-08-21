
//#include  "../Head file/CHAR_4080B.h"
//#include  "../Head file/CHAR_3572B.h"
//#include  "../Head file/CHAR_3264B.h"
#include  "../Head file/CHAR_3572B2.h"	// 对应 CHAR4080B
//#include  "../Head file/CHAR_2040B.h"
#include  "../Head file/ASCII_2040B.h"	// 对应 CHAR2040B

#include  "../Head file/CHAR_1324.h"	// 对应 CHAR1324 
#include  "../Head file/CHAR_1940.h"	// 对应 CHAR2240
#include  "../Head file/CHAR_1016.h"	// 对应 CHAR1016
#include  "../Head file/CHAR_1632.h"	// 对应 CHAR1632




//========================================================================
//																														
const  strPicCharStr  PicCharStr[] = 
{
//  子宽，字高，字库地址
	{ 12,  24,	CharDot_1324  },	  // 对应 CHAR1324 ----> CHINA2424
	{ 19,  40,	CharDot_2240  },	  // 对应 CHAR2240 ----> CHINA4040
	{ 10,  16,	CharDot_1016  },	  // 对应 CHAR1016 ----> CHINA1616
	{ 16,  32,	CharDot_1632  },	  // 对应 CHAR1632 ----> CHINA3232
	{ 12,  24,	CharDot_1324  },	  // 对应 CHAR1324 ----> CHINA2424L
//	{ 40,  80,	CharDot_4080B  },	  // 对应 CHAR4080B
	{ 35,  72,	CharDot_4080B  },	  // 对应 CHAR4080B	 单屏 库位和数量
	{ 20,  40,	CharDot_2040B  },	  // 对应 CHAR2040B	 双屏 库位和数量  ,参数设置对应数值
	{ 32,  64,	CharDot_3264B  },	  // 对应 CHAR3264B
//	{ 40,  80,	CharDot_4080  },	  // 对应 CHAR1324
//	{ 54,  112,	CharDot_54112 },
};




