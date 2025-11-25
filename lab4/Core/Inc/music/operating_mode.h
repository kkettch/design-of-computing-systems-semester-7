#ifndef OPERATING_MODE_H
#define OPERATING_MODE_H

#include "main.h"
#include <stdio.h>
#include "drivers/uart_driver.h"
#include "drivers/button_driver.h"
#include "music/notes_handler.h"
#include "config.h"

typedef enum {
    TEST_MODE,
	APPLICATION_MODE
} button_work_mode_t;

void project_init(void);
void handle_operating_mode(void);
void key_handler(void);

#endif
