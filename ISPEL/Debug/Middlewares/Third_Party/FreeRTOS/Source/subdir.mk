################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
../Middlewares/Third_Party/FreeRTOS/Source/list.c \
../Middlewares/Third_Party/FreeRTOS/Source/queue.c \
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
../Middlewares/Third_Party/FreeRTOS/Source/timers.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.o \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.o \
./Middlewares/Third_Party/FreeRTOS/Source/list.o \
./Middlewares/Third_Party/FreeRTOS/Source/queue.o \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.o \
./Middlewares/Third_Party/FreeRTOS/Source/timers.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.d \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.d \
./Middlewares/Third_Party/FreeRTOS/Source/list.d \
./Middlewares/Third_Party/FreeRTOS/Source/queue.d \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.d \
./Middlewares/Third_Party/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/%.o Middlewares/Third_Party/FreeRTOS/Source/%.su: ../Middlewares/Third_Party/FreeRTOS/Source/%.c Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Apli" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/croutine.d ./Middlewares/Third_Party/FreeRTOS/Source/croutine.o ./Middlewares/Third_Party/FreeRTOS/Source/croutine.su ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.d ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.o ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.su ./Middlewares/Third_Party/FreeRTOS/Source/list.d ./Middlewares/Third_Party/FreeRTOS/Source/list.o ./Middlewares/Third_Party/FreeRTOS/Source/list.su ./Middlewares/Third_Party/FreeRTOS/Source/queue.d ./Middlewares/Third_Party/FreeRTOS/Source/queue.o ./Middlewares/Third_Party/FreeRTOS/Source/queue.su ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.su ./Middlewares/Third_Party/FreeRTOS/Source/tasks.d ./Middlewares/Third_Party/FreeRTOS/Source/tasks.o ./Middlewares/Third_Party/FreeRTOS/Source/tasks.su ./Middlewares/Third_Party/FreeRTOS/Source/timers.d ./Middlewares/Third_Party/FreeRTOS/Source/timers.o ./Middlewares/Third_Party/FreeRTOS/Source/timers.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

