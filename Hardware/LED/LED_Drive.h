/*************************************************************/
/*头文件定义Header File Definition*****************************/
/*************************************************************/
#ifndef __LED_DRIVE_H__
#define __LED_DRIVE_H__

/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/
#define LED_ON   0
#define LED_OFF  1
#define MAX_PWM  5

#define BIT0   0x01  
#define BIT1   0x02
#define BIT2   0x04
#define BIT3   0x08
#define BIT4   0x10
#define BIT5   0x20
#define BIT6   0x40
#define BIT7   0x80
#define CBIT0  0xFE
#define CBIT1  0xFD
#define CBIT2  0xFB
#define CBIT3  0xF7
#define CBIT4  0xEF
#define CBIT5  0xDF
#define CBIT6  0xBF
#define CBIT7  0x7F

/*端口组定义*/
#define COM1(a) if(a)							\
					do{mP2 |=  BIT1;}while(0);	\
				else							\ 
					do{mP2 &= CBIT1;}while(0)
#define COM2(a) if(a)							\
					do{mP2 |=  BIT2;}while(0);	\
				else							\ 
					do{mP2 &= CBIT2;}while(0)
#define COM3(a) if(a)							\
					do{mP2 |=  BIT3;}while(0);	\
				else							\ 
					do{mP2 &= CBIT3;}while(0)
#define COM4(a) if(a)							\
					do{mP2 |=  BIT4;}while(0);	\
				else							\ 
					do{mP2 &= CBIT4;}while(0)
#define SEG1(a) if(a)							\
					do{mP3 |=  BIT0;}while(0);	\
				else							\ 
					do{mP3 &= CBIT0;}while(0)
#define SEG2(a) if(a)							\
					do{mP3 |=  BIT1;}while(0);	\
				else							\ 
					do{mP3 &= CBIT1;}while(0)
#define SEG3(a) if(a)							\
					do{mP3 |=  BIT2;}while(0);	\
				else							\ 
					do{mP3 &= CBIT2;}while(0)
#define SEG4(a) if(a)							\
					do{mP3 |=  BIT3;}while(0);	\
				else							\ 
					do{mP3 &= CBIT3;}while(0)
#define SEG5(a) if(a)							\
					do{mP3 |=  BIT4;}while(0);	\
				else							\ 
					do{mP3 &= CBIT4;}while(0)
#define SEG6(a) if(a)							\
					do{mP3 |=  BIT5;}while(0);	\
				else							\ 
					do{mP3 &= CBIT5;}while(0)
#define SEG7(a) if(a)							\
					do{mP3 |=  BIT6;}while(0);	\
				else							\ 
					do{mP3 &= CBIT6;}while(0)

/*SET具体功能定义*/
//SET4个"七段数码管"最左第一位
#define SET_1B()     do{LED_Reg4 |=  BIT1;}while(0)
#define SET_1C()     do{LED_Reg4 |=  BIT2;}while(0)

//SET4个"七段数码管"第二位
#define SET_2A()     do{LED_Reg1 |=  BIT0;}while(0)
#define SET_2B()     do{LED_Reg1 |=  BIT1;}while(0)
#define SET_2C()     do{LED_Reg1 |=  BIT2;}while(0)
#define SET_2D()     do{LED_Reg1 |=  BIT3;}while(0)
#define SET_2E()     do{LED_Reg1 |=  BIT4;}while(0)
#define SET_2F()     do{LED_Reg1 |=  BIT5;}while(0)
#define SET_2G()     do{LED_Reg1 |=  BIT6;}while(0)

//SET4个"七段数码管"第三位
#define SET_3A()     do{LED_Reg2 |=  BIT0;}while(0)
#define SET_3B()     do{LED_Reg2 |=  BIT1;}while(0)
#define SET_3C()     do{LED_Reg2 |=  BIT2;}while(0)
#define SET_3D()     do{LED_Reg2 |=  BIT3;}while(0)
#define SET_3E()     do{LED_Reg2 |=  BIT4;}while(0)
#define SET_3F()     do{LED_Reg2 |=  BIT5;}while(0)
#define SET_3G()     do{LED_Reg2 |=  BIT6;}while(0)

//SET4个"七段数码管"第四位
#define SET_4A()     do{LED_Reg3 |=  BIT0;}while(0)
#define SET_4B()     do{LED_Reg3 |=  BIT1;}while(0)
#define SET_4C()     do{LED_Reg3 |=  BIT2;}while(0)
#define SET_4D()     do{LED_Reg3 |=  BIT3;}while(0)
#define SET_4E()     do{LED_Reg3 |=  BIT4;}while(0)
#define SET_4F()     do{LED_Reg3 |=  BIT5;}while(0)
#define SET_4G()     do{LED_Reg3 |=  BIT6;}while(0)

