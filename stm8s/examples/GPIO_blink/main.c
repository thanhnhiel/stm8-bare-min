#include <stdint.h>
#include <stm8s.h>
#include <delay.h>

#define LED_PIN     5
//#define LED_PIN     8

void main() {
    PB_DDR |= (1 << LED_PIN);
    PB_CR1 |= (1 << LED_PIN);

    PB_ODR ^= (1 << LED_PIN);
    while (1) 
    {
        for (uint8_t i=0;i<3;i++)
        {
            PB_ODR ^= (1 << LED_PIN);
            delay_ms(100);
            PB_ODR ^= (1 << LED_PIN);
            delay_ms(100);
        }
        delay_ms(1000);
    }
}
