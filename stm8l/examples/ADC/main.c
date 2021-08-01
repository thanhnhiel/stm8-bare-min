#include <stdint.h>
#include <stdio.h>
#include <stm8l.h>
#include <uart.h>
#include <delay.h>

#define V_REF 3.3

void initADC(uint16_t channel);
void UART_LowLevel_Init(void);

/*
 * Redirect stdout to UART
 */
int putchar(int c);
int getchar();


uint16_t ADC_read() 
{
    uint8_t adcH, adcL;
    ADC1_CR1 |= (1 << ADC1_CR1_START);
    while (!(ADC1_SR & (1 << ADC1_SR_EOC)));
    adcL = ADC1_DRL;
    adcH = ADC1_DRH;
    //ADC1_CSR &= ~(1 << ADC1_CSR_EOC); // clear EOC flag
    return (adcL | (adcH << 8));
}

void main() 
{
    UART_LowLevel_Init();
    uart_init();
    printf("Test\r\n");
   
    /* configure ADC channel 4 (PD3)  
     * configure ADC channel 18 (PB0)
     * configure ADC channel 17 (PB1)
     * configure ADC channel 16 (PB3)
     */
    initADC(ADC_Channel_16);

    while (1) 
    {
        uint16_t val = ADC_read();
        float voltage = (V_REF / 4095.0) * val * 1000;
        printf("Channel4: %d mV\r\n", (uint16_t) voltage);
        delay_ms(250);
    }
}



void initADC(uint16_t channel)
{
    uint8_t regindex;

    /* Enable ADC1 clock */
    //CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    /* Enable the peripheral Clock */
    CLK_PCKENR2 |= (uint8_t)((uint8_t)1 << 0);
    
    /* Initialize and configure ADC1 */
    //ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
    /* clear CR1 register . ADC_CR1_CONT = 0x04, ADC_CR1_RES = 0x60*/
    ADC1_CR1 &= (uint8_t)~(0x04 | 0x60);
    /* set the resolution and the conversion mode 
    ADC_ConversionMode = 0 : single
    ADC_Resolution_12Bit : 0
    */
    //ADC1_CR1 |= (uint8_t)((uint8_t)0 | (uint8_t)0);
    /*clear CR2 register . ADC_CR2_PRESC = 0x80*/
    ADC1_CR2 &= (uint8_t)~(0x80);
    /* set the Prescaler ADC_Prescaler_2 = ((uint8_t)0x80 */
    ADC1_CR2 |= (uint8_t) 0x80;
    
    /* ADC channel used for IDD measurement */
    // ADC_SamplingTime_384Cycles = ((uint8_t)0x07
    //ADC_SamplingTimeConfig(ADC1, ADC_GROUP_SPEEDCHANNEL, ADC_SamplingTime_384Cycles);
    // ADC_Group_SlowChannels
    /* Configures the sampling time for the Fast ADC channel group. 
       ADC_CR3_SMPT2 = 0xE0
     */
    ADC1_CR3 &= (uint8_t)~0xE0;
    ADC1_CR3 |= (uint8_t)(0x07 << 5);
    
    /* Enable ADC1 */
    //ADC_Cmd(ADC1, ENABLE);
    ADC1_CR1 |= (1 << ADC1_CR1_ADON);
    
    /* Disable SchmittTrigger for ADC_Channel, to save power */
    //ADC_SchmittTriggerConfig(ADC1, ADC_IDD_MEASUREMENT_CHANNEL, DISABLE);
    regindex = (uint8_t)((uint16_t)channel >> 8);
    _SFR_(ADC1_SQR_ADR + regindex) |= (uint8_t)(channel);
    
    /* Enable ADC1 Channel used for IDD measurement */
    //ADC_ChannelCmd(ADC1, ADC_Channel_19, ENABLE);
    _SFR_(ADC1_SQR_ADR + regindex) |= (uint8_t)(channel);    
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
  
    PA_DDR |= (1 << 2); // PA2 - TXD,  PA3 - RXD 
    PA_CR1 |= (1 << 2);  
}

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