################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../W6100Lib/Src/CS74x_Queue.c \
../W6100Lib/Src/dhcpv4.c \
../W6100Lib/Src/dns.c \
../W6100Lib/Src/httpClient.c \
../W6100Lib/Src/httpParser.c \
../W6100Lib/Src/httpServer.c \
../W6100Lib/Src/httpUtil.c \
../W6100Lib/Src/socket.c \
../W6100Lib/Src/userHandler.c \
../W6100Lib/Src/w6100.c \
../W6100Lib/Src/wizchip_conf.c 

OBJS += \
./W6100Lib/Src/CS74x_Queue.o \
./W6100Lib/Src/dhcpv4.o \
./W6100Lib/Src/dns.o \
./W6100Lib/Src/httpClient.o \
./W6100Lib/Src/httpParser.o \
./W6100Lib/Src/httpServer.o \
./W6100Lib/Src/httpUtil.o \
./W6100Lib/Src/socket.o \
./W6100Lib/Src/userHandler.o \
./W6100Lib/Src/w6100.o \
./W6100Lib/Src/wizchip_conf.o 

C_DEPS += \
./W6100Lib/Src/CS74x_Queue.d \
./W6100Lib/Src/dhcpv4.d \
./W6100Lib/Src/dns.d \
./W6100Lib/Src/httpClient.d \
./W6100Lib/Src/httpParser.d \
./W6100Lib/Src/httpServer.d \
./W6100Lib/Src/httpUtil.d \
./W6100Lib/Src/socket.d \
./W6100Lib/Src/userHandler.d \
./W6100Lib/Src/w6100.d \
./W6100Lib/Src/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
W6100Lib/Src/%.o W6100Lib/Src/%.su W6100Lib/Src/%.cyclo: ../W6100Lib/Src/%.c W6100Lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../W6100Lib/Inc -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../User/Inc -O0 -ffunction-sections -fdata-sections -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-W6100Lib-2f-Src

clean-W6100Lib-2f-Src:
	-$(RM) ./W6100Lib/Src/CS74x_Queue.cyclo ./W6100Lib/Src/CS74x_Queue.d ./W6100Lib/Src/CS74x_Queue.o ./W6100Lib/Src/CS74x_Queue.su ./W6100Lib/Src/dhcpv4.cyclo ./W6100Lib/Src/dhcpv4.d ./W6100Lib/Src/dhcpv4.o ./W6100Lib/Src/dhcpv4.su ./W6100Lib/Src/dns.cyclo ./W6100Lib/Src/dns.d ./W6100Lib/Src/dns.o ./W6100Lib/Src/dns.su ./W6100Lib/Src/httpClient.cyclo ./W6100Lib/Src/httpClient.d ./W6100Lib/Src/httpClient.o ./W6100Lib/Src/httpClient.su ./W6100Lib/Src/httpParser.cyclo ./W6100Lib/Src/httpParser.d ./W6100Lib/Src/httpParser.o ./W6100Lib/Src/httpParser.su ./W6100Lib/Src/httpServer.cyclo ./W6100Lib/Src/httpServer.d ./W6100Lib/Src/httpServer.o ./W6100Lib/Src/httpServer.su ./W6100Lib/Src/httpUtil.cyclo ./W6100Lib/Src/httpUtil.d ./W6100Lib/Src/httpUtil.o ./W6100Lib/Src/httpUtil.su ./W6100Lib/Src/socket.cyclo ./W6100Lib/Src/socket.d ./W6100Lib/Src/socket.o ./W6100Lib/Src/socket.su ./W6100Lib/Src/userHandler.cyclo ./W6100Lib/Src/userHandler.d ./W6100Lib/Src/userHandler.o ./W6100Lib/Src/userHandler.su ./W6100Lib/Src/w6100.cyclo ./W6100Lib/Src/w6100.d ./W6100Lib/Src/w6100.o ./W6100Lib/Src/w6100.su ./W6100Lib/Src/wizchip_conf.cyclo ./W6100Lib/Src/wizchip_conf.d ./W6100Lib/Src/wizchip_conf.o ./W6100Lib/Src/wizchip_conf.su

.PHONY: clean-W6100Lib-2f-Src

