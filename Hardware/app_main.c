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
BOOL   gbPowerOn;//开关机状态，1为开机
BOOL   gbDispleyFull;//显示满，1为满
BOOL   SleepEnable;

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
uint8_t  idata cntAppTick;

BOOL		   gRTC_HalfSecond;
uint8_t  idata gRTC_Sec;//RTC数据 
uint8_t  idata gRTC_Sec_bk;
uint8_t  idata gRTC_Minute;
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
uint8_t  idata gRTC_Week; //一周7天
uint8_t  idata cntNoFlash;
uint8_t		   gbDimmer;//亮度
uint8_t  idata cnt0_5s;//计数到500ms就反转Flag_0_5s

uint8_t  idata sys_Volume;


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
	gRTC_Year=18;	//上电默认2018年
	gRTC_Month=11;	//上电默认11月
	gRTC_Day=0;		//上电默认为1号
	gRTC_Hour=0;	//上电默认为凌晨零点零分零秒
	gRTC_Minute=0;	//上电默认为
	gRTC_Sec=0;		//上电默认为
	gRTC_Hour_bk=0;	//上电默认为凌晨零点零分零秒
	gRTC_Minute_bk=0;//上电默认为
	gRTC_Sec_bk=0;	//上电默认为
	Flag_LeapYear=0;//2018年不是闰年
	
	cntNoFlash = 0;
	cnt0_5s=0;
	gbPowerOn = 0;
	gbDimmer=3;
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










