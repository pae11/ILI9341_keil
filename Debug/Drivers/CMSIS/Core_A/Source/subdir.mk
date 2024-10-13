################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.c 

OBJS += \
./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.o 

C_DEPS += \
./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Core_A/Source/%.o Drivers/CMSIS/Core_A/Source/%.su Drivers/CMSIS/Core_A/Source/%.cyclo: ../Drivers/CMSIS/Core_A/Source/%.c Drivers/CMSIS/Core_A/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/Drivers/CMSIS" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE/_ili9341" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/Drivers/CMSIS/RTOS2/Include" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE/_ili9341" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Core_A-2f-Source

clean-Drivers-2f-CMSIS-2f-Core_A-2f-Source:
	-$(RM) ./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.cyclo ./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.d ./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.o ./Drivers/CMSIS/Core_A/Source/irq_ctrl_gic.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Core_A-2f-Source

