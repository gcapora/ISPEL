################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Apli/Src/main_app.c \
../Apli/Src/task_Led.c 

OBJS += \
./Apli/Src/main_app.o \
./Apli/Src/task_Led.o 

C_DEPS += \
./Apli/Src/main_app.d \
./Apli/Src/task_Led.d 


# Each subdirectory must supply rules for building sources it contributes
Apli/Src/%.o Apli/Src/%.su: ../Apli/Src/%.c Apli/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Apli" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Apli-2f-Src

clean-Apli-2f-Src:
	-$(RM) ./Apli/Src/main_app.d ./Apli/Src/main_app.o ./Apli/Src/main_app.su ./Apli/Src/task_Led.d ./Apli/Src/task_Led.o ./Apli/Src/task_Led.su

.PHONY: clean-Apli-2f-Src

