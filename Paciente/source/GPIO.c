/*
 * GPIO.c
 * Se definen las funciones con las cuales se configuran y controlan los pines
 * de la tarjeta.
 *
 *  Created on: 26 ene. 2024
 *  Author: Daniel Alfaro y Frida Hernandez.
 */

#include "Bits.h"
#include "GPIO.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "stdbool.h"

static uint8_t gpio_swflag = 0;



//Funciones de configuracion de puerto y pin
void GPIO_init(void)
{
	gpio_pin_config_t gpio_input_config = {
					kGPIO_DigitalInput,
					0,
				};

	gpio_pin_config_t gpio_output_config = {
					kGPIO_DigitalOutput,
					1,
				};

	const port_pin_config_t button_config = {
			kPORT_PullUp,                     /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,               /* Fast slew rate is configured */
			kPORT_PassiveFilterEnable,        /* Passive filter is enabled */
			kPORT_OpenDrainDisable,           /* Open drain is disabled */
			kPORT_HighDriveStrength,          /* High drive strength is configured */
			kPORT_MuxAsGpio,                  /* Pin is configured as PTA4 */
			kPORT_UnlockRegister              /* Pin Control Register fields [15:0] are not locked */
		  };

	//Struct de configuracion de la LCD
	gpio_pin_config_t led_config = {
		        kGPIO_DigitalOutput,
		        1,
		    };

	//Struct de config del SPI
	const port_pin_config_t port_D = {
				kPORT_PullUp,                 /* Internal pull-up resistor is enabled */
				kPORT_FastSlewRate,           /* Fast slew rate is configured */
				kPORT_PassiveFilterEnable,    /* Passive filter is disabled */
				kPORT_OpenDrainDisable,       /* Open drain is disabled */
				kPORT_HighDriveStrength,      /* High drive strength is configured */
				kPORT_MuxAlt2,                /* Pin is configured as PTA4 */
				kPORT_UnlockRegister          /* Pin Control Register fields [15:0] are not locked */
				};

	//Activar clock de cada puerto
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	//Config LED k66 - ya corregido
	PORT_SetPinMux(PORTA, bit_11, kPORT_MuxAsGpio);		//Inicializacion de
	GPIO_PinInit(GPIOA, bit_11, &gpio_output_config);	//INICIALIZA EL GPIO

	PORT_SetPinMux(PORTC, bit_9, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, bit_9, &gpio_output_config);

	PORT_SetPinMux(PORTE, bit_6, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOE, bit_6, &gpio_output_config);

	//Config botones
	//Botones K66 - ya corregido
	GPIO_PinInit(GPIOD, bit_11, &gpio_input_config);
	PORT_SetPinConfig(PORTD, bit_11, &button_config);
	PORT_SetPinInterruptConfig(PORTD, bit_11, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOA, bit_10, &gpio_input_config);
	PORT_SetPinConfig(PORTA, bit_10, &button_config);
	PORT_SetPinInterruptConfig(PORTA, bit_10, kPORT_InterruptFallingEdge);
}

void GPIO_SetFlag()
{
	gpio_swflag = 1;
}

void GPIO_ClearFlag()
{
	gpio_swflag = 0;
}

uint8_t GPIO_GetFlag()
{
	return gpio_swflag;
}
