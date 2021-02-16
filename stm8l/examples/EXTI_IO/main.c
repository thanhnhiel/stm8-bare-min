#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include <uart.h>
#include <delay.h>
#include "mpr121.h"
#include <beep.h>

void setup();
int putchar(int c);
int getchar();
void UART_LowLevel_Init(void);
void I2C_LowLevel_Init(void);
void Tone(uint8_t times, uint16_t t);

__IO uint8_t pressed = 0;

INTERRUPT_HANDLER(EXTI2_IRQHandler, EXTI2_ISR)
{
  pressed = 1;
  /* Cleat Interrupt pending bit */
  //EXTI_ClearITPendingBit(EXTI_IT_Pin1);
  EXTI_SR1 = (uint8_t) (EXTI_IT_Pin2);
}

void main()
{
    uint8_t counter = 0;
    uint8_t id[2];

    setup();

    pressed = 0;
    mpr121_get_data(id);
    printf("Touch data:: %d %d\r\n", id[0], id[1]);

    Tone(3, 50);

    while (1) 
    {
        //printf("Test, %d\n\r", counter++);
        //delay_ms(500);

        if (pressed == 1) //if (!(PB_IDR & (1<<2)))
        {
            pressed = 0;
            mpr121_get_data(id);
            printf("Touch data:: %d %d\r\n", id[0], id[1]);
            if (!(id[0] == 0 && id[1]==0))
            {
                if (id[0]==16 && id[1] == 0)
                {
                    counter = 1;
                    Tone(1, 50);
                    printf("2\r\n");
                }
                else if (counter==1 && id[0]==0 && id[1] == 2)
                {
                    counter++;
                     Tone(1, 50);
                     printf("4\r\n");
                }
                else if (counter==2 && id[0]==64 && id[1] == 0)
                {
                    counter++;
                     Tone(1, 50);
                     printf("8\r\n");
                }
                else if (counter==3 && id[0]==2 && id[1] == 0)
                {
                    counter++;
                     Tone(1, 50);
                     printf("6\r\n");
                }
                else if (counter==4 && id[0]==8 && id[1] == 0)
                {
                    counter = 0;
                    Tone(1, 50);
                    delay_ms(1000);
                    Tone(3, 100);
                    printf("End\r\n");
                } 
                else
                {
                    counter = 0;
                    Tone(1, 50);
                }
             
            }
        }
    }
}

void I2C_LowLevel_Init(void)
{
  /* Enable the peripheral Clock */
  CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_I2C1);
}

void Tone(uint8_t times, uint16_t t)
{
    for (uint8_t i=0; i<times; i++)
    {
        if (i!=0) delay_ms(t);
        BEEP_Cmd(ENABLE);
        delay_ms(t);
        BEEP_Cmd(DISABLE);
    }
}

void setup()
{
    /* Init Beep driver */
    CLK_PCKENR1 |= (uint8_t)(1 << CLK_Peripheral1_BEEP);
    BEEP_LSICalibrationConfig(2000000);
    BEEP_Init(BEEP_Frequency_1KHz);
    Tone(1, 200);

    /* Init Uart */
    UART_LowLevel_Init();
    uart_init();
    printf("MCU Init\r\n");

    //EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
    EXTI_CR1 &=  (uint8_t)(~EXTI_CR1_P1IS);
    EXTI_CR1 |= (uint8_t)((uint8_t)(EXTI_Trigger_Falling) << EXTI_Pin_2);
    // Pull up
    PB_CR1 |= 1 << 2;
    // Interrupt
    PB_CR2 |= 1 << 2;
    enableInterrupts();


    printf("mpr121_setup...");
    I2C_LowLevel_Init();
    mpr121_setup();
    printf("done\r\n");

}

void UART_LowLevel_Init(void)
{
    /*!< USART1 Tx- Rx (PC3- PC2) remapping to PA2- PA3 */
    SYSCFG_RMPCR1 &= (uint8_t)((uint8_t)((uint8_t)0x011C << 4) | (uint8_t)0x0F);
    SYSCFG_RMPCR1 |= (uint8_t)((uint16_t)0x011C & (uint16_t)0x00F0);

    /* Enable USART clock */
    //CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
      /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_USART1);
}

/*
 * Redirect stdout to UART
 */
int putchar(int c) 
{
    uart_write(c);
    return 0;
}

/*
 * Redirect stdin to UART
 */
int getchar() 
{
    return uart_read();
}