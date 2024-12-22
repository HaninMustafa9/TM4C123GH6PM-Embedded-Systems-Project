/* Generates 50Hz and variable duty cycle on PF2 pin of TM4C123 Tiva C Launchpad */
/* PWM1 module and PWM generator 3 of PWM1 module is used. Hence PWM channel */
#include "TM4C123GH6PM.h"
void SystemInit() {}
	int duty_cycle;
	void Delay_ms(int);
	unsigned int adc_value;
int main(void)
{
 // Timer2_Init(16000000);
     duty_cycle  = 4999;
adc_value = 0;    // variable that contains ADC value
    // Initialize PWM1
	    SYSCTL_RCGCGPIO_R |= (1<<4);   /* Enable Clock to GPIOE or PE3/AN0 */
    Delay_ms(10);                  /* 10 msec delay to enable the clock */
    SYSCTL_RCGCADC_R |= (1<<0);    /* ADC0 clock enable */
	
	    /* initialize PE3 for AN0 input */
    GPIO_PORTE_AFSEL_R |= (1<<3);       /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~(1<<3);        /* disable digital function */
    GPIO_PORTE_AMSEL_R |= (1<<3);       /* enable analog function */
	
	    ADC0_ACTSS_R &= ~(1<<3);       /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;        /* software trigger conversion */
    ADC0_SSMUX3_R = 0;             /* get input from channel 0 */
    ADC0_SSCTL3_R |= (1<<1) | (1<<2); /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= (1<<3);        /* enable ADC0 sequencer 3 */
	
    SYSCTL_RCGCPWM_R |= 0x02;      // Enable clock to PWM1 module
    SYSCTL_RCGCGPIO_R |= 0x20;     // Enable clock to PORTF
    SYSCTL_RCC_R |= (1 << 20);     // Enable System Clock Divisor
    SYSCTL_RCC_R |= 0x000E0000;    // Use pre-divider value of 64
 
    GPIO_PORTF_AFSEL_R |= (1 << 2);  // Set PF2 as alternate function
    GPIO_PORTF_PCTL_R &= ~0x00000F00;
    GPIO_PORTF_PCTL_R |= 0x00000500; // Configure PF2 as PWM
    GPIO_PORTF_DEN_R |= (1 << 2);    // Enable digital function on PF2
 
    PWM1_3_CTL_R &= ~(1 << 0);      // Disable PWM generator
    PWM1_3_CTL_R &= ~(1 << 1);      // Select down-count mode
    PWM1_3_GENA_R = 0x0000008C;     // Configure PWM action
    PWM1_3_LOAD_R = 5000;           // Set load value for 50Hz
    PWM1_3_CMPA_R = 4999;           // Initialize duty cycle
    PWM1_3_CTL_R |= 1;              // Enable PWM generator
    PWM1_ENABLE_R |= 0x40;          // Enable PWM output
    
    while(1)
    {
			        ADC0_PSSI_R |= (1<<3);     /* Enable SS3 conversion or start sampling data from AN0 */
        while((ADC0_RIS_R & 8) == 0);  /* Wait until sample conversion is completed */
        adc_value = ADC0_SSFIFO3_R;   /* read ADC conversion result from SS3 FIFO */
        ADC0_ISC_R = 8;              /* clear conversion clear flag bit */
        duty_cycle = (adc_value*4000)/4095 +20 ;
				if(duty_cycle > 5000) duty_cycle = 5000;
        PWM1_3_CMPA_R = 5000-duty_cycle;
        Delay_ms(100);
    }
}

/* This function generates delay in ms */
/* Calculations are based on 16MHz system clock frequency */

