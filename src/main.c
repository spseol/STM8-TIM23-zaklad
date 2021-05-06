#include "stm8s.h"
#include <stdio.h>

#include "milis.h"
#include "keypad.h"

#define LEDR_PORT GPIOD
#define LEDR_PIN  GPIO_PIN_4
#define LEDG_PORT GPIOD
#define LEDG_PIN  GPIO_PIN_3
#define LEDB_PORT GPIOA
#define LEDB_PIN  GPIO_PIN_3


#define LED_ON(COLOR)   GPIO_WriteHigh(LED##COLOR##_PORT, LED##COLOR##_PIN);
#define LED_OFF(COLOR)  GPIO_WriteLow(LED##COLOR##_PORT, LED##COLOR##_PIN);
#define LED_REVERSE(COLOR) GPIO_WriteReverse(LED##COLOR##_PORT, LED##COLOR##_PIN);

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 


/*----------------------------------------------------------------------------------------------*/



void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovat MCU na 16MHz

    init_milis();
    init_keypad();

    /*----          GPIO setup           ---------*/
    GPIO_Init(LEDR_PORT, LEDR_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDG_PORT, LEDG_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDB_PORT, LEDB_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    /*----           TIM2 setup           ---------*/
    TIM2_TimeBaseInit(TIM2_PRESCALER_512, 31250 - 1 );
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);
    /*----           TIM3 setup           ---------*/
    TIM3_TimeBaseInit(TIM3_PRESCALER_512, 12345 - 1 );
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    TIM3_Cmd(ENABLE);

}

int main(void)
{
    int32_t time_led = 0;
    setup();

  /*------  nekonená smyčka    ---*/
    while (1) {
        if (milis() - time_led > 200) {
            time_led = milis();
            LED_REVERSE(R);  // červenou měním hlvním programu
        }
    }
}


INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    TIM2_ClearFlag(TIM2_FLAG_UPDATE); 
    LED_REVERSE(B);  // modrou měním z TIM2
}

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
    TIM3_ClearFlag(TIM3_FLAG_UPDATE); 
    LED_REVERSE(G); // zelenou měním pomocí TIM3
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