//SET标志LED显示
#define SET_ALA()    do{LED_Reg4 |=  BIT0;}while(0)
#define SET_PM()     do{LED_Reg4 |=  BIT3;}while(0)
#define SET_DOT1()   do{LED_Reg4 |=  BIT5;}while(0)
#define SET_DOT2()   do{LED_Reg4 |=  BIT4;}while(0)

/*CLR具体功能定义*/
//CLR4个"七段数码管"最左第一位
#define CLR_1B()     do{LED_Reg4 |= CBIT1;}while(0)
#define CLR_1C()     do{LED_Reg4 |= CBIT2;}while(0)

//CLR4个"七段数码管"第二位
#define CLR_2A()     do{LED_Reg1 |= CBIT0;}while(0)
#define CLR_2B()     do{LED_Reg1 |= CBIT1;}while(0)
#define CLR_2C()     do{LED_Reg1 |= CBIT2;}while(0)
#define CLR_2D()     do{LED_Reg1 |= CBIT3;}while(0)
#define CLR_2E()     do{LED_Reg1 |= CBIT4;}while(0)
#define CLR_2F()     do{LED_Reg1 |= CBIT5;}while(0)
#define CLR_2G()     do{LED_Reg1 |= CBIT6;}while(0)

//CLR4个"七段数码管"第三位
#define CLR_3A()     do{LED_Reg2 |= CBIT0;}while(0)
#define CLR_3B()     do{LED_Reg2 |= CBIT1;}while(0)
#define CLR_3C()     do{LED_Reg2 |= CBIT2;}while(0)
#define CLR_3D()     do{LED_Reg2 |= CBIT3;}while(0)
#define CLR_3E()     do{LED_Reg2 |= CBIT4;}while(0)
#define CLR_3F()     do{LED_Reg2 |= CBIT5;}while(0)
#define CLR_3G()     do{LED_Reg2 |= CBIT6;}while(0)

//CLR4个"七段数码管"第二四位
#define CLR_4A()     do{LED_Reg3 |= CBIT0;}while(0)
#define CLR_4B()     do{LED_Reg3 |= CBIT1;}while(0)
#define CLR_4C()     do{LED_Reg3 |= CBIT2;}while(0)
#define CLR_4D()     do{LED_Reg3 |= CBIT3;}while(0)
#define CLR_4E()     do{LED_Reg3 |= CBIT4;}while(0)
#define CLR_4F()     do{LED_Reg3 |= CBIT5;}while(0)
#define CLR_4G()     do{LED_Reg3 |= CBIT6;}while(0)

//CLR标志LED显示
#define CLR_ALA()    do{LED_Reg4 |= CBIT0;}while(0)
#define CLR_PM()     do{LED_Reg4 |= CBIT3;}while(0)
#define CLR_DOT1()   do{LED_Reg4 |= CBIT5;}while(0)
#define CLR_DOT2()   do{LED_Reg4 |= CBIT4;}while(0)
					
/*************************************************************/
/*类型定义Byte Definition**************************************/
/*************************************************************/

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
void Display_LED(void);
void DigNum1(uint8_t dat);
void DigNum2(uint8_t dat);
void DigNum3(uint8_t dat);
void DigNum4(uint8_t dat);
void LEDDisplayDrive(void);

/*************************************************************/
/*函数定义Function Definition**********************************/
/*************************************************************/

/*************************************************************/
/*外部调用_宏定义Macro Definition******************************/
/*************************************************************/

/*************************************************************/
/*外部调用_类型定义Byte Definition*****************************/
/*************************************************************/

/*************************************************************/
/*外部调用_标志位定义Flags Definition***************************/
/*************************************************************/

/*************************************************************/
/*外部调用_变量定义Variable Definition*************************/
/*************************************************************/
extern uint8_t  idata mP2;
extern uint8_t  idata mP3;
extern uint8_t  idata cnt200us;
extern uint8_t  idata LED_Tmp1;
extern uint8_t  idata LED_Tmp2;
extern uint8_t  idata LED_Tmp3;
extern uint8_t  idata LED_Tmp4;
extern uint8_t  idata LED_Tmp5;
extern uint8_t  idata LED_Tmp6;

/*************************************************************/
/*外部调用_数组定义Array Definition****************************/
/*************************************************************/
extern uint8_t  led_Buffer[4];
extern uint8_t  led_Buffer_Copy[4];

/*************************************************************/
/*外部调用_函数声明Function Declaration************************/
/*************************************************************/

/*************************************************************/
/*外部调用_函数定义Function Definition*************************/
/*************************************************************/






#endif





