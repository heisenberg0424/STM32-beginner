#include "stm32f10x.h"
#include "Delay.h"

int main(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    int i=0;
    int port_val = 0x1;
    int delay = 300;
    
    while(1) {
        port_val = 0x1;
        for (i=0;i<8;i++) {
            GPIO_Write(GPIOA, ~port_val);
            Delay_ms(delay);
            port_val = port_val | port_val << 1;
        }   
    }
}
