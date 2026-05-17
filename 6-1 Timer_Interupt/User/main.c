#include "stm32f10x.h"
#include "OLED.h"
#include "Timer.h"

uint16_t num = 0;

int main(void)
{
    OLED_Init();
    Timer_init();
    OLED_ShowString(1, 1, "CNT: ");
    while (1)
    {
        OLED_ShowNum(1, 6, num, 3);
    }
}
