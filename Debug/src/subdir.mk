################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/old_mpu9150.c 

OBJS += \
./src/old_mpu9150.o 

C_DEPS += \
./src/old_mpu9150.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"C:\Users\nnaka\Downloads\AIchip\workspace\CMSISv1p30_LPC13xx\inc" -I"C:\Users\nnaka\Downloads\AIchip\workspace\RT-AICHIP-sample\inc\PERIPHERAL" -I"C:\Users\nnaka\Downloads\AIchip\workspace\RT-AICHIP-sample\inc\USB" -I"C:\Users\nnaka\Downloads\AIchip\workspace\RT-AICHIP-sample\inc\OTHER" -I"C:\Users\nnaka\Downloads\AIchip\workspace\RT-AICHIP-sample\inc\MODE" -I"C:\Users\nnaka\Downloads\AIchip\workspace\RT-AICHIP-sample\inc\AICHIP" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


