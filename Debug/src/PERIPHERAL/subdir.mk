################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/PERIPHERAL/SystemTickTimer.c \
../src/PERIPHERAL/ad.c \
../src/PERIPHERAL/debug.c \
../src/PERIPHERAL/i2c.c \
../src/PERIPHERAL/io.c \
../src/PERIPHERAL/pwm.c \
../src/PERIPHERAL/ssp.c \
../src/PERIPHERAL/timer.c \
../src/PERIPHERAL/uart.c \
../src/PERIPHERAL/usbTransmission.c 

OBJS += \
./src/PERIPHERAL/SystemTickTimer.o \
./src/PERIPHERAL/ad.o \
./src/PERIPHERAL/debug.o \
./src/PERIPHERAL/i2c.o \
./src/PERIPHERAL/io.o \
./src/PERIPHERAL/pwm.o \
./src/PERIPHERAL/ssp.o \
./src/PERIPHERAL/timer.o \
./src/PERIPHERAL/uart.o \
./src/PERIPHERAL/usbTransmission.o 

C_DEPS += \
./src/PERIPHERAL/SystemTickTimer.d \
./src/PERIPHERAL/ad.d \
./src/PERIPHERAL/debug.d \
./src/PERIPHERAL/i2c.d \
./src/PERIPHERAL/io.d \
./src/PERIPHERAL/pwm.d \
./src/PERIPHERAL/ssp.d \
./src/PERIPHERAL/timer.d \
./src/PERIPHERAL/uart.d \
./src/PERIPHERAL/usbTransmission.d 


# Each subdirectory must supply rules for building sources it contributes
src/PERIPHERAL/%.o: ../src/PERIPHERAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv1p30_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"/home/kentaro/git/AIchipV3/sample_soft/CMSISv1p30_LPC13xx/inc" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/PERIPHERAL" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/USB" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/OTHER" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/MODE" -I"/home/kentaro/git/AIchipV3/sample_soft/RT-AICHIPV3-sample/inc/AICHIP" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


