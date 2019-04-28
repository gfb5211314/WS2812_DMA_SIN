#include   "bsp_ws2812.h"
#include "stm32f1xx_hal.h"
#include  "math.h"
#include   "bsp_dma.h"
#include "string.h"
extern TIM_HandleTypeDef htim1;






/****  Macro definition parameter  ****************/

#define BIT_1                      61u //1比较值为61 850us
#define BIT_0                      29u //0比较值为28 400us 
#define  LED_NUM                 10
#define  LED_NUM_MAX                6000
/* Private variables ---------------------------------------------------------*/
uint16_t ab[LED_NUM];
uint16_t reset_buf[200];
color_type    color_buf[35] = { {0xff, 0x00, 0x00}, //绿色
    {0x00, 0xff, 0x00}, //红色
    {0x00, 0x00, 0xff}, //蓝色
    {255, 255, 31}, //黄色
    {238, 118, 198},
    { 255, 84,  159},
    { 191,  0, 255},
    { 164, 244,  96},
    {0, 51, 51}
};
color_type    color_mie[35] = { 0 };


extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_tim1_ch1;
#define  reset_max        50

uint8_t    ws2812_state = 0;
uint16_t   ws28128_reset_buf[50] = {0}; //62.5us      大于50us低电平复位
uint16_t   ws28128_end_reset_buf[50] = {0};
uint16_t   ws28128_mie_buf[48] = {28};
uint8_t   ws28128_color_buf[LED_NUM_MAX][3] =
{
    {0, 2, 0},
    {0, 2, 0},
    {2, 0, 0},
    {2, 0, 0},
    {0, 2, 0},
    {0, 2, 0},
    {2, 0, 0},
    {2, 0, 0},
    {0, 2, 0},
    {0, 2, 0},
    {0, 0, 2},
    {255, 20, 147},
    {255, 255, 31},
    {0, 0, 255},
    {220, 20, 60}

};
//接收颜色buf
uint16_t   ws28128_color_data[48] = {30, 30, 30, 30, 30, 30, 30, 30,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     60, 60, 60, 60, 60, 60, 60, 60,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     30, 30, 30, 30, 30, 30, 30, 30,
                                     60, 60, 60, 60, 60, 60, 60, 60,


                                    };   //PWM数据


static uint16_t	 WS2812_LED_Index;
uint8_t   Shift_mode = 0;
/*******************/
uint8_t   reset_flag = 0;
uint8_t   send_flag = 0;
uint8_t  updata_flag = 0;
uint8_t   mie_flag = 0;
uint16_t   mie_led_count;
uint16_t   mie_led_max;
volatile uint8_t half_tx ;
volatile uint8_t full_tx ;
volatile uint8_t reset_end_flag = 0;
extern   uint8_t  systick_500ms_flag;
extern uint16_t  k3;

uint16_t light_led_max;
uint8_t  light_flag;
uint16_t light_led_count;
/******注册完成回调不行************/
/*DMA传输一半回调*/
uint8_t  k1;

void DMA_HalfCpltCallback(DMA_HandleTypeDef* _hdma)
{
//      half_tx=1;





}

/*DMA传输完成回调*/
void DMA_CpltCallback(DMA_HandleTypeDef* _hdma)
{
//              full_tx=1;


}
/*注册DMA传输一半回调中断函数**********/
void DMA_RegisterHalfCallback()
{
    HAL_DMA_RegisterCallback(&hdma_tim1_ch1, HAL_DMA_XFER_HALFCPLT_CB_ID, DMA_HalfCpltCallback);
}
/*注册DMA传输完成回调中断函数**********/

void DMA_RegisterCpltCallback()
{
    HAL_DMA_RegisterCallback(&hdma_tim1_ch1, HAL_DMA_XFER_CPLT_CB_ID, DMA_CpltCallback);
}
void DMA_Callback_init()
{
    DMA_RegisterHalfCallback();
    DMA_RegisterCpltCallback();

}


void  RGBTOGRB(uint16_t count, uint8_t p[][3])
{
    uint8_t tem_data;
    for(uint16_t i = 0; i < count; i++)
    {
        tem_data = p[i][0];;
        p[i][0] = p[i][1];
        p[i][1] = tem_data;
        p[i][2] = p[i][2];


    }



}

