#include "drivers/button_driver.h"

void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin)
{
    btn->port = port;
    btn->pin = pin;
    btn->last_raw_state = HAL_GPIO_ReadPin(port, pin);
    btn->last_stable_state = btn->last_raw_state;
    btn->last_change_time = HAL_GetTick();
    btn->pressed_event_flag = 0;
}


void button_process(button_t* btn)
{
	static uint32_t last_time = 0;
    GPIO_PinState raw = HAL_GPIO_ReadPin(btn->port, btn->pin);
    uint32_t now = HAL_GetTick();

    if (now - last_time < 10) return;
    last_time = now;

    if (raw != btn->last_raw_state) {
        btn->last_raw_state = raw;
        btn->last_change_time = now;
    }
    else
    {
        if (raw != btn->last_stable_state)
        {
            if ((now - btn->last_change_time) >= BUTTON_DEBOUNCE_MS)
            {
                if (btn->last_stable_state == GPIO_PIN_SET && raw == GPIO_PIN_RESET)
                    btn->pressed_event_flag = 1;
                btn->last_stable_state = raw;
            }
        }
    }
}

uint8_t button_is_pressed(button_t* btn)
{
    button_process(btn);

    if (btn->pressed_event_flag)
    {
        btn->pressed_event_flag = 0;
        return 1;
    }
    return 0;
}

