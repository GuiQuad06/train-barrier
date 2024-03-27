/*
 * rcc_driver.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#include "rcc_driver.h"

#include "stm32f1xx.h"

// RCC
// APB1 bus
#define TIM2EN   (1U << 0)
#define USART2EN (1U << 17)
// APB2 bus
#define AFIOEN   (1U << 0)
#define GPIOAEN  (1U << 2)
#define TIM1EN   (1U << 11)

void rcc_init(void)
{
    // Enable TIM1 for RCC PoV (CLK)
    RCC->APB2ENR |= TIM1EN;
    // Enable TIM2 for RCC PoV (CLK)
    RCC->APB1ENR |= TIM2EN;
    // Configure USART Module
    // Enable clock access to USART 2
    RCC->APB1ENR |= USART2EN;
    // Configure USART GPIO pin
    // Enable Clock access to GPIOA + Alternate Functions
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= AFIOEN;
}