void shift_buf_to_data_one(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
    gree_data = ws28128_color_buf[led_location][1];
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 0; i < 24; i++)
    {
        if(i < 8)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((7 < i) && (i < 16))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}
void shift_buf_to_data_two(uint16_t led_location)
{
    uint16_t gree_data, red_data, blue_data;
    gree_data = ws28128_color_buf[led_location][1];
    red_data = ws28128_color_buf[led_location][0];
    blue_data = ws28128_color_buf[led_location][2];
    for(uint16_t i = 24; i < 48; i++)
    {
        if(i < 32)
        {

            if(gree_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            gree_data = gree_data << 1;

        }
        else if((31 < i) && (i < 40))
        {

            if(red_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            red_data = red_data << 1;


        }
        else
        {
            if(blue_data & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            blue_data = blue_data << 1;


        }


    }

}
void shifting_GRBTOPWM(uint16_t led_location)
{

    for(uint16_t i = 0; i < 24; i++)
    {
        if(i < 8)
        {

            if(ws28128_color_buf[led_location][1] & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            ws28128_color_buf[led_location][1] = ws28128_color_buf[led_location][1] << 1;

        }
        else if((7 < i) && (i < 16))
        {

            if(ws28128_color_buf[led_location][0] & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            ws28128_color_buf[led_location][0] = ws28128_color_buf[led_location][0] << 1;


        }
        else
        {
            if(ws28128_color_buf[led_location][2] & 0x80)
            {
                ws28128_color_data[i] = BIT_1; //1

            }
            else
            {
                ws28128_color_data[i] = BIT_0; //0
            }
            ws28128_color_buf[led_location][2] = ws28128_color_buf[led_location][2] << 1;


        }


    }

}

void GRBTOPWM(uint16_t led_max)
{
    uint8_t    gree_data;
    uint8_t    blue_data;
    uint8_t    red_data;
    for(uint16_t j = 0; j < led_max; j++)
    {
        gree_data = ws28128_color_buf[j][1];
        red_data = ws28128_color_buf[j][0];
        blue_data = ws28128_color_buf[j][2];
        for(uint16_t i = 0; i < 24; i++)
        {

            if(i < 8)
            {

                if(gree_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                gree_data = gree_data << 1;

            }
            else if((7 < i) && (i < 16))
            {

                if(red_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                red_data = red_data << 1;


            }
            else
            {
                if(blue_data & 0x80)
                {
                    ws28128_color_data[i + (j * 24)] = BIT_1; //1

                }
                else
                {
                    ws28128_color_data[i + (j * 24)] = BIT_0; //0
                }
                blue_data = blue_data << 1;


            }


        }
    }
}
void ws2812_init()
{


}
uint8_t dma_mode;

void  SET_PWM_DMA_DODE(uint32_t mode)
{

    __HAL_RCC_TIM1_CLK_ENABLE();


    __HAL_RCC_TIM1_CLK_ENABLE();

    /* TIM1 DMA Init */
    /* TIM1_CH1 Init */
    hdma_tim1_ch1.Instance = DMA1_Channel2;
    hdma_tim1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim1_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim1_ch1.Init.Mode = mode;
    hdma_tim1_ch1.Init.Priority = DMA_PRIORITY_HIGH;
    if(HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);
    /* Prepare the DMA Channel configuration */


    /* Write to DMA Channel CR register */


}



/****DMA-PWM*******************/
uint8_t  led_index;
void  Dma_Pwm_Run_ONE()
{
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 24);

}
void  ws28128_rest()
{
    /***w2812reset***/

    full_tx = 0;
    half_tx = 0;

    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    SET_PWM_DMA_DODE(DMA_NORMAL);

    reset_flag = 1;
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, reset_max);

}


void  ws2812_run(uint16_t led_n)
{
    light_led_max = led_n;
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    GRBTOPWM(2);
    send_flag = 1;
    k3 = 1;
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);
}


void  ws28128_mie()
{
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_mie_buf, 24);

}
/**
 * @brief    A byte cache drives the LED.
 * @param  amount :   Maximum number of lighting leds
 * @param  cmd  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @retval None
 */
/***eeor function************/
ws128_state_type  Dma_Pwm_Run_demo(ws128_ctrl_type sta)
{
    static   ws128_state_type  state = w28_idle;
    if(sta == w28_start)
    {

        state = w28_start_reset;
//		  printf("222");
    }

    switch(state)
    {

    /* First Reseting Process */
    case w28_start_reset :
//			 printf("w28_start_reset\r\n");
        __HAL_RCC_DMA1_CLK_DISABLE() ;
        HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//		      __HAL_TIM_DISABLE(&htim1);
//		 __HAL_TIM_SET_COUNTER(&htim1,0);
        __HAL_RCC_DMA1_CLK_ENABLE();
        SET_PWM_DMA_DODE(DMA_NORMAL);
        /* Reset RAW Buffer to send out reset signals */
        memset(ws28128_reset_buf, 0, sizeof(ws28128_reset_buf));
        /* Configure DMA - Source Address, Mode, Data Length */
//		  __HAL_TIM_ENABLE(&htim1);

        HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, 50);
        /* Enable DMA and Timer/Channel */
        half_tx  = 0;
        full_tx  = 0;


        /* Check Reset Status */
        state = w28_reset_check;

        break;

    /* Check the first reset progress */
    case w28_reset_check :

        if(full_tx)
        {
//            printf("w28_reset_check\r\n");
            full_tx = 0;
            half_tx = 0;
            WS2812_LED_Index = 0;

            /* Re-configure DMA */
            __HAL_RCC_DMA1_CLK_DISABLE() ;
            HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//				        __HAL_TIM_DISABLE(&htim1);
//				        __HAL_TIM_SET_COUNTER(&htim1,0);
            __HAL_RCC_DMA1_CLK_ENABLE();
//				             __HAL_TIM_ENABLE(&htim1);
            SET_PWM_DMA_DODE(DMA_CIRCULAR);
            DMA_Callback_init();
            __HAL_DMA_ENABLE_IT(&hdma_tim1_ch1, DMA_IT_HT);

            HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);



            state = w28_data_update;
        }

        break;

    /* Enter Updating Progress */
    case w28_data_update:

        if(full_tx || half_tx)
        {
//             printf("w28_data_update\r\n");
            if(full_tx) full_tx = 0;
            if(half_tx) half_tx = 0;
            WS2812_LED_Index++; /* Next LED Data is sending */
            GRBTOPWM(2);
            send_flag = 1;
            k3 = 1;
            if(WS2812_LED_Index < lamp_bead_amount)
            {
                if((WS2812_LED_Index % 2) == 0)
                {
                    shift_buf_to_data_one(WS2812_LED_Index);

                }
                else
                {
                    shift_buf_to_data_two(WS2812_LED_Index);
                }
            }
            else
            {
                /**
                 * IMPORTANT!
                 * - We are supposed to wait PWM to generate the last pulse.
                 * - The last pulse can be sent out after MCU finishes following procedure.
                 */

                /* All LEDs have been updated, sending out reset signal */

                /* Disable DMA firstly, and then write new configurations */
//          printf("wanc\r\n");
                __HAL_RCC_DMA1_CLK_DISABLE() ;
                HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
//		      __HAL_TIM_DISABLE(&htim1);
//		   __HAL_TIM_SET_COUNTER(&htim1,0);

                WS2812_LED_Index = 0;
                full_tx = 0;
                half_tx = 0;

                /* Reset buffer to generate RESET PWM signals */
                memset(ws28128_reset_buf, 0, sizeof(ws28128_reset_buf));

                /* Configure DMA - Source Address, Mode, Data Length */
                __HAL_RCC_DMA1_CLK_ENABLE();
                SET_PWM_DMA_DODE(DMA_NORMAL);

                __HAL_TIM_ENABLE(&htim1);

                __HAL_DMA_DISABLE_IT(&hdma_tim1_ch1, DMA_IT_HT);

                /* Enable Channel to send out data flow */
                HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);

                state = w28_end_reset;
            }
        }
//


    /* Last Reset */
    case w28_end_reset:
        if(full_tx)
        {
//        printf("w28_end_reset\r\n");
            __HAL_RCC_DMA1_CLK_DISABLE() ;
            HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
            __HAL_TIM_DISABLE(&htim1);
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            full_tx = 0;
            state = w28_idle;
        }
        break;

    case w28_idle:
//			printf("w28_idle\r\n");
        break;

    default:
        break;
    }

    return state;
}


