/*************************************************************/
/*ͷ�ļ�Header File*******************************************/
/*************************************************************/
#include "app_main.h"
#include "key_mode.h"

/*************************************************************/
/*�궨��Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*���Ͷ���Byte Definition**************************************/
/*************************************************************/

/*************************************************************/
/*��־λ����Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*��������Variable Definition**********************************/
/*************************************************************/
uint8_t  gbKeyPress; //��������
uint8_t  gbKeyNone;  //û�а�������
uint8_t  KeyValue;   //����ֵ
uint8_t  KeyEvent;   //�����¼�
uint8_t  cntKeyLoop; //��������ѭ�����̰����������������İ���ʱ��
uint8_t  cntKeyLong;
uint8_t  KeyCurValueBK;//���ڰ�������
uint8_t  KeyCurValue;//��������ֵ
uint8_t  KeyLastValue;//������һ�ε�ֵ

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
* �������ܣ���ȡ����ֵ
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
uint8_t GetKeyValue(void)
{
	uint8_t keyNum=0;
	if(!KEY_ADD_DEC())
		keyNum=K_VOLINC;
	else if(!KEY_SUB_DEC())
		keyNum=K_VOLDEC;
	else if(!KEY_SNOOZE_DEC())
		keyNum=K_SNOOZE_DIMMER;
	else if(!KEY_ALARM_DEC())
		keyNum=K_ALARM;
	if((!KEY_ALARM_DEC()) && (!KEY_SNOOZE_DEC()))//
		keyNum=K_AL_SN;
	
	return keyNum;
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ������¼�����
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void KeyEventPorc(uint8_t KeyTmp) //������ʱ���յ���GetKeyValue()�ķ���ֵ  һ��̶���������
{
	gbKeyPress=0;
	if(KeyCurValue!=KeyTmp) //���if�����ȥ�����ᱻִ�����Σ������ν���ʱֱ������
	{//KeyTmpΪ��ʱֵ��
		if(KeyCurValueBK!=KeyTmp)
			KeyCurValueBK=KeyTmp;
		else //�ڶ��ν���ʱִ����һ��
			KeyCurValue=KeyTmp; //
		return;
	}
	if(KeyCurValue==KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0) //����ڵڶ��ν�KeyEventPorc()��������һ�ν���if(KeyCurValue==KeyLastValue)��֮ǰ��ʱ�����渳ֵΪ5
		{ //cntKeyLoopΪ5����50msʱ��
			KeyValue=KeyLastValue; //����֮���ȷ���İ�����ֵ��KeyValue
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 0:
					KeyEvent=1;
					KeyValue |= K_D;
					cntKeyLoop=cKEY_HOLD;
					break;
				case 1:
					KeyEvent=2;
					KeyValue |= K_H;
					cntKeyLoop=cKEY_RATE_VOL;
					break;
					case 2:
				case 3:
					KeyEvent=3;
					KeyValue |= K_R;
					cntKeyLoop=cKEY_RATE_VOL;
					if(cntKeyLong<250)
						cntKeyLong++;
					break;
			}
		}
	}
	else
	{
		if(KeyLastValue)
		{
			KeyValue=KeyLastValue;
			if(gbKeyNone==0)
				gbKeyPress=1;
			switch(KeyEvent)
			{
				case 1:KeyValue |= K_U; break;
				case 2:KeyValue |= K_LU;break;
				case 3:KeyValue |= K_LU;break;
			}
			KeyEvent=0;
		}
		else
		{
			gbKeyNone=0;
			cntKeyLong=0;
		}
		cntKeyLoop=cKEY_CLICK;
		KeyLastValue=KeyCurValue;
	}
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�����������
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע��
*******************************************************************************/
void KeyScan(void) //������������
{
	KeyEventPorc(GetKeyValue());
}

