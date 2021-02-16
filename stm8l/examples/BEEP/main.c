#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include <beep.h>
#include <delay.h>


void main() 
{
    CLK_PCKENR1 |= (uint8_t)(1 << 0x06);
    BEEP_LSICalibrationConfig(2000000);
    BEEP_Init(BEEP_Frequency_1KHz);

    while (1) 
    {
        BEEP_Init(BEEP_Frequency_1KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(200);
        BEEP_Cmd(DISABLE);
        delay_ms(200);

        BEEP_Init(BEEP_Frequency_2KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(200);
        BEEP_Cmd(DISABLE);
        delay_ms(200);

        BEEP_Init(BEEP_Frequency_4KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(200);
        BEEP_Cmd(DISABLE);
        delay_ms(1000);
    }
}
