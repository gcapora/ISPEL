################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/main_app.c \
../App/Src/task_Led.c 

OBJS += \
./App/Src/main_app.o \
./App/Src/task_Led.o 

C_DEPS += \
./App/Src/main_app.d \
./App/Src/task_Led.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/App" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/App/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/App/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/main_app.d ./App/Src/main_app.o ./App/Src/main_app.su ./App/Src/task_Led.d ./App/Src/task_Led.o ./App/Src/task_Led.su

.PHONY: clean-App-2f-Src

