#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "main.h"
#include "my_hal.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} led_t;


void led_init(led_t* led, GPIO_TypeDef* port, uint16_t pin);
void led_on(led_t* led);
void led_off(led_t* led);

#endif
