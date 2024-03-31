/*
 * stm32f1_it.c.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */
#include "main.h"
#include "tim_driver.h"
#include "us_sensor.h"

// Used for Register reading in ISR:
#include "stm32f1xx.h"
#define SR_RXNE  (1U << 5)
#define SR_CC1IF (1U << 1)
#define SR_CC2IF (1U << 2)
#define SR_UIF   (1U << 0)

#ifdef TEST
void USART2_IRQHandler(void)
{
    if (USART2->SR & SR_RXNE)
    {
        USART2_RX_callback();
    }
}
#endif

void TIM1_CC_IRQHandler(void)
{
    if (TIM1->SR & SR_CC2IF)
    {
        TIM1->SR &= ~(SR_CC2IF | SR_UIF);
        TIM1_callback();
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & SR_CC2IF)
    {
        TIM2->SR &= ~(SR_CC2IF | SR_UIF);
        TIM2_callback();
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & SR_CC1IF)
    {
        TIM3->SR &= ~(SR_CC1IF | SR_UIF);
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & SR_UIF)
    {
        TIM4->SR &= ~(SR_UIF);
        TIM4_callback();
    }
}
