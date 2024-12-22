#include "TExaS.h"
#include "GPIO_INIT.h"
#include "GPIO_REG.h"
#include "LCD_INIT.h"
#include "Delay.h"
#include "ADC_REG.h"
#include "UART_INIT.h"

char ch;



HHHHint main(void){ 


  LCD_init();
  TExaS_Init();
	UART_Init();
	GPIO_CLK_EN('F');
	GPIO_CLK_EN('C');
	
  GPIO_Init('F',1,1,1,0,0,0);    
  GPIO_Init('F',2,1,1,0,0,0); 
	GPIO_Init('C',4,0,1,0,0,0);

	//ADC0_Init();
  
  while(1){
				  
		  ch = UART_InChar();		  
		  HandleKeyPress(ch);
  }
}
