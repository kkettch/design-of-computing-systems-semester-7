#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "main.h"

#define BUTTON_DEBOUNCE_MS   40u   // время подавления дребезга (можно менять)

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    GPIO_PinState last_stable_state;   // последнее подтверждённое состояние
    GPIO_PinState last_raw_state;      // последнее мгновенное состояние
    uint32_t last_change_time;         // время последнего изменения raw
    uint8_t pressed_event_flag;        // флаг: есть событие нажатия
} button_t;

void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin);
uint8_t button_is_pressed(button_t* btn);
GPIO_PinState button_get_state(button_t* btn);

#endif
