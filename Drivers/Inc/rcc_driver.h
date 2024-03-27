/*
 * rcc_driver.h
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#ifndef INC_RCC_DRIVER_H_
#define INC_RCC_DRIVER_H_

// CLK public constants
#define SYS_FREQ (8000000U)
#define APB1_CLK SYS_FREQ

void rcc_init(void);

#endif /* INC_RCC_DRIVER_H_ */
