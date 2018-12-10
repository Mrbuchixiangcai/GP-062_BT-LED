/*******************************************************************************
*说明：LED控制说明：
*	   "P2 = 0x02;P3 = 0x07;"：这是4个“七段数码管”第二个位的后四段亮，
*	   "P2 = 0x02;P3 = ~0x07;"：这是4个“七段数码管”第二个位的前三段亮，
*	   即是低电平导通，因为P2给高电平
********************************************************************************/

/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*类型定义Byte Definition**************************************/
/*************************************************************/
TIME_TEMP_ST_TypeDef  Time_Temp_TD;
ALRAM_ST_TypeDef  AL1_TD;//闹钟相关
UCT_TIME_E_TypeDef UCT_TD;//时区

/*************************************************************/
/*标志位定义Flags Definition***********************************/
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
bit    Flag_LeapYear;//闰年标志，1为闰年，0为平年
BOOL   Flag_PowerOn;//开关机状态，1为开机
BOOL   gbDispleyFull;//显示满，1为满
BOOL   SleepEnable;

uint8_t idata Flag_DayFull_Add1Month;//每个月过完就加1月值，
uint8_t idata Flag_MonthFull_Add1Year;//每年过完就加1年值，

uint8_t idata Flag_ZoneStart;
uint8_t idata Flag_ZoneStop;

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
uint8_t  idata cntAppTick;

BOOL		   gRTC_HalfSecond;
uint8_t  idata gRTC_Sec;//RTC数据 
uint8_t  idata gRTC_Sec_bk;
char     idata gRTC_Minute;
uint8_t  idata gRTC_Minute_bk;
uint8_t  idata gRTC_Hour;
uint8_t  idata gRTC_Hour_bk;
uint8_t  idata gRTC_Hour_bk_24;//计数24小时
uint8_t  idata gRTC_Day;
uint8_t  idata gRTC_Day_bk;
uint8_t  idata gRTC_Month;
uint8_t  idata gRTC_Month_bk;
uint8_t  idata gRTC_Year;
uint8_t  idata gRTC_Year_bk;
uint8_t  idata gRTC_Zone;
uint8_t  idata gRTC_Zone_bk;
uint8_t  idata gRTC_Week; //一周7天
uint8_t  idata cntNoFlash;
uint8_t		   gbDimmer;//亮度
uint8_t  idata cnt0_5s;//计数到500ms就反转Flag_0_5s

uint8_t  idata sys_Volume;
uint8_t  idata cntBeep;//闹钟响时并且为beep模式
uint8_t  idata cntAlarmRuningMinuteOFF;//闹钟响多少分钟关闭


/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/

/*************************************************************/
/*函数声明Function Declaration*********************************/
/*************************************************************/