/********************************************************************
       以下函数采用半传输中断更新数据，优化的底层函数和应用函数
                    BOTTOM function
               *DMA_WS2812_Reset              ws2812复位
               *DMA_WS2812_light              ws2812亮
               *DMA_WS2812_Mie                ws2812灭
               *DMA_WS2812_SIN                正弦函数
                     APP
               *DMA_WS2812_Ramp                渐变效果一会灭一会亮,采用正弦
               *DMA_WS2812_Run                 依次亮(每个都亮，每个灯亮度不同)
               *DMA_WS2812_Running             几个跑(亮度相同,几个跑起来)
               *DMA_WS2812_Ramping             同一类颜色渐变 (深红,浅红)

**********************************************************************/
/********复位************/
void DMA_WS2812_Reset()
{
    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_NORMAL);
    reset_flag = 1;
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, reset_max);

}
void DMA_WS2812_light(uint16_t led_n)
{
    light_led_max = led_n;
    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    GRBTOPWM(2);
    light_flag = 1;
    light_led_count = 1;
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_color_data, 48);
}
void DMA_WS2812_Mie(uint16_t led_n)
{
    mie_led_max = led_n;
    __HAL_RCC_DMA1_CLK_DISABLE();
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    __HAL_RCC_DMA1_CLK_ENABLE();
    SET_PWM_DMA_DODE(DMA_CIRCULAR);
    DMA_Callback_init();
    mie_flag = 1;
    mie_led_count = 0;
    for(uint16_t i = 0; i < 48; i++)
    {
        ws28128_mie_buf[i] = 29;
    }
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_mie_buf, 48);
}
/**********数据的值*****************/
void DMA_WS2812_SIN(uint16_t amount, uint8_t pwm, colors_kind color)
{
    uint8_t     sin_y = 0;
//    printf("START");

    uint8_t point;
    float sin_x;  //弧度
    float sin_jiao;//角度 分度角
    sin_jiao = 360.000 / 255;
    point = pwm;

    sin_x = sin_jiao * pwm; //得到角度值
    sin_x = sin_x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
    sin_y = 128 * sin(sin_x + 4.75) + 128;

    for(uint16_t j = 0; j < amount; j++)
    {


        if(color == green)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = sin_y; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == red)
        {
            ws28128_color_buf[j][0] = sin_y; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = 0;	 //blue
        }
        else if(color == blue)
        {
            ws28128_color_buf[j][0] = 0; //red
            ws28128_color_buf[j][1] = 0; //gree
            ws28128_color_buf[j][2] = sin_y;	 //blue
        }
//         else if(color == yellow)
//        {
//            ws28128_color_buf[j][0] = sin_y; //red
//            ws28128_color_buf[j][1] = sin_y; //gree
//            ws28128_color_buf[j][2] = 0;	 //blue
//        }
    }

}
/***数据移位************/
void DMA_WS2812_data_shift(uint16_t led_location)
{
	
	    memset(ws28128_color_buf, 0, sizeof(ws28128_color_buf));
	          ws28128_color_buf[led_location+1][0] = 255; //gree
          	ws28128_color_buf[led_location+2][1] = 255; //red
            ws28128_color_buf[led_location][2] = 255;	 //blue
	          ws28128_color_buf[led_location+3][0] = 255;	 //blue
	          ws28128_color_buf[led_location+3][1] = 255;	 //blue
           
	        
       
	
}
void DMA_WS2812_Ramp(volatile uint16_t  amount, uint8_t pwm, colors_kind color)
{
    for(uint16_t m = 0; m < pwm; m++)
    {

        DMA_WS2812_SIN(amount, m, color);
        DMA_WS2812_Reset();
        HAL_Delay(10);
        DMA_WS2812_light(amount);
        HAL_Delay(10);


    }

}

