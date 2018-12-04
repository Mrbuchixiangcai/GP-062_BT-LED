/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*类型定义Byte Definition*************************************/
/*************************************************************/

/*************************************************************/
/*标志位定义Flags Definition**********************************/
/*************************************************************/
bit Flag_BT_work;
bit Flag_BT_Connect;//蓝牙连接标志位
uint8_t Flag_BT_Play;   //蓝牙播放,1是在播放，0是在暂停(停止)
bit Flag_UART1_RX_Finish_A;
bit Flag_UART1_RX_Finish_B;
bit Flag_UART_ReceiveBuffer_A_B;
bit Flag_BT_TF_Play;//蓝牙TF卡播放模式，1正在播放，0是在暂停(停止)
uint8_t Flag_BT_Pairing;//蓝牙一开机，但是没有连接，处在配对状态，为1为配对，为2位连接成功

/*************************************************************/
/*变量定义Variable Definition*********************************/
/*************************************************************/
uint8_t  idata bt_cmd;
uint8_t  idata cntMuteBT;
uint8_t  idata bt_Volume;
uint8_t  idata uart1_RX_Pointer;
uint8_t  idata uart1_TX_Pointer;
uint8_t  idata uart1_EnableSend;
uint8_t  idata uart1_TX_Timeout;
uint8_t  idata uart1_RX_Timeout;
uint8_t  idata Uart_Len;//本来是UART1_LEN_BUFFER，但是这是临时

/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/
uint8_t  uart1_TransmitBuffer[UART1_LEN_BUFFER];
uint8_t  idata uart1_ReceiveBuffer_A[UART1_LEN_BUFFER];
uint8_t  idata uart1_ReceiveBuffer_B[UART1_LEN_BUFFER];

/*************************************************************/
/*函数声明Function Declaration********************************/
/*************************************************************/

/*************************************************************/
/*函数定义Function Definition*********************************/
/*************************************************************/
/*******************************************************************************
 * 函数原型：
 * 输入参数：
 * 输出参数：
 * 函数功能：串口发送字符
 * 返回值说明：
 * 创建日期：
 * 创建人：
 * 修改日期
 * 修改人：
 * 第N次修改：
 * 修改原因：
 * 备注：
 *******************************************************************************/
void Uart1Transmit_SendString(char *str)
{
	uint8_t  i;
	while(uart1_EnableSend);
	for(i=0;i<UART1_LEN_BUFFER;i++)
	{
		uart1_TransmitBuffer[i]=0;
	}
	//for(i=0;*str != '\0';i++)
	for(i=0;i<Uart_Len;i++)
	{
		uart1_TransmitBuffer[i]=*str;
		str++;
	}
	uart1_TX_Pointer=0;
	uart1_EnableSend=1;
	UARTDR=uart1_TransmitBuffer[uart1_TX_Pointer++];
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：发送命令，通过传进来的数据判断选择哪个命令发送
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void BT_Send_CMD(uint8_t cmd)
{
	//uint8_t  code bt_VOL_Send_Tab[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	if(cmd<BT_PLAY)
	{
		
	}
	else
	{
		
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void BlueMode_Receive(void)
{
	uint8_t  i;
	char *BT_CMD;
	if((Flag_UART1_RX_Finish_A) || (Flag_UART1_RX_Finish_B))
	{
		if(Flag_UART1_RX_Finish_A)
		{
			Flag_UART1_RX_Finish_A=0;
			BT_CMD=uart1_ReceiveBuffer_A;
		}
		else 
		{
			Flag_UART1_RX_Finish_B=0;
			BT_CMD=uart1_ReceiveBuffer_B;
		}
		if((BT_CMD[0] == 'B') && (BT_CMD[1] == 'T'))
		{
			
			
		}
		if((BT_CMD[0] == 'S') && (BT_CMD[1] == 'D'))
		{
			
		}
		if((BT_CMD[0] == 'A') && (BT_CMD[1] == 'X'))
		{
			
		}
		if((BT_CMD[0] == 'C') && (BT_CMD[1] == 'O') && (BT_CMD[2] == 'M'))
		{
			
		}
		
		for(i=0;i<UART1_LEN_BUFFER;i++)
			BT_CMD[i]=0;
	}
}




/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void BlueMode_Handle(void) //接收到的数据信息/状态进行处理
{
	BlueMode_Receive();
		if (sys_Volume != bt_Volume)//这里有问题，会一直复位蓝牙，现写显示，回来再改
		{
			bt_Volume = sys_Volume;
			BT_Send_CMD(BT_VOL); //串口发送音量信息到蓝牙端
		}

}











