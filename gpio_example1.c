#include <lpc17xx.h>

void delay(void);

int main(void)
{
	LPC_GPIO1->FIODIR |= (1<<28) | (1<<29) | (1UL<<31); 	// P1.28, P1.29, P1.31 출력으로 설정
	while (1)
	{
		LPC_GPIO1->FIOSET |= (1<<28) | (1<<29) | (1UL<<31); // 출력 HIGH
		delay ();
		LPC_GPIO1->FIOCLR |= (1<<28) | (1<<29) | (1UL<<31); // 출력 LOW
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