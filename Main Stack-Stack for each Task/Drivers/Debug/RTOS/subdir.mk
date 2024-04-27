################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTOS/CortexMX_OS_porting.c \
../RTOS/MYRTOS_FIFO.c \
../RTOS/Scheduler.c 

OBJS += \
./RTOS/CortexMX_OS_porting.o \
./RTOS/MYRTOS_FIFO.o \
./RTOS/Scheduler.o 

C_DEPS += \
./RTOS/CortexMX_OS_porting.d \
./RTOS/MYRTOS_FIFO.d \
./RTOS/Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS/CortexMX_OS_porting.o: ../RTOS/CortexMX_OS_porting.c
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Diploma/STM LAB/Drivers/RTOS/inc" -I"C:/Diploma/STM LAB/Drivers/RTOS" -I"C:/Diploma/STM LAB/Drivers/CMSIS_V5" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/LCD_DRIVER" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Diploma/STM LAB/Drivers/HAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/CortexMX_OS_porting.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
RTOS/MYRTOS_FIFO.o: ../RTOS/MYRTOS_FIFO.c
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Diploma/STM LAB/Drivers/RTOS/inc" -I"C:/Diploma/STM LAB/Drivers/RTOS" -I"C:/Diploma/STM LAB/Drivers/CMSIS_V5" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/LCD_DRIVER" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Diploma/STM LAB/Drivers/HAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/MYRTOS_FIFO.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
RTOS/Scheduler.o: ../RTOS/Scheduler.c
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Diploma/STM LAB/Drivers/RTOS/inc" -I"C:/Diploma/STM LAB/Drivers/RTOS" -I"C:/Diploma/STM LAB/Drivers/CMSIS_V5" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/LCD_DRIVER" -I"C:/Diploma/STM LAB/Drivers/Stm32_F103C6_Drivers/inc" -I"C:/Diploma/STM LAB/Drivers/HAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/Scheduler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

