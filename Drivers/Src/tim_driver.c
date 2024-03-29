/*
 * tim_driver.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#include "tim_driver.h"

#include "rcc_driver.h"
#include "stm32f1xx.h"

// RCC
#define TIM2EN (1U << 0) // APB1

// TIM registers
// CR1
#define CEN    (1U << 0)
#define ARPE   (1U << 7)
// DIER
#define CC1IE  (1U << 1)
#define CC2IE  (1U << 2)
// CCMR1
#define CC1S_0 (1U << 0)
#define IC1F_1 (1U << 5)
#define IC1F_0 (1U << 4)
#define OC1M_2 (1U << 6)
#define OC1M_1 (1U << 5)
#define CC2S_1 (1U << 9)
#define OC2M_2 (1U << 14)
#define OC2M_1 (1U << 13)
// CCER
#define CC1P   (1U << 1)
#define CC1E   (1U << 0)
#define CC2P   (1U << 5)
#define CC2E   (1U << 4)
// SMCR
#define SMS_2  (1U << 2)
#define TS_2   (1U << 6)
#define TS_0   (1U << 4)

// CLK
#define TIM1_COUNT_FREQ  (25000u)   // 25kHz (40 us cycle)
#define TIM2_COUNT_FREQ  (100000u)  // 100kHz (10 us cycle)
#define TIM3_COUNT_FREQ  (4000000u) // 4 MHz (250 ns cycle)
#define TIM3_COUNT_F_MHZ (4u)
#define MAX_COUNT        UINT16_MAX

static inline uint16_t tune_prescaler(uint32_t occ, uint32_t clk)
{
    // PSC = (PS_CK / FREQ_TIM -1) + 1
    return (clk / occ) - 1;
}

void TIM2_callback(void)
{
    // When CNT > CCR the irq is triggered and the output is set to 0
    // So timer is stopped as we want only one pulse for the US sensor trigger
    tim2_stop();
}

void tim1_init(void)
{
    // IO PA8 -> TIM1_CH1 : intput dir / AFIO (already input mode by default) do nothing

    // Tune prescaler
    TIM1->PSC = tune_prescaler(TIM1_COUNT_FREQ, APB2_CLK);

    // Set period
    TIM1->ARR = MAX_COUNT - 1u;

    // Select active input => CC1S = 01 in CCMR1 register (select TI1 -> TIM1_CH1)
    TIM1->CCMR1 |= CC1S_0;

    // Select polarity for CC1P = 0 in CCER register (already reset, do nothing)

    // Select active input => CC2S = 10 in CCMR1 register (select TI1 -> TIM1_CH1)
    TIM1->CCMR1 |= CC2S_1;

    // Select polarity for CC2P = 1 in CCER register (falling edge)
    TIM1->CCER |= CC2P;

    // TS = 101 in SMCR register
    TIM1->SMCR |= (TS_2 | TS_0);

    // SMS = 100 in SMCR register
    TIM1->SMCR |= SMS_2;

    // CC1E = 1 & CC2E = 1 in CCER register
    TIM1->CCER |= (CC1E | CC2E);

    TIM1->DIER |= CC2IE;
}

void tim2_init(void)
{
    // IO PA1 -> TIM2_CH2 : output dir / AFIO push pull
    GPIOA->CRL |= (1U << 4);
    GPIOA->CRL &= ~(1U << 5);
    GPIOA->CRL &= ~(1U << 6);
    GPIOA->CRL |= (1U << 7);

    // Tune prescaler
    TIM2->PSC = tune_prescaler(TIM2_COUNT_FREQ, APB1_CLK);

    // Write the ARR & CCR
    TIM2->ARR  = MAX_COUNT - 1u; // 65535 x 10us = 655ms
    TIM2->CCR2 = 1u;             // 10us pulse width

    // Set CCxIE for interrupt
    TIM2->DIER |= CC2IE;

    // OCxM=110, OCxPE=0 (default), to OC2 PWM mode 1
    TIM2->CCMR1 |= (OC2M_2 | OC2M_1);
}

void tim3_init(uint16_t nom_pulse)
{
    // IO PA6 -> TIM3_CH1 : output dir / AFIO push pull
    GPIOA->CRL |= (1U << 24);
    GPIOA->CRL &= ~(1U << 25);
    GPIOA->CRL &= ~(1U << 26);
    GPIOA->CRL |= (1U << 27);

    // Tune prescaler
    TIM3->PSC = tune_prescaler(TIM3_COUNT_FREQ, APB1_CLK);

    // Write the ARR & CCR
    TIM3->ARR  = MAX_COUNT - 1u;               // 65535 x 250 ns = 20 ms
    TIM3->CCR1 = nom_pulse * TIM3_COUNT_F_MHZ; // 1500 us pulse width (nominal for servo)

    // Set CCxIE for interrupt
    TIM3->DIER |= CC1IE;

    // OCxM=110, OCxPE=0 (default), to OC2 PWM mode 1
    TIM3->CCMR1 |= (OC1M_2 | OC1M_1);
}

void tim1_start(void)
{
    // Enable Interrupts
    NVIC_EnableIRQ(TIM1_CC_IRQn);

    // Enable Timer with CEN
    TIM1->CR1 |= CEN;
}

void tim1_stop(void)
{
    // Disable Timer with CEN
    TIM1->CR1 &= ~(CEN);

    // Disable NVIC for the TIMER
    NVIC_DisableIRQ(TIM1_CC_IRQn);
}

void tim2_start(void)
{
    // CCxP=1 and CCxE=1
    TIM2->CCER |= (CC2P | CC2E);

    // Enable NVIC for the TIMER
    NVIC_EnableIRQ(TIM2_IRQn);

    // Enable Timer with CEN
    TIM2->CR1 |= CEN;
}

void tim2_stop(void)
{
    // CCxP=0 and CCxE=0
    TIM2->CCER &= ~(CC2P | CC2E);

    // Disable NVIC for the TIMER
    NVIC_DisableIRQ(TIM2_IRQn);

    // Disable Timer with CEN
    TIM2->CR1 &= ~(CEN);
}

void tim3_start(void)
{
    // CCxP=0 and CCxE=1
    TIM3->CCER |= CC1E;

    // Enable NVIC for the TIMER
    NVIC_EnableIRQ(TIM3_IRQn);

    // Enable Timer with CEN
    TIM3->CR1 |= CEN;
}

void tim3_stop(void)
{
    // CCxP=0 and CCxE=0
    TIM3->CCER &= ~(CC1E);

    // Disable NVIC for the TIMER
    NVIC_DisableIRQ(TIM3_IRQn);

    // Disable Timer with CEN
    TIM3->CR1 &= ~(CEN);
}
