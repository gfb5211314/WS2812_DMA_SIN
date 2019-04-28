/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
#include   "bsp_ws2812.h"

extern volatile uint8_t  reset_end_flag;
extern volatile uint8_t  reset_flag;
extern volatile uint8_t  send_flag;
 uint16_t  k3;
extern  uint16_t k2;
 uint16_t   k;
extern TIM_HandleTypeDef htim1;
uint16_t  systick_count=0;
uint8_t  systick_500ms_flag=0;
 extern uint8_t half_tx;
 extern uint8_t full_tx;
 extern uint8_t updata_flag;
 uint8_t idle_flag; 
 extern uint8_t refresh_rdy;
 
 extern uint8_t mie_flag;
 extern uint16_t mie_led_max;
 extern uint16_t mie_led_count;
 
 extern uint8_t light_flag;
 extern uint16_t light_led_max;
 extern uint16_t light_led_count;
  extern TIM_HandleTypeDef htim1;
 
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_tim1_ch1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
//  if(updata_flag==1)
//	{
  systick_count++;
	if(systick_count>500)
	{
		systick_count=0;
		updata_flag=0;
		systick_500ms_flag=1;


	
//	}
  }
	 	
	  
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles DMA1 channel2 global interrupt.
*/
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

		  /* Clear the half transfer complete flag */
  if(__HAL_DMA_GET_FLAG(&hdma_tim1_ch1, DMA_FLAG_HT2) )
	{
		   
		k=__HAL_DMA_GET_COUNTER(&hdma_tim1_ch1);
		 __HAL_DMA_CLEAR_FLAG(&hdma_tim1_ch1, DMA_FLAG_HT2);
		k2++;
		           half_tx=1;

		
	}
 if(__HAL_DMA_GET_FLAG(&hdma_tim1_ch1, DMA_FLAG_TC2) )
	{
	 		
	
               full_tx=1;
		 __HAL_DMA_CLEAR_FLAG(&hdma_tim1_ch1, DMA_FLAG_TC2);
	
	}
      if(__HAL_DMA_GET_FLAG(&hdma_tim1_ch1, DMA_FLAG_TE2) )
	{
		   

		 __HAL_DMA_CLEAR_FLAG(&hdma_tim1_ch1, DMA_FLAG_TE2);
		
	}  	
	      if(__HAL_DMA_GET_FLAG(&hdma_tim1_ch1, DMA_FLAG_GL2) )
	{
		   

		 __HAL_DMA_CLEAR_FLAG(&hdma_tim1_ch1, DMA_FLAG_GL2);
		
	} 

	
		
  			 if(reset_flag==1)
	{
		   reset_flag=0;
					
	  	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	
	
	} 
if(light_flag==1)
	 {		 
		 			 
             light_led_count++; 
		 		if(light_led_count>light_led_max)
				{
		
					light_flag=0;
				 light_led_count=0;
			
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1); 
//			  __HAL_RCC_DMA1_CLK_DISABLE();   £¨ÎÞÊýLDEmie£©
			
				}
		   
		       if((light_led_count%2)==0)
					 {
						shift_buf_to_data_one(light_led_count);
						 
					 }
					 else
					 {
						shift_buf_to_data_two(light_led_count); 
					 }
      
		
	  }

		if((mie_flag==1)&&(light_flag==0))
   {
   	    
	      	mie_led_count++;
		  if(mie_led_count>(mie_led_max-1))
			{
				mie_flag=0;
				mie_led_count=0;
			
		 HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
			
				
			}
			
				
		} 
 
  /* USER CODE END DMA1_Channel2_IRQn 0 */
// HAL_DMA_IRQHandler(&hdma_tim1_ch1);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */
	
  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
