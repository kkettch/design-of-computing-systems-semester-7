#include "drivers/pca9538_driver.h"


HAL_StatusTypeDef PCA9538_Read_Register(uint16_t addr, pca9538_register reg, uint8_t* buf)
{
	return HAL_I2C_Mem_Read(&hi2c1, addr, reg, 1, buf, 1, 100);
}

HAL_StatusTypeDef PCA9538_Write_Register(uint16_t addr, pca9538_register reg, uint8_t* buf)
{
	return HAL_I2C_Mem_Write(&hi2c1, addr, reg, 1, buf, 1, 100);
}

HAL_StatusTypeDef PCA9538_Read_Inputs(uint16_t addr, uint8_t* buf)
{
	return PCA9538_Read_Register(addr, INPUT_PORT, buf);
}

HAL_StatusTypeDef PCA9538_Write_Config(uint16_t addr, uint8_t* buf)
{
	return PCA9538_Write_Register(addr, CONFIG, buf);
}

HAL_StatusTypeDef PCA9538_Write_Polarity(uint16_t addr, uint8_t* buf)
{
	return PCA9538_Write_Register(addr, POLARITY_INVERSION, buf);
}

HAL_StatusTypeDef PCA9538_Write_Output(uint16_t addr, uint8_t* buf)
{
	return PCA9538_Write_Register(addr, OUTPUT_PORT, buf);
}



