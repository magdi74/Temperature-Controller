#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include "queue.h"
#include "task.h"
#include <semphr.h>
#include "main.h"

#define LCD_DATA_DATA GPIO_PORTB_DATA_R
#define LCD_CTRL_DATA GPIO_PORTA_DATA_R
#define RS (1U<<6) 
#define EN (1U<<7) 

void delayUs(int n);
void delayMs(int n);

void LCD_command(unsigned char command);
void LCD_start(void);
void LCD_data(unsigned char data);
void LCD_clear (void);
void LCD_line(uint8_t line);
void LCD_display(char* name);

