/*************************************************************/
/*ͷ�ļ�Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*�궨��Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*���Ͷ���Byte Definition*************************************/
/*************************************************************/

/*************************************************************/
/*��־λ����Flags Definition**********************************/
/*************************************************************/
bit Flag_BT_work;
bit Flag_BT_Connect;//�������ӱ�־λ
uint8_t Flag_BT_Play;   //��������,1���ڲ��ţ�0������ͣ(ֹͣ)
bit Flag_UART1_RX_Finish_A;
bit Flag_UART1_RX_Finish_B;
bit Flag_UART_ReceiveBuffer_A_B;
bit Flag_BT_TF_Play;//����TF������ģʽ��1���ڲ��ţ�0������ͣ(ֹͣ)
uint8_t Flag_BT_Pairing;//����һ����������û�����ӣ��������״̬��Ϊ1Ϊ��ԣ�Ϊ2λ���ӳɹ�
bit Flag_BT_Play_Payse;

/*************************************************************/
/*��������Variable Definition*********************************/
/*************************************************************/
uint8_t  idata bt_cmd;
uint8_t  idata cntMuteBT;
uint8_t  idata bt_Volume;
uint8_t  idata uart1_RX_Pointer;
uint8_t  idata uart1_TX_Pointer;
uint8_t  idata uart1_EnableSend;
uint8_t  idata uart1_TX_Timeout;
uint8_t  idata uart1_RX_Timeout;
uint8_t  idata Uart_Len;//������UART1_LEN_BUFFER������������ʱ

/*************************************************************/
/*���鶨��Array Definition************************************/
/*************************************************************/
uint8_t  uart1_TransmitBuffer[UART1_LEN_BUFFER];
uint8_t  idata uart1_ReceiveBuffer_A[UART1_LEN_BUFFER];
uint8_t  idata uart1_ReceiveBuffer_B[UART1_LEN_BUFFER];

char code BT_Command_Tab[][4]=
{
	{0x00,0x00,0x00,0x00},
	{0xAA,0x00,0x00,0x55},
	{0xAA,0x00,0x05,0x55},
	{0xAA,0x00,0x06,0x55},
};

/*************************************************************/
/*��������Function Declaration********************************/
/*************************************************************/

/*************************************************************/
/*��������Function Definition*********************************/
/*************************************************************/
/*******************************************************************************
 * ����ԭ�ͣ�
 * ���������
 * ���������
 * �������ܣ����ڷ����ַ�
 * ����ֵ˵����
 * �������ڣ�
 * �����ˣ�
 * �޸�����
 * �޸��ˣ�
 * ��N���޸ģ�
 * �޸�ԭ��
 * ��ע��
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
 * ����ԭ�ͣ�
 * ���������
 * ���������
 * �������ܣ����ڷ����ַ�
 * ����ֵ˵����
 * �������ڣ�
 * �����ˣ�
 * �޸�����
 * �޸��ˣ�
 * ��N���޸ģ�
 * �޸�ԭ��
 * ��ע��
 *******************************************************************************/
void Uart1Transmit_SendHex(char *str)
{
	uint8_t  i;
	while(uart1_EnableSend);
	for(i=0;i<UART1_LEN_BUFFER;i++)
	{
		uart1_TransmitBuffer[i]=0;
	}
	for(i=0;i<4;i++)
	{
		uart1_TransmitBuffer[i]=*str;
		str++;
	}
	uart1_TX_Pointer=0;
	uart1_EnableSend=1;
	UARTDR=uart1_TransmitBuffer[uart1_TX_Pointer++];
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ��������ͨ���������������ж�ѡ���ĸ������
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void BT_Send_CMD(uint8_t cmd)
{
	//uint8_t  code bt_VOL_Send_Tab[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	if(cmd<=BT_VOLDEC)
	{
		Uart1Transmit_SendHex(&BT_Command_Tab[cmd][0]);
	}
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
uint8_t  idata BT_CMDBuffer[UART1_LEN_BUFFER];
void BlueMode_Receive(void)
{
	uint8_t  i;
	//char *BT_CMD;
	if((Flag_UART1_RX_Finish_A) || (Flag_UART1_RX_Finish_B))
	{
		if(Flag_UART1_RX_Finish_A)
		{
			Flag_UART1_RX_Finish_A=0;
			for(i=0;i<UART1_LEN_BUFFER;i++)
				BT_CMDBuffer[i]=uart1_ReceiveBuffer_A[i];
		}
		else 
		{
			Flag_UART1_RX_Finish_B=0;
			for(i=0;i<UART1_LEN_BUFFER;i++)
				BT_CMDBuffer[i]=uart1_ReceiveBuffer_B[i];
		}
		if((BT_CMDBuffer[0]==0xAA) && (BT_CMDBuffer[1]==0x20))//20��������mcu
		{
			if(BT_CMDBuffer[2]==0x03)
			{
				Flag_BT_Connect=1;//���ӳɹ�
			}
			if(BT_CMDBuffer[2]==0x01)
			{
				Flag_BT_Connect=0;//�Ͽ����ӣ���ʱ�����������ģʽ
			}
			if(BT_CMDBuffer[2]==0x04)
			{
				Flag_BT_Play_Payse=0;//����û�в���
			}
			if(BT_CMDBuffer[2]==0x05)
			{
				Flag_BT_Play_Payse=1;//�����ڲ���
			}
		}
		
		for(i=0;i<UART1_LEN_BUFFER;i++)
			BT_CMDBuffer[i]=0;
	}
}




/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void BlueMode_Handle(void) //���յ���������Ϣ/״̬���д���
{
	BlueMode_Receive();
	if (bt_cmd)
	{
		BT_Send_CMD(bt_cmd);
		bt_cmd = BT_NONE;  //����
	}
}











