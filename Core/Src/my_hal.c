#include "my_hal.h"

void GPIO_EnableClock_(GPIO_TypeDef* port) {
    if (port == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; }
    else if (port == GPIOB){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; }
    else if (port == GPIOC){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; }
    else if (port == GPIOD){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }
    else if (port == GPIOE){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; }
#ifdef RCC_AHB1ENR_GPIOFEN
    else if (port == GPIOF){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; }
#endif
#ifdef RCC_AHB1ENR_GPIOGEN
    else if (port == GPIOG){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; }
#endif
#ifdef RCC_AHB1ENR_GPIOHEN
    else if (port == GPIOH){ RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; }
#endif
}

static uint32_t GPIO_PinIndex_(uint16_t pin){
    for (uint32_t i = 0; i < 16; ++i) {
        if (pin & (1u << i)) return i;
    }
    return 0;
}


void GPIO_ConfigPin(GPIO_TypeDef* port, uint16_t pin, GpioSimpleMode mode,
                           GpioPull pull, GpioSpeed speed)
{
    GPIO_EnableClock_(port);
    uint32_t pos = GPIO_PinIndex_(pin);

    /* MODER */
    port->MODER &= ~(0x3u << (pos*2));
    if (mode == GPIO_MODE_OUT_PP) {
        port->MODER |=  (0x1u << (pos*2));     // General purpose output
    } // для входа оставляем 00

    /* OTYPER (0 = push-pull) */
    if (mode == GPIO_MODE_OUT_PP) {
        port->OTYPER &= ~(1u << pos);
    }

    /* OSPEEDR */
    port->OSPEEDR &= ~(0x3u << (pos*2));
    port->OSPEEDR |=  ((uint32_t)speed << (pos*2));

    /* PUPDR */
    port->PUPDR &= ~(0x3u << (pos*2));
    port->PUPDR |=  ((uint32_t)pull << (pos*2));
}

void GPIO_WritePin(GPIO_TypeDef* port, uint16_t pin, GPIO_PinState state)
{
    if (state == GPIO_PIN_SET) {
        port->BSRR = pin;           // set
    } else {
        port->BSRR = (uint32_t)pin << 16; // reset
    }
}

void GPIO_WritePinsResetMask(GPIO_TypeDef* port, uint16_t reset_mask)
{
    if (reset_mask) {
        port->BSRR = (uint32_t)reset_mask << 16;
    }
}

GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin)
{
    return (port->IDR & pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

/* Инициализация всего нужного под задачу */
void My_GPIO_Init(void)
{
    /* Кнопка PC15: вход, без подтяжек (или выставьте нужную) */
    GPIO_ConfigPin(USER_BUTTON_PORT, USER_BUTTON_PIN, GPIO_MODE_IN, GPIO_NOPULL_, GPIO_SPEED_LOW_);

    /* Светодиоды PD13/14/15: выходы push-pull, без подтяжек, низкая/средняя скорость */
    GPIO_ConfigPin(LED_PORT, GREEN_LED_PIN,  GPIO_MODE_OUT_PP, GPIO_NOPULL_, GPIO_SPEED_LOW_);
    GPIO_ConfigPin(LED_PORT, YELLOW_LED_PIN, GPIO_MODE_OUT_PP, GPIO_NOPULL_, GPIO_SPEED_LOW_);
    GPIO_ConfigPin(LED_PORT, RED_LED_PIN,    GPIO_MODE_OUT_PP, GPIO_NOPULL_, GPIO_SPEED_LOW_);

    /* Погасим все светодиоды */
    GPIO_WritePinsResetMask(LED_PORT, GREEN_LED_PIN | YELLOW_LED_PIN | RED_LED_PIN);
}
