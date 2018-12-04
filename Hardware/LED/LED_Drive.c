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

/*************************************************************/
/*标志位定义Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
uint8_t  idata mP2;
uint8_t  idata mP3;
uint8_t  idata ScanCom;
uint8_t  idata PWMDuty;  //PWM周期
uint8_t  idata PWMCycle; //PWM循环
uint8_t  idata LED_Tmp1;
uint8_t  idata LED_Tmp2;
uint8_t  idata LED_Tmp3;
uint8_t  idata LED_Tmp4;
uint8_t  idata LED_Tmp5;
uint8_t  idata LED_Tmp6;
uint8_t  idata cnt200us;//用在定时器3中

/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/
uint8_t  led_Buffer[4];
uint8_t  led_Buffer_Copy[4];

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
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DigNum1(uint8_t dat)
{
	dat = DisplayCode[dat];
	if(dat & (1 << S_B))
		SET_1B();
	else 
		CLR_1B();
	if(dat & (1 << S_C))
		SET_1C();
	else 
		CLR_1C();
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DigNum2(uint8_t dat)
{
	dat = DisplayCode[dat];
	if(dat & (1 << S_A))
		SET_4A();
	else 
		CLR_4A();
	if(dat & (1 << S_B))
		SET_4B();
	else 
		CLR_4B();
	if(dat & (1 << S_C))
		SET_4C();
	else 
		CLR_4C();
	if(dat & (1 << S_D))
		SET_4D();
	else 
		CLR_4D();
	if(dat & (1 << S_E))
		SET_4E();
	else 
		CLR_4E();
	if(dat & (1 << S_F))
		SET_4F();
	else 
		CLR_4F();
	if((dat & (1 << S_G)) || (dat & (1 << S__)))
		SET_4G();
	else 
		CLR_4G();
}


/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DigNum3(uint8_t dat)
{
	dat = DisplayCode[dat];
	if(dat & (1 << S_A))
		SET_3A();
	else 
		CLR_3A();
	if(dat & (1 << S_B))
		SET_3B();
	else 
		CLR_3B();
	if(dat & (1 << S_C))
		SET_3C();
	else 
		CLR_3C();
	if(dat & (1 << S_D))
		SET_3D();
	else 
		CLR_3D();
	if(dat & (1 << S_E))
		SET_3E();
	else 
		CLR_3E();
	if(dat & (1 << S_F))
		SET_3F();
	else 
		CLR_3F();
	if(dat & (1 << S_G))
		SET_3G();
	else 
		CLR_3G();
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void DigNum4(uint8_t dat)
{
	dat = DisplayCode[dat];
	if(dat & (1 << S_A))
		SET_2A();
	else 
		CLR_2A();
	if(dat & (1 << S_B))
		SET_2B();
	else 
		CLR_2B();
	if(dat & (1 << S_C))
		SET_2C();
	else 
		CLR_2C();
	if(dat & (1 << S_D))
		SET_2D();
	else 
		CLR_2D();
	if(dat & (1 << S_E))
		SET_2E();
	else 
		CLR_2E();
	if(dat & (1 << S_F))
		SET_2F();
	else 
		CLR_2F();
	if(dat & (1 << S_G))
		SET_2G();
	else 
		CLR_2G();
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void LEDDisplayDrive(void)
{
	mP2=0xFF;
	mP3=0xFF;
	
	if(++PWMCycle >= MAX_PWM) //PWM周期循环
	{
		PWMCycle=0;
//		if(++ScanCom >= 4)
//			ScanCom=0;
		ScanCom=1;
		
	}
	if(PWMCycle >= gbDimmer)//如果大于设定的亮度值，就全灭，这样形成循环就是PWM
	{
		P2 = mP2 = 0xFF;
		P3 = mP3 = 0xFF;
		P3 = mP3 = 0xFF;
	}
	else if (ScanCom == 0)
	{
		COM1(LED_ON);
		if(LED_Tmp1 & BIT0) //2A
			SEG1(LED_ON);
		if(LED_Tmp1 & BIT1) //2B
			SEG2(LED_ON);
		if(LED_Tmp1 & BIT2) //2C
			SEG3(LED_ON);
		if(LED_Tmp1 & BIT3) //2D
			SEG4(LED_ON);
		if(LED_Tmp1 & BIT4) //2E
			SEG5(LED_ON);
		if(LED_Tmp1 & BIT5) //2F
			SEG6(LED_ON);
		if(LED_Tmp1 & BIT6) //2G
			SEG7(LED_ON);
	}
	else if (ScanCom == 1)
	{
		COM2(LED_ON);
		if(LED_Tmp2 & BIT0) //3A
			SEG1(LED_ON);
		if(LED_Tmp2 & BIT1) //3B
			SEG2(LED_ON);
		if(LED_Tmp2 & BIT2) //3C
			SEG3(LED_ON);
		if(LED_Tmp2 & BIT3) //3D
			SEG4(LED_ON);
		if(LED_Tmp2 & BIT4) //3E
			SEG5(LED_ON);
		if(LED_Tmp2 & BIT5) //3F
			SEG6(LED_ON);
		if(LED_Tmp2 & BIT6) //3G
			SEG7(LED_ON);
	}
	else if (ScanCom == 2)
	{
		COM3(LED_ON);
		if(LED_Tmp3 & BIT0) //4A
			SEG1(LED_ON);
		if(LED_Tmp3 & BIT1) //4B
			SEG2(LED_ON);
		if(LED_Tmp3 & BIT2) //4C
			SEG3(LED_ON);
		if(LED_Tmp3 & BIT3) //4D
			SEG4(LED_ON);
		if(LED_Tmp3 & BIT4) //4E
			SEG5(LED_ON);
		if(LED_Tmp3 & BIT5) //4F
			SEG6(LED_ON);
		if(LED_Tmp3 & BIT6) //4G
			SEG7(LED_ON);
	}
	else if (ScanCom == 3)
	{
		COM4(LED_ON);
		if(LED_Tmp4 & BIT0) //ALA
			SEG1(LED_ON);
		if(LED_Tmp4 & BIT1) //1B
			SEG2(LED_ON);
		if(LED_Tmp4 & BIT2) //1C
			SEG3(LED_ON);
		if(LED_Tmp4 & BIT3) //PM
			SEG4(LED_ON);
		if(LED_Tmp4 & BIT4) //CL1
			SEG5(LED_ON);
		if(LED_Tmp4 & BIT5) //CL
			SEG6(LED_ON);
	}
	P2 = mP2; //这里统一赋值，消除时间差
	P3 = mP3; 
	P3 = mP3;//测试用
}

/*******************************************************************************
* 函数原型：
* 输入参数：
* 输出参数：
* 函数功能：
* 返回值说明：
* 创建日期：
* 创建人：
* 修改日期
* 修改人：
* 第N次修改：
* 修改原因：
* 备注：
*******************************************************************************/
void Display_LED(void)
{
	
}









