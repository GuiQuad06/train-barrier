/*
 * cli.h
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#include <stdint.h>
#include <stdio.h>

#define INPUT_BUF_SIZE (32u)

typedef enum
{
    STATUS_OK,
    STATUS_KO,
    STATUS_TOO_MANY_ARGS,
    STATUS_PARAMETER_MISSING
} cli_status_t;

typedef struct
{
    cli_status_t sts;
    const char *str;
} str_status_t;

typedef cli_status_t (*cmd_callback_t)(int, char **);

void print_cli_menu(void);
cli_status_t cli_input(char *cli);
void print_feedback(cli_status_t sts);

cli_status_t cmd_servo(int argc, char **argv);
cli_status_t cmd_oled(int argc, char **argv);
cli_status_t cmd_us_sensor(int argc, char **argv);
cli_status_t cmd_send_pulse(int argc, char **argv);

#endif /* INC_CLI_H_ */