/*************************************************************/
/*函数定义Function Definition**********************************/
/*************************************************************/
/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：关闭闹铃打包多条语句
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Alarm1PowerOFF(void)
{
	AL1_TD.OnOff_TD = ALARM_OFF;//闹钟开关，关
	AL1_TD.RingRun_TD = ALARM_RING_RUN_OFF;//闹钟运行状态，关
	AL1_TD.Snooze_TD=ALARM_SNOOZE_OFF;//闹钟贪睡模式，没有贪睡
	_BEEP_MUTE(0);
	BUZ_OnOff(0);
	cntBeep=0;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：退出显示计数
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DisplayStatusExit(void)
{
	if (cntDisplayStatus)
	{
		if (++cnt0_5s >= cDISP_DELAY_500ms)//10ms调用一次，计数到500ms就进入
		{
			cnt0_5s = 0;
			Flag_0_5s = ~Flag_0_5s;
			if(Flag_0_5s)  //每当Flag_0_5s为1，Flag_Year_0_5s_Disp就反转，这样就是1s反转一次
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
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：时间比较，每秒钟进来一次
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Compare_1MinutePorc(void)
{
	//static uint8_t  cntSetVolume; //此变量只在上电时初始化一次，再调用此函数时不会执行此语句
	/*******************************************************************************
	 *功能：
	 *
	 *这里是1s进来一次
	 *******************************************************************************/
	if(gRTC_Sec!=gRTC_Sec_bk)
	{
		gRTC_Sec_bk=gRTC_Sec;
		
		/*******************************************************************************
		 *功能：
		 *
		 *这里是1s进来一次
		 *******************************************************************************/
		if(gRTC_Minute!=gRTC_Minute_bk)//分钟
		{
			gRTC_Minute_bk=gRTC_Minute;
			
			/*******************************************************************************
			 *功能：闹钟开启并且hour和minute相等，闹钟响应
			 *	因为是24小时的实际计时，所以不用判断上午下午
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.hour==gRTC_Hour) && (AL1_TD.minute==gRTC_Minute))
			{
				if(AL1_TD.RingRun_TD==ALARM_RING_RUN_OFF)//
				{
					AL1_TD.RingRun_TD=ALARM_RING_RUN_ON;//把闹钟运行状态设为ON
					if(Flag_BT_Play_Payse==1)
					{
						bt_cmd=BT_PLAY_PAUSE;
					}
				}
			}
			
			/*******************************************************************************
			 *功能：闹钟开启&&在运行&&贪睡时间和RTC时间相等&&贪睡模式开
			 *
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON) && (AL1_TD.Snooze_TD == ALARM_SNOOZE_ON)
			 && (AL1_TD.snoozeRTCHour==gRTC_Hour) && (AL1_TD.snoozeRTCMinute==gRTC_Minute))
			{
				AL1_TD.Snooze_TD = ALARM_SNOOZE_OFF;//再次开启贪睡
			}
			
			/*******************************************************************************
			 *功能：闹钟如果已经开启，并且在运行，计数30分钟关闭，无论是否是贪睡模式
			 *
			 *******************************************************************************/
			if((AL1_TD.OnOff_TD == ALARM_ON) && (AL1_TD.RingRun_TD == ALARM_RING_RUN_ON))//闹钟已经开启，并且在运行,
			{
				cntAlarmRuningMinuteOFF++;
				if(cntAlarmRuningMinuteOFF>=30)//30分钟到了
				{
					Alarm1PowerOFF();
				}
			}
			
			/*******************************************************************************
			 *功能：
			 *
			 *这里是1hour进来一次
			 *******************************************************************************/
			if(gRTC_Hour!=gRTC_Hour_bk)//小时
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
	
	if((AL1_TD.OnOff_TD == ALARM_ON) && (AL1_TD.RingRun_TD == ALARM_RING_RUN_ON))//闹钟已经开启，并且在运行,
	{
		if(AL1_TD.Snooze_TD == ALARM_SNOOZE_OFF)//闹钟如果不在贪睡模式
		{
			_BEEP_MUTE(1);//解mute
			P1 |= 0x40;
			cntBeep++;
			if(cntBeep<40)
				BUZ_OnOff(1);
			else if((cntBeep>=40) && (cntBeep<80))
				BUZ_OnOff(0);
			else 
				cntBeep=0;
		}
		else if(AL1_TD.Snooze_TD == ALARM_SNOOZE_ON)//闹钟如果在贪睡模式
		{
			cntBeep=0;
			_BEEP_MUTE(0);//因为是闹钟开启并且在运行状态，所以不会影响其他时间的蓝牙播放
			BUZ_OnOff(0);
		}
	}
	
	/*******************************************************************************
	*功能：gRTC_Zone1-7,选择对应的时区
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
*功能：闹钟开启，并且已经响应
*这里是10ms进来一次
*******************************************************************************/
void AlarmRunning(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：滴答定时器计时函数
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
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
		if((--uart1_TX_Timeout)==0)//发送超时就清零发送标志位
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
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：初始化函数
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void PowerON_Init(void)
{
	gRTC_Year=18;	  //上电默认2018年
	gRTC_Month=12;	  //上电默认12月
	gRTC_Day=1;		  //上电默认为1号
	gRTC_Week=6;	  //12月号是周六
	gRTC_Hour=0;	  //上电默认为凌晨零点零分零秒
	gRTC_Minute=0;	  //上电默认为
	gRTC_Sec=0;		  //上电默认为
	gRTC_Hour_bk=0;	  //上电默认为凌晨零点零分零秒
	gRTC_Minute_bk=0; //上电默认为
	gRTC_Sec_bk=0;	  //上电默认为
	Flag_LeapYear=0;  //2018年不是闰年
	Flag_12HourDisplay=1;//12小时制
	gRTC_Zone=6;	  //所在时区，初始化为华盛顿特区时间,西五区
	UCT_TD=UTC_SUB_5; //所在时区，初始化为华盛顿特区时间,西五区
	
	cntNoFlash = 0;
	cnt0_5s=0;
	Flag_PowerOn = 1;
	gbDimmer=18;
	AL1_TD.snoozeTime=10;//默认为10分钟

	
//	gRTC_Hour=23;
//	gRTC_Minute=39;
//	AL1_TD.OnOff_TD = ALARM_ON;
//	AL1_TD.hour=0;
//	AL1_TD.minute=10;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：app_main函数
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
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










