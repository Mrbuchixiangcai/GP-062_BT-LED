/*************************************************************/
/*ͷ�ļ�����Header File Definition*****************************/
/*************************************************************/
#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

typedef				   bit  BOOL;
typedef unsigned 	   char uint8_t;
typedef unsigned 	   int  uint16_t;
typedef unsigned long  int  uint32_t;
#define NULL  0

/*************************************************************/
/*ͷ�ļ�Header File*******************************************/
/*************************************************************/
#include "app_main.h"
#include "MC96F6508A.h"
#include "func_def.h"
#include "bt_mode.h"
#include "key_mode.h"
#include "led_mode.h"
#include "led_drive.h"

/*************************************************************/
/*�궨��Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*���Ͷ���Byte Definition**************************************/
/*************************************************************/
//����RTCʱ��ʱ��ʱ�ı���
typedef struct
{
	uint8_t  temp_RTC_Year;
	uint8_t  temp_RTC_Month;
	uint8_t  temp_RTC_Day;
	uint8_t  temp_RTC_Hour;
	uint8_t  temp_RTC_Minute;
}TIME_TEMP_ST_TypeDef;//ST:struct

typedef enum
{
	ALARM_NONE = 0,
	ALARM_BT 	  ,
	ALARM_BEEP	  ,
}ALARM_WORK_MODE_E_TypeDef;//��������ģʽѡ���������Դ������������beep,E:enum

typedef enum
{
	ALARM_OFF = 0,
	ALARM_ON    ,
}ALARM_ONOFF_E_TypeDef;//E:enum

typedef enum
{
	ALARM_SNOOZE_OFF = 0, //��������֮����û��̰˯��0��û��
	ALARM_SNOOZE_ON  = 1, //1����̰˯
}ALARM_SNOOZE_E_TypeDef;//E:enum

typedef enum
{
	ALARM_RING_TIME_OFF =0 ,
	ALARM_RING_TIME_ONE =60,//��������ʱ��60����
}ALARM_RING_TIMER_E_TypeDef;//E:enum

typedef enum
{
	ALARM_RING_RUN_OFF = 0,//���ӻ�û����
	ALARM_RING_RUN_ON  = 1,//��������
}ALARM_RING_RUN_E_TypeDef;//E:enum

typedef struct
{
	ALARM_ONOFF_E_TypeDef		OnOff_TD;//�����ܿ���
	uint8_t						week;
	uint8_t						hour;  //�жϴ˱�����ֵ��ͨ��LED��ʾ������Ҳ����ֱ�ӶԴ˱�����������
	uint8_t						minute;//�жϴ˱�����ֵ��ͨ��LED��ʾ������Ҳ����ֱ�ӶԴ˱�����������
	uint8_t						tempHour;  //��Ϊ���������и�ȷ������(��Ȼ���ò���),������Ҫ�и���ʱ������Ϊ�м�ֵ����
	uint8_t						tempMinute;//��ȱ�����û���30s���Զ�ȷ���ٰ�tempXxx�⼸�������������ӵ�����趨ʱ��
	ALARM_RING_RUN_E_TypeDef	RingRun_TD;
	ALARM_RING_TIMER_E_TypeDef 	RingTimer_TD;//��������ʱ��Ϊ60����
	ALARM_WORK_MODE_E_TypeDef 	WorkMode_TD;
	ALARM_SNOOZE_E_TypeDef 		Snooze_TD;
	uint8_t 					snoozeTime;//̰˯ʱ��
}ALRAM_ST_TypeDef;//ST:struct

/*************************************************************/
/*��־λ����Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*��������Variable Definition**********************************/
/*************************************************************/

/*************************************************************/
/*���鶨��Array Definition************************************/
/*************************************************************/

/*************************************************************/
/*��������Function Declaration*********************************/
/*************************************************************/
void Sys_Tick(void);
void app_main(void);

/*************************************************************/
/*��������Function Definition**********************************/
/*************************************************************/

/*************************************************************/
/*�ⲿ����_�궨��Macro Definition******************************/
/*************************************************************/

/*************************************************************/
/*�ⲿ����_���Ͷ���Byte Definition*****************************/
/*************************************************************/
extern ALRAM_ST_TypeDef  AL1_TD;//�������
/*************************************************************/
/*�ⲿ����_��־λ����Flags Definition***************************/
/*************************************************************/
extern bit    Flag_12HourDisplay;
extern bit    Flag_0_5s;
extern bit    Flag_LeapYear;
/*************************************************************/
/*�ⲿ����_��������Variable Definition*************************/
/*************************************************************/
extern uint8_t  idata cntAppTick;
extern BOOL		   gRTC_HalfSecond;
extern uint8_t  idata gRTC_Sec;//RTC���� 
extern uint8_t  idata gRTC_Sec_bk;
extern uint8_t  idata gRTC_Minute;
extern uint8_t  idata gRTC_Minute_bk;
extern uint8_t  idata gRTC_Hour;
extern uint8_t  idata gRTC_Hour_bk;
extern uint8_t  idata gRTC_Hour_bk_24;//����24Сʱ
extern uint8_t  idata gRTC_Day;
extern uint8_t  idata gRTC_Day_bk;
extern uint8_t  idata gRTC_Month;
extern uint8_t  idata gRTC_Month_bk;
extern uint8_t  idata gRTC_Year;
extern uint8_t  idata gRTC_Year_bk;
extern uint8_t  idata gRTC_Zone;
extern uint8_t  idata gRTC_Week; //һ��7��
extern uint8_t  idata cntNoFlash;
extern uint8_t		  gbDimmer;//����

extern uint8_t  idata sys_Volume;

/*************************************************************/
/*�ⲿ����_���鶨��Array Definition****************************/
/*************************************************************/

/*************************************************************/
/*�ⲿ����_��������Function Declaration************************/
/*************************************************************/

/*************************************************************/
/*�ⲿ����_��������Function Definition*************************/
/*************************************************************/








#endif








