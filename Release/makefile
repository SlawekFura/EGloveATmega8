################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Sources/subdir.mk
-include MYUART/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
EGlove_Mikro.lss \

FLASH_IMAGE += \
EGlove_Mikro.hex \

EEPROM_IMAGE += \
EGlove_Mikro.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: EGlove_Mikro.elf secondary-outputs

# Tool invocations
EGlove_Mikro.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,EGlove_Mikro.map -mmcu=atmega8 -o "EGlove_Mikro.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

EGlove_Mikro.lss: EGlove_Mikro.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S EGlove_Mikro.elf  >"EGlove_Mikro.lss"
	@echo 'Finished building: $@'
	@echo ' '

EGlove_Mikro.hex: EGlove_Mikro.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex EGlove_Mikro.elf  "EGlove_Mikro.hex"
	@echo 'Finished building: $@'
	@echo ' '

EGlove_Mikro.eep: EGlove_Mikro.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex EGlove_Mikro.elf  "EGlove_Mikro.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: EGlove_Mikro.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 EGlove_Mikro.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) EGlove_Mikro.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
