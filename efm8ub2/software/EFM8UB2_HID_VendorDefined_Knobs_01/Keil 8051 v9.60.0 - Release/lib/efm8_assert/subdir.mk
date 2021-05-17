################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_assert/assert.c 

OBJS += \
./lib/efm8_assert/assert.OBJ 


# Each subdirectory must supply rules for building sources it contributes
lib/efm8_assert/assert.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Lib/efm8_assert/assert.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '


