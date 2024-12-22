#include "GPIO_INIT.h" // Include your GPIO driver header
#include "GPIO_REG.h"
#include "Delay.h"
#include "math.h"
#include "Timer0.h"


// Define ports and pins for LCD control
#define LCD_CTRL_PORT 'A' // Control port (RS, RW, E)
#define LCD_DATA_PORT 'B' // Data port (D0-D7)

// Control pins
#define RS_PIN 5
#define RW_PIN 6
#define E_PIN 7


//Variables
int i, A, digits[3];
double volt,Temp, TempF;
const unsigned char array [10] ={'0','1','2','3','4','5','6','7','8','9'};



// Function to send a command to the LCD
void LCD_command(unsigned char command) {
    GPIO_PORTA_DATA_R &= ~(1 << RS_PIN); // RS = 0 for command
    GPIO_PORTA_DATA_R &= ~(1 << RW_PIN); // RW = 0 for write
    GPIO_PORTB_DATA_R = command;         // Write command to data port
    GPIO_PORTA_DATA_R |= (1 << E_PIN);   // Pulse E
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~(1 << E_PIN);  // Clear E

    if (command < 4) {
        Delay_ms(2); // Commands 1 and 2 need up to 1.64ms
    } else {
        Delay_Us(40); // Other commands need 40us
    }
}

// Function to send data to the LCD
void LCD_data(unsigned char data) {
    GPIO_PORTA_DATA_R |= (1 << RS_PIN);  // RS = 1 for data
    GPIO_PORTA_DATA_R &= ~(1 << RW_PIN); // RW = 0 for write
    GPIO_PORTB_DATA_R = data;            // Write data to data port
    GPIO_PORTA_DATA_R |= (1 << E_PIN);   // Pulse E
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~(1 << E_PIN);  // Clear E
    Delay_Us(40);
}

// LCD initialization function
void LCD_init(void) {
    // Enable clocks for the control and data ports
    GPIO_CLK_EN(LCD_CTRL_PORT);
    GPIO_CLK_EN(LCD_DATA_PORT);

    // Initialize the control pins
    GPIO_Init(LCD_CTRL_PORT, RS_PIN, 1, 1, 0, 0, 0, 0); // RS as output
    GPIO_Init(LCD_CTRL_PORT, RW_PIN, 1, 1, 0, 0, 0, 0); // RW as output
    GPIO_Init(LCD_CTRL_PORT, E_PIN, 1, 1, 0, 0, 0, 0);  // E as output

    // Initialize the data pins (all 8 pins on the data port)
    for (i = 0; i < 8; i++) {
        GPIO_Init(LCD_DATA_PORT, i, 1, 1,0, 0, 0, 0); // Data pins as output
    }

    // Wake up LCD sequence
    Delay_ms(20);            // Wait >15 ms after power is applied
    LCD_command(0x30);       // Wake up
    Delay_ms(10);            
    LCD_command(0x30);       // Wake up #2
    Delay_ms(10);           
    LCD_command(0x30);       // Wake up #3
    Delay_ms(10);           

    // Prepare LCD operation and function
    LCD_command(0x38);       // Set 8-bit data, 2-line, 5x7 font
    LCD_command(0x06);       // Move cursor right
    LCD_command(0x01);       // Clear screen, move cursor to home
    LCD_command(0x0F);       // Turn on display, cursor blinking
}


