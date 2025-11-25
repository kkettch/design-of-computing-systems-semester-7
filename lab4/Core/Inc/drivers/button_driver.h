#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "main.h"

#define BUTTON_DEBOUNCE_MS   40u

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    GPIO_PinState last_stable_state;
    GPIO_PinState last_raw_state;
    uint32_t last_change_time;
    uint8_t pressed_event_flag;
} button_t;

void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin);
void button_process(button_t* btn);
uint8_t button_is_pressed(button_t* btn);

#endif
