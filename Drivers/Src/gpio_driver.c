/*
 * gpio_driver.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */
#include "gpio_driver.h"

void gpio_init(void)
{
    // IO PA5 -> output dir / GPIO push pull
    GPIOA->CRL |= (1U << 20);
    GPIOA->CRL &= ~(1U << 21);
    GPIOA->CRL &= ~(1U << 22);
    GPIOA->CRL &= ~(1U << 23);

    GPIOA->BRR = (1U << 5);
}

void gpio_set(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    GPIOx->BSRR = (1U << pin);
}

void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    GPIOx->BRR = (1U << pin);
}