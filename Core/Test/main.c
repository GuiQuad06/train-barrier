/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Guillaume CUADRADO
 * @brief          : Test program body
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

#include "cli.h"
#include "rcc_driver.h"
#include "servo.h"
#include "stm32f1xx.h"
#include "tim_driver.h"
#include "us_sensor.h"
#include "usart_driver.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

servo_handler_t barrier;
us_handler_t us_sensor;

volatile uint8_t message_received = 0;
volatile uint8_t cnt              = 0;

static uint8_t rx_buf;
static uint8_t byte_count = 0;
static char cli_cmd[INPUT_BUF_SIZE]; /** Input buffer for the command line interpreter. */

static void init_drivers(void)
{
    rcc_init();

    // Com UART for CLI
    usart2_init();
}

void USART2_RX_callback(void)
{
    rx_buf = USART2->DR;

    if (rx_buf != '\n')
    {
        cli_cmd[byte_count++] = rx_buf;
    }
    else
    {
        NVIC_DisableIRQ(USART2_IRQn);
        message_received = 1;
    }
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

    printf("Bienvenue dans l'application de test hardware de la barriere de Train !!\n");

    print_cli_menu();

    /* Loop forever */
    for (;;)
    {
        if (message_received)
        {
            message_received = 0;

            print_feedback(cli_input(cli_cmd));

            (void) memset(cli_cmd, 0, INPUT_BUF_SIZE);
            byte_count = 0;

            // Enable NVIC for the next RX message
            NVIC_EnableIRQ(USART2_IRQn);
        }
    }
}
