#include "traffic_light_logic/command_process.h"

void process_command_line(const char *line)
{
    if (strcmp(line, "set interrupts on") == 0) {
        uart_set_interrupts(1);
        uart_println_string("Interrupts ON");
    } else if (strcmp(line, "set interrupts off") == 0) {
        uart_set_interrupts(0);
        uart_println_string("Interrupts OFF");
    } else if (strcmp(line, "?") == 0) {
        uart_print_string("Mode: ");
        uart_println_string(uart_get_interrupts() ? "Interrupt" : "Polling");
    } else {
        uart_println_string("Unknown command");
    }
}
