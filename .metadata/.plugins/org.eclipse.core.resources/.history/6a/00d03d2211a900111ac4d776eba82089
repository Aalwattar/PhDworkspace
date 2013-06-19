################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
MB1_Slave1.elf.size \

ELFCHECK += \
MB1_Slave1.elf.elfcheck \


# All Target
all: MB1_Slave1.elf secondary-outputs

# Tool invocations
MB1_Slave1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../standalone_bsp_1/microblaze_1/lib -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.00.b -mno-xl-soft-mul -o"MB1_Slave1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

MB1_Slave1.elf.size: MB1_Slave1.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size MB1_Slave1.elf  |tee "MB1_Slave1.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

MB1_Slave1.elf.elfcheck: MB1_Slave1.elf
	@echo 'Invoking: Xilinx ELF Check'
	elfcheck MB1_Slave1.elf -hw ../../hw_platform_0/system.xml -pe microblaze_1  |tee "MB1_Slave1.elf.elfcheck"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) MB1_Slave1.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
