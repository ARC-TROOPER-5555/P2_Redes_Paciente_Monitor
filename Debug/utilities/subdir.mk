################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_FREE_RTOS -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\source" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\mdio" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\phy" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src\include\lwip\apps" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\port" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\lwip\src\include" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\drivers" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\utilities" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\device" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\uart" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\serial_manager" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\component\lists" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\CMSIS" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\freertos\freertos_kernel\include" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\REDES\Practicas\P2_Hernandez_Alfaro_potes\frdmk66f_lwip_mqtt_freertos\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