void DMA_WS2812_Running(volatile uint16_t  amount)
{
	 for(uint16_t m = 0; m < amount; m++)
    {

       DMA_WS2812_data_shift(m);
        DMA_WS2812_Reset();
        HAL_Delay(10);
        DMA_WS2812_light(amount);
        HAL_Delay(30);
		}



}	
/********渐变*************/
void  w28128_runing(volatile uint16_t  amount, uint8_t pwm)
{

    uint8_t     y = 0;
//    printf("START");
    uint8_t i = 0;
    uint8_t point;
    float x;  //弧度
    float jiao;//角度 分度角
    jiao = 360.000 / pwm;
    point = pwm;

    for(i = 0; i < point; i++)
    {
        x = jiao * i; //得到角度值
        x = x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
        y = 128 * sin(x + 4.75) + 128;



        ws2812_run(0);
        for(uint16_t j = 0; j < amount; j++)
        {
            for(uint16_t k = 0; k < 3; k++)
            {
                ws28128_color_buf[j][k] = y;
            }

        }

    }



}
///**********常灭********************/

//void HAL_SYSTICK_Callback(void)
//{
//
//
//
//}
/********渐变*************/
void  app_runing(volatile uint16_t  amount, colors_kind  color, uint8_t pwm, uint8_t flag, uint32_t time)
{

    uint8_t     sin_y = 0;
//    printf("START");

    uint8_t point;
    float sin_x;  //弧度
    float sin_jiao;//角度 分度角
    sin_jiao = 360.000 / pwm;
    point = pwm;

// for(uint8_t i=0;i<pwm;i++)
//{
    sin_x = sin_jiao * 10; //得到角度值
    sin_x = sin_x * 0.01744; //角度转弧度 ?弧度=角度*（π/180）
    sin_y = 128 * sin(sin_x + 4.75) + 128;


    printf("i=%d-sin_y=%d\r\n", pwm, sin_y);



//        led_run(amount, color, y);

//				    if(Read_det_Value()==flag)
//						{
//
//						}
//						else
//						{
//				    break;
//						}
    HAL_Delay(time);
//}



}
/**
 * @brief  Control the color of single LED.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @retval None
 */
