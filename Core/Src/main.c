/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Guillaume CUADRADO
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "gpio_driver.h"
#include "rcc_driver.h"
#include "servo.h"
#include "us_sensor.h"
#include "usart_driver.h"

#include <stdint.h>
#include <stdio.h>

#define THRESHOLD_DIST (5000u)
#define LED_PIN        (5u)

servo_handler_t barrier;
us_handler_t us_sensor;

static void init_drivers(void)
{
    rcc_init();

    gpio_init();

    // Com UART for CLI
    usart2_init();
}

int __io_putchar(int c)
{
    usart2_write(c);

    return c;
}

int main(void)
{
    init_drivers();

    (void) servo_init(&barrier);
    (void) us_sensor_init(&us_sensor);

    printf("Bienvenue dans l'application de la barriere de Train !!\n");

    (void) us_sensor_start();

    /* Loop forever */
    for (;;)
    {
        while (!us_sensor.is_ready)
        {
            // Wait for the pulse to be sent and echo to be received
        }

        us_sensor.is_ready = 0;

        if (us_sensor.dist < THRESHOLD_DIST)
        {
            servo_move("open");
            gpio_set(GPIOA, LED_PIN);
        }
        else
        {
            servo_move("close");
            gpio_reset(GPIOA, LED_PIN);
        }
    }
}
