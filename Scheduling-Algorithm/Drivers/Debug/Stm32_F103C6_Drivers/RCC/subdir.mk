################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.c 

OBJS += \
./Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.o 

C_DEPS += \
./Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.o: ../Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.c
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Diploma/STM LAB/Drivers/RTOS/inc" -I"C:/Diploma/STM LAB/Drivers/RTOS" -I"C:/Diploma/STM LAB/Drivers/CMSIS_V5" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/LCD_DRIVER" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Diploma/STM LAB/Drivers/HAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Stm32_F103C6_Drivers/RCC/Stm32_F103C6_RCC_driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

