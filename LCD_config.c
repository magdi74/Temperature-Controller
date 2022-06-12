#include "LCD_Config.h"

void delayMs(int n)
{  
	volatile int i,j;             //volatile is important for variables incremented in code
	for(i=0;i<n;i++)
		for(j=0;j<3180;j++)         //delay for 1 msec
		{}
}

void delayUs(int n)             
{
	volatile int i,j;						  // volatile is important for variables incremented in code
	for(i=0;i<n;i++)
		for(j=0;j<100;j++)          // delay for 100 micro second
		{}
}
// Exceute a given command on LCD
void LCD_command(unsigned char command)
{
    LCD_CTRL_DATA = 0;          // RS = 0, R/W = 0 
    LCD_DATA_DATA = command;
    LCD_CTRL_DATA = EN;         // pulse E 
    delayUs(1);									// Enable pulse Width */
    LCD_CTRL_DATA = 0;
    if (command < 4)
        delayMs(2);         		// command 1 and 2 needs up to 1.64ms 
    else
        delayUs(40);        		// all others 40 us 
}

void LCD_start(void){
   // GPIO initialization
    SYSCTL_RCGCGPIO_R |= 0x00000003;
    GPIO_PORTB_DIR_R = 0xFF;		// Make ports as output
    GPIO_PORTA_DIR_R = 0xFF;		// Make ports as output
    GPIO_PORTA_DEN_R = 0xFF;		// digital enable to pins
    GPIO_PORTB_DEN_R = 0xFF;		// digital enable to pins
   // LCD initialization sequence 
    delayMs(20);           			// Wait >15 ms after power is applied
    LCD_command(0x38);					// command 0x30 = Wake up
    delayMs(5);									// must wait 5ms, busy flag not available
    LCD_command(0x38);					// command 0x30 = Wake up #2
    delayUs(1);									// must wait 100us, busy flag not available
    LCD_command(0x38);					// command 0x30 = Wake up #3
    delayUs(1);									// must wait 100us, busy flag not available
  // Prepare LCD Operation and Function  
    LCD_command(0x38);     		 	// set 8-bit data, 2-line, 5x7 font 
    LCD_command(0x06);      		// move cursor right 
    LCD_command(0x01);      		// clear screen, move cursor to home 
    LCD_command(0x0F);      		// turn on display, cursor blinking 
}
// Display a given charcter on LCD 
void LCD_data(unsigned char data){       
    LCD_CTRL_DATA = RS;    			// RS = 1, R/W = 0 
    LCD_DATA_DATA = data;
    LCD_CTRL_DATA = EN | RS;   	// pulse E 
    delayUs(1);
    LCD_CTRL_DATA = 0;
    delayMs(40);
}
// Clear LCD
void LCD_clear (void){
    LCD_command(0x01);				
}
// Change the cursor position on LCD
void LCD_line(uint8_t line){
    if (line == 1)
        LCD_command(0x80);
    else if (line == 2)
        LCD_command(0xC0);
}

// Display a given string on LCD
void LCD_display(char* name){
    for(int i=0; i<strlen(name);i++){
        LCD_data(name[i]);
    }
}

