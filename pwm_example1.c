#include <lpc17xx.h>

/* 밀리초 단위의 딜레이를 생성하는 함수 */
void delay_ms(unsigned int ms)
{
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<50000;j++);
} 


/* TCR 레지스터에서 카운터를 활성화하는 비트 정의 */
#define SBIT_CNTEN     0 

/* TCR 레지스터에서 PWM 모드를 활성화하는 비트 정의 */
#define SBIT_PWMEN     2

/* MCR 레지스터에서 타이머를 리셋하도록 설정하는 비트 정의 */
#define SBIT_PWMMR0R   1

/* PCR 레지스터에서 PWM 채널 출력을 활성화하는 비트 정의 */
#define SBIT_PWMENA3   11
#define SBIT_PWMENA4   12
#define SBIT_PWMENA5   13
#define SBIT_PWMENA6   14

/* PWM 핀의 PINSEL 설정 비트 위치 정의 */
#define PWM_3          4 // P2_2 (PINSEL4의 4-5 비트)
#define PWM_4          6 // P2_3 (PINSEL4의 6-7 비트)
#define PWM_5          8 // P2_4 (PINSEL4의 8-9 비트)
#define PWM_6          10 // P2_5 (PINSEL4의 10-11 비트)

int main(void)
{
    int dutyCycle;
    SystemInit(); // 시스템 초기화
    /* 핀(P2_2 - P2_5)을 PWM 모드로 설정 */
    LPC_PINCON->PINSEL4 = (1<<PWM_3) | (1<<PWM_4) | (1<<PWM_5) | (1<<PWM_6); 

    /* 카운터 및 PWM 모듈 활성화 */ 
    LPC_PWM1->TCR = (1<<SBIT_CNTEN) | (1<<SBIT_PWMEN);

    LPC_PWM1->PR  =  0x0;               /* 프리스케일러 없음 */
    LPC_PWM1->MCR = (1<<SBIT_PWMMR0R);  /* 타이머 리셋 */

    LPC_PWM1->MR0 = 100;                /* PWM 주기(Ton+Toff)를 100으로 설정 */
    LPC_PWM1->MR3 = 50;                 /* 모든 4개의 채널에 대해 50% 듀티 사이클 설정 */
    LPC_PWM1->MR4 = 50;
    LPC_PWM1->MR5 = 50;
    LPC_PWM1->MR6 = 50; 

    /* 핀(P2_2 - P2_5)에 대해 PWM 출력 핀 활성화 */
    LPC_PWM1->PCR = (1<<SBIT_PWMENA3) | (1<<SBIT_PWMENA4) | (1<<SBIT_PWMENA5) | (1<<SBIT_PWMENA6);

    while(1)
    {
        for(dutyCycle=0; dutyCycle<100; dutyCycle++)
        {
            LPC_PWM1->MR3 = dutyCycle;  /* 듀티 사이클을 0-100으로 증가 */
            LPC_PWM1->MR4 = dutyCycle;
            LPC_PWM1->MR5 = dutyCycle;
            LPC_PWM1->MR6 = dutyCycle;

            delay_ms(5);
        }

        for(dutyCycle=100; dutyCycle>0; dutyCycle--)
        {
            LPC_PWM1->MR3 = dutyCycle;  /* 듀티 사이클을 100-0으로 감소 */
            LPC_PWM1->MR4 = dutyCycle;
            LPC_PWM1->MR5 = dutyCycle;
            LPC_PWM1->MR6 = dutyCycle;
  
            delay_ms(5);
        }
    }
} 