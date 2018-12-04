/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"
#include "key_mode.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/

/*************************************************************/
/*类型定义Byte Definition**************************************/
/*************************************************************/

/*************************************************************/
/*标志位定义Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
uint8_t  gbKeyPress; //按键按下
uint8_t  gbKeyNone;  //没有按键按下
uint8_t  KeyValue;   //按键值
uint8_t  KeyEvent;   //按键事件
uint8_t  cntKeyLoop; //计数按键循环，短按，长按，超长按的按下时间
uint8_t  cntKeyLong;
uint8_t  KeyCurValueBK;//现在按键备份
uint8_t  KeyCurValue;//按键现在值
uint8_t  KeyLastValue;//按键上一次的值

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
* 函数功能：获取按键值
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
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
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：按键事件处理
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void KeyEventPorc(uint8_t KeyTmp) //被调用时接收的是GetKeyValue()的返回值  一般固定，不更改
{
	gbKeyPress=0;
	if(KeyCurValue!=KeyTmp) //这个if语句是去抖，会被执行两次，第三次进来时直接跳过
	{//KeyTmp为临时值，
		if(KeyCurValueBK!=KeyTmp)
			KeyCurValueBK=KeyTmp;
		else //第二次进来时执行这一条
			KeyCurValue=KeyTmp; //
		return;
	}
	if(KeyCurValue==KeyLastValue)
	{
		if((KeyLastValue==0) || (cntKeyLoop==0))
			return;
		if(--cntKeyLoop==0) //这个在第二次进KeyEventPorc()函数（第一次进“if(KeyCurValue==KeyLastValue)”之前）时被下面赋值为5
		{ //cntKeyLoop为5就是50ms时间
			KeyValue=KeyLastValue; //防抖之后把确定的按键赋值给KeyValue
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
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：主函数调用
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void KeyScan(void) //被主函数调用
{
	KeyEventPorc(GetKeyValue());
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：按键对应处理
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：闰年说明：
*		目前使用的格里高利历闰年规则如下[2]：
*		公元年分除以4不可整除，為平年。
*		公元年分除以4可整除但除以100不可整除，为闰年。
*		公元年分除以100可整除但除以400不可整除，为平年。
*		公元年分除以400可整除但除以3200不可整除[來源請求]，为闰年。
*		计算方法：*
*		if ((y % 4 == 0 and y % 100 != 0) or (y % 400 == 0 and y % 3200 != 0)):
*			print("閏年")
*		else:
*			print("平年")
*******************************************************************************/
void KeyComMsg(void) 
{
	if(gbKeyPress)
	{ 
	   	switch(KeyValue)
		{
			case KU(K_VOLINC): //value67 没有break所以继续执行下一条语句
			case KR(K_VOLINC): //value131
			{
				switch(Flag_DisplayStatus)
				{
					case ADJ_YEAR:
					{
						if(++gRTC_Year > 99)
							gRTC_Year=18;
						if(gRTC_Month==2)
						{
							if((gRTC_Year % 4) ==0)//这里数值是2018-2099，所以不用"gRTC_Year % 100"
							{
								Flag_LeapYear=1;//闰年
							}
							else
							{
								Flag_LeapYear=0;
							}
						}
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_MONTH:
					{
						if(--gRTC_Month<1)
							gRTC_Month=12;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_DAY:
					{
						gRTC_Day++;
						if(gRTC_Month==2)
						{
							if(Flag_LeapYear==1)//闰年
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
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_HOUR:
					{
						if(++gRTC_Hour>23)
							gRTC_Hour=0;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_MINUTE:
					{
						if(++gRTC_Minute>59)
							gRTC_Minute=0;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_DAYLIGHT_SAVING_ZONES:
					{
						if(++gRTC_Zone>7)
							gRTC_Zone=1;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_SNOOZE_TIME:
					{
						AL1_TD.snoozeTime++;
						break;
					}
					default:
						break;
				}
				break;
			}
			case KU(K_VOLDEC): //value68
			case KR(K_VOLDEC): //value132
			{
				switch(Flag_DisplayStatus)
				{
					case ADJ_YEAR:
					{
						if(--gRTC_Year < 18)
							gRTC_Year=99;
						if(gRTC_Month==2)
						{
							if((gRTC_Year % 4) ==0)//这里数值是2018-2099，所以不用"gRTC_Year % 100"
							{
								Flag_LeapYear=1;//闰年
							}
							else
							{
								Flag_LeapYear=0;
							}
						}
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_MONTH:
					{
						if(--gRTC_Month<1)
							gRTC_Month=12;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_DAY:
					{
						if(--gRTC_Day<1)
						{
							if(gRTC_Month==2)
							{
								if(Flag_LeapYear==1)
									gRTC_Day=29;	//闰年
								else
									gRTC_Day=28;
							}
							else if((gRTC_Month==4) || (gRTC_Month==6)|| (gRTC_Month==9)|| (gRTC_Month==11))
								gRTC_Day=30;
							else
								gRTC_Day=31;
						}
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_HOUR:
					{
						if(--gRTC_Hour<0)
							gRTC_Hour=23;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_MINUTE:
					{
						if(--gRTC_Minute<0)
							gRTC_Minute=59;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_DAYLIGHT_SAVING_ZONES:  //夏令时区选择
					{
						if(--gRTC_Zone<1)
							gRTC_Zone=7;
						cntDisplayStatus=cDISP_DELAY_2SEC;
						break;
					}
					case ADJ_SNOOZE_TIME:
					{
						if(AL1_TD.snoozeTime>0)
							AL1_TD.snoozeTime--;
						break;
					}
					default:
						break;
				}
				break;
			}
			case KU(K_SNOOZE_DIMMER): 
			{
				
				break;
			}
			case KH(K_SNOOZE_DIMMER):
			{
				
				break;
			}
			case KU(K_ALARM): 
			{
				if(Flag_DisplayStatus==ADJ_YEAR)//
				{
					SetDisplayState2s(ADJ_MONTH);//设置月份
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_MONTH)//
				{
					SetDisplayState2s(ADJ_DAY);//设置日期
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_DAY)//
				{
					SetDisplayState2s(ADJ_HOUR);//设置小时
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_HOUR)//
				{
					SetDisplayState2s(ADJ_MINUTE);//设置分钟
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_MINUTE)//
				{
					SetDisplayState2s(ADJ_DAYLIGHT_SAVING_ZONES);//设置夏令时去选择
					gRTC_Sec = 0;
				}
				else if(Flag_DisplayStatus==ADJ_DAYLIGHT_SAVING_ZONES)//
				{
					SetDisplayState2s(ADJ_SNOOZE_TIME);//设置贪睡时间
					gRTC_Sec = 0;
				}
				break;
			}
			case KH(K_ALARM):
			{
				
				break;
			}
			case KR(K_AL_SN):
			{
				if(cntKeyLong>=15)//长按6s设置RTC时间
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



