void  led_run(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm)
{
    uint16_t  count = 0;
    volatile	uint16_t    amount_size = 0;
    uint16_t   LED_count = 0;
    volatile    uint8_t  pwm_1 = 0;
    amount_size = amount * 24 + 1;
    pwm_1 = pwm;

    for(LED_count = 0; LED_count < amount; LED_count++)
    {

        pwm = pwm_1;
        for(count = 0; count < 24; count++)
        {
            //绿色
            if(color == 1)
            {
                if(count < 8)
                {

                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;

                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_0;


                }
            }
            else if(color == 2)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = BIT_0;

                }
                else if((7 < count) && (count < 16))
                {
                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;
                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_0; //0

                }
            }
            else if(color == 3)
            {
                if(count < 16)
                {

                    ab[(count + (LED_count * 24))] = BIT_0; //0

                }
                else
                {


                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;

                }



            }
            else if(color == 4)
            {

                ab[(count + (LED_count * 24))] = BIT_0; //0








            }
        }
        if(LED_count == (amount - 1))
        {
            ab[amount * 24] = 0;
        }
    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}




/**
 * @brief  Control single LED flashing.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    The brightness of the light (1-256)
 * @param  time    :    Flicker time size(ms)
 * @retval None
 */
void  led_runing(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time)
{
    led_run(amount, 4, pwm);
    HAL_Delay(time);
    led_run(amount, color, pwm);
    HAL_Delay(time);


}
/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @retval None
 */

