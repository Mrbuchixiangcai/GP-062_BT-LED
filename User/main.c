//======================================================
// Main program routine
// - Device name  : MC96F6508A
// - Package type : 52LQFP
//======================================================
// For XDATA variable : V1.041.00 ~
#define		MAIN	1

// Generated    : Wed, Nov 28, 2018 (10:41:47)
#include	"MC96F6508A.h"
#include	"func_def.h"
#include    "app_main.h"

void main()
{
	cli();          	// disable INT. during peripheral setting
	port_init();    	// initialize ports
	clock_init();   	// initialize operation clock
	BUZ_init();     	// initialize Buzzer
	LCD_init();     	// initialize LCD
	Timer0_init();  	// initialize Timer0
	Timer3_init();  	// initialize Timer3
	UART_init();    	// initialize UART interface
	WT_init();      	// initialize Watch timer
	sei();          	// enable INT.
	
	// TODO: add your main code here
	app_main();
	while(1);
}

//======================================================
// interrupt routines
//======================================================

void INT_UART_Rx() interrupt 9
{
	// UART Rx interrupt
	// TODO: add your code here
	uint8_t temp_RxData;
	temp_RxData=UARTDR;
	UARTST &= (~0x20);  //置零中断标志，不然会一直进来
	uart1_RX_Timeout=10;
	if(Flag_UART_ReceiveBuffer_A_B)
	{
		if(uart1_RX_Pointer<UART1_LEN_BUFFER)
			uart1_ReceiveBuffer_A[uart1_RX_Pointer++]=temp_RxData;
		if(temp_RxData==0x55)
		{
			uart1_RX_Pointer=0;
			Flag_UART1_RX_Finish_A=1;
			Flag_UART_ReceiveBuffer_A_B=~Flag_UART_ReceiveBuffer_A_B;
		}
	}
	else
	{
		if(uart1_RX_Pointer<UART1_LEN_BUFFER)
			uart1_ReceiveBuffer_B[uart1_RX_Pointer++]=temp_RxData;
		if(temp_RxData==0x55)
		{
			uart1_RX_Pointer=0;
			Flag_UART1_RX_Finish_B=1;
			Flag_UART_ReceiveBuffer_A_B=~Flag_UART_ReceiveBuffer_A_B;
		}
	}
}

void INT_UART_Tx() interrupt 10
{
	// UART Tx interrupt
	// TODO: add your code here
	//read_UARTData=UARTST;
	UARTST &= ~0x80;
	if(uart1_EnableSend)
	{
		uart1_TX_Timeout=10;
		UARTDR=uart1_TransmitBuffer[uart1_TX_Pointer++];
		if(uart1_TransmitBuffer[(uart1_TX_Pointer-1)]==0x55)
			uart1_EnableSend=0;
	}
}

void INT_Timer0() interrupt 13
{
	// Timer0 interrupt
	// TODO: add your code here
	Sys_Tick();
}

void INT_Timer3() interrupt 16
{
	// Timer3 interrupt
	// TODO: add your code here
	if(++cnt200us>=4)
	{
		cnt200us=0;
		LEDDisplayDrive();
	}
}

void INT_WT() interrupt 20
{
	// Watch timer interrupt
	// TODO: add your code here
	gRTC_HalfSecond=(!gRTC_HalfSecond);
	if(gRTC_HalfSecond)
	{
		if(++gRTC_Sec>59)
		{
			gRTC_Sec=0;
			if(++gRTC_Minute>59)
			{
				gRTC_Minute=0;
				if(++gRTC_Hour>23)
				{
					gRTC_Hour=0;
					gRTC_Week<<=1;
					if(gRTC_Week==0x80)//如果移位之后等于0x80，说明是第8次移位，这时候就是
						gRTC_Week=0x01;//周一，所以赋值为0x01。
				}
			}
		}
//		if(++gRTC_Minute>59)//这里把分钟提到和秒一样的等级，是测试用，可以缩短测试时间时的时间
//		{
//			gRTC_Minute=0;
//			if(++gRTC_Hour>23)
//			{
//				gRTC_Hour=0;
//				gRTC_Week<<=1;
//				if(gRTC_Week==0x80)//如果移位之后等于0x80，说明是第8次移位，这时候就是
//					gRTC_Week=0x01;//周一，所以赋值为0x01。
//			}
//		}
	}
	Flag_Dot=~Flag_Dot;
	
}

//======================================================
// peripheral setting routines
//======================================================

unsigned char UART_read()
{
	unsigned char dat;
	
	while(!(UARTST & 0x20));	// wait
	dat = UARTDR;   	// read
	return	dat;
}

void BUZ_OnOff(unsigned char On)
{
	// Buzzer ON(On=1) / OFF(On=0)
	BUZCR = (On)? (BUZCR | 1) : (BUZCR & (~1));	// ON / OFF
}

void BUZ_init()
{
	// initialize Buzzer
	// Frequency (Hz) = 1000.000000
	BUZCR = 0x00;   	// clock source
	BUZDR = 0xF9;   	// count value
}

