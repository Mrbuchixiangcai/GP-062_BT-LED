/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/
uint8_t displayBuff[5];
#define DIG_NUM1 displayBuff[0]
#define DIG_NUM2 displayBuff[1]
#define DIG_NUM3 displayBuff[2]
#define DIG_NUM4 displayBuff[3]
#define DIG_NUM5 displayBuff[4]

/*************************************************************/
/*类型定义Byte Definition**************************************/
/*************************************************************/
LED_BRIGHTNESS_E_TypeDef LED_Brightness_TD;//TD:TypeDef
/*************************************************************/
/*标志位定义Flags Definition***********************************/
/*************************************************************/
BOOL	gbUser_AdjClk;

uint8_t bdata LCD_BitRam0;
uint8_t bdata LCD_BitRam1;
sbit gbLCD_Beep1    = LCD_BitRam0^0;
sbit gbLCD_Dot      = LCD_BitRam0^1;
sbit gbLCD_AM       = LCD_BitRam0^2;
sbit gbLCD_PM       = LCD_BitRam0^3;
sbit gbLCD_BT_LED   = LCD_BitRam0^4;
sbit gbLCD_AL_LED   = LCD_BitRam0^5;

uint8_t  Flag_DisplayStatus;//显示状态，显示某个状态时会被赋值

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
uint16_t cntDisplayStatus;
uint8_t  idata LED_Reg1;
uint8_t  idata LED_Reg2;
uint8_t  idata LED_Reg3;
uint8_t  idata LED_Reg4;
uint8_t  idata LED_Reg5;
uint8_t  idata LED_Reg6;
uint8_t  idata Disp_cnt;

