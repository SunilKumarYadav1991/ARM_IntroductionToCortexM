################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := 5.InterruptPriorityConfiguration
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
5.InterruptPriorityConfiguration.elf \

MAP_FILES += \
5.InterruptPriorityConfiguration.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
5.InterruptPriorityConfiguration.list \


# All Target
all: main-build

# Main-build Target
main-build: 5.InterruptPriorityConfiguration.elf secondary-outputs

# Tool invocations
5.InterruptPriorityConfiguration.elf 5.InterruptPriorityConfiguration.map: $(OBJS) $(USER_OBJS) D:\Win7_Vostro\G_Drive\VectorCAST_Copy\SKY\11.udemy\3.ARM\ CortexM\Workspace\STM32F411\5.InterruptPriorityConfiguration\STM32F411VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "5.InterruptPriorityConfiguration.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\Win7_Vostro\G_Drive\VectorCAST_Copy\SKY\11.udemy\3.ARM CortexM\Workspace\STM32F411\5.InterruptPriorityConfiguration\STM32F411VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="5.InterruptPriorityConfiguration.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

5.InterruptPriorityConfiguration.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "5.InterruptPriorityConfiguration.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 5.InterruptPriorityConfiguration.elf 5.InterruptPriorityConfiguration.list 5.InterruptPriorityConfiguration.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
