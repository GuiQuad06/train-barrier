/*
 * cli.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */
#include "cli.h"

#include "gpio_driver.h"
#include "servo.h"
#include "tim_driver.h"
#include "us_sensor.h"

#include <string.h>

#define MAX_ARGC (2u)

typedef struct
{
    const char *title;
    cmd_callback_t callback;
    const char *desc;
} cli_menu_t;

cli_menu_t cli_menu[] = {{"barrier", cmd_servo, "Control the servomotor : (open/close)"},
                         {"display", cmd_oled, "Display a msg on the OLED screen : (<string>)"},
                         {"distance", cmd_us_sensor, "Read the obstacle distance from the US sensor"},
                         {"pulse", cmd_send_pulse, "Send a 10us pulse"},
                         {"pwm", cmd_pwm_servo, "Control PWM : (start/stop/<integer>)"},
                         {"read_dist", cmd_read_dist, "Read the distance from the US sensor"},
                         {"gpio", cmd_write_gpio, "Write to GPIO : (set/reset)"},
                         {0, 0, 0}};

str_status_t str_status[] = {{STATUS_OK, "Tout va bien"},
                             {STATUS_KO, "Oups, erreur..."},
                             {STATUS_TOO_MANY_ARGS, "Trop de parametres..."},
                             {STATUS_PARAMETER_MISSING, "Parametre manquant!"}};

void print_cli_menu(void)
{
    cli_menu_t *ptr;
    ptr       = cli_menu;
    uint8_t n = 1;

    while (ptr->title != 0)
    {
        printf("%d) %s : %s\n", n, ptr->title, ptr->desc);

        ptr++;
        n++;
    }
    printf("\n");
}

cli_status_t cli_input(char *cli)
{
    uint8_t argc = 0;
    char *ptr, *end, *argv[MAX_ARGC + 1];
    cli_menu_t *ptr_cli_menu;

    ptr = cli;

    while ((end = strchr(ptr, ' ')))
    {
        // When found, replace it by \0
        *end = 0;

        argv[argc] = ptr;

        argc++;

        // Start after the space for the next search
        ptr = end + 1;

        if (MAX_ARGC < argc)
        {
            return STATUS_TOO_MANY_ARGS;
        }
    }

    // Command without parameters case
    if (!argc)
    {
        argv[0] = ptr;
        argc    = 1;
    }
    // Command with parameters case
    else
    {
        argv[argc] = ptr;
    }

    ptr_cli_menu = &cli_menu[0];

    // Lookup for the Command callback to call :
    while (ptr_cli_menu->callback)
    {
        if (!strncmp(ptr_cli_menu->title, argv[0], strlen(ptr_cli_menu->title)))
        {
            return (ptr_cli_menu->callback(argc, argv));
        }
        ptr_cli_menu++;
    }

    return STATUS_KO;
}

void print_feedback(cli_status_t sts)
{
    printf("%s\n", str_status[sts].str);
}

cli_status_t cmd_servo(int argc, char **argv)
{
    if (argc < 1)
    {
        return STATUS_PARAMETER_MISSING;
    }
    else
    {
        if (strcmp(argv[1], "open") && strcmp(argv[1], "close"))
        {
            return STATUS_PARAMETER_UNKNOWN;
        }

        if (servo_move(argv[1]) != SERVO_OK)
        {
            return STATUS_SERVO_NOK;
        }
    }

    return STATUS_OK;
}

cli_status_t cmd_oled(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    if (argc < 1)
    {
        return STATUS_PARAMETER_MISSING;
    }
    else
    {
        printf("Message is :%s\n", argv[1]);
    }
    return status;
}

cli_status_t cmd_us_sensor(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    return status;
}

cli_status_t cmd_send_pulse(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    tim2_start();

    return status;
}

cli_status_t cmd_pwm_servo(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    if (argc < 1)
    {
        return STATUS_PARAMETER_MISSING;
    }
    else
    {
        if (!strcmp(argv[1], "start"))
        {
            tim3_start();
        }
        else if (!strcmp(argv[1], "stop"))
        {
            tim3_stop();
        }
        else
        {
            (void) servo_set_pulse(atoi(argv[1]));
        }
    }
    return status;
}

cli_status_t cmd_read_dist(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    tim2_start();

    us_sensor_read_dist();

    return status;
}

cli_status_t cmd_write_gpio(int argc, char **argv)
{
    cli_status_t status = STATUS_OK;

    if (argc < 1)
    {
        return STATUS_PARAMETER_MISSING;
    }
    else
    {
        if (!strcmp(argv[1], "set"))
        {
            gpio_set(GPIOA, 5u);
        }
        else if (!strcmp(argv[1], "reset"))
        {
            gpio_reset(GPIOA, 5u);
        }
    }
    return status;
}

/**
 * @brief Function to convert string to integer
 *
 * @return int
 */
int atoi(char *str)
{
    // Initialize result
    int res = 0;

    // Iterate through all characters
    // of input string and update result
    // take ASCII character of corresponding digit and
    // subtract the code from '0' to get numerical
    // value and multiply res by 10 to shuffle
    // digits left to update running total
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    // return result.
    return res;
}
