#include "lpc17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_clkpwr.h"

uint8_t g_ulFlags = 0;

void initTimer0();
void TIMER0_IRQHandler();

TIM_TIMERCFG_Type TIM_ConfigStruct;        // Timer 설정 구조체
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;  // Timer 일치 동작 구조체

int main(void)
{
    SystemInit();
    
    GPIO_SetDir(1, 0xB0000000, 1);

    initTimer0();

    while(1){
        if(g_ulFlags == 0)
        {
            GPIO_SetValue(1, 0xB0000000);
        }
        else if(g_ulFlags == 1)
        {
            GPIO_ClearValue(1, 0xB0000000);
        }
    }
}

void TIMER0_IRQHandler(void)
{
    TIM_ClearIntPending(LPC_TIM0,0);       // Timer0 인터럽트 클리어
    
    g_ulFlags++;
    
    if(g_ulFlags == 2)
        g_ulFlags = 0;
}

void initTimer0(void)
{
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_TICKVAL;         // Prescale로 설정한 값만큼 클럭이 카운트되어야 타이머에서 1클록을 카운트
    TIM_ConfigStruct.PrescaleValue = 25000;                         // 25000 클록 카운트되어야 타이머의 1클록이 카운트 

    TIM_MatchConfigStruct.MatchChannel = 0;                         // 타이머0을 사용하도록 설정
    TIM_MatchConfigStruct.IntOnMatch = ENABLE;                      // 타이머 일치 동작시 인터럽트가 발생
    TIM_MatchConfigStruct.ResetOnMatch = ENABLE;                    // 타이머 일치 동작시 타이머의 값을 리셋
    TIM_MatchConfigStruct.StopOnMatch = DISABLE;                    // 타이머 일치 동작을 주기적으로 발생
    TIM_MatchConfigStruct.ExtMatchOutputType=TIM_EXTMATCH_NOTHING;  // 타이머의 외부 출력핀은 사용하지 않음
    TIM_MatchConfigStruct.MatchValue = 500;                         // 500  * 1 / 160,000,000 = 약 0.8초 간격으로 타이머 일치 동작이 발생

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);           // 타이머/카운터 장치를 초기화 한다.
    TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);               // 타이머 일치 동작을 설정한다.

    NVIC_EnableIRQ(TIMER0_IRQn);                                    // TIMER0_IRQn 인터럽트 활성화
    TIM_Cmd(LPC_TIM0,ENABLE);                                        // TIM0 동작
}