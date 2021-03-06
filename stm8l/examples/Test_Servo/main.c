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

uint8_t count = 0;

INTERRUPT_HANDLER(TIM4_IRQHandler, TIM4_ISR) 
{
    /* Clear the IT pending Bit */
    TIM4_SR = (uint8_t)(~TIM4_IT_UPDATE);
}


INTERRUPT_HANDLER(TIM2_UPD_IRQHandler, TIM2_UPD_ISR)
{
    TIM2_SR1 = (uint8_t)(~TIM2_IT_UPDATE);
}

INTERRUPT_HANDLER(TIM3_CC_IRQHandler, TIM3_CC_ISR)
{
    TIM3_SR1 = (uint8_t)(~TIM3_IT_CC1);
}

__IO uint16_t tol_pulse = 0;
uint8_t start = 0;
//__IO uint16_t count = 2000;
INTERRUPT_HANDLER(TIM3_UPD_IRQHandler, TIM3_UPD_ISR)
{
    if (tol_pulse)
    {
        if (start==1)
        {
            start = 0;
            //TIM3_BKR |= (TIM_BKR_MOE);
        }

        tol_pulse--;
        if (tol_pulse == 0)
        {
            /* Disable Output */
            //TIM3_BKR &= ~(TIM_BKR_MOE);
            //PB_ODR &= (uint8_t)(~(1 << 1));
            TIM3_IER &= (uint8_t)~TIM3_IT_UPDATE;
        }
    }

    //TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
}

void start_servo(uint16_t ms, uint16_t duration)
{
    TIM3_CCR1H = (uint8_t)(ms >> 8);
    TIM3_CCR1L = (uint8_t)(ms);

    start = 1;
    tol_pulse = (duration / 20);

    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
    TIM3_IER |= (uint8_t)TIM3_IT_UPDATE;   
}


void setPos(uint16_t pos)
{
    if (pos > 180) pos = pos - 180;
    uint16_t count = (40 * pos) / 180 + 10;
    
    start_servo(count * 100, 1000);
    while (tol_pulse);
}

void main() 
{
    PC_DDR |= (1 << LED_PIN);
    PC_CR1 |= (1 << LED_PIN);

    // Beep - PA0
    CLK_PCKENR1 |= (uint8_t)(1 << CLK_Peripheral1_BEEP);
    BEEP_LSICalibrationConfig(2000000);
    BEEP_Init(BEEP_Frequency_1KHz);

    //tim2_init();
    tim3_init();
    //tim4_init();

    enableInterrupts();


    /*
       1000 - 5000
    */
    while (1) 
    {
        // for (uint16_t i=1; i<7; i++)
        // {
        //     setPos(i*1000);
        // }
        setPos(0);
        setPos(90);
        setPos(45);
        setPos(180);
        setPos(90+45);

        delay_ms(3000);
               
        

        PC_ODR ^= (1 << LED_PIN);
    }
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

    //TIM4_IER |= (1 << TIM4_IER_UIE); // Enable Update Interrupt
    TIM4_IER |= (uint8_t)TIM4_IT_UPDATE;

    TIM4_CR1 |= (1 << TIM4_CR1_CEN); // Enable TIM4
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


void tim2_init()
{
    /* REMAP_Pin_TIM2Channel1: TIM2 Channel 1 (PB0) remapping to PC5
     * REMAP_Pin_TIM2Channel2: TIM2 Channel 2 (PB2) remapping to PC6
     */
    // 0b1110 0000
	//GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Clear Data */
    PB_ODR &= (uint8_t)(~(1 << 0));
    /* Set Output mode */
    PB_DDR |= (uint8_t)(1 << 0);
    /* Push-Pull/Open-Drain (Output) modes selection */
    PB_CR1 |= (uint8_t)(1 << 0);
    /* Slow slope */
    PB_CR2 |= (uint8_t)(1 << 0);

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
    /* Disable the Channel 1 - PB0 : Reset the CCE Bit, Set the Output State , the Output Polarity */
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

    /* TIM2_Ctrl PWM Outputs */
    TIM2_BKR |= TIM_BKR_MOE;
}
