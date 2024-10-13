################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.c 

OBJS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/%.o Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/%.su Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/%.cyclo: ../Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/%.c Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/Drivers/CMSIS" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE/_ili9341" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/Drivers/CMSIS/RTOS2/Include" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE/_ili9341" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM" -I"C:/Users/pae/STM32CubeIDE/workspace_1.16.1/keil/ili9341/MDK-ARM/RTE" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_sin_cos_example

clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_sin_cos_example:
	-$(RM) ./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.cyclo ./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.d ./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.o ./Drivers/CMSIS/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-Examples-2f-ARM-2f-arm_sin_cos_example

