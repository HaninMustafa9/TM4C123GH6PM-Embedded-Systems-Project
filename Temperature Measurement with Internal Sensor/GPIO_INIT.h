
unsigned long GPIO_GetBase(char port);
void GPIO_Init(char port, unsigned char pin, unsigned char direction, unsigned char pull, unsigned char alternateFun, unsigned char analogSelect, unsigned char PCTL);
void GPIO_CLK_EN(char port);
void GPIO_Write(char port, unsigned char pin, unsigned char value);
void GPIO_ConfigInterrupt(char port, unsigned char pin, unsigned char edge);
void GPIO_EnableInterrupt(char port);
void GPIO_Handler(char port, char pin, long result);





