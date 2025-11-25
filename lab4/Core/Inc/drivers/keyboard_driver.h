#ifndef KEYBOARD_DRIVER
#define KEYBOARD_DRIVER

#include "main.h"
#include "drivers/uart_driver.h"
#include "drivers/pca9538_driver.h"

#define DEBOUNCE_TIME_MS 10

#define KBRD_ADDR 0xE2

#define ROWS 4
#define COLUMNS 3

HAL_StatusTypeDef keyboard_init(void);
uint8_t keyboard_get_raw(void);
uint8_t keyboard_update(void);

#endif
