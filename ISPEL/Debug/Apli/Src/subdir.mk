################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Apli/Src/apli.c \
../Apli/Src/tarea_leds.c \
../Apli/Src/tarea_test_1.c \
../Apli/Src/task_Led.c 

OBJS += \
./Apli/Src/apli.o \
./Apli/Src/tarea_leds.o \
./Apli/Src/tarea_test_1.o \
./Apli/Src/task_Led.o 

C_DEPS += \
./Apli/Src/apli.d \
./Apli/Src/tarea_leds.d \
./Apli/Src/tarea_test_1.d \
./Apli/Src/task_Led.d 


# Each subdirectory must supply rules for building sources it contributes
Apli/Src/%.o Apli/Src/%.su: ../Apli/Src/%.c Apli/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Core/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Control/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Apli/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Supporting_Functions/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Apli-2f-Src

clean-Apli-2f-Src:
	-$(RM) ./Apli/Src/apli.d ./Apli/Src/apli.o ./Apli/Src/apli.su ./Apli/Src/tarea_leds.d ./Apli/Src/tarea_leds.o ./Apli/Src/tarea_leds.su ./Apli/Src/tarea_test_1.d ./Apli/Src/tarea_test_1.o ./Apli/Src/tarea_test_1.su ./Apli/Src/task_Led.d ./Apli/Src/task_Led.o ./Apli/Src/task_Led.su

.PHONY: clean-Apli-2f-Src

