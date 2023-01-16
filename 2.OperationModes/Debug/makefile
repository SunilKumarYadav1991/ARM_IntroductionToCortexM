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


BUILD_ARTIFACT_NAME := 2.OperationModes_and_AccessLevel
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
2.OperationModes_and_AccessLevel.elf \

MAP_FILES += \
2.OperationModes_and_AccessLevel.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
2.OperationModes_and_AccessLevel.list \


# All Target
all: main-build

# Main-build Target
main-build: 2.OperationModes_and_AccessLevel.elf secondary-outputs

# Tool invocations
2.OperationModes_and_AccessLevel.elf 2.OperationModes_and_AccessLevel.map: $(OBJS) $(USER_OBJS) D:\Win7_Vostro\G_Drive\VectorCAST_Copy\SKY\11.udemy\3.ARM\ CortexM\Workspace\STM32F411\OperationModes\STM32F411VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "2.OperationModes_and_AccessLevel.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\Win7_Vostro\G_Drive\VectorCAST_Copy\SKY\11.udemy\3.ARM CortexM\Workspace\STM32F411\OperationModes\STM32F411VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="2.OperationModes_and_AccessLevel.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

2.OperationModes_and_AccessLevel.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "2.OperationModes_and_AccessLevel.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 2.OperationModes_and_AccessLevel.elf 2.OperationModes_and_AccessLevel.list 2.OperationModes_and_AccessLevel.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
