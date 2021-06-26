#include <stdint.h>
#include <stm8l.h>
#include <delay.h>

#define LED_PIN     1

void main() 
{
    PB_DDR |= (1 << LED_PIN);
    PB_CR1 |= (1 << LED_PIN);
    PB_ODR &= ~(1 << LED_PIN);
    delay_ms(5000);
    PB_ODR |= (1 << LED_PIN);    

    // Input PU
    PA_DDR &= (uint8_t)(~(1<<0 | 1<<1 | 1<<2 | 1<<3));
    PA_CR1 |= (uint8_t)(1<<0 | 1<<1 | 1<<2 | 1<<3);
    PB_DDR &= (uint8_t)(~(1<<0 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6  | 1<<7));
    PB_CR1 |= (uint8_t)(1<<0 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    PC_DDR &= (uint8_t)(~(1<<0 | 1<<1 | 1<<4 | 1<<5 | 1<<6 ));
    PC_CR1 |= (uint8_t)(1<<0 | 1<<1 | 1<<4 | 1<5 | 1<<6);
    PD_DDR &= (uint8_t)(~(1<<0));
    PD_CR1 |= (uint8_t)(1<<0);

    while (1) {
        PB_ODR ^= (1 << LED_PIN);
        delay_ms(250);
    }
}
