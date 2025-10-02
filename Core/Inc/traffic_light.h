#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "main.h"
#include "led_driver.h"
#include "button_driver.h"
#include "my_hal.h"

#define TIME_2_SEC			2000
#define TIME_3_SEC			3000
#define TIME_5_SEC			5000

#define GREEN_TIME_MS		TIME_5_SEC
#define GREEN_BLINK_TIME_MS	TIME_3_SEC
#define YELLOW_TIME_MS		TIME_2_SEC
#define RED_TIME_FULL_MS	(GREEN_TIME_MS * 4)
#define RED_TIME_SHORT_MS	GREEN_TIME_MS

typedef enum {
    STATE_RED,
    STATE_GREEN,
    STATE_GREEN_BLINKING,
    STATE_YELLOW
} traffic_light_state_t;

typedef enum {
    COLOR_RED,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_NONE
} led_color_t;

extern led_t green_led, yellow_led, red_led;
extern button_t pedestrian_button;
extern traffic_light_state_t current_state;
extern uint32_t state_start_time;
extern uint8_t next_red_short;

void traffic_light_init(void);
void set_traffic_light(led_color_t color);
void traffic_light_handler(void);
void handle_button_press(void);

#endif
