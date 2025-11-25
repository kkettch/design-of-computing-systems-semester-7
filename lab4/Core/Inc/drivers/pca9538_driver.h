#ifndef PCA9538_DRIVER
#define PCA9538_DRIVER

#include "main.h"
#include <stdio.h>
#include "drivers/uart_driver.h"
#include "i2c.h"

#define REGISTERS 4

typedef enum{
    INPUT_PORT = 0x00,
    OUTPUT_PORT = 0x01,
    POLARITY_INVERSION = 0x02,
    CONFIG = 0x03
} pca9538_register;

HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_register reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_register reg, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Config(uint16_t addr, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Polarity(uint16_t addr, uint8_t* buf);
HAL_StatusTypeDef PCA9538_Write_Output(uint16_t addr, uint8_t* buf);

#endif
