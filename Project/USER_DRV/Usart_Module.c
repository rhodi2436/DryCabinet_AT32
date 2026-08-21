/************************************************************************
** 功能描述：串口初始化，串口数据传输函数
*************************************************************************
** 模块名：  串口模块  
** 作  者：  黎昌其
** 日  期：  2014.8.14 
** 修  改：
** 日  期：
** 版  本：  1.0 
*************************************************************************
** 备 注:
USART1  做485通讯
USART2	做升级调试口
************************************************************************/


#include "../Head file/includes.h" 


/************************************************************************
** 功能描述：发送8位数据
** 备注：由于USART DMA与FSMC DMA有冲突，暂时采用查询模式发送数据  		  
************************************************************************/
void USART_SendByte(_UART uart,INT8U dat)
{   

	if (uart == UART1)
	{		
		USART_SendData(USART1,dat);
		/* Wait until end of transmit */
		while(USART_GetFlagStatus(USART1, USART_FLAG_TRAC) == RESET);
	}
	else if (uart == UART2)
	{
		USART_SendData(USART2,dat);
		/* Wait until end of transmit */
		while(USART_GetFlagStatus(USART2, USART_FLAG_TRAC) == RESET);

	}
	else if (uart == UART3)
	{
		USART_SendData(USART3,dat);
		/* Wait until end of transmit */
		while(USART_GetFlagStatus(USART3, USART_FLAG_TRAC) == RESET);
	}

}

/************************************************************************
** 功能描述：发送buff数据		  
************************************************************************/
void USART_SendBuff(_UART uart,INT8U *buff,INT16U num)
{
    INT8U i;

	if (uart == UART1)
	{
//		LED_RT_ON();		 // 打开通讯灯
//		SETCTRL485();		 // 打开485发送DE端口，关闭接收/RE端口
		RS485CTRL_SEND_EN();	// 打开485发送端口，关闭接收端口
	}

	for (i = 0; i < num; i++)
	{	USART_SendByte(uart,*(buff+i));	   }

	if (uart == UART1)
	{
//		LED_RT_OFF();		 // 关闭通讯灯
//		RESETCTRL485();		 // 关闭发送DE端口，打开接收/RE485端口，
		RS485CTRL_RECV_EN();  // 打开485接收端口，关闭发送端口
	}	
}















