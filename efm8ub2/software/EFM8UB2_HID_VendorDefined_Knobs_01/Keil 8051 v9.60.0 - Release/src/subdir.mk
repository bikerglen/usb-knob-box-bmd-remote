################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
A51_UPPER_SRCS += \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/SILABS_STARTUP.A51 

C_SRCS += \
../src/InitDevice.c \
../src/Interrupts.c \
../src/callback.c \
../src/descriptors.c \
../src/idle.c \
../src/main.c 

OBJS += \
./src/InitDevice.OBJ \
./src/Interrupts.OBJ \
./src/SILABS_STARTUP.OBJ \
./src/callback.OBJ \
./src/descriptors.OBJ \
./src/idle.OBJ \
./src/main.OBJ 


# Each subdirectory must supply rules for building sources it contributes
src/%.OBJ: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/InitDevice.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/InitDevice.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/descriptors.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDLIB.H

src/Interrupts.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/SILABS_STARTUP.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/SILABS_STARTUP.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/callback.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/descriptors.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/idle.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDLIB.H

src/descriptors.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDLIB.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/descriptors.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h

src/idle.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/idle.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/main.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/descriptors.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/idle.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/InitDevice.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDLIB.H


