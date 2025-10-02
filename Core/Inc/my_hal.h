#ifndef MY_HAL_H
#define MY_HAL_H

#include "main.h"

#define USER_BUTTON_PORT GPIOC
#define USER_BUTTON_PIN GPIO_PIN_15
#define GREEN_LED_PIN GPIO_PIN_13
#define YELLOW_LED_PIN GPIO_PIN_14
#define RED_LED_PIN GPIO_PIN_15
#define LED_PORT GPIOD
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define DEBOUNCE_MS 15

typedef enum { GPIO_MODE_IN = 0, GPIO_MODE_OUT_PP = 1 } GpioSimpleMode;
typedef enum { GPIO_NOPULL_ = 0, GPIO_PULLUP_ = 1, GPIO_PULLDOWN_ = 2 } GpioPull;
typedef enum { GPIO_SPEED_LOW_ = 0, GPIO_SPEED_MEDIUM_ = 1, GPIO_SPEED_FAST_ = 2, GPIO_SPEED_HIGH_ = 3 } GpioSpeed;

void GPIO_EnableClock_(GPIO_TypeDef* port);
void GPIO_ConfigPin(GPIO_TypeDef* port, uint16_t pin, GpioSimpleMode mode, GpioPull pull, GpioSpeed speed);
void GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state);
void GPIO_WritePinsResetMask(GPIO_TypeDef* port, uint16_t reset_mask);
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin);
void My_GPIO_Init(void);

#endif
