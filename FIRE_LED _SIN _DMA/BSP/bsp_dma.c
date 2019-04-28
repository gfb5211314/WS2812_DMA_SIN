#include   "bsp_dma.h"
#include "stm32f1xx_hal.h"



/** 
  * Enable DMA controller clock
  */
 void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

}