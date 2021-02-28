#include <stdint.h>
#include <stm8s.h>
#include <delay.h>

#define LED_PIN     3

void main() {
    PD_DDR |= (1 << LED_PIN);
    PD_CR1 |= (1 << LED_PIN);

    while (1) 
    {
        for (uint8_t i=0;i<3;i++)
        {
            PD_ODR ^= (1 << LED_PIN);
            delay_ms(100);
            PD_ODR ^= (1 << LED_PIN);
            delay_ms(100);
        }
        delay_ms(1000);
    }
}
