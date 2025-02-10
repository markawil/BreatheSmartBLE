################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/CustomDrivers/BME280/bme280.c 

OBJS += \
./Core/CustomDrivers/BME280/bme280.o 

C_DEPS += \
./Core/CustomDrivers/BME280/bme280.d 


# Each subdirectory must supply rules for building sources it contributes
Core/CustomDrivers/BME280/%.o Core/CustomDrivers/BME280/%.su Core/CustomDrivers/BME280/%.cyclo: ../Core/CustomDrivers/BME280/%.c Core/CustomDrivers/BME280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers" -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers/BLE" -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers/BME280" -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers/CCS811" -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers/MPU6050" -I"/Users/markwilkinson/STM32CubeIDE/workspace_1.16.1/NUCLEO-L432KC/Core/CustomDrivers/SSD1306" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-CustomDrivers-2f-BME280

clean-Core-2f-CustomDrivers-2f-BME280:
	-$(RM) ./Core/CustomDrivers/BME280/bme280.cyclo ./Core/CustomDrivers/BME280/bme280.d ./Core/CustomDrivers/BME280/bme280.o ./Core/CustomDrivers/BME280/bme280.su

.PHONY: clean-Core-2f-CustomDrivers-2f-BME280

