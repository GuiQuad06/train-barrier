/*
 * usart_driver.c
 *
 *  Created on: Jan 20, 2024
 *      Author: gucd
 */
#include "usart_driver.h"

#include "stm32f1xx.h"

#include <stdint.h>

// RCC
#define GPIOAEN  (1U << 2)  // APB2
#define AFIOEN   (1U << 0)  // APB2
#define USART2EN (1U << 17) // APB1

// USART
#define CR1_TE (1U << 3)
#define CR1_UE (1U << 13)
#define SR_TXE (1U << 7)

// CLK
#define SYS_FREQ      (8000000U)
#define APB1_CLK      SYS_FREQ
#define UART_BAUDRATE (9600U)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baud_rate);
static uint16_t compute_uart_speed_div(uint32_t periph_clk, uint32_t baud_rate);

void usart2_write(int ch)
{
    // Make sure transmit data reg is empty
    while (!(USART2->SR & SR_TXE))
    {
    }
    // Write to Tx data reg
    USART2->DR = (ch & 0xFF);
}

void usart2_init(void)
{
    // Configure USART GPIO pin
    // Enable Clock access to GPIOA + Alternate Functions
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= AFIOEN;

    // Set PA2 to AF Out dir
    GPIOA->CRL |= (1U << 8);
    GPIOA->CRL &= ~(1U << 9);
    GPIOA->CRL &= ~(1U << 10);
    GPIOA->CRL |= (1U << 11);

    // Configure USART Module
    // Enable clock access to USART 2
    RCC->APB1ENR |= USART2EN;

    // Configure the USART
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
    // Configure xfer direction
    USART2->CR1 = CR1_TE;
    // Enable the USART module
    USART2->CR1 |= CR1_UE;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periph_clk, uint32_t baud_rate)
{
    USARTx->BRR = compute_uart_speed_div(periph_clk, baud_rate);
}

static uint16_t compute_uart_speed_div(uint32_t periph_clk, uint32_t baud_rate)
{
    return ((periph_clk + (baud_rate / 2U)) / baud_rate);
}
