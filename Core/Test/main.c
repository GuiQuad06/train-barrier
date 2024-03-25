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

#include "usart_driver.h"

#include <stdint.h>
#include <stdio.h>

int __io_putchar(int c)
{
    usart2_write(c);

    return c;
}

int main(void)
{
    usart2_init();

    printf("Bienvenue dans l'application de test hardware de la barriere de Train !!\n");

    /* Loop forever */
    for (;;)
    {
    }
}
