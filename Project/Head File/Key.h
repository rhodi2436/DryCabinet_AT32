#ifndef  KEY_H
#define  KEY_H



//按键键值
#define KEYFUN_NOKEY		0x00            // 无按键
//#define KEYFUN_UP			0x01            // +键按下
//#define KEYFUN_DOWN			0x02            // -键按下
//#define KEYFUN_FORWARD		0x08            // 前进键按下
//#define KEYFUN_BACK			0x10            // 后退键按下
//#define KEYFUN_FUNC			0x04            // 功能键按下

#define KEYFUN_ADD			0x80            // +键按下
#define KEYFUN_SUB			0x20            // -键按下
#define KEYFUN_FORWARD		0x08            // 前进键按下
#define KEYFUN_BACK			0x40            // 后退键按下
#define KEYFUN_FUNC			0x10            // 功能键按下

#define KEYFUN_OK			0x01            // OK键按下
#define KEYFUN_FUNC_L		0x02            // 功能键长按
#define KEYFUN_QUITE		0x04            // OK键按下

//时间
#define TIMER_COUNT_32ms	31//32//15
#define TIMER_KEY5ms		5
#define TIMER_KEY60ms		60

#define	KEYTIME_SHORT		1				//短按触发
#define KEYTIME_LONG		30				//长按触发	16bit
#define KEYTIME_FREE		1				//按键释放  8bit
#define KEYLONGTIME			21////20//30				//长按时间，32ms为单位
#define KEYLONGPROTIME		3				//长按间隔执行时间，32ms为单位


extern float last_humi_set;
#endif

