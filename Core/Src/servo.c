/*
 * servo.c
 *
 *  Created on: Mar 28, 2024
 *      Author: gucd
 */
#include "servo.h"

#include "stm32f1xx.h"
#include "tim_driver.h"

#include <string.h>

#define PULSE_WIDTH_OPEN  5000u
#define PULSE_WIDTH_CLOSE 2000u

static void servo_open(void);
static void servo_close(void);

status_servo_t servo_init(servo_handler_t *servo)
{
    servo->open_pulse  = PULSE_WIDTH_OPEN;
    servo->close_pulse = PULSE_WIDTH_CLOSE;

    tim3_init(servo->close_pulse);

    tim3_start();

    return SERVO_OK;
}

status_servo_t servo_move(const char *cmd)
{
    !strcmp(cmd, "open") ? servo_open() : servo_close();

    return SERVO_OK;
}

static void servo_open(void)
{
    for (uint32_t i = barrier.close_pulse; i <= barrier.open_pulse; i++)
    {
        TIM3->CCR1 = i;
    }
}

static void servo_close(void)
{
    for (uint32_t i = barrier.open_pulse; i >= barrier.close_pulse; i--)
    {
        TIM3->CCR1 = i;
    }
}

status_servo_t servo_set_pulse(int pulse)
{
    TIM3->CCR1 = (uint32_t) pulse;

    return SERVO_OK;
}
