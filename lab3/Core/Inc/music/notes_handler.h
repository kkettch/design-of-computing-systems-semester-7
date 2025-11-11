#ifndef NOTES_DRIVER_H
#define NOTES_DRIVER_H

#include "main.h"
#include "tim.h"
#include "drivers/uart_driver.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#define F_TICK 1000000UL
#define F_TIM 120000000UL

#define OCT_COUNT 9
#define OCT_MIN_INDEX 0
#define OCT_MAX_INDEX (OCT_COUNT - 1)
#define OCT_HALF (0.5f)
#define OCT_TWO (2.0f)


typedef enum {
    CMD_UNKNOWN = 0,
    CMD_NOTE,
    CMD_DURATION,
    CMD_OCTAVE,
    CMD_PLAY_HARMONY
} note_command_t;

void uart_notes_handler(void);
void log_note_info(int index);
void change_octava(char ch);
void harmony_cancel(void);
void play_harmony_start();
void play_harmony_update();
void change_duration(char ch);
void tone_update(void);
void tone_stop(void);
void tone_start(uint32_t freq_hz);

#endif
