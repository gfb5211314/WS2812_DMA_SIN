#include     "bsp_gpio.h"
#include "stm32f1xx_hal.h"





/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
 void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BAT_LED_Pin|AC_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DET_DC_Pin */
  GPIO_InitStruct.Pin = DET_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DET_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BAT_LED_Pin AC_LED_Pin */
  GPIO_InitStruct.Pin = BAT_LED_Pin|AC_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


