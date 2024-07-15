################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AVERAGE\ FILTER/average_filter.c 

OBJS += \
./Core/Src/AVERAGE\ FILTER/average_filter.o 

C_DEPS += \
./Core/Src/AVERAGE\ FILTER/average_filter.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/AVERAGE\ FILTER/average_filter.o: ../Core/Src/AVERAGE\ FILTER/average_filter.c Core/Src/AVERAGE\ FILTER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/AVERAGE FILTER/average_filter.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-AVERAGE-20-FILTER

clean-Core-2f-Src-2f-AVERAGE-20-FILTER:
	-$(RM) ./Core/Src/AVERAGE\ FILTER/average_filter.cyclo ./Core/Src/AVERAGE\ FILTER/average_filter.d ./Core/Src/AVERAGE\ FILTER/average_filter.o ./Core/Src/AVERAGE\ FILTER/average_filter.su

.PHONY: clean-Core-2f-Src-2f-AVERAGE-20-FILTER

