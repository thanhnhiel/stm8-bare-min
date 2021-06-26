#include "uart.h"
#include "stm8s.h"

void uart_init() 
{
    // /* round to nearest integer */
    // uint16_t div = (F_CPU + BAUDRATE / 2) / BAUDRATE;
    // /* madness.. */
    // UART1_BRR2 = ((div >> 8) & 0xF0) + (div & 0x0F);
    // UART1_BRR1 = div >> 4;
    // /* enable transmitter and receiver */
    // UART1_CR2 = (1 << UART1_CR2_TEN) | (1 << UART1_CR2_REN);
    uint32_t BaudRate_Mantissa = 0;
    uint32_t BaudRate_Mantissa100 = 0;
    
    UART1_CR1 &= (uint8_t)(~(0x10 | 0x04 | 0x02));  // 8 bits - No Parity
    UART1_CR3 &= (uint8_t)(~0x30);  // 1 bits
           
    /* Clear the LSB mantissa of UART1DIV  */
    UART1_BRR1 &= (uint8_t)(~0xFF);  
    /* Clear the MSB mantissa of UART1DIV  */
    UART1_BRR2 &= (uint8_t)(~0xFF);  
        
    /* Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
    BaudRate_Mantissa    = ((uint32_t)F_CPU / (BAUDRATE << 4));
    BaudRate_Mantissa100 = (((uint32_t)F_CPU * 100) / (BAUDRATE << 4));
    /* Set the fraction of UART1DIV  */
    UART1_BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
    /* Set the MSB mantissa of UART1DIV  */
    UART1_BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
    /* Set the LSB mantissa of UART1DIV  */
    UART1_BRR1 |= (uint8_t)BaudRate_Mantissa;           
    
    /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
    UART1_CR2 &= (uint8_t)~(0x08 | 0x04); 
    /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
    UART1_CR3 &= (uint8_t)~(0x04 | 0x02 | 0x01 | 0x08); 
    /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
    UART1_CR3 |= (uint8_t)((uint8_t)0x80 & (uint8_t)(0x04 | 0x02 | 0x01));  
    // UART1_MODE_TX_ENABLE - RX_ENABLE
    UART1_CR2 |= (uint8_t) (0x08 | 0x04);  
}

void uart_write(uint8_t data) {
    UART1_DR = data;
    while (!(UART1_SR & (1 << UART1_SR_TC)));
}

uint8_t uart_read() {
    while (!(UART1_SR & (1 << UART1_SR_RXNE)));
    return UART1_DR;
}

uint8_t uart_rx_available() {
    if (UART1_SR & (1 << UART1_SR_RXNE)) return 1;
    return 0;
}
