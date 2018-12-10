/*******************************************************************************
*˵����LED����˵����
*	   "P2 = 0x02;P3 = 0x07;"������4�����߶�����ܡ��ڶ���λ�ĺ��Ķ�����
*	   "P2 = 0x02;P3 = ~0x07;"������4�����߶�����ܡ��ڶ���λ��ǰ��������
*	   ���ǵ͵�ƽ��ͨ����ΪP2���ߵ�ƽ
********************************************************************************/

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
UCT_TIME_E_TypeDef UCT_TD;//ʱ��

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
BOOL   Flag_PowerOn;//���ػ�״̬��1Ϊ����
BOOL   gbDispleyFull;//��ʾ����1Ϊ��
BOOL   SleepEnable;

uint8_t idata Flag_DayFull_Add1Month;//ÿ���¹���ͼ�1��ֵ��
uint8_t idata Flag_MonthFull_Add1Year;//ÿ�����ͼ�1��ֵ��

uint8_t idata Flag_ZoneStart;
uint8_t idata Flag_ZoneStop;

/*************************************************************/
/*��������Variable Definition**********************************/
/*************************************************************/
uint8_t  idata cntAppTick;

BOOL		   gRTC_HalfSecond;
uint8_t  idata gRTC_Sec;//RTC���� 
uint8_t  idata gRTC_Sec_bk;
char     idata gRTC_Minute;
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
uint8_t  idata gRTC_Zone_bk;
uint8_t  idata gRTC_Week; //һ��7��
uint8_t  idata cntNoFlash;
uint8_t		   gbDimmer;//����
uint8_t  idata cnt0_5s;//������500ms�ͷ�תFlag_0_5s

