#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include <uart.h>
#include <delay.h>

/*
 * Redirect stdout to UART
 */
int putchar(int c) {
    uart_write(c);
    return 0;
}

/*
 * Redirect stdin to UART
 */
int getchar() {
    return uart_read();
}


void UART_LowLevel_Init(void)
{
    /*!< USART1 Tx- Rx (PC3- PC2) remapping to PA2- PA3 */
    SYSCFG_RMPCR1 &= (uint8_t)((uint8_t)((uint8_t)0x011C << 4) | (uint8_t)0x0F);
    SYSCFG_RMPCR1 |= (uint8_t)((uint16_t)0x011C & (uint16_t)0x00F0);

    /* Enable USART clock */
    //CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
      /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << 0x05);
}

void main() {
    uint8_t counter = 0;
    UART_LowLevel_Init();
    uart_init();

    while (1) {
        printf("Test, %d\n\r", counter++);
        delay_ms(500);
    }
}
