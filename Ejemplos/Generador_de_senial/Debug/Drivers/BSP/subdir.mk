################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/stm32f4xx_nucleo_144.c 

OBJS += \
./Drivers/BSP/stm32f4xx_nucleo_144.o 

C_DEPS += \
./Drivers/BSP/stm32f4xx_nucleo_144.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o Drivers/BSP/%.su: ../Drivers/BSP/%.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/guille/Documentos/CESE_docs/TF/SW/Ejemplos/Generador_de_senial/Drivers/API" -I"/home/guille/Documentos/CESE_docs/TF/SW/Ejemplos/Generador_de_senial/Drivers/API/Inc" -I"/home/guille/Documentos/CESE_docs/TF/SW/Ejemplos/Generador_de_senial/Drivers/API/Src" -I"/home/guille/Documentos/CESE_docs/TF/SW/Ejemplos/Generador_de_senial/Drivers/BSP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/stm32f4xx_nucleo_144.d ./Drivers/BSP/stm32f4xx_nucleo_144.o ./Drivers/BSP/stm32f4xx_nucleo_144.su

.PHONY: clean-Drivers-2f-BSP