uint8_t  idata sys_Volume;
uint8_t  idata cntBeep;//������ʱ����Ϊbeepģʽ
uint8_t  idata cntAlarmRuningMinuteOFF;//��������ٷ��ӹر�


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
* �������ܣ��ر��������������
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void Alarm1PowerOFF(void)
{
	AL1_TD.OnOff_TD = ALARM_OFF;//���ӿ��أ���
	AL1_TD.RingRun_TD = ALARM_RING_RUN_OFF;//��������״̬����
	AL1_TD.Snooze_TD=ALARM_SNOOZE_OFF;//����̰˯ģʽ��û��̰˯
	_BEEP_MUTE(0);
	BUZ_OnOff(0);
	cntBeep=0;
}

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
			if(Flag_0_5s)  //ÿ��Flag_0_5sΪ1��Flag_Year_0_5s_Disp�ͷ�ת����������1s��תһ��
				Flag_Year_0_5s_Disp=~Flag_Year_0_5s_Disp;
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
			 *���ܣ����ӿ�������hour��minute��ȣ�������Ӧ
			 *	��Ϊ��24Сʱ��ʵ�ʼ�ʱ�����Բ����ж���������
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.hour==gRTC_Hour) && (AL1_TD.minute==gRTC_Minute))
			{
				if(AL1_TD.RingRun_TD==ALARM_RING_RUN_OFF)//
				{
					AL1_TD.RingRun_TD=ALARM_RING_RUN_ON;//����������״̬��ΪON
					if(Flag_BT_Play_Payse==1)
					{
						bt_cmd=BT_PLAY_PAUSE;
					}
				}
			}
			
			/*******************************************************************************
			 *���ܣ����ӿ���&&������&&̰˯ʱ���RTCʱ�����&&̰˯ģʽ��
			 *
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON) && (AL1_TD.Snooze_TD == ALARM_SNOOZE_ON)
			 && (AL1_TD.snoozeRTCHour==gRTC_Hour) && (AL1_TD.snoozeRTCMinute==gRTC_Minute))
			{
				AL1_TD.Snooze_TD = ALARM_SNOOZE_OFF;//�ٴο���̰˯
			}
			
			/*******************************************************************************
			 *���ܣ���������Ѿ����������������У�����30���ӹرգ������Ƿ���̰˯ģʽ
			 *
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD == ALARM_ON) && (AL1_TD.RingRun_TD == ALARM_RING_RUN_ON))//�����Ѿ�����������������,
			{
				cntAlarmRuningMinuteOFF++;
				if(cntAlarmRuningMinuteOFF>=30)//30���ӵ���
				{
					Alarm1PowerOFF();
				}
			}
			
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
	
	if((AL1_TD.OnOff_TD == ALARM_ON) && (AL1_TD.RingRun_TD == ALARM_RING_RUN_ON))//�����Ѿ�����������������,
	{
		if(AL1_TD.Snooze_TD == ALARM_SNOOZE_OFF)//�����������̰˯ģʽ
		{
			_BEEP_MUTE(1);//��mute
			P1 |= 0x40;
			cntBeep++;
			if(cntBeep<40)
				BUZ_OnOff(1);
			else if((cntBeep>=40) && (cntBeep<80))
				BUZ_OnOff(0);
			else 
				cntBeep=0;
		}
		else if(AL1_TD.Snooze_TD == ALARM_SNOOZE_ON)//���������̰˯ģʽ
		{
			cntBeep=0;
			_BEEP_MUTE(0);//��Ϊ�����ӿ�������������״̬�����Բ���Ӱ������ʱ�����������
			BUZ_OnOff(0);
		}
	}
	
	/*******************************************************************************
	*���ܣ�gRTC_Zone1-7,ѡ���Ӧ��ʱ��
	*
	********************************************************************************/
	if(gRTC_Zone==1)
		UCT_TD=UTC_SUB_10;
	else if(gRTC_Zone==2)
		UCT_TD=UTC_SUB_9;
	else if(gRTC_Zone==3)
		UCT_TD=UTC_SUB_8;
	else if(gRTC_Zone==4)
		UCT_TD=UTC_SUB_7;
	else if(gRTC_Zone==5)
		UCT_TD=UTC_SUB_6;
	else if(gRTC_Zone==6)
		UCT_TD=UTC_SUB_5;
	else if(gRTC_Zone==7)
		UCT_TD=UTC_SUB_4;
}
/*******************************************************************************
*���ܣ����ӿ����������Ѿ���Ӧ
*������10ms����һ��
*******************************************************************************/
void AlarmRunning(void)
{
	
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
	gRTC_Year=18;	  //�ϵ�Ĭ��2018��
	gRTC_Month=12;	  //�ϵ�Ĭ��12��
	gRTC_Day=1;		  //�ϵ�Ĭ��Ϊ1��
	gRTC_Week=6;	  //12�º�������
	gRTC_Hour=0;	  //�ϵ�Ĭ��Ϊ�賿����������
	gRTC_Minute=0;	  //�ϵ�Ĭ��Ϊ
	gRTC_Sec=0;		  //�ϵ�Ĭ��Ϊ
	gRTC_Hour_bk=0;	  //�ϵ�Ĭ��Ϊ�賿����������
	gRTC_Minute_bk=0; //�ϵ�Ĭ��Ϊ
	gRTC_Sec_bk=0;	  //�ϵ�Ĭ��Ϊ
	Flag_LeapYear=0;  //2018�겻������
	Flag_12HourDisplay=1;//12Сʱ��
	gRTC_Zone=6;	  //����ʱ������ʼ��Ϊ��ʢ������ʱ��,������
	UCT_TD=UTC_SUB_5; //����ʱ������ʼ��Ϊ��ʢ������ʱ��,������
	
	cntNoFlash = 0;
	cnt0_5s=0;
	Flag_PowerOn = 1;
	gbDimmer=18;
	AL1_TD.snoozeTime=10;//Ĭ��Ϊ10����

	
//	gRTC_Hour=23;
//	gRTC_Minute=39;
//	AL1_TD.OnOff_TD = ALARM_ON;
//	AL1_TD.hour=0;
//	AL1_TD.minute=10;
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
	CheckDC();
	while(1)
	{
		if(AppTick1ms)
		{
			AppTick1ms=0;
			CheckDC();
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
			BlueMode_Handle();
			
		}
		if(AppTick3)
		{
			AppTick3=0;
			Display();
			//AlarmRunning();
			//P1 |= 0x40;
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










