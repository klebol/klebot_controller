################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nRF24/SPI/SPI.c 

OBJS += \
./nRF24/SPI/SPI.o 

C_DEPS += \
./nRF24/SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
nRF24/SPI/%.o: ../nRF24/SPI/%.c nRF24/SPI/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\miqix\Documents\MkClipse\AVR Toolchain\avr\include\avr\iom328p.h" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


