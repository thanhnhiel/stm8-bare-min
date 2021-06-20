#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include <beep.h>
#include <delay.h>


void main() 
{
    /* HSI clock prescaler */
    CLK_CKDIVR = CLK_SYSCLKDiv_8;

    CLK_PCKENR1 |= (uint8_t)(1 << CLK_Peripheral1_BEEP);
    //CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << 6);
    
    /* Enable LSI for BEEP */
    CLK_CBEEPR |= (1<<CLK_CBEEPR_CLKBEEPSEL0);

    BEEP_LSICalibrationConfig(32000);
    BEEP_Init(BEEP_Frequency_1KHz);

    delay_ms(5000);

    PA_DDR |= (1 << 0); 
    PA_CR1 |= (1 << 0);

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
