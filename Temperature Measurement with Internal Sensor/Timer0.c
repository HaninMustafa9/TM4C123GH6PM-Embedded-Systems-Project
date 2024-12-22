#include "Timer0.h"
#include "Timer_REG.h"


int Tdelay,j;


void Timer0_Init(void) {
    SYSCTL_RCGCTIMER_R |= 0x01;          // Enable clock for Timer 0
	  Tdelay = SYSCTL_RCGCTIMER_R;
    TIMER0_CTL_R = 0x00;                 // Disable Timer 0 during configuration
    TIMER0_CFG_R = 0x00;                 // Configure for 32-bit mode
    TIMER0_TAMR_R = 0x02;                // Configure for periodic mode
    TIMER0_TAILR_R = 16000000 - 1;       // Load value for 1-second delay (16 MHz clock)
    TIMER0_ICR_R = 0x01;                 // Clear timeout flag
    TIMER0_CTL_R |= 0x01;                // Enable Timer 0
}


void Timer0_DelaySeconds(int seconds) {
    for (j = 0; j < seconds; j++) {
        while ((TIMER0_RIS_R & 0x01) == 0); // Wait for timeout flag
        TIMER0_ICR_R = 0x01;               // Clear timeout flag
    }
}


