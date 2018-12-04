/*************************************************************/
/*头文件定义Header File Definition*****************************/
/*************************************************************/
#ifndef __LED_MODE_H__
#define __LED_MODE_H__

/*************************************************************/
/*头文件Header File*******************************************/
/*************************************************************/
#include "app_main.h"

/*************************************************************/
/*宏定义Macro Definition**************************************/
/*************************************************************/
#define S_NC  7
#define S_A   0		//  AAAAA
#define S_B   1		// F     B
#define S_C   2		// F     B
#define S_D   3		//  GGGGG
#define S_E   4		// E     C
#define S_F   5		// E     C
#define S_G   6		//  DDDDD
#define S__   6

#define DIG_0   (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)  //0是6个段，上面的是对应的端口
#define DIG_1            (1<<S_B)|(1<<S_C)
#define DIG_2   (1<<S_A)|(1<<S_B)         |(1<<S_D)|(1<<S_E)         |(1<<S_G)
#define DIG_3   (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)                  |(1<<S_G)
#define DIG_4            (1<<S_B)|(1<<S_C)                  |(1<<S_F)|(1<<S_G)
#define DIG_5   (1<<S_A)         |(1<<S_C)|(1<<S_D)         |(1<<S_F)|(1<<S_G)
#define DIG_6   (1<<S_A)         |(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)|(1<<S_G)
#define DIG_7   (1<<S_A)|(1<<S_B)|(1<<S_C)
#define DIG_8   (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)|(1<<S_G)
#define DIG_9   (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)         |(1<<S_F)|(1<<S_G)
#define DIG_NONE  0x00                    // SPACE
#define CH_a    (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)         |(1<<S_G)
#define CH_b                      (1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_c                               (1<<S_D)|(1<<S_E)         |(1<<S_G)
#define CH_d             (1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)         |(1<<S_G)
#define CH_h                      (1<<S_C)         |(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_l             (1<<S_B)|(1<<S_C)
#define CH_n                      (1<<S_C)         |(1<<S_E)         |(1<<S_G)
#define CH_o                      (1<<S_C)|(1<<S_D)|(1<<S_E)         |(1<<S_G)
#define CH_r                                        (1<<S_E)         |(1<<S_G)
#define CH_t                               (1<<S_D)|(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_u                      (1<<S_C)|(1<<S_D)|(1<<S_E)
#define CH_y             (1<<S_B)|(1<<S_C)|(1<<S_D)         |(1<<S_F)|(1<<S_G)
#define CH_A    (1<<S_A)|(1<<S_B)|(1<<S_C)         |(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_C    (1<<S_A)                  |(1<<S_D)|(1<<S_E)|(1<<S_F)
#define CH_E    (1<<S_A)                  |(1<<S_D)|(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_F    (1<<S_A)                           |(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_G    (1<<S_A)         |(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)
#define CH_H             (1<<S_B)|(1<<S_C)         |(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_I             (1<<S_B)|(1<<S_C)
#define CH_J             (1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)
#define CH_L                               (1<<S_D)|(1<<S_E)|(1<<S_F)
#define CH_N    (1<<S_A)|(1<<S_B)|(1<<S_C)         |(1<<S_E)|(1<<S_F)
#define CH_O    (1<<S_A)|(1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)
#define CH_P    (1<<S_A)|(1<<S_B)                  |(1<<S_E)|(1<<S_F)|(1<<S_G)
#define CH_S    (1<<S_A)         |(1<<S_C)|(1<<S_D)         |(1<<S_F)|(1<<S_G)
#define CH_U             (1<<S_B)|(1<<S_C)|(1<<S_D)|(1<<S_E)|(1<<S_F)
#define CH__    (1<<S_G)

#define cDISP_DELAY_0SEC    0
#define cDISP_DELAY_60ms    6     // 6    ×10ms = 60ms   
#define cDISP_DELAY_500ms   50    // 50   ×10ms = 500ms   = 0.5s
#define cDISP_DELAY_1SEC    100   // 100  ×10ms = 1000ms  = 1s
#define cDISP_DELAY_2SEC    200   // 200  ×10ms = 2000ms  = 2s
#define cDISP_DELAY_3SEC    300   // 300  ×10ms = 3000ms  = 3s
#define cDISP_DELAY_4SEC    400   // 400  ×10ms = 4000ms  = 4s
#define cDISP_DELAY_5SEC    500   // 500  ×10ms = 5000ms  = 5s
#define cDISP_DELAY_6SEC    600   // 600  ×10ms = 6000ms  = 6s
#define cDISP_DELAY_6_5SEC  650   // 650  ×10ms = 6500ms  = 6.5s
#define cDISP_DELAY_10SEC   1000  // 1000 ×10ms = 10000ms = 10s
#define cDISP_DELAY_14SEC   1400  // 1400 ×10ms = 15000ms = 14s
#define cDISP_DELAY_14_5SEC 1450  // 1450 ×10ms = 15000ms = 14.5s
#define cDISP_DELAY_15SEC   1500  // 1500 ×10ms = 15000ms = 15s

