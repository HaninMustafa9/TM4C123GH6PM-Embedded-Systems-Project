#include "GPIO_REG.h"
#include "GPIO_INIT.h"
#include "LCD_INIT.h"


#define RISING_EDGE 1
#define FALLING_EDGE 2
#define BOTH_EDGES 3

unsigned long delay;
//int flag = 0;


unsigned long GPIO_GetBase(char port) {
    switch (port) {
        case 'A': return GPIO_PORTA_BASE;
        case 'B': return GPIO_PORTB_BASE;
        case 'C': return GPIO_PORTC_BASE;
        case 'D': return GPIO_PORTD_BASE;
        case 'E': return GPIO_PORTE_BASE;
        case 'F': return GPIO_PORTF_BASE;
        default: return 0; 
    }
}

void GPIO_CLK_EN(char port){
	 int bitNo;
    switch (port) {
        case 'A': bitNo = 0; break;
        case 'B': bitNo = 1; break;
        case 'C': bitNo = 2; break;
        case 'D': bitNo = 3; break;
        case 'E': bitNo = 4; break;
        case 'F': bitNo = 5; break;
        default: bitNo = 0; 
    }
		SYSCTL_RCGC2_R |= (1<<bitNo);
		delay = SYSCTL_RCGC2_R;

}
void GPIO_Init(char port, unsigned char pin, unsigned char direction, unsigned char pull, unsigned char alternateFun, unsigned char analogSelect, unsigned char PCTL) {
    unsigned long base = GPIO_GetBase(port);
    if (base == 0) return; 

    // Unlock and commit for special pins
    if (base == GPIO_PORTF_BASE && (pin == 0 || pin == 4)) {
        *((volatile unsigned long *)(base + GPIO_LOCK_OFFSET)) = GPIO_UNLOCK_KEY; // Unlock GPIOCR
        *((volatile unsigned long *)(base + GPIO_CR_OFFSET)) |= (1 << pin);      // Allow changes
    }

    // Set direction
    if (direction) {
        *((volatile unsigned long *)(base + GPIO_DIR_OFFSET)) |= (1 << pin);  // Output
    } else {
        *((volatile unsigned long *)(base + GPIO_DIR_OFFSET)) &= ~(1 << pin); // Input
    }

    // Enable Digital Function
    *((volatile unsigned long *)(base + GPIO_DEN_OFFSET)) |= (1 << pin);

    // Configure Pull-up resistor
    if (pull == 1) {
        *((volatile unsigned long *)(base + GPIO_PUR_OFFSET)) |= (1 << pin);  // Pull-up
    } else {
        *((volatile unsigned long *)(base + GPIO_PUR_OFFSET)) &= ~(1 << pin); // No Pull-up
    }

    // Configure Alternate Function
    if (alternateFun) {
        *((volatile unsigned long *)(base + GPIO_AFSEL_OFFSET)) |= (1 << pin); 
    } else {
        *((volatile unsigned long *)(base + GPIO_AFSEL_OFFSET)) &= ~(1 << pin); 
    }

    // Configure Analog Mode
    if (analogSelect) {
        *((volatile unsigned long *)(base + GPIO_AMSEL_OFFSET)) |= (1 << pin); 
    } else {
        *((volatile unsigned long *)(base + GPIO_AMSEL_OFFSET)) &= ~(1 << pin); 
    }		

    // Configure PCTL for alternate function (only if alternateFun is enabled)
    if (alternateFun) {
        volatile unsigned long *pctlReg = (volatile unsigned long *)(base + GPIO_PCTL_OFFSET);
        *pctlReg &= ~(0xF << (pin * 4)); // Clear the 4 bits for the pin
        *pctlReg |= (PCTL << (pin * 4)); // Set the required function
    }
}
void GPIO_Write(char port, unsigned char pin, unsigned char value)
{
    unsigned long base = GPIO_GetBase(port);
    if (base == 0) return;
    
    if (value) {
        *((volatile unsigned long *)(base + GPIO_DATA_OFFSET)) |= (1 << pin);
    } else {
        *((volatile unsigned long *)(base + GPIO_DATA_OFFSET)) &= ~(1 << pin);
    }
}


void GPIO_ConfigInterrupt(char port, unsigned char pin, unsigned char edge)
{
    unsigned long base = GPIO_GetBase(port);
    if (base == 0) return;

    // Set interrupt edge type (rising, falling, or both)
    if (edge == RISING_EDGE) {
        *((volatile unsigned long *)(base + GPIO_IEV_OFFSET)) |= (1 << pin); // Rising edge
        *((volatile unsigned long *)(base + GPIO_IS_OFFSET)) &= ~(1 << pin); // Edge-sensitive
        *((volatile unsigned long *)(base + GPIO_IBE_OFFSET)) &= ~(1 << pin); // Not both edges
    }
    else if (edge == FALLING_EDGE) {
        *((volatile unsigned long *)(base + GPIO_IEV_OFFSET)) &= ~(1 << pin); // Falling edge
        *((volatile unsigned long *)(base + GPIO_IS_OFFSET)) &= ~(1 << pin); // Edge-sensitive
        *((volatile unsigned long *)(base + GPIO_IBE_OFFSET)) &= ~(1 << pin); // Not both edges
    }
    else if (edge == BOTH_EDGES) {
        *((volatile unsigned long *)(base + GPIO_IEV_OFFSET)) |= (1 << pin); // Both edges
        *((volatile unsigned long *)(base + GPIO_IS_OFFSET)) &= ~(1 << pin); // Edge-sensitive
        *((volatile unsigned long *)(base + GPIO_IBE_OFFSET)) |= (1 << pin);  // Both edges
    }
		*((volatile unsigned long *)(base + GPIO_ICR_OFFSET)) |= (1<<pin);
    // Enable interrupt for this pin
    *((volatile unsigned long *)(base + GPIO_IM_OFFSET)) |= (1 << pin);
		
}


void GPIO_EnableInterrupt(char port)
{
    unsigned long nvicPort;
    switch (port) {
        case 'A': nvicPort = INT_GPIOA; break;
        case 'B': nvicPort = INT_GPIOB; break;
        case 'C': nvicPort = INT_GPIOC; break;
        case 'D': nvicPort = INT_GPIOD; break;
        case 'E': nvicPort = INT_GPIOE; break;
        case 'F': nvicPort = INT_GPIOF; break;
        default: return;
    }
    // Enable the interrupt in NVIC
    *((volatile unsigned long *)(NVIC_EN0_R)) |= (1 << nvicPort);
		NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF00) | 0x20; // Set priority to 1
}



