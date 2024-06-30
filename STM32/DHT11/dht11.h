#ifndef DHT11_H
#define DHT11_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_gpio.h"

#define OUTPUT 1
#define INPUT 0

struct Dht11
{
	GPIO_TypeDef* port;
	uint16_t pin;
	TIM_HandleTypeDef* htim;
	uint8_t data[5];
};
typedef struct Dht11 Dht11;

void Init(Dht11* dht11, TIM_HandleTypeDef* htim, GPIO_TypeDef* port, uint16_t pin);
void Delay_us(TIM_HandleTypeDef* htim, uint16_t us);
void Delay_ms(TIM_HandleTypeDef* htim, uint16_t ms);
void Set_Pin_Mode(Dht11* dht11, uint8_t pMode);
void DHT_Start(Dht11* dht11);
void DHT_Check_Response(Dht11* dht11);
uint8_t DHT_Read(Dht11* dht11);
uint8_t DHT_GetData(Dht11* dht11);

#endif
