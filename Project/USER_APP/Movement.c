#include  "../Head file/includes.h" 

// 定义控制开关的状态
#define SWITCH_ON  1
#define SWITCH_OFF 0
//报警扫描时间
#define alarm_scan_point 30
volatile enum STATE state;



// 控制风扇开关
void fan_switch(uint8_t state1)
{
	if (state1 == SWITCH_ON) 
	{
		FAN_ON;
	} 
	else 
	{
		FAN_OFF;	
	}
}
// 控制加热PTC开关
void ptc_switch(uint8_t state1)
{
	if (state1 == SWITCH_ON) 
	{
		DOOR_ON;
	} 
	else 
	{
		DOOR_OFF;
	}
}

// 控制加热云母片开关
void mica_switch(uint8_t state1)
{
	if (state1 == SWITCH_ON) 
	{
		HOT_ON;
	} 
	else 
	{
		HOT_OFF;
	}
}

// 红色指示灯根据湿度差值是否点亮
void alarm_scan(void)
{
//	static INT8U alarm_scan_num = 0;
//	static INT8U alarm_scan_execute = 0;
	static INT8U alarm_scan_switch = 0;
	
//	alarm_scan_num++;

//	if(sht30_par.SHT30_Humidity <= para.str.SHT30_Humidity_SET)
//		alarm_scan_switch = 1;
//	if(alarm_scan_num > alarm_scan_point)
//	{
//		alarm_scan_execute = 1;
//		alarm_scan_num	= 0;
//	}
	
//	if((alarm_scan_execute == 1)&&(alarm_scan_switch == 1)&&(sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET + 3))
//	{
//		LED_WRN_ON();
//		LED_DO_ON();
//		alarm_scan_switch = 0;
//	}
	
	//if((alarm_scan_switch == 1)&&(sht30_par.SHT30_Humidity > para.str.SHT30_Humidity_SET + 3))
	if((alarm_scan_switch == 1)&&(Cur_SHT30_Humidity > para.str.SHT30_Humidity_SET + 3))
	{
//		LED_WRN_ON();
		LED_R_ON();
		alarm_scan_switch = 0;
	}
//	else if(sht30_par.SHT30_Humidity < para.str.SHT30_Humidity_SET + 1)
	else if(Cur_SHT30_Humidity < para.str.SHT30_Humidity_SET + 1)
	{
		LED_R_OFF();
//		LED_WRN_OFF();
		alarm_scan_switch = 1;

	}
}	


// 主程序的状态检测
void check_state(void)
{
	switch (state) 
	{
		case STATE_OPEN:
			// 等待定时器中断触发
			lcd_type_sel.lcd_state = humidify;	//吸湿中小图标
		break;
		case STATE_FAST:
			lcd_type_sel.lcd_state = humidify;
			alarm_scan();
		break;
		case STATE_DRY:
			lcd_type_sel.lcd_state = Dehumidify;
		break;
		case STATE_KEEP:
			lcd_type_sel.lcd_state = humidify;
		break;
		default:
			lcd_type_sel.lcd_state = Dehumidify;
		break;
	}
}


void state_test_485(void)
{
		INT8U hot,fan,door; 

	static INT8U fan_backup = 0;
	static INT8U hot_backup = 0;
	static INT8U door_backup = 0;
	static	INT8U num_hum;

	num_hum  = sht30_par.SHT30_Humidity ;
	
		door = READ_DOOR_BIT;
		hot = READ_HOT_BIT;
		fan = READ_FAN_BIT;
		if((door_backup != door)||(hot_backup != hot)||(fan_backup != fan))
		{
			uart.wBuffer[0] = fan;
			uart.wBuffer[1] = hot;
			uart.wBuffer[2] = door;
			uart.wBuffer[3] = num_hum;
//			usCRC = CRC16(uart.wBuffer, 3);
//			uart.wBuffer[3] = (INT8U)usCRC;
//			uart.wBuffer[4] = (INT8U)(usCRC>>8);
			uart.wLen = 4; 
			USART_SendBuff(UART1,(INT8U *)uart.wBuffer, uart.wLen);    // 串口发送
			fan_backup = fan;
			hot_backup = hot;
			door_backup = door;
//			RS485CTRL_RECV_EN();	// 打开485发送端口，关闭接收端口

		}
}




