################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/OTHER/cr_startup_lpc13xx.c \
../src/OTHER/crp.c \
../src/OTHER/serial.c 

OBJS += \
./src/OTHER/cr_startup_lpc13xx.o \
./src/OTHER/crp.o \
./src/OTHER/serial.o 

C_DEPS += \
./src/OTHER/cr_startup_lpc13xx.d \
./src/OTHER/crp.d \
./src/OTHER/serial.d 


# Each subdirectory must supply rules for building sources it contributes
src/OTHER/%.o: ../src/OTHER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"/home/kentaro/git/AIchipV3/sample_soft/CMSISv1p30_LPC13xx/inc" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/PERIPHERAL" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/USB" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/OTHER" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/MODE" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/AICHIP" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


