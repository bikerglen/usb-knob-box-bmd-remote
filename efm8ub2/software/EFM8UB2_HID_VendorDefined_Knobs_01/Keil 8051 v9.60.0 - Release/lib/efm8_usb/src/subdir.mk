################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbd.c \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdch9.c \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdep.c \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdint.c 

OBJS += \
./lib/efm8_usb/src/efm8_usbd.OBJ \
./lib/efm8_usb/src/efm8_usbdch9.OBJ \
./lib/efm8_usb/src/efm8_usbdep.OBJ \
./lib/efm8_usb/src/efm8_usbdint.OBJ 


# Each subdirectory must supply rules for building sources it contributes
lib/efm8_usb/src/efm8_usbd.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbd.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

lib/efm8_usb/src/efm8_usbd.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/ASSERT.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDIO.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDLIB.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h

lib/efm8_usb/src/efm8_usbdch9.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdch9.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

lib/efm8_usb/src/efm8_usbdch9.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h

lib/efm8_usb/src/efm8_usbdep.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdep.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

lib/efm8_usb/src/efm8_usbdep.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h

lib/efm8_usb/src/efm8_usbdint.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/src/efm8_usbdint.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

lib/efm8_usb/src/efm8_usbdint.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_usb/inc/efm8_usb.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/endian.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h C:/Users/glen/SimplicityStudio/v4_workspace/EFM8UB2_HID_VendorDefined_Knobs_01/inc/config/usbconfig.h C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STRING.H C:/SiliconLabs/SimplicityStudio/v4/developer/toolchains/keil_8051/9.60/INC/STDDEF.H C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/peripheral_driver/inc/usb_0.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8UB2/inc/SI_EFM8UB2_Defs.h