/*******************************************************************************
* ����ԭ�ͣ�
* ���������
* ���������
* �������ܣ�������Ӧ����
* ����ֵ˵����
* �������ڣ�
* �����ˣ�
* �޸�����
* �޸��ˣ�
* ��N���޸ģ�
* �޸�ԭ��
* ��ע������˵����
*		Ŀǰʹ�õĸ�������������������[2]��
*		��Ԫ��ֳ���4������������ƽ�ꡣ
*		��Ԫ��ֳ���4������������100����������Ϊ���ꡣ
*		��Ԫ��ֳ���100������������400����������Ϊƽ�ꡣ
*		��Ԫ��ֳ���400������������3200��������[��ԴՈ��]��Ϊ���ꡣ
*		���㷽����*
*		if ((y % 4 == 0 and y % 100 != 0) or (y % 400 == 0 and y % 3200 != 0)):
*			print("�c��")
*		else:
*			print("ƽ��")
*******************************************************************************/
void KeyComMsg(void) 
{
	if(gbKeyPress)
	{ 
	   	switch(KeyValue)
		{
			case KU(K_VOLINC): //value67 û��break���Լ���ִ����һ�����
			case KR(K_VOLINC): //value131
			{
				if(Flag_DisplayStatus==0)//Ϊ0˵��������������������������
				{
					bt_cmd=BT_VOLINC;
				}
				else
				{
					switch(Flag_DisplayStatus)
					{
						case ADJ_YEAR: //�������
						{
							if(++gRTC_Year > 99)
								gRTC_Year=18;
							if(gRTC_Month==2)
							{
								if((gRTC_Year % 4) ==0)//������ֵ��2018-2099�����Բ���"gRTC_Year % 100"
								{
									Flag_LeapYear=1;//����
								}
								else
								{
									Flag_LeapYear=0;
								}
							}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MONTH://�����·�
						{
							if(--gRTC_Month<1)
								gRTC_Month=12;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAY://��������
						{
							gRTC_Day++;
							if(gRTC_Month==2)
							{
								if(Flag_LeapYear==1)//����
								{
									if(gRTC_Day>29)
										gRTC_Day=1;
								}
								else
								{
									if(gRTC_Day>28)
										gRTC_Day=1;
								}
							}
							else if((gRTC_Month==4) || (gRTC_Month==6)|| (gRTC_Month==9)|| (gRTC_Month==11))
							{
								if(gRTC_Day>30)
									gRTC_Day=1;
							}
							else
							{
								if(gRTC_Day>31)
									gRTC_Day=1;
							}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_HOUR: //����RTCСʱ
						{
							if(++gRTC_Hour>23)
								gRTC_Hour=0;
	//						if (Flag_12HourDisplay)//12Сʱ����ʾ
	//						{
	//							if(gRTC_Hour<12)//���С��12�����賿0�㵽11:59��Ϊ����
	//							{
	//								Flag_APM=0;  //����
	//							}
	//							else if (gRTC_Hour > 12)  
	//							{
	//								Flag_APM = 1;   //�������12����12:00��23:59��Ϊ����//��־λΪ1�����ö�Ӧ��LED�Ƿ�����
	//								gRTC_Hour -= 12;//��Ϊ��12Сʱ�ƣ����������ʱ��Ҳ��12Сʱ����ʾ��Ҫ��12
	//							}
	//							if (gRTC_Hour == 0) //�������ʾ12�ģ�����12��Ҳ����ʾ12��
	//							{
	//								gRTC_Hour = 12;
	//							}
	//						}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MINUTE://����RTC����
						{
							if(++gRTC_Minute>59)
								gRTC_Minute=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAYLIGHT_SAVING_ZONES://��������ʱ��
						{
							if(++gRTC_Zone>8)
								gRTC_Zone=1;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_SNOOZE_TIME://����̰˯ʱ��
						{
							if(++AL1_TD.snoozeTime>10)
								AL1_TD.snoozeTime=1;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_ALARM1_HOUR: //��������Сʱ
							if(++AL1_TD.hour>23)
								AL1_TD.hour=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						case ADJ_ALARM1_MINUTE: //�������ӷ���
							if(++AL1_TD.minute>59)
								AL1_TD.minute=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						default:
							break;
					}
					cntNoFlash=cDISP_DELAY_500ms;//ÿ�ν�������1s����˸��Ϊ���ڳ�������/��Сʱ��һ�ν�����KU(),�ڶ��β���KR()
				}
				break;
			}
			case KU(K_VOLDEC): //value68
			case KR(K_VOLDEC): //value132
			{
				if(Flag_DisplayStatus==0)//Ϊ0˵��������������������������
				{
					bt_cmd=BT_VOLDEC;//������
				}
				else
				{
					switch(Flag_DisplayStatus)
					{
						case ADJ_YEAR://�������
						{
							if(--gRTC_Year < 18)
								gRTC_Year=99;
							if(gRTC_Month==2)
							{
								if((gRTC_Year % 4) ==0)//������ֵ��2018-2099�����Բ���"gRTC_Year % 100"
								{
									Flag_LeapYear=1;//����
								}
								else
								{
									Flag_LeapYear=0;
								}
							}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MONTH://�����·�
						{
							if(--gRTC_Month<1)
								gRTC_Month=12;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAY://��������
						{
							if(--gRTC_Day<1)
							{
								if(gRTC_Month==2)
								{
									if(Flag_LeapYear==1)
										gRTC_Day=29;	//����
									else
										gRTC_Day=28;
								}
								else if((gRTC_Month==4) || (gRTC_Month==6)|| (gRTC_Month==9)|| (gRTC_Month==11))
									gRTC_Day=30;
								else
									gRTC_Day=31;
							}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_HOUR://����RTCСʱ
						{
							if(--gRTC_Hour<0)
								gRTC_Hour=23;
	//						if (Flag_12HourDisplay)//12Сʱ����ʾ
	//						{
	//							if(gRTC_Hour<12)//���С��12�����賿0�㵽11:59��Ϊ����
	//							{
	//								Flag_APM=0;  //����
	//							}
	//							else if (gRTC_Hour > 12)  
	//							{
	//								Flag_APM = 1;   //�������12����12:00��23:59��Ϊ����//��־λΪ1�����ö�Ӧ��LED�Ƿ�����
	//								gRTC_Hour -= 12;//��Ϊ��12Сʱ�ƣ����������ʱ��Ҳ��12Сʱ����ʾ��Ҫ��12
	//							}
	//							if (gRTC_Hour == 0) //�������ʾ12�ģ�����12��Ҳ����ʾ12��
	//							{
	//								gRTC_Hour = 12;
	//							}
	//						}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MINUTE://����RTC����
						{
							if(--gRTC_Minute<0)
								gRTC_Minute=59;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAYLIGHT_SAVING_ZONES:  //����ʱ��ѡ��
						{
							if(--gRTC_Zone<1)
								gRTC_Zone=8;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_SNOOZE_TIME://����̰˯ʱ��
						{
							if(--AL1_TD.snoozeTime<1)
								AL1_TD.snoozeTime=10;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_ALARM1_HOUR: //��������Сʱ
							if(--AL1_TD.hour<0)
								AL1_TD.hour=23;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						case ADJ_ALARM1_MINUTE: //�������ӷ���
							if(--AL1_TD.minute<0)
								AL1_TD.minute=59;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						default:
							break;
					}
					cntNoFlash=cDISP_DELAY_500ms;//ÿ�ν�������1s����˸��Ϊ���ڳ�������/��Сʱ��һ�ν�����KU(),�ڶ��β���KR()
				}
				break;
			}
			case KU(K_SNOOZE_DIMMER): 
			{
				if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON))//���ӿ���������������
				{
					AL1_TD.Snooze_TD=ALARM_SNOOZE_ON;//����̰˯
					AL1_TD.snoozeHour=gRTC_Hour;//����̰˯�����ѵ�ǰ��RTCʱ�丳ֵ��̰˯��ʱ��̰˯ʱ�䵽ʱ������
					AL1_TD.snoozeMinute=gRTC_Minute;
					break;
				}
				else if(Flag_DisplayStatus==0)
				{
					if(gbDimmer==18)//�����γ���������
						gbDimmer=10;
					else if(gbDimmer==10)
						gbDimmer=6;
					else if(gbDimmer==6)
						gbDimmer=18;
				}
				break;
			}
			case KH(K_SNOOZE_DIMMER):
			{
				
				break;
			}
			case KU(K_ALARM): 
			{
				/*�����ա�RTCʱ�䡢����ʱ����̰˯ʱ������*/
				cntNoFlash=cDISP_DELAY_500ms;//ÿ�ν�������1s����˸��Ϊ���ڳ�������/��Сʱ��һ�ν�����KU(),�ڶ��β���KR()
				if(Flag_DisplayStatus==ADJ_YEAR)//
				{
					SetDisplayState2s(ADJ_MONTH);//�����·�
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_MONTH)//
				{
					SetDisplayState2s(ADJ_DAY);//��������
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_DAY)//
				{
					SetDisplayState2s(ADJ_HOUR);//����Сʱ
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_HOUR)//
				{
					SetDisplayState2s(ADJ_MINUTE);//���÷���
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_MINUTE)//
				{
					SetDisplayState2s(ADJ_DAYLIGHT_SAVING_ZONES);//��������ʱȥѡ��
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_DAYLIGHT_SAVING_ZONES)//
				{
					SetDisplayState2s(ADJ_SNOOZE_TIME);//����̰˯ʱ��
					gRTC_Sec = 0;
				}
				
				/*��������*/
				if(AL1_TD.OnOff_TD==ALARM_ON)
				{
					//��������ڴ�״̬�����ǻ�û�����֣��̰�ALARM�͹ر�
					Alarm1PowerOFF();
				}
				else if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON))
				{
					//������������У��Ƕ̰�ALARM���ر�����
					Alarm1PowerOFF();
				}
				else if(Flag_DisplayStatus==0)
				{
					SetDisplayState2s(ADJ_ALARM1_HOUR);//��������Сʱ
					AL1_TD.OnOff_TD=ALARM_ON;//������
				}
				else if(Flag_DisplayStatus==ADJ_ALARM1_HOUR)
				{
					SetDisplayState2s(ADJ_ALARM1_MINUTE);//��������Сʱ
				}
				else if(Flag_DisplayStatus==ADJ_ALARM1_MINUTE)
				{
					Flag_DisplayStatus=0;//ȷ������ʱ��,
					cntDisplayStatus=0;//����˸��
					cntNoFlash=0;//
				}
				break;
			}
			case KH(K_ALARM):
			{
				
				break;
			}
			case KR(K_AL_SN):
			{
				if(cntKeyLong>=5)//����1s����RTCʱ��
				{
					SetDisplayState2s(ADJ_YEAR);
					gRTC_Sec = 0;
				}
				break;
			}
			default:
				break;
		}
	}
}



















