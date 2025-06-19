/*
 * ADC.h
 *
 *  Created on: 4 abr. 2025
 *      Author: sebal
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC16_BASE          ADC1
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define ADC16_Pulso 23U
#define ADC16_Temp 18U

void ADC_init_P();
void ADC_init_T();

typedef struct {
    uint32_t ADC_pulso;
    uint32_t real_pulso;
    uint32_t ADC_temperatura;
    uint32_t real_temperatura;
}adc_opciones;

uint32_t ADC_to_Pulsos(uint32_t adc_value);
uint32_t ADC_to_Temp(uint32_t adc_value);


adc_opciones ADC_Pc_Temp(void);


#endif /* ADC_H_ */
