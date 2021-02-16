#include "uart.h"
#include "stm8l.h"

void uart_init() {
    /* round to nearest integer */
    uint16_t div = (F_CPU + BAUDRATE / 2) / BAUDRATE;
    /* madness.. */
    USART1_BRR2 = ((div >> 8) & 0xF0) + (div & 0x0F);
    USART1_BRR1 = div >> 4;
    /* enable transmitter and receiver */
    USART1_CR2 = (1 << USART1_CR2_TEN) | (1 << USART1_CR2_REN);
}

void uart_write(uint8_t data) {
    USART1_DR = data;
    while (!(USART1_SR & (1 << USART1_SR_TC)));
}

uint8_t uart_read() {
    while (!(USART1_SR & (1 << USART1_SR_RXNE)));
    return USART1_DR;
}
