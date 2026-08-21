#ifndef __MOVEMENT_H
#define __MOVEMENT_H
#include "includes.h"

// 定义系统状态
enum STATE
{
	STATE_INIT,
	STATE_OPEN,
	STATE_FAST,
	STATE_KEEP,
	STATE_DRY,
};
enum HOT_STATE
{
	STATE_NULL,
	STATE_HEAT_READY,
	STATE_HEAT	
};

void mica_switch(uint8_t state);
void ptc_switch(uint8_t state);
void fan_switch(uint8_t state);

void check_state(void);

void state_test_485(void);






extern volatile enum STATE state;


#endif


