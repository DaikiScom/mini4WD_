################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MODE/modeSelect.c \
../src/MODE/mode_BluetoothSetting.c \
../src/MODE/mode_controlRun.c \
../src/MODE/mode_debug.c \
../src/MODE/mode_dutyMax.c \
../src/MODE/mode_selectDuty.c 

OBJS += \
./src/MODE/modeSelect.o \
./src/MODE/mode_BluetoothSetting.o \
./src/MODE/mode_controlRun.o \
./src/MODE/mode_debug.o \
./src/MODE/mode_dutyMax.o \
./src/MODE/mode_selectDuty.o 

C_DEPS += \
./src/MODE/modeSelect.d \
./src/MODE/mode_BluetoothSetting.d \
./src/MODE/mode_controlRun.d \
./src/MODE/mode_debug.d \
./src/MODE/mode_dutyMax.d \
./src/MODE/mode_selectDuty.d 


# Each subdirectory must supply rules for building sources it contributes
src/MODE/%.o: ../src/MODE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"/home/kentaro/git/AIchipV3/sample_soft/CMSISv1p30_LPC13xx/inc" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/PERIPHERAL" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/USB" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/OTHER" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/MODE" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/AICHIP" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


