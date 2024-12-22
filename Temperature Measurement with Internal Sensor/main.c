#include "TExaS.h"
#include "GPIO_INIT.h"
#include "GPIO_REG.h"
#include "LCD_INIT.h"
#include "Delay.h"
#include "ADC_REG.h"


int result, flag;
void PortC_Interrupt_Init(void);
void ADC0_Init(void);


int main(void){ 

	flag = 0;
  LCD_init();
	
	GPIO_CLK_EN('F');
	GPIO_CLK_EN('C');
	
  GPIO_Init('F',1,1,1,0,0,0);    
  GPIO_Init('F',2,1,1,0,0,0); 
	GPIO_Init('C',4,0,1,0,0,0);
  PortC_Interrupt_Init();	
	ADC0_Init();
	
  while(1){
			ADC0_PSSI_R |= 0x8;  //start a conversion ss3
			while((ADC0_RIS_R & 0x08)==0); //wait for conversion to complete
			result=ADC0_SSFIFO3_R & 0xfff;  //read conversion result
		  ADC0_ISC_R = 0x08;  // Clear flag*/
			
  }
}
void PortC_Interrupt_Init(void) {
    GPIO_PORTC_IS_R &= ~0x10;           // PC4 is edge-sensitive+
    GPIO_PORTC_IBE_R &= ~0x10;          // Not both edges
    GPIO_PORTC_IEV_R &= ~0x10;          // Falling edge trigger
    GPIO_PORTC_ICR_R = 0x10;            // Clear interrupt flag for PC4
    GPIO_PORTC_IM_R |= 0x10;            // Enable interrupt for PC4
    NVIC_EN0_R |= 0x04;                 // Enable interrupt for Port C (IRQ2)
    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF00) | 0x20; // Set priority to 1
}


void GPIOPortC_Handler(void) {
    if (GPIO_PORTC_RIS_R & 0x10) {      // Check if PC4 triggered the interrupt
        GPIO_PORTC_ICR_R = 0x10;        // Clear interrupt flag for PC4
				if(flag == 0){
						tempchar_celsius(result); //convert to temp and display
						flag = 1;
				}
				else{
					tempchar_fahrenheit(result);   // Display temperature in Fahrenheit
					flag = 0;
				}
    }
}


void ADC0_Init(void){
	SYSCTL_RCGC0_R |=0x00010000; //ADC0 clock
	while ((SYSCTL_RCGC0_R &0x00010000)== 0); //ADC0 clock to settle
	SYSCTL_RCGC0_R &=~0x00000300; 
	ADC0_ACTSS_R &= ~0x8; //disable ss3 during config.
	ADC0_EMUX_R &= ~0xF000; //continuous sampling
	ADC0_SSCTL3_R |= 0xe;   //one sample at atime, set flag, temp sensor
	ADC0_ACTSS_R |= 0x8;  //enable ss3 after config.
}