void tempchar_fahrenheit(long value)
{
	 Timer0_Init();
	 GPIO_Init('A',2,1, 1,1,0,0,0);
	 GPIO_Init('A',3,1,1,1,0,0,0);
	 GPIO_Init('A',4,1,1,1,0,0,0);
	
    // Clear display
    LCD_command(1); // clear display 
    LCD_command(0x80); // lcd cursor location at left
    Delay_ms(500);

    // Display text
    LCD_data('T');
    LCD_data('e');
    LCD_data('m');
    LCD_data('p');
    LCD_data('e');
    LCD_data('r');
    LCD_data('a');
    LCD_data('t');
    LCD_data('u');
    LCD_data('r');
    LCD_data('e');
    LCD_data('=');

    LCD_command(0xc0); // lcd cursor location 
    
    // Convert ADC value to voltage and temperature
    volt = ((3.3 / 4096) * value);
    Temp = ((2.7 - volt) * 75) - 55; // Convert ADC value to temperature in Celsius
    TempF = (Temp * 1.8) + 32; // Convert Celsius to Fahrenheit

    A = ceil(TempF * 10); // Scale to one decimal place
    digits[0] = A / 100;
    digits[1] = (A - (digits[0] * 100)) / 10;
    digits[2] = (A - ((digits[0] * 100) + (digits[1] * 10)));

    // Converting reading to char for LCD monitoring
    for (i = 0; i <= 9; i++)
    {
        if (digits[0] == i)
        {
            LCD_data(array[i]);
        }
    }
    for (i = 0; i <= 9; i++)
    {
        if (digits[1] == i)
        {
            LCD_data(array[i]);
            LCD_data('.');
        }
    }
    for (i = 0; i <= 9; i++)
    {
        if (digits[2] == i)
        {
            LCD_data(array[i]);
            LCD_data(' ');
            LCD_data('`');
            LCD_data('F');
        }
    }

    // Logic for controlling GPIO pins based on temperature
    if (TempF > 80) // Fahrenheit equivalent of 20�C
    {
        // Use GPIO_Write to set pins
        GPIO_Write('A', 2, 1); // Set PA2 high
        GPIO_Write('A', 3, 1); // Set PA3 high
        
			  Timer0_DelaySeconds(3);
        
        // Turn off the pins
        GPIO_Write('A', 2, 0); // Set PA2 low
        GPIO_Write('A', 3, 0); // Set PA3 low
    }
    else
    {
        // Turn on PA4 for low temperature
        GPIO_Write('A', 4, 1); // Set PA4 high
    }

    // Delay before the next reading
    Delay_ms(10000000);
}

void tempchar_celsius(long value)
{
	 Timer0_Init();
	 GPIO_Init('A',2,1, 1,1,0,0,0);
	 GPIO_Init('A',3,1, 1,1,0,0,0);
	 GPIO_Init('A',4,1, 1,1,0,0,0);

    // Clear display
    LCD_command(1); // clear display 
    LCD_command(0x80); // lcd cursor location 
    Delay_ms(500);

    // Display text
    LCD_data('T');
    LCD_data('e');
    LCD_data('m');
    LCD_data('p');
    LCD_data('e');
    LCD_data('r');
    LCD_data('a');
    LCD_data('t');
    LCD_data('u');
    LCD_data('r');
    LCD_data('e');
    LCD_data('=');

    LCD_command(0xc0); // lcd cursor location 
    
    // Convert ADC value to voltage and temperature
    volt = ((3.3 / 4096) * value);
    Temp = ((2.7 - volt) * 75) - 55; // Convert ADC value to temperature in Celsius


    A = ceil(Temp * 10); // Scale to one decimal place
    digits[0] = A / 100;
    digits[1] = (A - (digits[0] * 100)) / 10;
    digits[2] = (A - ((digits[0] * 100) + (digits[1] * 10)));

    // Converting reading to char for LCD monitoring
    for (i = 0; i <= 9; i++)
    {
        if (digits[0] == i)
        {
            LCD_data(array[i]);
        }
    }
    for (i = 0; i <= 9; i++)
    {
        if (digits[1] == i)
        {
            LCD_data(array[i]);
            LCD_data('.');
        }
    }
    for (i = 0; i <= 9; i++)
    {
        if (digits[2] == i)
        {
            LCD_data(array[i]);
            LCD_data(' ');
            LCD_data('`');
            LCD_data('C');
        }
    }

    // Logic for controlling GPIO pins based on temperature
    if (Temp > 10) // Fahrenheit equivalent of 20�C
    {
        // Use GPIO_Write to set pins
        GPIO_Write('A', 2, 1); // Set PA2 high
        GPIO_Write('A', 3, 1); // Set PA3 high
			  
        Timer0_DelaySeconds(3);
        
        // Turn off the pins
        GPIO_Write('A', 2, 0); // Set PA2 low
        GPIO_Write('A', 3, 0); // Set PA3 low
    }
    else
    {
        // Turn on PA4 for low temperature
        GPIO_Write('A', 4, 1); // Set PA4 high
    }

    // Delay before the next reading
    Delay_ms(10000000);
}


