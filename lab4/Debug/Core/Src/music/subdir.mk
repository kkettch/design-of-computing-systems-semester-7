################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/music/notes_handler.c \
../Core/Src/music/operating_mode.c 

OBJS += \
./Core/Src/music/notes_handler.o \
./Core/Src/music/operating_mode.o 

C_DEPS += \
./Core/Src/music/notes_handler.d \
./Core/Src/music/operating_mode.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/music/%.o Core/Src/music/%.su Core/Src/music/%.cyclo: ../Core/Src/music/%.c Core/Src/music/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-music

clean-Core-2f-Src-2f-music:
	-$(RM) ./Core/Src/music/notes_handler.cyclo ./Core/Src/music/notes_handler.d ./Core/Src/music/notes_handler.o ./Core/Src/music/notes_handler.su ./Core/Src/music/operating_mode.cyclo ./Core/Src/music/operating_mode.d ./Core/Src/music/operating_mode.o ./Core/Src/music/operating_mode.su

.PHONY: clean-Core-2f-Src-2f-music

