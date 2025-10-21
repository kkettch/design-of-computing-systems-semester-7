#ifndef COMMAND_PROCESS_H
#define COMMAND_PROCESS_H


#include "main.h"
#include "traffic_light_logic/traffic_light.h"
#include "drivers/uart_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

enum {
	CMD_UNKNOWN,
	CMD_STATUS,
	CMD_SET_MODE,
	CMD_SET_TIMEOUT,
	CMD_SET_INTERRUPTS
} command_type = CMD_UNKNOWN;

void process_command_line(const char *line);

#endif
