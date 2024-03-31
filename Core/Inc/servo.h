/*
 * servo.h
 *
 *  Created on: Mar 28, 2024
 *      Author: gucd
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include <stdint.h>

typedef enum
{
    SERVO_OK,
    SERVO_ERR
} status_servo_t;

typedef struct
{
    uint16_t open_pulse;  // Pulse width in us
    uint16_t close_pulse; // Pulse width in us
} servo_handler_t;

extern servo_handler_t barrier;

status_servo_t servo_init(servo_handler_t *servo);
status_servo_t servo_move(const char *cmd);
status_servo_t servo_set_pulse(int pulse);

#endif /* INC_SERVO_H_ */
