#include <lpc17xx.h>

#define PRESCALE (25000-1)                                  // 25000개의 PCLK 주기마다 TC가 1 증가

uint8_t g_ulFlags = 0;

void initTimer0();
void TIMER0_IRQHandler();

int main(void) 
{
	LPC_GPIO1->FIODIR |= (1<<28) | (1<<29) | (1UL<<31);     // P1.28, P1.29, P1.31을 출력으로 설정
	initTimer0();
	
	while(1)
	{
      if(g_ulFlags == 0)
      {
        LPC_GPIO1->FIOSET |= (1<<28) | (1<<29) | (1UL<<31); // LED 켜기
      }
      else if(g_ulFlags == 1)
      {
        LPC_GPIO1->FIOCLR |= (1<<28) | (1<<29) | (1UL<<31); // LED 끄기
      }
	}
	//return 0; //normally this won't execute
}

void TIMER0_IRQHandler(void)
{
    LPC_TIM0->IR |= (1<<0);             // MR0 인터럽트 플래그를 클리어
   
    g_ulFlags++;
    
    if(g_ulFlags == 2)
        g_ulFlags = 0;
}


void initTimer0(void) {
    LPC_SC->PCONP |= (1<<1);            // Timer0 전원 활성화
    
    LPC_TIM0->CTCR = 0x0;               // Timer 모드 설정
    LPC_TIM0->PR = PRESCALE;            // Prescaler 설정 (25000-1)
    // Prescaler로 25,000 PCLK 주기마다 TC 값이 1 증가

    LPC_TIM0->MR0 = 500;                // Match Register 설정 (500ms)
    LPC_TIM0->MCR |= (1<<0) | (1<<1);   // Match 시 인터럽트 발생 및 TC 리셋
    LPC_TIM0->TCR |= (1<<1);            // 타이머 리셋
    
    NVIC_EnableIRQ(TIMER0_IRQn);        // NVIC에서 Timer0 인터럽트 활성화
    LPC_TIM0->TCR = 0x01;               // 타이머 시작
}
