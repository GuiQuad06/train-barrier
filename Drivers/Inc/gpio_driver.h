/*
 * gpio_driver.h
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_

#include "stm32f1xx.h"

void gpio_init(void);
void gpio_set(GPIO_TypeDef *GPIOx, uint8_t pin);
void gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif /* INC_GPIO_DRIVER_H_ */
