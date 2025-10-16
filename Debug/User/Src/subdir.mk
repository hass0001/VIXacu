################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/Base64Enc.c \
../User/Src/CheckSum.c \
../User/Src/EthernetDriver.c \
../User/Src/FacAutoTest.c \
../User/Src/Firmware.c \
../User/Src/Flashrom.c \
../User/Src/History.c \
../User/Src/I2C_Eeprom.c \
../User/Src/I2C_PCA9552.c \
../User/Src/InTask.c \
../User/Src/MD5.c \
../User/Src/RTC_Proc.c \
../User/Src/SCI1FwdTask.c \
../User/Src/SCI2Rs485Task.c \
../User/Src/SCI3FacTask.c \
../User/Src/SPI1NetTask.c \
../User/Src/StartTask.c \
../User/Src/Version_Plus.c \
../User/Src/WiegandTask.c \
../User/Src/des.c \
../User/Src/vixDebugConsole.c \
../User/Src/vixEEPROM.c \
../User/Src/vixEthernet.c \
../User/Src/vixExNorFlash.c \
../User/Src/vixLibrary.c \
../User/Src/vixSpiDriver.c \
../User/Src/vixUartQueue.c 

OBJS += \
./User/Src/Base64Enc.o \
./User/Src/CheckSum.o \
./User/Src/EthernetDriver.o \
./User/Src/FacAutoTest.o \
./User/Src/Firmware.o \
./User/Src/Flashrom.o \
./User/Src/History.o \
./User/Src/I2C_Eeprom.o \
./User/Src/I2C_PCA9552.o \
./User/Src/InTask.o \
./User/Src/MD5.o \
./User/Src/RTC_Proc.o \
./User/Src/SCI1FwdTask.o \
./User/Src/SCI2Rs485Task.o \
./User/Src/SCI3FacTask.o \
./User/Src/SPI1NetTask.o \
./User/Src/StartTask.o \
./User/Src/Version_Plus.o \
./User/Src/WiegandTask.o \
./User/Src/des.o \
./User/Src/vixDebugConsole.o \
./User/Src/vixEEPROM.o \
./User/Src/vixEthernet.o \
./User/Src/vixExNorFlash.o \
./User/Src/vixLibrary.o \
./User/Src/vixSpiDriver.o \
./User/Src/vixUartQueue.o 

