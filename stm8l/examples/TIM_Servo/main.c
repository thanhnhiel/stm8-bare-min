#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include "uart.h"
#include "delay.h"
#include "beep.h"

#define LED_PIN     4

void TIM3_DeInit(void);
void tim2_init();
void tim3_init();
void tim4_init();
__IO uint8_t tim3_ov = 0;



uint8_t count = 0;

// INTERRUPT_HANDLER(TIM4_IRQHandler, TIM4_ISR) 
// {
//     count++;
//     if (count == 100)
//     {
//         count = 0;
//         PC_ODR ^= (1 << LED_PIN);
//     }

//     TIM4_SR &= ~(1 << TIM4_SR_UIF);
       //TIM4_SR1 = (u8)(~(1 << TIM4_SR_UIF));
// }

INTERRUPT_HANDLER(TIM2_UPD_IRQHandler, TIM2_UPD_ISR)
{
    count++;
    if (count == 50)
    {
        count = 0;
        PC_ODR ^= (1 << LED_PIN);
    } 
    //TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
    TIM2_SR1 = (uint8_t)(~TIM2_IT_UPDATE);
}

INTERRUPT_HANDLER(TIM3_CC_IRQHandler, TIM3_CC_ISR)
{
    // count++;
    // if (count == 100)
    // {
    //     count = 0;
    //     PC_ODR ^= (1 << LED_PIN);
    // }
    TIM3_SR1 = (uint8_t)(~TIM3_IT_CC1);
}

INTERRUPT_HANDLER(TIM3_UPD_IRQHandler, TIM3_UPD_ISR)
{
    // count++;
    // if (count == 100)
    // {
    //     count = 0;
    //     PC_ODR ^= (1 << LED_PIN);
    // }

    //TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
}

void main() 
{
    PC_DDR |= (1 << LED_PIN);
    PC_CR1 |= (1 << LED_PIN);

    CLK_PCKENR1 |= (uint8_t)(1 << CLK_Peripheral1_BEEP);
    BEEP_LSICalibrationConfig(2000000);
    BEEP_Init(BEEP_Frequency_1KHz);

    tim2_init();
    //tim3_init();
    //tim4_init();

    enableInterrupts();
  
    while (1) 
    {
        //PC_ODR ^= (1 << LED_PIN);
        //delay_ms(2000);
       // PC_ODR &= ~(1 << LED_PIN);
    }
}

// void TIM4_DeInit(void)
// {
//     TIM4_CR1 = TIM4_CR1_RESET_VALUE;
//     TIM4_IER = TIM4_IER_RESET_VALUE;
//     TIM4_CNTR = TIM4_CNTR_RESET_VALUE;
//     TIM4_PSCR = TIM4_PSCR_RESET_VALUE;
//     TIM4_ARR = TIM4_ARR_RESET_VALUE;
//     TIM4_SR = TIM4_SR1_RESET_VALUE;
// }

void tim3_init()
{
    TIM3_DeInit();
    /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM3);
    /* Set TIM3 Frequency to 2Mhz */ 
    //TIM3_TimeBaseInit(TIM3_PRESCALER_1, 40000); //update every 20ms (50Hz) the duty cycle
    /* Set the Prescaler value */
    TIM3_PSCR = (uint8_t)(TIM3_PRESCALER_1);
    /* Set the Autoreload value */
    TIM3_ARRH = (uint8_t)(40000 >> 8);
    TIM3_ARRL = (uint8_t)(40000);
    
    /* Enable TIM3 */
    //TIM3_ITConfig(TIM3_IT_UPDATE ,ENABLE);
    TIM3_IER |= (uint8_t)TIM3_IT_UPDATE;

    /* Enables TIM3 peripheral Preload register on ARR */
    //TIM3_ARRPreloadConfig(ENABLE);
    TIM3_CR1 |= (uint8_t)TIM3_CR1_ARPE;

    //TIM3_Cmd(ENABLE);
    TIM3_CR1 |= (uint8_t)TIM3_CR1_CEN;
}

