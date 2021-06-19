#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8s.h>
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

void main() {
    uint8_t counter = 0;

    /* HSI Max : 16Mhz */
    CLK_CKDIVR &= (uint8_t)(~CLK_PRESCALER_HSIDIV8); // 16 / 8 = 2MHz
    CLK_CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV8;    

    uart_init();
    
    while (1) {
        printf("Test, %d\r\n", counter++);
        delay_ms(1000);
        if (uart_rx_available())
        {
            printf("rx: %c\r\n", getchar());
        }
    }
}
