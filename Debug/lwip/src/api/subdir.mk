################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/api/api_lib.c \
../lwip/src/api/api_msg.c \
../lwip/src/api/err.c \
../lwip/src/api/if_api.c \
../lwip/src/api/netbuf.c \
../lwip/src/api/netdb.c \
../lwip/src/api/netifapi.c \
../lwip/src/api/sockets.c \
../lwip/src/api/tcpip.c 

C_DEPS += \
./lwip/src/api/api_lib.d \
./lwip/src/api/api_msg.d \
./lwip/src/api/err.d \
./lwip/src/api/if_api.d \
./lwip/src/api/netbuf.d \
./lwip/src/api/netdb.d \
./lwip/src/api/netifapi.d \
./lwip/src/api/sockets.d \
./lwip/src/api/tcpip.d 

OBJS += \
./lwip/src/api/api_lib.o \
./lwip/src/api/api_msg.o \
./lwip/src/api/err.o \
./lwip/src/api/if_api.o \
./lwip/src/api/netbuf.o \
./lwip/src/api/netdb.o \
./lwip/src/api/netifapi.o \
./lwip/src/api/sockets.o \
./lwip/src/api/tcpip.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/api/%.o: ../lwip/src/api/%.c lwip/src/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\source" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\mdio" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\phy" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src\include\lwip\apps" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\port" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src\include" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\drivers" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\utilities" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\device" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\uart" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\serial_manager" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\lists" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\CMSIS" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\freertos\freertos_kernel\include" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-api

clean-lwip-2f-src-2f-api:
	-$(RM) ./lwip/src/api/api_lib.d ./lwip/src/api/api_lib.o ./lwip/src/api/api_msg.d ./lwip/src/api/api_msg.o ./lwip/src/api/err.d ./lwip/src/api/err.o ./lwip/src/api/if_api.d ./lwip/src/api/if_api.o ./lwip/src/api/netbuf.d ./lwip/src/api/netbuf.o ./lwip/src/api/netdb.d ./lwip/src/api/netdb.o ./lwip/src/api/netifapi.d ./lwip/src/api/netifapi.o ./lwip/src/api/sockets.d ./lwip/src/api/sockets.o ./lwip/src/api/tcpip.d ./lwip/src/api/tcpip.o

.PHONY: clean-lwip-2f-src-2f-api

