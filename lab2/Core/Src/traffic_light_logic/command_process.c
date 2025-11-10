#include "traffic_light_logic/command_process.h"

extern traffic_mode_t current_mode;
extern uint32_t red_timeout_sec;
extern traffic_light_state_t current_state;

void process_command_line(const char *line)
{
    char cmd[32];
    char arg1[32];
    char arg2[32];

    int count = sscanf(line, "%31s %31s %31s", cmd, arg1, arg2);

    if (count <= 0) {
    	uart_transmitln_string("Empty command");
        return;
    }

    command_type = CMD_UNKNOWN;

    if (strcmp(cmd, "?") == 0)
        command_type = CMD_STATUS;
    else if (strcmp(cmd, "set") == 0 && count >= 2)
    {
        if (strcmp(arg1, "mode") == 0) command_type = CMD_SET_MODE;
        else if (strcmp(arg1, "timeout") == 0) command_type = CMD_SET_TIMEOUT;
        else if (strcmp(arg1, "interrupts") == 0) command_type = CMD_SET_INTERRUPTS;
    }

    switch (command_type) {

    case CMD_STATUS:
    {
        char buffer[64];
        char *color = "";

        switch (current_state) {
			case STATE_RED:             color = "red"; break;
			case STATE_GREEN:           color = "green"; break;
			case STATE_GREEN_BLINKING:  color = "blinking green"; break;
			case STATE_YELLOW:          color = "yellow"; break;
			default:                 	color = "unknown"; break;
		}

        // Color
        uart_transmit_string("State: ");
        uart_transmitln_string((char *)color);

        // Mode
        uart_transmit_string("Mode: ");
        snprintf(buffer, sizeof(buffer), "%d", current_mode);
        uart_transmitln_string(buffer);

        // Timeout
        uart_transmit_string("Timeout (sec): ");
        snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)red_timeout_sec / 1000);
        uart_transmitln_string(buffer);

        // Interruption
        uart_transmit_string("Interruption: ");
        snprintf(buffer, sizeof(buffer), "%c", uart_get_interrupts() ? 'I' : 'P');
        uart_transmitln_string(buffer);

        break;
    }
    case CMD_SET_MODE:
    {
        if (count < 3) {
        	uart_transmitln_string("Usage: set mode 1|2");
            break;
        }
        int mode = atoi(arg2);
        if (mode == 1 || mode == 2) {
            current_mode = mode;
            uart_transmitln_string("OK");
        } else {
        	uart_transmitln_string("ERROR: Invalid mode. Usage: set mode 1|2");
        }
        break;
    }
    case CMD_SET_TIMEOUT: {
        if (count < 3) {
        	uart_transmitln_string("Usage: set timeout X");
            break;
        }
        int t = atoi(arg2) * 1000;
        if (t > 0 && t > RED_TIME_SHORT_MS) {
            red_timeout_sec = (uint32_t)t;
            uart_transmitln_string("OK");
        } else {
        	uart_transmitln_string("ERROR: Timeout must be more than RED_TIME_SHORT");
        }
        break;
    }
    case CMD_SET_INTERRUPTS: {
        if (count < 3) {
        	uart_transmitln_string("Usage: set interrupts on|off");
            break;
        }
        if (strcmp(arg2, "on") == 0) {
            uart_set_interrupts(1);
            uart_transmitln_string("Interrupts ON");
        } else if (strcmp(arg2, "off") == 0) {
            uart_set_interrupts(0);
            uart_transmitln_string("Interrupts OFF");
        } else {
        	uart_transmitln_string("ERROR: Invalid parameter (use on|off)");
        }
        break;
    }
    default:
    	uart_transmitln_string("Unknown command");
        break;
    }
}
