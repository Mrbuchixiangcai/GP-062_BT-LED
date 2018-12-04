/*************************************************************/
/*ͷ�ļ�Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*�궨��Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*���Ͷ���Byte Definition**************************************/
/*************************************************************/
TIME_TEMP_ST_TypeDef  Time_Temp_TD;
ALRAM_ST_TypeDef  AL1_TD;//�������

/*************************************************************/
/*��־λ����Flags Definition***********************************/
/*************************************************************/
bit    AppTick0;
bit    AppTick1;
bit    AppTick2;
bit    AppTick3;
bit    AppTick4;
bit    AppTick5;
bit    AppTick1ms;

bit    Flag_12HourDisplay;
bit    Flag_0_5s; //0.5s
bit    Flag_LeapYear;//�����־��1Ϊ���꣬0Ϊƽ��
BOOL   gbPowerOn;//���ػ�״̬��1Ϊ����
BOOL   gbDispleyFull;//��ʾ����1Ϊ��
BOOL   SleepEnable;

/*************************************************************/
/*��������Variable Definition**********************************/
/*************************************************************/
uint8_t  idata cntAppTick;

BOOL		   gRTC_HalfSecond;
uint8_t  idata gRTC_Sec;//RTC���� 
uint8_t  idata gRTC_Sec_bk;
uint8_t  idata gRTC_Minute;
uint8_t  idata gRTC_Minute_bk;
uint8_t  idata gRTC_Hour;
uint8_t  idata gRTC_Hour_bk;
uint8_t  idata gRTC_Hour_bk_24;//����24Сʱ
uint8_t  idata gRTC_Day;
uint8_t  idata gRTC_Day_bk;
uint8_t  idata gRTC_Month;
uint8_t  idata gRTC_Month_bk;
uint8_t  idata gRTC_Year;
uint8_t  idata gRTC_Year_bk;
uint8_t  idata gRTC_Zone;
uint8_t  idata gRTC_Week; //һ��7��
uint8_t  idata cntNoFlash;
uint8_t		   gbDimmer;//����
uint8_t  idata cnt0_5s;//������500ms�ͷ�תFlag_0_5s

uint8_t  idata sys_Volume;


/*************************************************************/
/*���鶨��Array Definition************************************/
/*************************************************************/

/*************************************************************/
/*��������Function Declaration*********************************/
/*************************************************************/

/*************************************************************/
/*��������Function Definition**********************************/
/*************************************************************/
/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ��˳���ʾ����
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void DisplayStatusExit(void)
{
	if (cntDisplayStatus)
	{
		if (++cnt0_5s >= cDISP_DELAY_500ms)//10ms����һ�Σ�������500ms�ͽ���
		{
			cnt0_5s = 0;
			Flag_0_5s = ~Flag_0_5s;
		}
		if (--cntDisplayStatus == 0)
		{
			switch (Flag_DisplayStatus)
			{
				case DISP_CLK:
					break;
				default:
					cntDisplayStatus = cDISP_DELAY_60ms;
					Flag_DisplayStatus = DISP_CLK;
					break;
			}
		}
	}
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�ʱ��Ƚϣ�ÿ���ӽ���һ��
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void Compare_1MinutePorc(void)
{
	//static uint8_t  cntSetVolume; //�˱���ֻ���ϵ�ʱ��ʼ��һ�Σ��ٵ��ô˺���ʱ����ִ�д����
	/*******************************************************************************
	 *���ܣ�
	 *
	 *������1s����һ��
	 *******************************************************************************/
	if(gRTC_Sec!=gRTC_Sec_bk)
	{
		gRTC_Sec_bk=gRTC_Sec;
		
		/*******************************************************************************
		 *���ܣ�
		 *
		 *������1s����һ��
		 *******************************************************************************/
		if(gRTC_Minute!=gRTC_Minute_bk)//����
		{
			gRTC_Minute_bk=gRTC_Minute;
			
			/*******************************************************************************
			 *���ܣ�
			 *
			 *������1hour����һ��
			 *******************************************************************************/
			if(gRTC_Hour!=gRTC_Hour_bk)//Сʱ
			{
				gRTC_Hour_bk=gRTC_Hour_bk;
				gRTC_Hour_bk_24++;
				if(gRTC_Hour_bk_24>=24)
				{
					gRTC_Hour_bk_24=0;
					
				}
			}
		}
	}
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ��δ�ʱ����ʱ����
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void Sys_Tick(void)
{
	AppTick1ms=1;
	if(cntAppTick==0)
		AppTick0=1;
	if(cntAppTick==1)
		AppTick1=1;
	if(cntAppTick==2)
		AppTick2=1;
	if(cntAppTick==3)
		AppTick3=1;
	if(cntAppTick==4)
		AppTick4=1;
	if(cntAppTick==5)
		AppTick5=1;
	if(cntAppTick==6)
	{
		if((--uart1_TX_Timeout)==0)//���ͳ�ʱ�����㷢�ͱ�־λ
			uart1_EnableSend=0;
		if((--uart1_RX_Timeout)==0)
			uart1_RX_Pointer=0;
	}
	if(++cntAppTick>=10)
	{
		cntAppTick=0;
	}
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ���ʼ������
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void PowerON_Init(void)
{
	gRTC_Year=18;	//�ϵ�Ĭ��2018��
	gRTC_Month=11;	//�ϵ�Ĭ��11��
	gRTC_Day=0;		//�ϵ�Ĭ��Ϊ1��
	gRTC_Hour=0;	//�ϵ�Ĭ��Ϊ�賿����������
	gRTC_Minute=0;	//�ϵ�Ĭ��Ϊ
	gRTC_Sec=0;		//�ϵ�Ĭ��Ϊ
	gRTC_Hour_bk=0;	//�ϵ�Ĭ��Ϊ�賿����������
	gRTC_Minute_bk=0;//�ϵ�Ĭ��Ϊ
	gRTC_Sec_bk=0;	//�ϵ�Ĭ��Ϊ
	Flag_LeapYear=0;//2018�겻������
	
	cntNoFlash = 0;
	cnt0_5s=0;
	gbPowerOn = 0;
	gbDimmer=3;
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�app_main����
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void app_main(void)
{
	PowerON_Init();
	while(1)
	{
		if(AppTick1ms)
		{
			AppTick1ms=0;
		}
		if(AppTick0)
		{
			AppTick0=0;
			KeyScan();
			KeyComMsg();
		}
		if((AppTick1) || (AppTick2))
		{
			AppTick1=0;
			AppTick2=0;
			//BlueMode_Handle();
			
		}
		if(AppTick3)
		{
			AppTick3=0;
			Display();
		}
		if(AppTick4)
		{
			AppTick4=0;
			DisplayStatusExit();
		}
		if(AppTick5)
		{
			AppTick5=0;
			//Timing_Handle();
			Compare_1MinutePorc();
		}
	}
}









