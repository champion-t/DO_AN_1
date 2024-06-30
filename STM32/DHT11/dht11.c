#include "dht11.h"

void Init(Dht11* dht11, TIM_HandleTypeDef* htim, GPIO_TypeDef* port, uint16_t pin)
{
	dht11->htim = htim;
	dht11->port = port;
	dht11->pin = pin;
}

void Delay_us(TIM_HandleTypeDef* htim, uint16_t us)
{
	htim->Instance->CNT = 0;
	HAL_TIM_Base_Start(htim);
	while(htim->Instance->CNT < us);
	HAL_TIM_Base_Stop(htim);
}

//void Delay_ms(TIM_HandleTypeDef* htim, uint16_t ms)
//{
//	for(int i = 0; i < ms; i++){
//		Delay_us(htim, 1000);
//	}
//}

void Set_Pin_Mode(Dht11* dht11, uint8_t pMode)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	if(pMode == OUTPUT)
	{
		GPIO_InitStructure.Pin = dht11->pin;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(dht11->port, &GPIO_InitStructure);
	}
	else if(pMode == INPUT)
	{
		GPIO_InitStructure.Pin = dht11->pin;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(dht11->port, &GPIO_InitStructure);
	}
}

void DHT_Start(Dht11* dht11)
{
	Set_Pin_Mode(dht11, OUTPUT);
	HAL_GPIO_WritePin(dht11->port, dht11->pin, 1);
	HAL_GPIO_WritePin(dht11->port, dht11->pin, 0);
	HAL_Delay(20);
	HAL_GPIO_WritePin(dht11->port, dht11->pin, 1);
	Delay_us(dht11->htim, 30);
	Set_Pin_Mode(dht11, INPUT);
}

void DHT_Check_Response(Dht11* dht11)
{
	while(HAL_GPIO_ReadPin(dht11->port, dht11->pin) != 0);
	while(HAL_GPIO_ReadPin(dht11->port, dht11->pin) == 0);
	while(HAL_GPIO_ReadPin(dht11->port, dht11->pin) != 0);
}

uint8_t DHT_Read(Dht11* dht11)
{
	uint8_t i,j;
	for(j = 0; j < 9; j++)
	{
		while(HAL_GPIO_ReadPin(dht11->port, dht11->pin) == 0);
		Delay_us(dht11->htim, 50);
		if(!(HAL_GPIO_ReadPin(dht11->port, dht11->pin)))
		{
			i&=~(1<<(7-j));
		}
		else
		{
			i|=(1<<(7-j));
		}
		while(HAL_GPIO_ReadPin(dht11->port, dht11->pin) != 0);
	}
	return i;
}

uint8_t DHT_GetData(Dht11* dht11)
{
	dht11->data[0] = DHT_Read(dht11);
	dht11->data[1] = DHT_Read(dht11);
	dht11->data[2] = DHT_Read(dht11);
	dht11->data[3] = DHT_Read(dht11);
	dht11->data[4] = DHT_Read(dht11);
	return 1;
}