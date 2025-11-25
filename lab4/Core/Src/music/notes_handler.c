#include <music/notes_handler.h>
#include <stdint.h>

static int octave_index = 4;
static float octava = 1.0f;
static const char* OCTAVE_NAMES[OCT_COUNT] = {
    "Subcontra",
    "Contra",
    "Great",
    "Small",
    "First",
    "Second",
    "Third",
    "Fourth",
    "Fifth"
};

static const char* NOTE_NAMES[7] = { "do", "re", "mi", "fa", "sol", "la", "si" };
static const uint16_t NOTE_FREQS[7] = { 262, 294, 330, 349, 392, 440, 494 };

static volatile uint8_t tone_active = 0;
static volatile uint32_t tone_deadline_ms = 0;
static volatile uint32_t duration_ms = 1000;

static volatile uint8_t harmony_active = 0;
static int harmony_index = 0;
static uint32_t harmony_deadline_ms = 0;

void notes_handler(void)
{
	uint8_t key = keyboard_update();
	if (key != 0)
	{
		notes_handle_key(key);
	}

	tone_update();
	play_harmony_update();
}

void notes_handle_key(uint8_t key)
{
    harmony_cancel();

    if (key >= 1 && key <= 7)
    {
        int note_index = key - 1;
        tone_start((int)(octava * NOTE_FREQS[note_index]));
        log_note_info(note_index);
        tone_active = 1;
        tone_deadline_ms = HAL_GetTick() + duration_ms;
        return;
    }
    if (key == 8) {
        change_duration('A');
        return;
    }
    if (key == 9) {
        change_duration('a');
        return;
    }

    if (key == 10) {
        change_octava('+');
        return;
    }
    if (key == 11) {
        change_octava('-');
        return;
    }
    if (key == 12) {
        play_harmony_start();
    }
}

void log_note_info(int index)
{
    char msg[80];
    sprintf(msg, "Note: %s, Octave: %s, Duration (ms): %lu", NOTE_NAMES[index], OCTAVE_NAMES[octave_index], duration_ms);
    uart_transmitln_string("");
    uart_transmitln_string(msg);
}

void change_octava(char ch)
{
    if (ch == '+')
    {
        if (octave_index >= OCT_MAX_INDEX)
        {
            uart_transmitln_string("");
            uart_transmitln_string("LIMIT: highest octave reached");
        } else
        {
            octave_index++;
            octava *= OCT_TWO;

            char msg[64];
            sprintf(msg, "Octave: %s, Duration (ms): %lu", OCTAVE_NAMES[octave_index], duration_ms);
            uart_transmitln_string("");
            uart_transmitln_string(msg);
        }
    }
    else if (ch == '-')
    {
        if (octave_index <= OCT_MIN_INDEX) {
            uart_transmitln_string("");
            uart_transmitln_string("LIMIT: lowest octave reached");
        } else {
            octave_index--;
            octava *= OCT_HALF;

            char msg[64];
            sprintf(msg, "Octave: %s, Duration (ms): %lu", OCTAVE_NAMES[octave_index], duration_ms);
            uart_transmitln_string("");
            uart_transmitln_string(msg);
        }
    }
}

void harmony_cancel(void)
{
    if (harmony_active) {
        harmony_active = 0;
        tone_stop();
    }
}

void play_harmony_start()
{
	harmony_index = 0;
	harmony_active = 1;

	tone_start((int)(octava * NOTE_FREQS[harmony_index]));
	log_note_info(harmony_index);
	harmony_deadline_ms = HAL_GetTick() + duration_ms;
}

void play_harmony_update()
{
	if (!harmony_active) return;

	if ((int32_t)(HAL_GetTick() - harmony_deadline_ms) >= 0) {
		harmony_index += 1;
		if (harmony_index < (int)(sizeof(NOTE_FREQS) / sizeof(NOTE_FREQS[0])))
		{
			tone_start((int)(octava * NOTE_FREQS[harmony_index]));
			log_note_info(harmony_index);
			harmony_deadline_ms = HAL_GetTick() + duration_ms;
		}
		else
		{
			harmony_active = 0;
			tone_stop();
		}
	}
}

void change_duration(char ch)
{
	if (ch == 'A')
	{
		if (duration_ms < 5000)
		{
			duration_ms += 100;
			char msg[32];
			sprintf(msg, "New duration (ms): %lu", duration_ms);
			uart_transmitln_string("");
			uart_transmitln_string(msg);
		}
		else
		{
			uart_transmitln_string("");
			uart_transmitln_string("LIMIT: max duration reached");
		}
	}
	else if (ch == 'a')
	{
		if (duration_ms > 100)
		{
			duration_ms -= 100;
			char msg[32];
			sprintf(msg, "New duration (ms): %lu", duration_ms);
			uart_transmitln_string("");
			uart_transmitln_string(msg);
		}
		else
		{
			uart_transmitln_string("");
			uart_transmitln_string("LIMIT: min duration reached");
		}
	}

}

void tone_update(void)
{
    if (tone_active) {
        if ((int32_t)(HAL_GetTick() - tone_deadline_ms) >= 0) {
            tone_stop();
        }
    }
}

void tone_stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    tone_active = 0;
}

void tone_start(uint32_t freq_hz)
{
    const uint16_t PSC = (F_TIM / F_TICK) - 1;
    const uint16_t ARR = (F_TICK / freq_hz) - 1;
    const uint16_t CCR = (ARR + 1) / 2;

    __HAL_TIM_SET_PRESCALER(&htim1, PSC);
    __HAL_TIM_SET_AUTORELOAD(&htim1, ARR);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CCR);

    htim1.Instance->EGR = TIM_EGR_UG;
}
