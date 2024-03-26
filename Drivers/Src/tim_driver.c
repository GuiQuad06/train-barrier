/*
 * tim_driver.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#include "tim_driver.h"

#include "stm32f1xx.h"

#include <stdint.h>

// RCC
#define TIM2EN (1U << 0) // APB1

// TIM registers
// CR1
#define CEN    (1U << 0)
// DIER
#define CC2IE  (1U << 2)
// CCMR1
#define OC2M_2 (1U << 14)
#define OC2M_1 (1U << 13)
// CCER
#define CC2P   (1U << 5)
#define CC2E   (1U << 4)

// CLK
#define TIM2_COUNT_FREQ (100000u) // 100kHz (10 us period)
#define MAX_COUNT       UINT16_MAX

static inline uint16_t tune_prescaler(uint32_t occ, uint32_t clk)
{
    // PSC = (PS_CK / FREQ_TIM -1) + 1
    return (clk / occ) - 1;
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
