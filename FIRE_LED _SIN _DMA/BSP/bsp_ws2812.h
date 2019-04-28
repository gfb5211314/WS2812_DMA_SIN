#ifndef   __BSP_WS2812_H__
#define   __BSP_WS2812_H__


#include "stm32f1xx_hal.h"

#define  ledmax          66  
#define lamp_bead_amount           15          //lamp bead amount 
typedef  struct
{
    uint8_t  gree ;
    uint8_t red;
    uint8_t blue;
} color_type;


typedef enum
{  
	  w28_idle=0,
    w28_start_reset=1,
    w28_reset_check=2,
	  w28_data_update=3,
	  w28_end_reset=4,
	 
	 
} ws128_state_type;
typedef enum
{  
      w28_start=5,
	    w28_check_state=6
	 
} ws128_ctrl_type;


/**
  * @brief  Color selection enumeration
  */
typedef enum
{
    white = 0,
    green,
    red,
    blue,
    mie,
    yellow,
    blue_green,

} colors_kind;



void  led_run(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm);






/**
 * @brief  Control single LED flashing.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @param  time    :    Flicker time size(ms)
 * @retval None
 */
void  led_runing(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time);

/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @retval None
 */

void  led_open(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm);

    





   


/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @param  time   :    Turn on the light up time(ms)
 * @retval None
 */

void  led_opening(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time);





/**
 * @brief  Light color type.
 * @param  amount :   Maximum number of lighting leds
 * @param  p      :    Color combination array
 * @retval None
 */
void  led_type(volatile uint16_t  amount,  color_type* p);








#endif

