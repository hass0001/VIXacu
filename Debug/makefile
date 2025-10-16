################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include W6100Lib/Src/subdir.mk
-include User/Src/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := VIXacu_STM32L4SZIT_FW_1.15.31_SG
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
VIXacu_STM32L4SZIT_FW_1.15.31_SG.elf \

MAP_FILES += \
VIXacu_STM32L4SZIT_FW_1.15.31_SG.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
VIXacu_STM32L4SZIT_FW_1.15.31_SG.list \

OBJCOPY_HEX += \
VIXacu_STM32L4SZIT_FW_1.15.31_SG.hex \

OBJCOPY_BIN += \
VIXacu_STM32L4SZIT_FW_1.15.31_SG.bin \


# All Target
all: main-build

# Main-build Target
main-build: VIXacu_STM32L4SZIT_FW_1.15.31_SG.elf secondary-outputs

# Tool invocations
VIXacu_STM32L4SZIT_FW_1.15.31_SG.elf VIXacu_STM32L4SZIT_FW_1.15.31_SG.map: $(OBJS) $(USER_OBJS) D:\Project\VIXacu\23\ Firmware\FW_VIXacu_STM32L4_MardPin_251001\STM32L4S5ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "VIXacu_STM32L4SZIT_FW_1.15.31_SG.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\Project\VIXacu\23 Firmware\FW_VIXacu_STM32L4_MardPin_251001\STM32L4S5ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="VIXacu_STM32L4SZIT_FW_1.15.31_SG.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

VIXacu_STM32L4SZIT_FW_1.15.31_SG.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "VIXacu_STM32L4SZIT_FW_1.15.31_SG.list"
	@echo 'Finished building: $@'
	@echo ' '

VIXacu_STM32L4SZIT_FW_1.15.31_SG.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "VIXacu_STM32L4SZIT_FW_1.15.31_SG.hex"
	@echo 'Finished building: $@'
	@echo ' '

VIXacu_STM32L4SZIT_FW_1.15.31_SG.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "VIXacu_STM32L4SZIT_FW_1.15.31_SG.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) VIXacu_STM32L4SZIT_FW_1.15.31_SG.bin VIXacu_STM32L4SZIT_FW_1.15.31_SG.elf VIXacu_STM32L4SZIT_FW_1.15.31_SG.hex VIXacu_STM32L4SZIT_FW_1.15.31_SG.list VIXacu_STM32L4SZIT_FW_1.15.31_SG.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
