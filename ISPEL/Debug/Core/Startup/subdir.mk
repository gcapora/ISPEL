################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f429zitx.s 

OBJS += \
./Core/Startup/startup_stm32f429zitx.o 

S_DEPS += \
./Core/Startup/startup_stm32f429zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Apli" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Core" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/STM32F4xx_HAL_Driver/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/guille/Documentos/CESE_docs/TF/SW/ISPEL/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f429zitx.d ./Core/Startup/startup_stm32f429zitx.o

.PHONY: clean-Core-2f-Startup

