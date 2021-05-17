################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Keil tools exit code 1 is warnings only, so don't treat as an error in make
RC := test $$? -lt 2

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include lib/efm8ub2/peripheralDrivers/src/subdir.mk
-include lib/efm8_usb/src/subdir.mk
-include lib/efm8_assert/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EFM8UB2_HID_VendorDefined_Knobs_01.omf

# Tool invocations
EFM8UB2_HID_VendorDefined_Knobs_01.omf: echo_path $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Keil 8051 Linker'
	LX51 "@EFM8UB2_HID_VendorDefined_Knobs_01.lnp" || $(RC)
	@echo 'Finished building target: $@'

# Change from all upper case to the expected case
	@-mv "EFM8UB2_HID_VENDORDEFINED_KNOBS_01.OMF.CRBUILD" "$@"

# Generate hex file: EFM8UB2_HID_VendorDefined_Knobs_01.hex
	@Ohx51 "EFM8UB2_HID_VendorDefined_Knobs_01.omf" "HEXFILE (EFM8UB2_HID_VendorDefined_Knobs_01.hex)" "H386" 2>&1 >/dev/null

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS) EFM8UB2_HID_VendorDefined_Knobs_01.omf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
# echo the path
echo_path:
	@echo PATH=$$PATH
	@echo ' '

