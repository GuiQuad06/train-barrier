/*
 * us_sensor.c
 *
 *  Created on: Mar 28, 2024
 *      Author: gucd
 */

#include "us_sensor.h"

#include "stm32f1xx.h"
#include "tim_driver.h"

#include <string.h>

status_us_t us_sensor_init(us_handler_t *sensor)
{
    sensor->dist        = 0;
    sensor->pulse_width = 0;
    sensor->start_trigg = 1u;

    // TIM2 (Pulse) init
    tim2_init();
    // TODO TIM4 init -> Master timeout every 2secs and trigg the slave TIM2 Pulse

    return US_OK;
}
