/*
 * us_sensor.h
 *
 *  Created on: Mar 28, 2024
 *      Author: gucd
 */

#ifndef INC_US_SENSOR_H_
#define INC_US_SENSOR_H_

#include <stdint.h>

typedef enum
{
    US_OK,
    US_ERR
} status_us_t;

typedef struct
{
    uint8_t start_trigg : 1;
    uint16_t pulse_width; // Pulse width in us
    uint16_t dist;        // Distance in cm
} us_handler_t;

extern us_handler_t us_sensor;

status_us_t us_sensor_init(us_handler_t *sensor);

#endif /* INC_US_SENSOR_H_ */
