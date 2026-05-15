#include "stm32f10x.h"
#include "LED.h"
#include "KEY.h"
#include "Delay.h"

int main(void) {
    LED_init();
    KEY_init();
    
    uint8_t key_pressed = 0;
    while(1) {
        key_pressed = KEY_pressed();
        if (key_pressed == 1) {
            LED1_ON();
            Delay_ms(300);
            LED1_OFF();
        }
        
        if (key_pressed == 2) {
            LED2_ON();
            Delay_ms(300);
            LED2_OFF();
        }
    }
}
