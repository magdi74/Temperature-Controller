#include "LCD_config.h"
#include "ADC.h"


// Using ANO on PE3	
void adc_init (){
	
	// GIVING CLCOK
	SYSCTL_RCGCGPIO_R|=(1<<4);  			// give clock to port E 
	SYSCTL_RCGCADC_R |=  1; 					// AD0 clock enable
	delayUs(1);

	GPIO_PORTE_AFSEL_R |= (1UL<<3);   // enable alternate function 
  GPIO_PORTE_DEN_R &= ~(1UL<<3);    // disable digital function 
  GPIO_PORTE_AMSEL_R |= (1UL<<3);   // enable analog function 
	
	ADC0_ACTSS_R &= ~(1UL<<3);				// disable SS3 during configuration 
  ADC0_EMUX_R &= ~0xF000;    				// software trigger conversion */
	ADC0_SSMUX3_R = 0;         				// get input from channel 0 */
	ADC0_SSCTL3_R |= (1<<1)|(1<<2);   // take one sample at a time, set flag at 1st sample
	ADC0_ACTSS_R |= (1<<3);         	// enable ADC0 sequencer 3 */
		
}
