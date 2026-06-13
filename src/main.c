#include "ch32fun.h"
#include <stdio.h>

/*
 * AFIO mapping of TIM1
 *
 Timer 1 pin mappings by AFIO->PCFR1
    00	AFIO_PCFR1_TIM1_REMAP_NOREMAP
        (ETR/PC5, BKIN/PC2)
        CH1/CH1N PD2/PD0
        CH2/CH2N PA1/PA2
        CH3/CH3N PC3/PD1
        CH4 PC4
    01	AFIO_PCFR1_TIM1_REMAP_PARTIALREMAP1
        (ETR/PA12, CH1/PA8, CH2/PA9, CH3/PA10, CH4/PA11, BKIN/PA6, CH1N/PA7, CH2N/PB0, CH3N/PB1)
        CH1/CH1N PC6/PC3
        CH2/CH2N PC7/PC4
        CH3/CH3N PC0/PD1
        CH4 PD3
    10	AFIO_PCFR1_TIM1_REMAP_PARTIALREMAP2
        (ETR/PD4, CH1/PD2, CH2/PA1, CH3/PC3, CH4/PC4, BKIN/PC2, CH1N/PD0, CN2N/PA2, CH3N/PD1)
        CH1/CH1N PD2/PD0
        CH2/CH2N PA1/PA2
        CH3/CH3N PC3/PD1
        CH4 PC4
    11	AFIO_PCFR1_TIM1_REMAP_FULLREMAP
        (ETR/PE7, CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8, CH2N/PE10, CH3N/PE12)
        CH1/CH1N PC4/PC3
        CH2/CH2N PC7/PD2
        CH3/CH3N PC5/PC6
        CH4 PD2
*/

#define TIM1_PERIOD     6800              // 6800/480kHz = 14ms
#define TIM1_DUTY_CYCLE (TIM1_PERIOD / 2) // 50% duty cycle

/*
 * initialize TIM1 for PWM
 */
static void init_t1(void)
{
    // Enable GPIOC, TIM1, and AFIO clocks
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO;

    AFIO->PCFR1 &= ~AFIO_PCFR1_PA12_REMAP;

    // PA1 is T1CH2, 10MHz Output alt func, open-drain
    funPinMode(PA1, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF);

    // PA2 is T1CH2_N, 10MHz Output alt func, open-drain
    funPinMode(PA2, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF);

    // Reset TIM1 to init all regs
    RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
    RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;

    // CTLR1: default is up, events generated, edge align
    TIM1->CTLR1 = 0;

    // CTLR2: set output idle states (MOE off) via OIS2 and OIS2N bits
    TIM1->CTLR2 = 0;

    // SMCFGR: default clk input is 48MHz CK_INT

    // Prescaler: divide by 100 => 480kHz
    TIM1->PSC = 0x0065;

    // Auto Reload - sets period = 6800/480kHz = 14ms
    TIM1->ATRLR = TIM1_PERIOD;

    // Reload immediately
    TIM1->SWEVGR |= TIM_UG;

    // Enable CH2 output, positive polarity
    TIM1->CCER |= TIM_CC2E | TIM_CC2P;

    // Enable CH2N output, positive polarity
    TIM1->CCER |= TIM_CC2NE | TIM_CC2NP;

    // CH2 Mode is output, PWM1 (CC1S = 00, OC1M = 110)
    TIM1->CHCTLR1 |= TIM_OC2M_2 | TIM_OC2M_1;

    // Set the Capture Compare Register value to 50% initially
    TIM1->CH2CVR = TIM1_DUTY_CYCLE;

    // Enable TIM1 outputs (also see OSSI and OSSR bits)
    TIM1->BDTR |= TIM_MOE;

    // Enable TIM1
    TIM1->CTLR1 |= TIM_CEN;
}

// low 5 bits are deadtime, upper 3 bits are deadtime prescaler and offset
void t1pwm_set_deadtime(uint16_t deadtime)
{
    TIM1->BDTR = (TIM1->BDTR & ~TIM_DTG) | (deadtime & TIM_DTG);
}

int main()
{
    uint32_t count = 0;

    SystemInit();
    Delay_Ms(100);

    init_t1();

    // Repeat changing deadtime every 20ms
    for(;;) {
        t1pwm_set_deadtime(count);
        count++;
        count &= 255;
        Delay_Ms(20);
    }
}
