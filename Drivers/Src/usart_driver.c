/*
 * usart_driver.c
 *
 *  Created on: Jan 20, 2024
 *      Author: gucd
 */
#include "usart_driver.h"

#include "rcc_driver.h"
#include "stm32f1xx.h"

#include <stdint.h>

// USART
#define CR1_RE     (1U << 2)
#define CR1_TE     (1U << 3)
#define CR1_RXNEIE (1U << 5)
#define CR1_UE     (1U << 13)

#define SR_TXE (1U << 7)

// CLK
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
    // Set PA2 to AF Out dir (PA3 set as input by default)
    GPIOA->CRL |= (1U << 8);
    GPIOA->CRL &= ~(1U << 9);
    GPIOA->CRL &= ~(1U << 10);
    GPIOA->CRL |= (1U << 11);

    // Configure the USART
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Configure xfer direction (TX channel and RX by interrupt)
    USART2->CR1 = (CR1_TE | CR1_RE | CR1_RXNEIE);

    // Enable NVIC for the RX
    NVIC_EnableIRQ(USART2_IRQn);

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
