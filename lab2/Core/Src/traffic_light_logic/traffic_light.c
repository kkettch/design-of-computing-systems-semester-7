#include "traffic_light_logic/traffic_light.h"

led_t green_led, yellow_led, red_led;
button_t pedestrian_button;
traffic_light_state_t current_state = STATE_RED;
uint32_t state_start_time = 0;
uint8_t next_red_short = 0;
traffic_mode_t current_mode = MODE_BUTTON_ENABLED; 	// for set mode 1 or set mode 2
uint32_t red_timeout_sec = RED_TIME_FULL_MS;


void traffic_light_init(void) {
    led_init(&green_led, LED_PORT, GREEN_LED);
    led_init(&yellow_led, LED_PORT, YELLOW_LED);
    led_init(&red_led, LED_PORT, RED_LED);
    button_init(&pedestrian_button, BUTTON_PORT, GPIO_PIN_15);

    current_state = STATE_RED;
    state_start_time = HAL_GetTick();
    set_traffic_light(COLOR_RED);
}

void set_traffic_light(led_color_t color) {
    led_off(&red_led);
    led_off(&yellow_led);
    led_off(&green_led);

    switch(color) {
        case COLOR_RED:
            led_on(&red_led);
            break;
        case COLOR_YELLOW:
            led_on(&yellow_led);
            break;
        case COLOR_GREEN:
            led_on(&green_led);
            break;
        case COLOR_NONE:
            break;
    }
}

void traffic_light_handler(void) {
	static uint32_t last_time = 0;
	uint32_t current_time = HAL_GetTick();
	uint32_t state_elapsed = current_time - state_start_time;
	uint32_t red_time = (current_mode == MODE_BUTTON_ENABLED) ? (next_red_short ? RED_TIME_SHORT_MS : red_timeout_sec) : red_timeout_sec;

	if (current_time - last_time < 10) return;
	last_time = current_time;

	switch (current_state) {

		case STATE_RED:
			set_traffic_light(COLOR_RED);

			if (state_elapsed >= red_time) {
				current_state = STATE_GREEN;
				state_start_time = current_time;
				next_red_short = 0;
			}
			break;

		case STATE_GREEN:
			set_traffic_light(COLOR_GREEN);

			if (state_elapsed >= GREEN_TIME_MS) {
				current_state = STATE_GREEN_BLINKING;
				state_start_time = current_time;
			}
			break;

		case STATE_GREEN_BLINKING:
			if (state_elapsed % 1000 < 500) {
				set_traffic_light(COLOR_NONE);
			} else {
				set_traffic_light(COLOR_GREEN);
			}

			if (state_elapsed >= GREEN_BLINK_TIME_MS) {
				current_state = STATE_YELLOW;
				state_start_time = current_time;
			}
			break;

		case STATE_YELLOW:
			set_traffic_light(COLOR_YELLOW);

			if (state_elapsed >= YELLOW_TIME_MS) {
				current_state = STATE_RED;
				state_start_time = current_time;
			}
			break;
	}
}


void handle_button_press(void) {

	if (current_mode == MODE_BUTTON_DISABLED) return;

    if (current_state == STATE_RED) {
        uint32_t red_elapsed = HAL_GetTick() - state_start_time;

        if (red_elapsed >= RED_TIME_SHORT_MS) {
            current_state = STATE_GREEN;
            state_start_time = HAL_GetTick();
            next_red_short = 0;
        } else {
            next_red_short = 1;
        }
    } else if (current_state == STATE_YELLOW || current_state == STATE_GREEN_BLINKING) {
        next_red_short = 1;
    }
}
