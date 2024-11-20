#include "lpc17xx_gpio.h"

void delay(void);

int main(void)
{
	SystemInit();

	GPIO_SetDir(1, 0xB0000000, 1);		// P1.28, P1.29, P1.31 출력으로 설정

	while(1)
	{
		GPIO_SetValue(1, 0xB0000000);	// 출력 HIGH
		delay (); 
		GPIO_ClearValue(1, 0xB0000000); // 출력 HIGH
		delay ();
	}
	return 0;
}

void delay(void)
{
	int count, i=0;
	for (count=0; count < 6000000; count++)
	{
		i++;
	}
}