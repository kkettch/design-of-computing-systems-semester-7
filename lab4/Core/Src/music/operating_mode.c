#include "music/operating_mode.h"

button_t switch_mode_button;
uint32_t last_button_update = 0;
button_work_mode_t current_button_mode = APPLICATION_MODE;

void project_init(void) {
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	if (keyboard_init() != HAL_OK)
		uart_transmitln_string("Keyboard init FAILED");

	button_init(&switch_mode_button, BUTTON_PORT, GPIO_PIN_15);
}

void handle_operating_mode(void)
{
	uint32_t current_time = HAL_GetTick();

	if (current_time - last_button_update >= 10)
	{
		last_button_update = current_time;
		if (button_is_pressed(&switch_mode_button)) {
			current_button_mode = (current_button_mode == TEST_MODE ? APPLICATION_MODE : TEST_MODE);

			char msg[64];
			sprintf(msg, "Button pressed. Current mode: %s", current_button_mode == TEST_MODE ? "TEST" : "APPLICATION");
			uart_transmitln_string(msg);
		}
	}

	if (current_button_mode == TEST_MODE)
		key_handler();
	if (current_button_mode == APPLICATION_MODE)
		notes_handler();
}

void key_handler(void)
{
	uint8_t key = keyboard_update();
	if (key != 0)
	{
		char msg[32];
		sprintf(msg, "Key=%u", key);
		uart_transmitln_string("");
		uart_transmitln_string(msg);
	}
}
