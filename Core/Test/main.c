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
#include "tim_driver.h"
#include "usart_driver.h"

// Used for Register reading in ISR:
#include "stm32f1xx.h"
#define SR_RXNE  (1U << 5)
#define SR_CC1IF (1U << 1)
#define SR_CC2IF (1U << 2)

#include <stdint.h>
#include <stdio.h>
#include <string.h>

volatile uint8_t message_received = 0;

static uint8_t rx_buf;
static uint8_t byte_count = 0;
static char cli_cmd[INPUT_BUF_SIZE]; /** Input buffer for the command line interpreter. */

static void USART2_RX_callback(void)
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

static void TIM2_callback(void)
{
    // When CNT > CCR the irq is triggered and the output is set to 0
    // So timer is stopped as we want only one pulse for the US sensor trigger
    tim2_stop();
}

void USART2_IRQHandler(void)
{
    if (USART2->SR & SR_RXNE)
    {
        USART2_RX_callback();
    }
}

void TIM1_CC_IRQHandler(void)
{
    if (TIM1->SR & SR_CC1IF)
    {
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & SR_CC2IF)
    {
        TIM2->SR &= ~(SR_CC2IF | SR_UIF);
        TIM2_callback();
    }
}

int __io_putchar(int c)
{
    usart2_write(c);

    return c;
}

int main(void)
{
    rcc_init();
    tim1_init();
    tim2_init();
    usart2_init();

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
