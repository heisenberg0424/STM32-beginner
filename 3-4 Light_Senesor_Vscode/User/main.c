#include "stm32f10x.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "Delay.h"

int main(void)
{
    BUZZER_init();
    LightSensor_init();

    BUZZER_ON();
    Delay_s(1);
    BUZZER_OFF();
    Delay_s(1);
    BUZZER_ON();
    Delay_s(1);
    BUZZER_OFF();
    Delay_s(1);
    BUZZER_ON();
    Delay_s(1);
    BUZZER_OFF();
    Delay_s(1);
    BUZZER_ON();
    Delay_s(1);
    BUZZER_OFF();

    while (1)
    {
        if (LightSensor_Get())
            BUZZER_ON();
        else
            BUZZER_OFF();
    }
}