/*************************************************************/
/*类型定义Byte Definition*************************************/
/*************************************************************/
enum
{
	eD_0 = 0,  //D:display
	eD_1,
	eD_2,
	eD_3,
	eD_4,
	eD_5,
	eD_6,
	eD_7,
	eD_8,
	eD_9,
	eD_NONE,
	eD_a,
	eD_b,
	eD_c,
	eD_d,
	eD_e,
	eD_f,
	eD_g,
	eD_h,
	eD_i,
	eD_j,
	eD_k,
	eD_l,
	eD_m,
	eD_n,
	eD_o,
	eD_p,
	eD_q,
	eD_r,
	eD_s,
	eD_t,
	eD_u,
	eD_v,
	eD_w,
	eD_x,
	eD_y,
	eD_z,
	eD_A,
	eD_B,
	eD_C,
	eD_D,
	eD_E,
	eD_F,
	eD_G,
	eD_H,
	eD_I,
	eD_J,
	eD_K,
	eD_L,
	eD_M,
	eD_N,
	eD_O,
	eD_P,
	eD_Q,
	eD_R,
	eD_S,
	eD_T,
	eD_U,
	eD_V,
	eD_W,
	eD_X,
	eD_Y,
	eD_Z,
	eD__,
};

enum
{
	eDS_RTC = 0, //DS:display segment
	eDS_AL1,
};

enum
{
	DISP_CLK = 0,
	DISP_AL1,
	DISP_AL2,
	ADJ_CLK,
	ADJ_ZONE,
	ADJ_YEAR,
	ADJ_MONTH,
	ADJ_DAY,
	ADJ_HOUR,
	ADJ_MINUTE,
	ADJ_DAYLIGHT_SAVING_ZONES,//	夏令时区设置
	ADJ_SNOOZE_TIME,
	ADJ_ALARM1,
	ADJ_ALARM2,
	ALARM1_DISP,
	ALARM2_DISP,
	ADJ_ALARM1_MIN,
	ADJ_ALARM2_MIN,
	ADJ_ALARM1_HOUR,
	ADJ_ALARM2_HOUR,
	ADJ_ALARM1_MODE,
	ADJ_ALARM2_MODE,
	DISP_VOL,
	DISP_SLP,
	DISP_AUX,
	DISP_RADIO,
	DISP_GET_MEN,
	DISP_SAVE_MEN,
	POWER_OFF_DISP,
};
enum
{
	USA = 0,
	GEN,
	JAPAN,
	RUSSA,
};

typedef enum
{
	LED_TURN_OFF = 0,
	LED_LOW			,
	LED_MEDIUM   	,
	LED_HIGH	 	,
}LED_BRIGHTNESS_E_TypeDef;
/*************************************************************/
/*标志位定义Flags Definition***********************************/
/*************************************************************/

/*************************************************************/
/*变量定义Variable Definition**********************************/
/*************************************************************/
extern uint16_t cntDisplayStatus;
/*************************************************************/
/*数组定义Array Definition************************************/
/*************************************************************/

/*************************************************************/
/*函数声明Function Declaration*********************************/
/*************************************************************/
void SetDisplayState10s(uint8_t status);
void SetDisplayState2s(uint8_t status);
void Display(void);

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
extern uint8_t  Flag_DisplayStatus;
/*************************************************************/
/*外部调用_变量定义Variable Definition*************************/
/*************************************************************/
extern uint16_t cntDisplayStatus;
extern uint8_t  idata LED_Reg1;
extern uint8_t  idata LED_Reg2;
extern uint8_t  idata LED_Reg3;
extern uint8_t  idata LED_Reg4;
extern uint8_t  idata LED_Reg5;
extern uint8_t  idata LED_Reg6;

/*************************************************************/
/*外部调用_数组定义Array Definition****************************/
/*************************************************************/
extern code uint8_t DisplayCode[];

/*************************************************************/
/*外部调用_函数声明Function Declaration************************/
/*************************************************************/

/*************************************************************/
/*外部调用_函数定义Function Definition*************************/
/*************************************************************/






#endif





