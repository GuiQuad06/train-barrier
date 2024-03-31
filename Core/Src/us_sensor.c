/*
 * us_sensor.c
 *
 *  Created on: Mar 28, 2024
 *      Author: gucd
 */

#include "us_sensor.h"

#include "gpio_driver.h"
#include "stm32f1xx.h"
#include "tim_driver.h"

#include <stdio.h>
#include <string.h>

#define FIVE_CM (5000u)

static uint16_t us_sensor_width_to_dist(uint16_t pulse_width);

void TIM1_callback(void)
{
    us_sensor.pulse_width = TIM1->CCR2 * 40u;

    us_sensor.dist = us_sensor_width_to_dist(us_sensor.pulse_width);

    us_sensor.is_ready = 1u;
}

void TIM4_callback(void)
{
    us_sensor.start_trigg = 1;

    // Send a 10u pulse every TIM4 timeout
    tim2_start();
}

status_us_t us_sensor_init(us_handler_t *sensor)
{
    sensor->dist        = 0;
    sensor->pulse_width = 0;
    sensor->start_trigg = 1u;
    sensor->is_ready    = 0;

    // TIM1 (Echo) init
    tim1_init();
    // TIM2 (Pulse) init
    tim2_init();
    // TIM4 init -> Master timeout every 2secs and trigg the slave TIM2 Pulse
    tim4_init(1);

    return US_OK;
}

status_us_t us_sensor_start(void)
{
    tim1_start();
    tim4_start();

    return US_OK;
}

status_us_t us_sensor_stop(void)
{
    tim1_stop();
    tim4_stop();

    return US_OK;
}

status_us_t us_sensor_read_dist(void)
{
    while (!us_sensor.is_ready)
    {
        // Wait for the pulse to be sent and echo to be received
    }

    us_sensor.is_ready = 0;

    printf("%ld\n", us_sensor.dist);

    if (us_sensor.dist < FIVE_CM)
    {
        // Set GPIO
        gpio_set(GPIOA, 5u);
    }
    else
    {
        // Reset GPIO
        gpio_reset(GPIOA, 5u);
    }
    return US_OK;
}

/*
 * @brief This function calculates the distance from the pulse width.
 * @param pulse_width: The echo pulse width in us.
 * @return The distance in mm.
 */
static uint16_t us_sensor_width_to_dist(uint16_t pulse_width)
{
    return pulse_width * 1000 / 58;
}
