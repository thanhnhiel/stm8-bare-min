#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stm8l.h>
#include <delay.h>
#include <uart.h>
#include "mpr121.h"


int putchar(int c) {
    uart_write(c);
    return 0;
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


void I2C_LowLevel_Init(void)
{
  /* Enable the peripheral Clock */
  CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_I2C1);
  //CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
  
  //GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_6, ENABLE);
  // GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_7, ENABLE);
  // GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5, ENABLE);
  //GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
}

void main() 
{
    uint8_t counter = 0;
    uint8_t id[2];
    UART_LowLevel_Init();
    uart_init();
    printf("MCU Init\r\n");

    //GPIO_ExternalPullUpConfig(GPIO_TypeDef* GPIOB, 1 << 1, 1);
    PB_CR1 |= 1 << 2;

    printf("mpr121_setup...");
    I2C_LowLevel_Init();
    mpr121_setup();
    printf("done\r\n");

    while (1) {
        //measure();
        printf("Test, %d\r\n", counter++);
        delay_ms(2000);

        if (!(PB_IDR & (1<<2)))
        {
            mpr121_get_data(id);
            printf("Touch data:: %d %d\r\n", id[0], id[1]);
        }
    }
}
