//======================================================
// Function and global variables definition
//======================================================

void port_init();             	// initialize ports
void clock_init();            	// initialize operation clock
void BUZ_init();              	// initialize Buzzer
void BUZ_OnOff(unsigned char On);// Buzzer ON(On=1) / OFF(On=0)
void LCD_init();              	// initialize LCD
void Timer0_init();           	// initialize Timer0
void Timer3_init();           	// initialize Timer3
void UART_init();             	// initialize UART interface
void UART_write(unsigned char dat);	// write UART
unsigned char UART_read();    	// read UART
void WT_init();               	// initialize Watch timer

