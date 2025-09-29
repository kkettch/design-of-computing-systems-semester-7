#include "led_driver.h"

void led_init(led_t* led, GPIO_TypeDef* port, uint16_t pin) {
    led->port = port;
    led->pin = pin;
}

void led_on(led_t* led) {
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void led_off(led_t* led) {
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

