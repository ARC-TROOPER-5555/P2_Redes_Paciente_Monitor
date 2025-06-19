/*
 * RGB.c
 * Definicion de funciones para manejo de encendido y apagado de RGB.
 *
 *  Created on: 13 mar. 2024
 *  Author: Daniel Alfaro y Frida Hernandez.
 */

#include "fsl_gpio.h"
#include "MK66F18.h"
#include "RGB.h"
#include "GPIO.h"
#include "bits.h"

//Funciones control led RGB
void RGB_on_red ()
{
	GPIO_PortClear(GPIOC, 1<<bit_9);
}

void RGB_on_blue ()
{
	GPIO_PortClear(GPIOA, 1<<bit_11);
}

void RGB_on_green ()
{
	GPIO_PortClear(GPIOE, 1<<bit_6);
}

void RGB_off_all ()
{
	GPIO_PortSet(GPIOC, 1<<bit_9);
	GPIO_PortSet(GPIOE, 1<<bit_6);
	GPIO_PortSet(GPIOA, 1<<bit_11);
}

void RGB_select_color_on(led_color_t color)
{
	switch(color)
	{
		case Red:
			RGB_off_all();
			RGB_on_red();
			break;
		case Blue:
			RGB_off_all();
			RGB_on_blue();
			break;
		case Green:
			RGB_off_all();
			RGB_on_green();
			break;
		case Purple:
			RGB_off_all();
			RGB_on_red();
			RGB_on_blue();
			break;
		case Yellow:
			RGB_off_all();
			RGB_on_red();
			RGB_on_green();
			break;
		case Cyan:
			RGB_off_all();
			RGB_on_blue();
			RGB_on_green();
			break;
		case White:
			RGB_on_red();
			RGB_on_blue();
			RGB_on_green();
			break;
		case Black:
			RGB_off_all();
			break;
		default:
			RGB_off_all();
	}
}