C_DEPS += \
./User/Src/Base64Enc.d \
./User/Src/CheckSum.d \
./User/Src/EthernetDriver.d \
./User/Src/FacAutoTest.d \
./User/Src/Firmware.d \
./User/Src/Flashrom.d \
./User/Src/History.d \
./User/Src/I2C_Eeprom.d \
./User/Src/I2C_PCA9552.d \
./User/Src/InTask.d \
./User/Src/MD5.d \
./User/Src/RTC_Proc.d \
./User/Src/SCI1FwdTask.d \
./User/Src/SCI2Rs485Task.d \
./User/Src/SCI3FacTask.d \
./User/Src/SPI1NetTask.d \
./User/Src/StartTask.d \
./User/Src/Version_Plus.d \
./User/Src/WiegandTask.d \
./User/Src/des.d \
./User/Src/vixDebugConsole.d \
./User/Src/vixEEPROM.d \
./User/Src/vixEthernet.d \
./User/Src/vixExNorFlash.d \
./User/Src/vixLibrary.d \
./User/Src/vixSpiDriver.d \
./User/Src/vixUartQueue.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../W6100Lib/Inc -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -O0 -ffunction-sections -fdata-sections -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/Base64Enc.cyclo ./User/Src/Base64Enc.d ./User/Src/Base64Enc.o ./User/Src/Base64Enc.su ./User/Src/CheckSum.cyclo ./User/Src/CheckSum.d ./User/Src/CheckSum.o ./User/Src/CheckSum.su ./User/Src/EthernetDriver.cyclo ./User/Src/EthernetDriver.d ./User/Src/EthernetDriver.o ./User/Src/EthernetDriver.su ./User/Src/FacAutoTest.cyclo ./User/Src/FacAutoTest.d ./User/Src/FacAutoTest.o ./User/Src/FacAutoTest.su ./User/Src/Firmware.cyclo ./User/Src/Firmware.d ./User/Src/Firmware.o ./User/Src/Firmware.su ./User/Src/Flashrom.cyclo ./User/Src/Flashrom.d ./User/Src/Flashrom.o ./User/Src/Flashrom.su ./User/Src/History.cyclo ./User/Src/History.d ./User/Src/History.o ./User/Src/History.su ./User/Src/I2C_Eeprom.cyclo ./User/Src/I2C_Eeprom.d ./User/Src/I2C_Eeprom.o ./User/Src/I2C_Eeprom.su ./User/Src/I2C_PCA9552.cyclo ./User/Src/I2C_PCA9552.d ./User/Src/I2C_PCA9552.o ./User/Src/I2C_PCA9552.su ./User/Src/InTask.cyclo ./User/Src/InTask.d ./User/Src/InTask.o ./User/Src/InTask.su ./User/Src/MD5.cyclo ./User/Src/MD5.d ./User/Src/MD5.o ./User/Src/MD5.su ./User/Src/RTC_Proc.cyclo ./User/Src/RTC_Proc.d ./User/Src/RTC_Proc.o ./User/Src/RTC_Proc.su ./User/Src/SCI1FwdTask.cyclo ./User/Src/SCI1FwdTask.d ./User/Src/SCI1FwdTask.o ./User/Src/SCI1FwdTask.su ./User/Src/SCI2Rs485Task.cyclo ./User/Src/SCI2Rs485Task.d ./User/Src/SCI2Rs485Task.o ./User/Src/SCI2Rs485Task.su ./User/Src/SCI3FacTask.cyclo ./User/Src/SCI3FacTask.d ./User/Src/SCI3FacTask.o ./User/Src/SCI3FacTask.su ./User/Src/SPI1NetTask.cyclo ./User/Src/SPI1NetTask.d ./User/Src/SPI1NetTask.o ./User/Src/SPI1NetTask.su ./User/Src/StartTask.cyclo ./User/Src/StartTask.d ./User/Src/StartTask.o ./User/Src/StartTask.su ./User/Src/Version_Plus.cyclo ./User/Src/Version_Plus.d ./User/Src/Version_Plus.o ./User/Src/Version_Plus.su ./User/Src/WiegandTask.cyclo ./User/Src/WiegandTask.d ./User/Src/WiegandTask.o ./User/Src/WiegandTask.su ./User/Src/des.cyclo ./User/Src/des.d ./User/Src/des.o ./User/Src/des.su ./User/Src/vixDebugConsole.cyclo ./User/Src/vixDebugConsole.d ./User/Src/vixDebugConsole.o ./User/Src/vixDebugConsole.su ./User/Src/vixEEPROM.cyclo ./User/Src/vixEEPROM.d ./User/Src/vixEEPROM.o ./User/Src/vixEEPROM.su ./User/Src/vixEthernet.cyclo ./User/Src/vixEthernet.d ./User/Src/vixEthernet.o ./User/Src/vixEthernet.su ./User/Src/vixExNorFlash.cyclo ./User/Src/vixExNorFlash.d ./User/Src/vixExNorFlash.o ./User/Src/vixExNorFlash.su ./User/Src/vixLibrary.cyclo ./User/Src/vixLibrary.d ./User/Src/vixLibrary.o ./User/Src/vixLibrary.su ./User/Src/vixSpiDriver.cyclo ./User/Src/vixSpiDriver.d ./User/Src/vixSpiDriver.o ./User/Src/vixSpiDriver.su ./User/Src/vixUartQueue.cyclo ./User/Src/vixUartQueue.d ./User/Src/vixUartQueue.o ./User/Src/vixUartQueue.su

.PHONY: clean-User-2f-Src

