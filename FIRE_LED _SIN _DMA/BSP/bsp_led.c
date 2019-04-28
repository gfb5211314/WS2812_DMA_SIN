
/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  FuBiao.Guo
  * @version V1.1.0
  * @date    25-2-2019
  * @brief   Lamp underlying driver function file.
	**/


#include "stm32f1xx_hal.h"
#include "bsp_usart.h"
#include "bsp_led.h"


uint8_t   Read_det_Value()
{
	
	  return HAL_GPIO_ReadPin(DET_DC_GPIO_Port, DET_DC_Pin);
}


void AC_LED_OPEN()
{
	
	  HAL_GPIO_WritePin(AC_LED_GPIO_Port,AC_LED_Pin, GPIO_PIN_RESET);
	
}


void AC_LED_CLOSE()
{
	
	  HAL_GPIO_WritePin(AC_LED_GPIO_Port,AC_LED_Pin, GPIO_PIN_SET);
	

}
 void DC_LED_OPEN()
{
	
	  HAL_GPIO_WritePin(BAT_LED_GPIO_Port,BAT_LED_Pin, GPIO_PIN_RESET);
	
}


void DC_LED_CLOSE()
{
	
	  HAL_GPIO_WritePin(BAT_LED_GPIO_Port,BAT_LED_Pin, GPIO_PIN_SET);
}


