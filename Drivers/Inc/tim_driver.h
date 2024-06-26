/*
 * tim_driver.h
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#ifndef INC_TIM_DRIVER_H_
#define INC_TIM_DRIVER_H_

#include <stdint.h>

void TIM2_callback(void);
void tim1_init(void);
void tim2_init(void);
void tim3_init(uint16_t nom_pulse);
void tim4_init(uint16_t period_in_sec);
void tim1_start(void);
void tim1_stop(void);
void tim2_start(void);
void tim2_stop(void);
void tim3_start(void);
void tim3_stop(void);
void tim4_start(void);
void tim4_stop(void);

#endif /* INC_TIM_DRIVER_H_ */
