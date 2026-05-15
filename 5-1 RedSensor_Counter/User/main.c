#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "RedSensor.h"

int main(void)
{
    OLED_Init();
    RedSensor_init();
    OLED_ShowString(1, 1, "CNT: ");
    while (1)
    {
        OLED_ShowNum(1, 5, get_cnt(), 8);
    }
}
