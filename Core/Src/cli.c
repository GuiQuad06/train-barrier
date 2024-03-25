/*
 * cli.c
 *
 *  Created on: Mar 25, 2024
 *      Author: gucd
 */
#include "cli.h"

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
    cli_status_t status = STATUS_OK;

    if (argc < 1)
    {
        return STATUS_PARAMETER_MISSING;
    }
    else
    {
        if (strcmp(argv[1], "open") && strcmp(argv[1], "close"))
        {
            status = STATUS_KO;
        }
    }

    return status;
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
