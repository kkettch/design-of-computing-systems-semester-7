################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/button_driver.c \
../Core/Src/drivers/keyboard_driver.c \
../Core/Src/drivers/led_driver.c \
../Core/Src/drivers/pca9538_driver.c \
../Core/Src/drivers/uart_driver.c 

OBJS += \
./Core/Src/drivers/button_driver.o \
./Core/Src/drivers/keyboard_driver.o \
./Core/Src/drivers/led_driver.o \
./Core/Src/drivers/pca9538_driver.o \
./Core/Src/drivers/uart_driver.o 

C_DEPS += \
./Core/Src/drivers/button_driver.d \
./Core/Src/drivers/keyboard_driver.d \
./Core/Src/drivers/led_driver.d \
./Core/Src/drivers/pca9538_driver.d \
./Core/Src/drivers/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/%.o Core/Src/drivers/%.su Core/Src/drivers/%.cyclo: ../Core/Src/drivers/%.c Core/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers

clean-Core-2f-Src-2f-drivers:
	-$(RM) ./Core/Src/drivers/button_driver.cyclo ./Core/Src/drivers/button_driver.d ./Core/Src/drivers/button_driver.o ./Core/Src/drivers/button_driver.su ./Core/Src/drivers/keyboard_driver.cyclo ./Core/Src/drivers/keyboard_driver.d ./Core/Src/drivers/keyboard_driver.o ./Core/Src/drivers/keyboard_driver.su ./Core/Src/drivers/led_driver.cyclo ./Core/Src/drivers/led_driver.d ./Core/Src/drivers/led_driver.o ./Core/Src/drivers/led_driver.su ./Core/Src/drivers/pca9538_driver.cyclo ./Core/Src/drivers/pca9538_driver.d ./Core/Src/drivers/pca9538_driver.o ./Core/Src/drivers/pca9538_driver.su ./Core/Src/drivers/uart_driver.cyclo ./Core/Src/drivers/uart_driver.d ./Core/Src/drivers/uart_driver.o ./Core/Src/drivers/uart_driver.su

.PHONY: clean-Core-2f-Src-2f-drivers

