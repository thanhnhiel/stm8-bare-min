#include <stdint.h>
#include <stdio.h>
#include <stm8l.h>
#include <uart.h>
#include <delay.h>



void initADC(uint16_t channel);
void UART_LowLevel_Init(void);
void tim3_init();

/*
 * Redirect stdout to UART
 */
int putchar(int c);
int getchar();
uint16_t ADC_read();
void start_servo(uint16_t count);

INTERRUPT_HANDLER(TIM3_UPD_IRQHandler, TIM3_UPD_ISR)
{
    // if (tol_pulse)
    // {
    //     if (start==1)
    //     {
    //         start = 0;
    //         //TIM3_BKR |= (TIM_BKR_MOE);
    //     }

    //     tol_pulse--;
    //     if (tol_pulse == 0)
    //     {
    //         /* Disable Output */
    //         //TIM3_BKR &= ~(TIM_BKR_MOE);
    //         //PB_ODR &= (uint8_t)(~(1 << 1));
    //         TIM3_IER &= (uint8_t)~TIM3_IT_UPDATE;
    //     }
    // }

    //TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
}

void main() 
{
    UART_LowLevel_Init();
    uart_init();
    tim3_init();
    printf("Test\r\n");
   
    /* configure ADC channel 4 (PD3)  
     * configure ADC channel 18 (PB0)
     * configure ADC channel 17 (PB1)
     * configure ADC channel 16 (PB3)
     */
    initADC(ADC_Channel_16);

    enableInterrupts();


    start_servo(2000);

    while (1) 
    {
        uint16_t val = ADC_read();
        // 0.5mS => 2.5mS, delta: 2mS
        float voltage = ((5000 - 900) / 4095.0) * val + 900;
        start_servo(voltage);
        printf("Channel4: %d \r\n", (uint16_t) voltage);
        delay_ms(40);
    }
}

void start_servo(uint16_t count)
{
    TIM3_CCR1H = (uint8_t)(count >> 8);
    TIM3_CCR1L = (uint8_t)(count);

//    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
//    TIM3_IER |= (uint8_t)TIM3_IT_UPDATE;   
}

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

void tim3_init()
{
    /* REMAP_Pin_TIM3Channel1: TIM3 Channel 1 (PB1) remapping to PI0 
     * REMAP_Pin_TIM3Channel2: TIM3 Channel 2 (PD0) remapping to PI3 
     */
    // 0b1110 0000
	//GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Clear Data */
    PB_ODR &= (uint8_t)(~(1 << 1));
    /* Set Output mode */
    PB_DDR |= (uint8_t)(1 << 1);
    /* Push-Pull/Open-Drain (Output) modes selection */
    PB_CR1 |= (uint8_t)(1 << 1);
    /* Slow slope */
    PB_CR2 |= (uint8_t)(1 << 1);

    /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM3);
    /* Set TIM3 Frequency to 2Mhz */ 
    //TIM3_TimeBaseInit(TIM3_PRESCALER_1, 40000); //update every 20ms (50Hz) the duty cycle
    /* Set the Prescaler value */
    TIM3_PSCR = (uint8_t)(TIM3_PRESCALER_1);
    /* Set the Autoreload value */
    TIM3_ARRH = (uint8_t)(40000 >> 8);
    TIM3_ARRL = (uint8_t)(40000);

    /* Channel 1 PWM configuration - PB1 */ 
    //TIM3_OC1Init(TIM3_OCMODE_PWM1, TIM3_OUTPUTSTATE_ENABLE, ppm, TIM3_OCPOLARITY_HIGH);
    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
    TIM3_CCER1 &= (uint8_t)(~( TIM3_CCER1_CC1E | TIM3_CCER1_CC1P));
    /* Set the Output State &  Set the Output Polarity  */
    TIM3_CCER1 |= (uint8_t)((TIM3_OUTPUTSTATE_ENABLE & TIM3_CCER1_CC1E) | (TIM3_OCPOLARITY_HIGH & TIM3_CCER1_CC1P));

    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM3_CCMR1 = (uint8_t)((TIM3_CCMR1 & (uint8_t)(~TIM3_CCMR_OCM)) | (uint8_t)TIM3_OCMODE_PWM1);

    /* Set the Pulse value */
    uint16_t ppm2 = 2000;
    TIM3_CCR1H = (uint8_t)(ppm2 >> 8);
    TIM3_CCR1L = (uint8_t)(ppm2);

    /* Enable TIM3 */
    //TIM3_ITConfig(TIM3_IT_UPDATE ,ENABLE);
    TIM3_IER |= (uint8_t)TIM3_IT_UPDATE;

    //TIM3_OC1PreloadConfig(ENABLE);
    TIM3_CCMR1 |= (uint8_t)TIM3_CCMR_OCxPE;

    /* Enables TIM3 peripheral Preload register on ARR */
    //TIM3_ARRPreloadConfig(ENABLE);
    TIM3_CR1 |= (uint8_t)(1 << TIM3_CR1_ARPE);

    //TIM3_Cmd(ENABLE);
    TIM3_CR1 |= (uint8_t)(1 << TIM3_CR1_CEN);

    /* TIM2_Ctrl PWM Outputs */
    TIM3_BKR |= TIM_BKR_MOE;
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