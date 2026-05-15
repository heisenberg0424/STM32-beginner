#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 3, "Hello World");
    while (1)
    {
        "What do u mean?"
    }
}
