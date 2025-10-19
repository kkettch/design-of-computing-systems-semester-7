#include "button_driver.h"

void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin) {
    btn->port = port;
    btn->pin = pin;
    btn->last_raw_state = HAL_GPIO_ReadPin(port, pin);
    btn->last_stable_state = btn->last_raw_state;
    btn->last_change_time = HAL_GetTick();
    btn->pressed_event_flag = 0;
}


/**
 * Внутренняя обработка — неблокирующая.
 * Проверяет сигнал и подтверждает изменение, если оно стабильно BUTTON_DEBOUNCE_MS.
 */
static void button_process(button_t* btn) {
    GPIO_PinState raw = HAL_GPIO_ReadPin(btn->port, btn->pin);
    uint32_t now = HAL_GetTick();

    if (raw != btn->last_raw_state) {
        // обнаружено новое значение — фиксируем момент
        btn->last_raw_state = raw;
        btn->last_change_time = now;
    } else {
        // состояние не меняется, проверим стабилизацию
        if (raw != btn->last_stable_state) {
            if ((now - btn->last_change_time) >= BUTTON_DEBOUNCE_MS) {
                // изменение подтверждено
                if (btn->last_stable_state == GPIO_PIN_SET && raw == GPIO_PIN_RESET) {
                    // фронт "нажата" (активный низ)
                    btn->pressed_event_flag = 1;
                }
                btn->last_stable_state = raw;
            }
        }
    }
}

uint8_t button_is_pressed(button_t* btn) {
    button_process(btn);

    if (btn->pressed_event_flag) {
        btn->pressed_event_flag = 0;
        return 1;
    }
    return 0;
}

GPIO_PinState button_get_state(button_t* btn) {
    button_process(btn);
    return btn->last_stable_state;
}
