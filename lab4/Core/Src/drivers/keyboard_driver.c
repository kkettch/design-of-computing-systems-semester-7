#include "drivers/keyboard_driver.h"

static uint8_t stable_key = 0;
static uint8_t last_raw = 0;
static uint32_t change_time = 0;

static const uint8_t ROW_MASKS[4] = {0xFE, 0xFD, 0xFB, 0xF7};
static const uint8_t COLUMN_MASKS[3] = {0x10, 0x20, 0x40};

HAL_StatusTypeDef keyboard_init(void)
{
    uint8_t zero = 0;

    if (PCA9538_Write_Polarity(KBRD_ADDR, &zero) != HAL_OK)
        return HAL_ERROR;

    if (PCA9538_Write_Output(KBRD_ADDR, &zero) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}


uint8_t keyboard_get_raw(void)
{
    uint8_t input_port;
    uint8_t detected_row = 0xFF;
    uint8_t detected_column = 0xFF;
    uint8_t press_count = 0;

    for (uint8_t row = 0; row < ROWS; row++)
    {
        uint8_t config_row = ROW_MASKS[row];
        PCA9538_Write_Config(KBRD_ADDR, &config_row);

        uint32_t t0 = HAL_GetTick();
        while (HAL_GetTick() - t0 < 1) {}

        PCA9538_Read_Inputs(KBRD_ADDR, &input_port);
        uint8_t pushed_columns = input_port & 0x70;
        if (pushed_columns != 0x70)
        {
            for (uint8_t column = 0; column < COLUMNS; column++)
            {
                if (!(pushed_columns & COLUMN_MASKS[column]))
                {
                    press_count++;
                    if (press_count > 1)
                        return 0;
                    detected_row = row;
                    detected_column = column;
                }
            }
        }
    }
    if (press_count == 1)
        return detected_row * 3 + detected_column + 1;

    return 0;
}

uint8_t keyboard_update(void)
{
    uint8_t raw = keyboard_get_raw();

    if (raw != last_raw)
    {
        last_raw = raw;
        change_time = HAL_GetTick();
        return 0;
    }
    if (HAL_GetTick() - change_time < DEBOUNCE_TIME_MS)
        return 0;

    if (last_raw != stable_key)
    {
        stable_key = last_raw;
        return stable_key;
    }

    return 0;
}

