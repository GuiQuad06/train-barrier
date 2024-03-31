/*
 * usart_driver.h
 *
 *  Created on: Jan 20, 2024
 *      Author: gucd
 */

#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#ifndef TEST
void usart2_init(void);
#else
void usart2_init_test(void);
#endif

void usart2_write(int ch);

#endif /* USART_DRIVER_H_ */
