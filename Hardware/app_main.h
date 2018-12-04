/*************************************************************/
/*头文件定义Header File Definition*****************************/
/*************************************************************/
#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__

typedef				   bit  BOOL;
typedef unsigned 	   char uint8_t;
typedef unsigned 	   int  uint16_t;
typedef unsigned long  int  uint32_t;
#define NULL  0

/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"
#include "MC96F6508A.h"
#include "func_def.h"
#include "bt_mode.h"
#include "key_mode.h"
#include "led_mode.h"
#include "led_drive.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*类型定义Byte Definition**************************************/
/*************************************************************/
//设置RTC时间时临时的变量
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
}ALARM_WORK_MODE_E_TypeDef;//闹钟响闹模式选择的声音来源，是蓝牙还是beep,E:enum

typedef enum
{
	ALARM_OFF = 0,
	ALARM_ON    ,
}ALARM_ONOFF_E_TypeDef;//E:enum

typedef enum
{
	ALARM_SNOOZE_OFF = 0, //闹钟响了之后有没有贪睡，0是没有
	ALARM_SNOOZE_ON  = 1, //1是在贪睡
}ALARM_SNOOZE_E_TypeDef;//E:enum

typedef enum
{
	ALARM_RING_TIME_OFF =0 ,
	ALARM_RING_TIME_ONE =60,//闹钟响闹时间60分钟
}ALARM_RING_TIMER_E_TypeDef;//E:enum

typedef enum
{
	ALARM_RING_RUN_OFF = 0,//闹钟还没有响
	ALARM_RING_RUN_ON  = 1,//闹钟响了
}ALARM_RING_RUN_E_TypeDef;//E:enum

typedef struct
{
	ALARM_ONOFF_E_TypeDef		OnOff_TD;//闹钟总开关
	uint8_t						week;
	uint8_t						hour;  //判断此变量的值，通过LED显示出来，也可以直接对此变量进行设置
	uint8_t						minute;//判断此变量的值，通过LED显示出来，也可以直接对此变量进行设置
	uint8_t						tempHour;  //因为设置闹钟有个确认设置(虽然作用不大),所以需要有个临时变量作为中间值，等
	uint8_t						tempMinute;//到缺人设置或者30s后自动确认再把tempXxx这几个变量赋给闹钟的最后设定时间
	ALARM_RING_RUN_E_TypeDef	RingRun_TD;
	ALARM_RING_TIMER_E_TypeDef 	RingTimer_TD;//闹钟响闹时间为60分钟
	ALARM_WORK_MODE_E_TypeDef 	WorkMode_TD;
	ALARM_SNOOZE_E_TypeDef 		Snooze_TD;
	uint8_t 					snoozeTime;//贪睡时间
}ALRAM_ST_TypeDef;//ST:struct

/*************************************************************/
/*标志位定义Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/

/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/

/*************************************************************/
/*函数声明Function Declaration*********************************/
/*************************************************************/
void Sys_Tick(void);
void app_main(void);

/*************************************************************/
/*函数定义Function Definition**********************************/
/*************************************************************/

/*************************************************************/
/*外部调用_宏定义Macro Definition******************************/
/*************************************************************/

/*************************************************************/
/*外部调用_类型定义Byte Definition*****************************/
/*************************************************************/
extern ALRAM_ST_TypeDef  AL1_TD;//闹钟相关
/*************************************************************/
/*外部调用_标志位定义Flags Definition***************************/
/*************************************************************/
extern bit    Flag_12HourDisplay;
extern bit    Flag_0_5s;
extern bit    Flag_LeapYear;
/*************************************************************/
/*外部调用_变量定义Variable Definition*************************/
/*************************************************************/
extern uint8_t  idata cntAppTick;
extern BOOL		   gRTC_HalfSecond;
extern uint8_t  idata gRTC_Sec;//RTC数据 
extern uint8_t  idata gRTC_Sec_bk;
extern uint8_t  idata gRTC_Minute;
extern uint8_t  idata gRTC_Minute_bk;
extern uint8_t  idata gRTC_Hour;
extern uint8_t  idata gRTC_Hour_bk;
extern uint8_t  idata gRTC_Hour_bk_24;//计数24小时
extern uint8_t  idata gRTC_Day;
extern uint8_t  idata gRTC_Day_bk;
extern uint8_t  idata gRTC_Month;
extern uint8_t  idata gRTC_Month_bk;
extern uint8_t  idata gRTC_Year;
extern uint8_t  idata gRTC_Year_bk;
extern uint8_t  idata gRTC_Zone;
extern uint8_t  idata gRTC_Week; //一周7天
extern uint8_t  idata cntNoFlash;
extern uint8_t		  gbDimmer;//亮度

extern uint8_t  idata sys_Volume;

/*************************************************************/
/*外部调用_数组定义Array Definition****************************/
/*************************************************************/

/*************************************************************/
/*外部调用_函数声明Function Declaration************************/
/*************************************************************/

/*************************************************************/
/*外部调用_函数定义Function Definition*************************/
/*************************************************************/








#endif








