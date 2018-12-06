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
				if(Flag_DisplayStatus==0)//为0说明不是其他操作，是音量操作
				{
					bt_cmd=BT_VOLINC;
				}
				else
				{
					switch(Flag_DisplayStatus)
					{
						case ADJ_YEAR: //设置年份
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
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MONTH://设置月份
						{
							if(--gRTC_Month<1)
								gRTC_Month=12;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAY://设置日期
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
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_HOUR: //设置RTC小时
						{
							if(++gRTC_Hour>23)
								gRTC_Hour=0;
	//						if (Flag_12HourDisplay)//12小时制显示
	//						{
	//							if(gRTC_Hour<12)//如果小于12则是凌晨0点到11:59，为上午
	//							{
	//								Flag_APM=0;  //上午
	//							}
	//							else if (gRTC_Hour > 12)  
	//							{
	//								Flag_APM = 1;   //如果大于12则是12:00到23:59，为下午//标志位为1，设置对应的LED是否亮起
	//								gRTC_Hour -= 12;//因为是12小时制，所以下午的时间也是12小时制显示，要减12
	//							}
	//							if (gRTC_Hour == 0) //零点是显示12的，中午12点也是显示12的
	//							{
	//								gRTC_Hour = 12;
	//							}
	//						}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MINUTE://设置RTC分钟
						{
							if(++gRTC_Minute>59)
								gRTC_Minute=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAYLIGHT_SAVING_ZONES://设置夏令时区
						{
							if(++gRTC_Zone>8)
								gRTC_Zone=1;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_SNOOZE_TIME://设置贪睡时间
						{
							if(++AL1_TD.snoozeTime>10)
								AL1_TD.snoozeTime=1;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_ALARM1_HOUR: //设置闹钟小时
							if(++AL1_TD.hour>23)
								AL1_TD.hour=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						case ADJ_ALARM1_MINUTE: //设置闹钟分钟
							if(++AL1_TD.minute>59)
								AL1_TD.minute=0;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						default:
							break;
					}
					cntNoFlash=cDISP_DELAY_500ms;//每次进来就有1s不闪烁，为了在长按增加/减小时第一次进来是KU(),第二次才是KR()
				}
				break;
			}
			case KU(K_VOLDEC): //value68
			case KR(K_VOLDEC): //value132
			{
				if(Flag_DisplayStatus==0)//为0说明不是其他操作，是音量操作
				{
					bt_cmd=BT_VOLDEC;//音量减
				}
				else
				{
					switch(Flag_DisplayStatus)
					{
						case ADJ_YEAR://摄制年份
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
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MONTH://设置月份
						{
							if(--gRTC_Month<1)
								gRTC_Month=12;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAY://设置日期
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
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_HOUR://设置RTC小时
						{
							if(--gRTC_Hour<0)
								gRTC_Hour=23;
	//						if (Flag_12HourDisplay)//12小时制显示
	//						{
	//							if(gRTC_Hour<12)//如果小于12则是凌晨0点到11:59，为上午
	//							{
	//								Flag_APM=0;  //上午
	//							}
	//							else if (gRTC_Hour > 12)  
	//							{
	//								Flag_APM = 1;   //如果大于12则是12:00到23:59，为下午//标志位为1，设置对应的LED是否亮起
	//								gRTC_Hour -= 12;//因为是12小时制，所以下午的时间也是12小时制显示，要减12
	//							}
	//							if (gRTC_Hour == 0) //零点是显示12的，中午12点也是显示12的
	//							{
	//								gRTC_Hour = 12;
	//							}
	//						}
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_MINUTE://设置RTC分钟
						{
							if(--gRTC_Minute<0)
								gRTC_Minute=59;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_DAYLIGHT_SAVING_ZONES:  //夏令时区选择
						{
							if(--gRTC_Zone<1)
								gRTC_Zone=8;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_SNOOZE_TIME://设置贪睡时间
						{
							if(--AL1_TD.snoozeTime<1)
								AL1_TD.snoozeTime=10;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						}
						case ADJ_ALARM1_HOUR: //设置闹钟小时
							if(--AL1_TD.hour<0)
								AL1_TD.hour=23;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						case ADJ_ALARM1_MINUTE: //设置闹钟分钟
							if(--AL1_TD.minute<0)
								AL1_TD.minute=59;
							cntDisplayStatus=cDISP_DELAY_5SEC;
							break;
						default:
							break;
					}
					cntNoFlash=cDISP_DELAY_500ms;//每次进来就有1s不闪烁，为了在长按增加/减小时第一次进来是KU(),第二次才是KR()
				}
				break;
			}
			case KU(K_SNOOZE_DIMMER): 
			{
				if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON))//闹钟开启，并且在运行
				{
					AL1_TD.Snooze_TD=ALARM_SNOOZE_ON;//开启贪睡
					AL1_TD.snoozeHour=gRTC_Hour;//按下贪睡按键把当前的RTC时间赋值给贪睡计时，贪睡时间到时再响闹
					AL1_TD.snoozeMinute=gRTC_Minute;
					break;
				}
				else if(Flag_DisplayStatus==0)
				{
					if(gbDimmer==18)//这里形成三级调光
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
				/*年月日、RTC时间、夏令时区和贪睡时间设置*/
				cntNoFlash=cDISP_DELAY_500ms;//每次进来就有1s不闪烁，为了在长按增加/减小时第一次进来是KU(),第二次才是KR()
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
				
				/*设置闹钟*/
				if(AL1_TD.OnOff_TD==ALARM_ON)
				{
					//如果闹钟在打开状态，但是还没有响闹，短按ALARM就关闭
					Alarm1PowerOFF();
				}
				else if((AL1_TD.OnOff_TD==ALARM_ON) && (AL1_TD.RingRun_TD==ALARM_RING_RUN_ON))
				{
					//如果闹钟在运行，那短按ALARM键关闭闹钟
					Alarm1PowerOFF();
				}
				else if(Flag_DisplayStatus==0)
				{
					SetDisplayState2s(ADJ_ALARM1_HOUR);//设置闹钟小时
					AL1_TD.OnOff_TD=ALARM_ON;//开闹钟
				}
				else if(Flag_DisplayStatus==ADJ_ALARM1_HOUR)
				{
					SetDisplayState2s(ADJ_ALARM1_MINUTE);//设置闹钟小时
				}
				else if(Flag_DisplayStatus==ADJ_ALARM1_MINUTE)
				{
					Flag_DisplayStatus=0;//确认闹钟时间,
					cntDisplayStatus=0;//不闪烁，
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
				if(cntKeyLong>=5)//长按1s设置RTC时间
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



















