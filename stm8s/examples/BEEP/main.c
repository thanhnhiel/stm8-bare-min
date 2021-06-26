#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8s.h>
#include <beep.h>
#include <delay.h>

#define T_BEEP   1000
void main() 
{
    CLK_CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

    CLK_PCKENR1 |= (uint8_t)(1 << CLK_Peripheral1_BEEP);
    /* PD4 - BEEP */
    BEEP_LSICalibrationConfig(128000);
    BEEP_Init(BEEP_Frequency_1KHz);
    

    while (1) 
    {
        BEEP_Init(BEEP_Frequency_1KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(T_BEEP);
        BEEP_Cmd(DISABLE);
        delay_ms(T_BEEP);

        BEEP_Init(BEEP_Frequency_2KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(T_BEEP);
        BEEP_Cmd(DISABLE);
        delay_ms(T_BEEP);

        BEEP_Init(BEEP_Frequency_4KHz);
        BEEP_Cmd(ENABLE);
        delay_ms(T_BEEP);
        BEEP_Cmd(DISABLE);
        delay_ms(T_BEEP);
    }
}
