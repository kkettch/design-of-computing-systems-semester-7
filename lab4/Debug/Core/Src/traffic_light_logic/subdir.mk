################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/traffic_light_logic/command_process.c \
../Core/Src/traffic_light_logic/traffic_light.c 

OBJS += \
./Core/Src/traffic_light_logic/command_process.o \
./Core/Src/traffic_light_logic/traffic_light.o 

C_DEPS += \
./Core/Src/traffic_light_logic/command_process.d \
./Core/Src/traffic_light_logic/traffic_light.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/traffic_light_logic/%.o Core/Src/traffic_light_logic/%.su Core/Src/traffic_light_logic/%.cyclo: ../Core/Src/traffic_light_logic/%.c Core/Src/traffic_light_logic/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-traffic_light_logic

clean-Core-2f-Src-2f-traffic_light_logic:
	-$(RM) ./Core/Src/traffic_light_logic/command_process.cyclo ./Core/Src/traffic_light_logic/command_process.d ./Core/Src/traffic_light_logic/command_process.o ./Core/Src/traffic_light_logic/command_process.su ./Core/Src/traffic_light_logic/traffic_light.cyclo ./Core/Src/traffic_light_logic/traffic_light.d ./Core/Src/traffic_light_logic/traffic_light.o ./Core/Src/traffic_light_logic/traffic_light.su

.PHONY: clean-Core-2f-Src-2f-traffic_light_logic

