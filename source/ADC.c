///*
// * ADC.c
// *
// *  Created on: 4 abr. 2025
// *      Author: sebal
// */
#include <stdint.h>
#include "ADC.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adc16.h"
#include "fsl_debug_console.h"

void ADC_init_P()
{
    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC16_BASE, false);
    // Configurar canal
    adc16ChannelConfigStruct.channelNumber                        = ADC16_Pulso;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
}

void ADC_init_T()
{
    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;


    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(ADC16_BASE, false);
    // Configurar canal
    adc16ChannelConfigStruct.channelNumber                        = ADC16_Temp;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;
    ADC16_SetChannelConfig(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
}

uint32_t ADC_to_Pulsos(uint32_t adc_value)
{
    return (adc_value * 3300) / 4095;
}

uint32_t ADC_to_Temp(uint32_t adc_value)
{
    return adc_value / 70;
}


adc_opciones ADC_Pc_Temp(void)
{
	adc_opciones opciones;
    adc16_channel_config_t adc16ChannelConfigStruct;

    // Leer Pulso
    adc16ChannelConfigStruct.channelNumber = ADC16_Pulso;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    ADC16_SetChannelConfig(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (!(kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)));
    opciones.ADC_pulso = ADC16_GetChannelConversionValue(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    opciones.real_pulso = ADC_to_Pulsos(opciones.ADC_pulso);

    // Leer temperatura
	adc16ChannelConfigStruct.channelNumber = ADC16_Temp;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
	ADC16_SetChannelConfig(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
	while (!(kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)));
	opciones.ADC_temperatura = ADC16_GetChannelConversionValue(ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
	opciones.real_temperatura = ADC_to_Temp(opciones.ADC_temperatura);

    return opciones;
}
