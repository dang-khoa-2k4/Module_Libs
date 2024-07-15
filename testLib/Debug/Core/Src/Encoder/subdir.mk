################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Encoder/Encoder.c 

OBJS += \
./Core/Src/Encoder/Encoder.o 

C_DEPS += \
./Core/Src/Encoder/Encoder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Encoder/%.o Core/Src/Encoder/%.su Core/Src/Encoder/%.cyclo: ../Core/Src/Encoder/%.c Core/Src/Encoder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Encoder

clean-Core-2f-Src-2f-Encoder:
	-$(RM) ./Core/Src/Encoder/Encoder.cyclo ./Core/Src/Encoder/Encoder.d ./Core/Src/Encoder/Encoder.o ./Core/Src/Encoder/Encoder.su

.PHONY: clean-Core-2f-Src-2f-Encoder

