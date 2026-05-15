#include "stm32f10x.h"
#include "Delay.h"

void KEY_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t KEY_pressed(void) {
    uint8_t pressed = 0;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0 ){
        Delay_ms(20);
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0) {}    
        Delay_ms(20);
        pressed = 1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0 ){
        Delay_ms(20);
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {}    
        Delay_ms(20);
        pressed = 2;
    }
    return pressed;
}