/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/
code uint8_t DisplayCode[]=
{
	DIG_0,            
	DIG_1,           
	DIG_2,           
	DIG_3,           
	DIG_4,          
	DIG_5,           
	DIG_6,           
	DIG_7,           
	DIG_8,           
	DIG_9,           
	DIG_NONE,             
	CH_a,            
	CH_b,           
	CH_c,            
	CH_d,            
	CH_h,            
	CH_l,            
	CH_n,            
	CH_o,           
	CH_r,           
	CH_t,            
	CH_u,             
	CH_y,             
	CH_A,            
	CH_C,           
	CH_E,            
	CH_F,            
	CH_G,            
	CH_H,            
	CH_I,             
	CH_J,            
	CH_L,           
	CH_N,            
	CH_O,            
	CH_P,            
	CH_S,            
	CH_U,             
	CH__
};
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
* 函数功能：清除显示缓存
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void ClearDisplayBuff(void)
{
	uint8_t i;
	for(i=0;i<5;i++)
		displayBuff[i]=eD_NONE;
	LCD_BitRam0=0;
	LCD_BitRam1=0;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：把数值转化为显示格式
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DisplayNum12(uint8_t dat)
{
	if(dat / 10)
		DigNum1((dat/10));
	DigNum2((dat%10));
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：把数值转化为显示格式
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DisplayNum34(uint8_t dat)
{
	DigNum3((dat/10));
	DigNum4((dat%10));
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：显示OFF
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_Off(void)
{
	DIG_NUM1=eD_NONE;
	DIG_NUM2=eD_O;
	DIG_NUM3=eD_F;
	DIG_NUM4=eD_F;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：时间分割
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_HH_MM(uint8_t hhmmType)
{
	uint8_t tmpHour,tmpMinute;
	switch(hhmmType)
	{
		case eDS_RTC:  	//显示RTC
			tmpHour = gRTC_Hour;
			tmpMinute = gRTC_Minute;
			break;
		case ADJ_CLK:  	//设置RTC
			tmpHour = gRTC_Hour;
			tmpMinute = gRTC_Minute;
			break;
		case eDS_AL1:  	//显示闹钟时间
			tmpHour = gRTC_Hour;
			tmpMinute = gRTC_Minute;
			break;
		case ADJ_ALARM1://设置时显示闹钟时间
			tmpHour = gRTC_Hour;
			tmpMinute = gRTC_Minute;
			break;
	}
	if(Flag_12HourDisplay)
	{
		if(tmpHour<12)
		{ 
			gbLCD_AM=1;
		}
		else
		{ 
			gbLCD_PM=1;
			tmpHour-=12;
		}
		if(tmpHour==0)
			tmpHour=12;           
	}
	DisplayNum12(tmpHour);
	DisplayNum34(tmpMinute);
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：不闪
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
BOOL NoFlash(void)
{
	if(cntNoFlash != 0)
		return 1;
	return 0;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：RTC显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_RTC(void)
{
	if((!gbUser_AdjClk)||(gRTC_HalfSecond))
	{
		Display_HH_MM(eDS_RTC);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetRTC(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		Display_HH_MM(eDS_RTC);
	}
}


/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的年份显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
uint8_t cntYearDisplay=0;
void Display_SetRTCYear(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		//0.5进来一次，20和18交替显示
		if(cntYearDisplay==0)
		{
			cntYearDisplay=1;
			//DisplayNum12(20);
			SET_4G();//显示一个"-"
			DisplayNum34(20);
		}
		else
		{
			cntYearDisplay=0;
			SET_4G();//显示一个"-"
			DisplayNum34(gRTC_Year);
		}
		
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的月的LED显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetRTCDate(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum12(gRTC_Month);//不显示
		DisplayNum34(gRTC_Day);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的日的LED显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
//这里暂时不用
void Display_SetRTCDay(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum34(gRTC_Day);
	}
}


/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的小时的LED显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetRTCHour(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum12(ADJ_CLK);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置RTC时LED的分钟的LED显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetRTCMinute(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum34(ADJ_CLK);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：夏令时区选择
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetZone(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum34(gRTC_Zone);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：贪睡时间
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SnoozeTime(void)
{
	if((Flag_0_5s)||(NoFlash()))
	{
		DisplayNum34(gRTC_Day);
	}
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：闹钟非设置时的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_Alarm1(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：闹钟设置时的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetAlarm1(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置闹钟小时的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetAlarm1_Hour(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置闹钟分钟的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetAlarm1_Min(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：设置闹钟工作模式的显示
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_SetAlarm1_Work(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：在某个状态时显示10秒，比如设置RTC或者闹钟时闪烁或者显示长度
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void SetDisplayState10s(uint8_t status)
{
	Flag_DisplayStatus=status;//把要显示的状态赋值给全局变量
	cntDisplayStatus=cDISP_DELAY_10SEC;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：在某个状态时显示2秒，比如设置RTC或者闹钟时闪烁或者显示长度
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void SetDisplayState2s(uint8_t status)
{
	Flag_DisplayStatus=status;//把要显示的状态赋值给全局变量
	cntNoFlash=cntDisplayStatus=cDISP_DELAY_15SEC;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：更新要显示的内容
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void UpdateDisplay(void)
{
	switch (Flag_DisplayStatus)
	{
		case ADJ_CLK:
			Display_SetRTC(); 		 //设置RTC时的显示
			break;
		case ADJ_YEAR:				 //设置RTC的年
			Display_SetRTCYear();
			break;
		case ADJ_MONTH:				 //设置RTC的日期
		case ADJ_DAY:				 //设置RTC的日期
			Display_SetRTCDate();
			break;
		case ADJ_HOUR:				 //设置RTC小时的显示
			Display_SetRTCHour();
			break;
		case ADJ_MINUTE:			 //设置RTC分钟的显示
			Display_SetRTCMinute();
			break;
		case ADJ_DAYLIGHT_SAVING_ZONES://设置夏令时区
			Display_SetZone();
			break; 
		case ADJ_SNOOZE_TIME:		 //设置贪睡时间
			Display_SnoozeTime();
			break; 
		case DISP_AL1:				 //闹钟非设置时的显示
			Display_Alarm1();   
			break;
		case ADJ_ALARM1:			 //设置闹钟的显示
			Display_SetAlarm1(); 	 
			break;
		case ADJ_ALARM1_HOUR:		 //设置闹钟小时的显示
			Display_SetAlarm1_Hour();
			break; 
		case ADJ_ALARM1_MIN:		 //设置闹钟分钟的显示
			Display_SetAlarm1_Min(); 
			break; 
		case POWER_OFF_DISP:		 //关闭显示
			Display_Off();
			break; 
		default:
			Display_RTC();    		 //RTC走时状态显示
			break;
	}
	if (cntNoFlash != 0)
		cntNoFlash--;
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：需要亮的一些标记，比如点，或者闹钟标志
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_Flag(void)
{
	
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：更新要显示的内容
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display(void)
{
	//uint8_t i;
	//if(cntDisplayStatus)
	//{
		ClearDisplayBuff();
		UpdateDisplay();
		Display_Flag();
		Display_LED();
		cli();
		LED_Tmp1 = LED_Reg1;
		LED_Tmp2 = LED_Reg2;
		LED_Tmp3 = LED_Reg3;
		LED_Tmp4 = LED_Reg4;
		sei();
	//}
}