void  led_open(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm)
{
    uint16_t  count = 0;
    volatile	uint16_t    amount_size = 0;
    uint8_t  bright;
    amount_size = amount * 24 + 1;
    bright = (pwm / amount);
    uint16_t   LED_count = 0;
    volatile    uint8_t  pwm_1 = 0;
    pwm_1 = pwm;

    for(LED_count = 0; LED_count < amount; LED_count++)
    {

        pwm = bright * (LED_count + 1);
        for(count = 0; count < 24; count++)
        {
            //绿色
            if(color == 1)
            {
                if(count < 8)
                {

                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;

                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_0;


                }
            }
            //红色
            else if(color == 2)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = BIT_0;

                }
                else if((7 < count) && (count < 16))
                {
                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;
                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_0; //0

                }
            }
            //蓝色
            else if(color == 3)
            {
                if(count < 16)
                {

                    ab[(count + (LED_count * 24))] = BIT_0; //0

                }
                else
                {


                    if(pwm & 0x80)
                    {
                        ab[(count + (LED_count * 24))] = BIT_1; //1

                    }
                    else
                    {
                        ab[(count + (LED_count * 24))] = BIT_0; //0
                    }
                    pwm = pwm << 1;

                }



            }
            //灭灯
            else if(color == 4)
            {

                ab[(count + (LED_count * 24))] = BIT_0; //0








            }
            //蓝绿色
            else if(color == 5)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = BIT_1; //1

                }
                else if((7 < count) && (count < 16))
                {

                    ab[(count + (LED_count * 24))] = BIT_0; //0


                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_1; //255

                }
            }
            //黄色
            else if(color == 6)
            {

                if(count < 8)
                {
                    ab[(count + (LED_count * 24))] = BIT_1; //

                }
                else if((7 < count) && (count < 16))
                {

                    ab[(count + (LED_count * 24))] = BIT_1; //


                }
                else
                {


                    ab[(count + (LED_count * 24))] = BIT_0; //

                }
            }

        }
        if(LED_count == (amount - 1))
        {
            ab[amount * 24 + 1] = 0;
        }
    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}


/**
 * @brief  Light up the led in turn, and the color gradually brightens.
 * @param  amount :   Maximum number of lighting leds
 * @param  color  :    The color of the light
 * @param  pwm    :    Brightness level (1-256)
 * @param  time   :    Turn on the light up time(ms)
 * @retval None
 */

void  led_opening(volatile uint16_t  amount, colors_kind  color, volatile uint8_t pwm, uint32_t time)
{

    for(uint16_t i = 1; i < (amount + 1); i++)
    {
        HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ws28128_reset_buf, 200);
        HAL_Delay(1);
        led_open(i, color, 250);
        HAL_Delay(time);
    }

}




/**
 * @brief  Light color type.
 * @param  amount :   Maximum number of lighting leds
 * @param  p      :    Color combination array
 * @retval None
 */
void  led_type(volatile uint16_t  amount,  color_type* p)
{

    uint16_t amount_size = 0;
    uint16_t n = 0;
    color_type p_tem[amount];

    for(n = 0; n < amount; n++)
    {
        p_tem[n].gree = p[n].gree;
        p_tem[n].red = p[n].red;
        p_tem[n].blue = p[n].blue;


    }
    amount_size = amount * 24 + 1;

    for(uint16_t i = 0; i < amount; i++)
    {
//			 p[i].greed=0;
//			 p[i].red=0;
//			 p[i].blue=0;
        printf("%d.gree=%d\r\n", i, p[i].gree);
        printf("%d.red=%d\r\n", i, p[i].red);
        printf("%d.blue=%d\r\n", i, p[i].blue);
        for(uint16_t j = 0; j < 24; j++)
        {
            if(j < 8)
            {
                if(p_tem[i].gree & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].gree = p_tem[i].gree << 1;
            }
            else if((7 < j) && (j < 16))
            {
                if(p_tem[i].red & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].red = p_tem[i].red << 1;


            }
            else
            {
                if(p_tem[i].blue & 0x80)
                {
                    ab[(j + (i * 24))] = 60; //1

                }
                else
                {
                    ab[(j + (i * 24))] = 30; //0
                }
                p_tem[i].blue = p_tem[i].blue << 1;

            }
        }


        if(i == (amount - 1))
        {
            ab[amount * 24] = 0;
        }


    }

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)ab, amount_size);

}