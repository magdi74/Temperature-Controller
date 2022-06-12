#include "LCD_config.h"
#include "ADC.h"

QueueHandle_t xUARTQueue;
QueueHandle_t xLCDQueue;
QueueHandle_t xBuzzerQueue;

// Define a new struct message
typedef struct Message{
	char Txt1[4]; 
	char Txt2[4];
} AMessage;

// Function to convert to string
void toString (int tim, char text []){
	int number=tim;
	int digits=0;
	while (number) {
        number /= 10;
        digits++;
    }
	for (int j =0; j<digits; j++){
		text[j]='0';
	}
	// put numbers in char array
  int i = digits;
	while (tim != 0){
		i--;		
		int rem=tim%10;
		text[i]=rem + '0';
		tim=(tim/10)|0;
	}
	text[digits]='\0';// add null terminator
}

// Function to print on the terminal through uart
void printchar(char x)
{
	while ((UART0_FR_R & (1<<5)) != 0);
	UART0_DR_R = x ;									// display given char 
}

// Function takes a string and print it on the terminal
void print(char* string)
{
	while (*string)
	{
		printchar(*(string++));					
	}
	
	printchar('\r');
	printchar('\n');
}


// Initialization of the GPIO ports and the uart
void GPIO_init(){

	SYSCTL_RCGCGPIO_R |= 0x00000019;	// Initialize PORTA,PORTE,PORTD
	delayUs(1);
  GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_CR_R = 0xFF;       	// Enable change to PORTD
  GPIO_PORTD_DIR_R = 0xFF;      	// Make led ports as output
  GPIO_PORTD_DEN_R = 0xFF;      	// digital enable to pins
		
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
  GPIO_PORTE_CR_R = 0x2F;       	// Enable change to PORTE
  GPIO_PORTE_DIR_R = 0x2F;      	// Make led ports as output
  GPIO_PORTE_DEN_R = 0x2F;      	// digital enable to pins	
	
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	GPIO_PORTA_AFSEL_R |= (1<<0 | 1<<1);              // choosing the alternative function for pins 0 & 1 (Tx & Rx)
	GPIO_PORTA_PCTL_R  |= (1<<(0*4) | 1<<(1*4));      // choosing the alternative function to the pin
	GPIO_PORTA_DEN_R   |= (1<<0 | 1<<1);   						// digital enable to pins	
	
	SYSCTL_RCGCUART_R |= 1<<0 ;				// uart initialization
	delayUs(1);
	UART0_CTL_R &=~ 1<<0;    		 			// turning off the uart control 
	UART0_IBRD_R=8;										// setting baud rate to 115200
	UART0_FBRD_R=44;
	UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART0_CC_R =0x05;
	UART0_CTL_R=0x0301;
		
	LCD_start();											// initialize LCD
}

// Main Controller 
void TASK1(void *pvParameters){

	unsigned char setpoint=30;
	volatile int Temperature;
	volatile int mV;
	const int on = 1;
	const int off = 0;
	AMessage msg;
	const int AlarmValue = 50; 
	adc_init() ;

 while(1){
	ADC0_PSSI_R |= (1UL<<3);
	while((ADC0_RIS_R & 8) == 0) {} 						// wait for conversion to be completed
	
	unsigned int val = ADC0_SSFIFO3_R; 					// reads the value
	ADC0_ISC_R = 8; 														// Read ADC

	mV = ((val * 3300)	/ 4096);								// in mV
	Temperature = ((mV - 200)/10) + 25;					// Temp in C
	xQueueReceive(xUARTQueue,	&setpoint, 0);		// Receive data
		
	if(Temperature < setpoint){									// If cold		
			GPIO_PORTE_DATA_R |= 0x02;							// Heater LED ON
	}else																				// If hot
	{			
		GPIO_PORTE_DATA_R &=~ 0x02;								// Heater Led OFF
	}
	toString(Temperature, msg.Txt1);						// Measured value
	toString(setpoint, msg.Txt2);								// setpoint value

	xQueueSendToBack(xLCDQueue, &msg,0);				// Send both temperatures via LCD Queue

	if(Temperature > AlarmValue){ 							// check the current Temperature value with Alarm value
		xQueueSend(xBuzzerQueue, &on, 0); 				// Buzzer ON
	}
	else{
	xQueueSend(xBuzzerQueue,&off,0);						// Buzzer off
	}
	vTaskDelay(500/portTICK_RATE_MS);						// delay for 500ms

		}
}

// UART controller
void TASK2(void* pvParameters){
	// Take User Input First Time
		int N;
		int Total;

		while (1)
		{
			N = 0;
			Total = 0;
			print("\n Enter Temperature Setpoint (Degrees): ");
			while(1){
				while ((UART0_FR_R & (1<<4) )!= 0);
				N = UART0_DR_R;
				printchar(N);						// print the number user clicked
				if (N == '\r')					// check if user hits enter
					break;
				N = N - '0';
				Total = 10 * Total + N;
		}
		print("\nTemperature setpoint changed");
		xQueueSendToBack(xUARTQueue,&Total, portMAX_DELAY);		// Send Total via UART Queue
		
	}
}

// LCD controller
void TASK3(void *pvParameters){
	AMessage msg;
	LCD_clear ();
	while(1){
		xQueueReceive(xLCDQueue,&msg,portMAX_DELAY);			// print on the lcd the measured and setpoint temperature
		LCD_line(1); 																			// intiates first line in LCD
		LCD_display("Measured: ");   											
		LCD_display(msg.Txt1);  													// display the value of measured Temperature
		LCD_line(2);																			// intiates second line in LCD
		LCD_display("Setpoint: ");												
		LCD_display(msg.Txt2); 														// display the value of setpoint Temperature
	}
}

// Buzzer controller
void TASK4(void *pvParameters){
int state=0;
    while(1){
			xQueueReceive(xBuzzerQueue,&state,portMAX_DELAY);
			if(state==1){
				GPIO_PORTE_DATA_R |= 0x05;			// Buzzer & Buzzer Led ON 
			}else{
				GPIO_PORTE_DATA_R &=~ 0x05;			// Buzzer & Buzzer Led OFF
			}
    }
}


int main(void)
{

	xUARTQueue = xQueueCreate(1,sizeof(int));						// create UART queue 
	xLCDQueue = xQueueCreate(1,sizeof(AMessage));				// create LCD queue
	xBuzzerQueue = xQueueCreate(1,sizeof(int));					// create Buzzer queue
	
  GPIO_init();
	xTaskCreate(TASK3,"LCD_Controller",50,NULL, 4,NULL);
	xTaskCreate(TASK1,"MAIN_Controller",50,NULL, 3,NULL);
	xTaskCreate(TASK4,"Buzzer_Controller",50,NULL, 2,NULL);
	xTaskCreate(TASK2,"UART_Controller",100,NULL, 1,NULL);
	

	vTaskStartScheduler();

}

/*
Buzzer Port E Pin 2
Buzzer Led Port E pin 0
Green LED Port E pin 1
LCD Port A,B 
*/