void tim4_init()
{
    //===============================
     /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM4);

    /* Prescaler = 128 */
    TIM4_PSCR = 0b00000111;

    /* Frequency = F_CLK / (2 * prescaler * (1 + ARR))
     *           = 2 MHz / (2 * 128 * (1 + 77)) = 100 Hz */
    TIM4_ARR = 77;

    TIM4_IER |= (1 << TIM4_IER_UIE); // Enable Update Interrupt
    TIM4_CR1 |= (1 << TIM4_CR1_CEN); // Enable TIM4
}

void tim2_init()
{
    /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM2);

    //clock at 2MHz
    /* Set TIM2 Frequency to 2Mhz */ 
    //TIM2_TimeBaseInit(TIM2_PRESCALER_1, 40000);
    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)(TIM2_PRESCALER_1);
    /* Set the Autoreload value */
    TIM2_ARRH = (uint8_t)(40000 >> 8);
    TIM2_ARRL = (uint8_t)(40000);
  
	/* Channel 1 PWM configuration */ 
    //TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ppm, TIM2_OCPOLARITY_HIGH);
    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
    TIM2_CCER1 &= (uint8_t)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P));
    /* Set the Output State &  Set the Output Polarity  */
    TIM2_CCER1 |= (uint8_t)((TIM2_OUTPUTSTATE_ENABLE  & TIM2_CCER1_CC1E   ) | (TIM2_OCPOLARITY_HIGH   & TIM2_CCER1_CC1P   ));

    /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
    TIM2_CCMR1 = (uint8_t)((TIM2_CCMR1 & (uint8_t)(~TIM2_CCMR_OCM)) | (uint8_t)TIM2_OCMODE_PWM1);

    /* Set the Pulse value */
    uint16_t ppm = 2000;
    TIM2_CCR1H = (uint8_t)(ppm >> 8);
    TIM2_CCR1L = (uint8_t)(ppm);

    /* Enable TIM2 */
    //TIM2_ITConfig(TIM2_IT_UPDATE ,ENABLE);
    TIM2_IER |= (uint8_t)TIM2_IT_UPDATE;

    //TIM2_OC1PreloadConfig(ENABLE);
    TIM2_CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE;
  
	/* Enables TIM2 peripheral Preload register on ARR */
	//TIM2_ARRPreloadConfig(ENABLE);
	TIM2_CR1 |= (uint8_t)TIM2_CR1_ARPE;

    /* Enable TIM2 */
    TIM2_CR1 |= (uint8_t)TIM2_CR1_CEN;
}

void TIM3_DeInit(void)
{
    TIM3_CR1 = (uint8_t)TIM3_CR1_RESET_VALUE;
    TIM3_IER = (uint8_t)TIM3_IER_RESET_VALUE;
    TIM3_SR2 = (uint8_t)TIM3_SR2_RESET_VALUE;

    /* Disable channels */
    TIM3_CCER1 = (uint8_t)TIM3_CCER1_RESET_VALUE;

    /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
    TIM3_CCER1 = (uint8_t)TIM3_CCER1_RESET_VALUE;
    TIM3_CCMR1 = (uint8_t)TIM3_CCMR1_RESET_VALUE;
    TIM3_CCMR2 = (uint8_t)TIM3_CCMR2_RESET_VALUE;
    TIM3_CNTRH = (uint8_t)TIM3_CNTRH_RESET_VALUE;
    TIM3_CNTRL = (uint8_t)TIM3_CNTRL_RESET_VALUE;
    TIM3_PSCR = (uint8_t)TIM3_PSCR_RESET_VALUE;
    TIM3_ARRH  = (uint8_t)TIM3_ARRH_RESET_VALUE;
    TIM3_ARRL  = (uint8_t)TIM3_ARRL_RESET_VALUE;
    TIM3_CCR1H = (uint8_t)TIM3_CCR1H_RESET_VALUE;
    TIM3_CCR1L = (uint8_t)TIM3_CCR1L_RESET_VALUE;
    TIM3_CCR2H = (uint8_t)TIM3_CCR2H_RESET_VALUE;
    TIM3_CCR2L = (uint8_t)TIM3_CCR2L_RESET_VALUE;
    TIM3_SR1 = (uint8_t)TIM3_SR1_RESET_VALUE;
}