void LCD_init()
{
	// initialize LCD
	// Frame freq. = 16.000000Hz
	LCDCRH = 0x00;  	// LCD control High
	LCDCRL = 0x00;  	// LCD control Low
	LCDCCR = 0x00;  	// LCD Contrast control
}

void Timer0_init()
{
	// initialize Timer0
	// 8bit timer, period = 1.000000mS
	T0CR = 0x92;    	// timer setting
	T0DR = 0x7C;    	// period count
	IE2 |= 0x02;    	// Enable Timer0 interrupt
	T0CR |= 0x01;   	// clear counter
}

void Timer3_init()
{
	// initialize Timer3
	// 8bit timer, period = 0.050000mS
	T3DR = 0x18;    	// period count
	T3CR = 0x8A;    	// timer setting
	IE2 |= 0x10;    	// Enable Timer3 interrupt
	T3CR |= 0x01;   	// clear counter
}

void UART_init()
{
	// initialize UART interface
	// UART : 9615bps N 8 1
	UARTCR2 = 0x02; 	// activate UART
	UARTCR1 = 0x06; 	// bit count, parity
	UARTCR2 |= 0xEC;	// interrupt, speed
	UARTCR3 = 0x00; 	// stop bit
	UARTBD = 0x67;  	// baud rate
	IE1 |= 0x18;    	// enable UART interrupt
}

void UART_write(unsigned char dat)
{
	while(!(UARTST & 0x80));	// wait
	UARTDR = dat;   	// write
}

void WT_init()
{
	// initialize Watch timer
	WTCR = 0x88;    	// setting
	WTDR = 0x01;    	// set duty
	WTDR = 0x80;    	// clear WT
	IE3 |= 0x04;    	// Enable WT interrupt
}

void clock_init()
{
	// internal RC clock (16.000000MHz)
	//OSCCR = 0x29;   	// Set Int. OSC
	OSCCR = (IRCS_16MHZ | IRC_EN | (1 << SCLKE));
	SCCR  = 0x00;    	// Use Int. OSC
//	// external clock//外部时钟源配置
//	OSCCR = 0x0A;   	// Enable int. 1MHz and Ext. OSC
//	BITCR = 0x09;   	// Set waiting time : 16ms@1MHz
//	while((BITCR & 0x80) == 0);	// Ext. OSC stabilizing time
//	SCCR = 0x01;    	// Change to Ext. OSC
//	OSCCR |= 0x04;  	// Disable Int. OSC
}

void port_init()
{
	// initialize ports
	//   1 : P13      in  KEY3
	//  11 : SXIN     in  32.768
	//  12 : SXOUT    out 32.768
	//  14 : P20      in  AC_DET
	//  15 : P21      out COM1
	//  16 : P22      out COM2
	//  17 : P23      out COM3
	//  18 : P24      out COM4
	//  22 : P30      out SEG1
	//  23 : P31      out SEG2
	//  24 : P32      out SEG3
	//  25 : P33      out SEG4
	//  26 : P34      out SEG5
	//  27 : P35      out SEG6
	//  28 : P36      out SEG7
	//  44 : TXD      out TXD
	//  45 : RXD      in  RXD
	//  48 : BUZo     out BUZZ
	//  50 : P10      in  KEY0
	//  51 : P11      in  KEY1
	//  52 : P12      in  KEY2
	P0IO = 0xFF;    	// direction
	P0PU = 0x00;    	// pullup
	P0OD = 0x00;    	// open drain
	P0DB = 0x00;    	// bit7~6(debounce clock), bit1~0=P01~00 debounce
	P0   = 0x00;    	// port initial value

	P1IO = 0xF0;    	// direction
	P1PU = 0x0F;    	// pullup
	P1OD = 0x00;    	// open drain
	P1DB = 0x00;    	// P17~10 debounce
	P1   = 0x00;    	// port initial value

	P2IO = 0xFE;    	// direction
	P2PU = 0x00;    	// pullup
	P2OD = 0x00;    	// open drain
	P2   = 0x00;    	// port initial value

	P3IO = 0xFF;    	// direction
	P3PU = 0x00;    	// pullup
	P3OD = 0x00;    	// open drain
	P3   = 0x00;    	// port initial value

	P4IO = 0xFF;    	// direction
	P4PU = 0x00;    	// pullup
	P4OD = 0x00;    	// open drain
	P4   = 0x00;    	// port initial value

	P5IO = 0x7F;    	// direction
	P5PU = 0x00;    	// pullup
	P5OD = 0x00;    	// open drain
	P5   = 0x00;    	// port initial value

	P6IO = 0xF7;    	// direction
	P6PU = 0x00;    	// pullup
	P6OD = 0x00;    	// open drain
	P6   = 0x00;    	// port initial value

	// Set port functions
	P0FSR = 0x02;   	// P0 selection//开启BUZE
	P2FSR = 0x00;   	// P2 selection
	P3FSR = 0x00;   	// P3 selection
	P4FSR = 0x00;   	// P4 selection
	P5FSRH = 0x18;  	// P5 selection High
	P5FSRL = 0x00;  	// P5 selection Low
	P6FSR = 0x0C;   	// P6 selection
}

