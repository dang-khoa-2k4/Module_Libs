################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/PWM\ control/PWMcontrol.c 

OBJS += \
./Core/Src/PWM\ control/PWMcontrol.o 

C_DEPS += \
./Core/Src/PWM\ control/PWMcontrol.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/PWM\ control/PWMcontrol.o: ../Core/Src/PWM\ control/PWMcontrol.c Core/Src/PWM\ control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/PWM control/PWMcontrol.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-PWM-20-control

clean-Core-2f-Src-2f-PWM-20-control:
	-$(RM) ./Core/Src/PWM\ control/PWMcontrol.cyclo ./Core/Src/PWM\ control/PWMcontrol.d ./Core/Src/PWM\ control/PWMcontrol.o ./Core/Src/PWM\ control/PWMcontrol.su

.PHONY: clean-Core-2f-Src-2f-PWM-20-control

