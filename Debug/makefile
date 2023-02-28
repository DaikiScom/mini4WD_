################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/USB/subdir.mk
-include src/PERIPHERAL/subdir.mk
-include src/OTHER/subdir.mk
-include src/MODE/subdir.mk
-include src/AICHIP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RT-AICHIPV3-sample.axf

# Tool invocations
RT-AICHIPV3-sample.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/kentaro/git/AIchipV3/sample_soft/CMSISv1p30_LPC13xx/Debug" -Xlinker -Map="RT-AICHIPV3-sample.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "RT-AICHIPV3-sample_Debug.ld" -o "RT-AICHIPV3-sample.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) RT-AICHIPV3-sample.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "RT-AICHIPV3-sample.axf" ; arm-none-eabi-objcopy -v -O binary "RT-AICHIPV3-sample.axf" "RT-AICHIPV3-sample.bin" ; checksum -p LPC1343 -d "RT-AICHIPV3-sample.bin"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
