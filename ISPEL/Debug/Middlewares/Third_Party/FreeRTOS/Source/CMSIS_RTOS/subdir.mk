################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.o Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.su: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.c Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Core/Src" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Apli" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Core" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Core/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Src" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Control/Inc" -I"/home/guille/Documentos/CESE/TF/SW/ISPEL/Apli/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d ./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o ./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS

