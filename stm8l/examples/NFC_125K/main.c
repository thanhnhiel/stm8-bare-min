#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include "uart.h"
#include "delay.h"
//#include "beep.h"

#define LED_PIN     4
#define LED2_PIN     7

#define LED_INIT() { \
       PC_DDR |= (1 << LED_PIN); \
       PC_CR1 |= (1 << LED_PIN); \
       PB_DDR |= (1 << LED2_PIN); \
       PB_CR1 |= (1 << LED2_PIN); \
       }
      
#define LED_OFF()  PC_ODR &= ~(1 << LED_PIN)
#define LED_ON()  PC_ODR |= (1 << LED_PIN)
#define LED_TOGGLE()  PC_ODR ^= (1 << LED_PIN)

#define LED2_OFF()  PB_ODR &= ~(1 << LED2_PIN)
#define LED2_ON()  PB_ODR |= (1 << LED2_PIN)
#define LED2_TOGGLE()  PB_ODR ^= (1 << LED2_PIN)

void TIM3_DeInit(void);
void tim2_init();

//void tim3_init();
void TIM3_PWMIConfig();
void tim3Input_init();
void tim4_init();

__IO uint8_t tim3_ov = 0;


uint8_t count = 0;


void main() 
{
    /* HSI clock prescaler */
    CLK_CKDIVR = CLK_SYSCLKDiv_8;
    LED_INIT();
    LED_ON();
    LED2_ON();
    delay_ms(2000);
    LED2_OFF();
    LED_ON();

    // tim2_init();
    // tim3Input_init();
    tim4_init();

    enableInterrupts();

    while (1) 
    {
        //LED_TOGGLE();
        //delay_ms(2000);
    }
}

INTERRUPT_HANDLER(TIM4_IRQHandler, TIM4_ISR) 
{
    count++;
    if (count == 100)
    {
        count = 0;
        LED2_TOGGLE();
    }

    /* Clear the IT pending Bit */
    TIM4_SR = (uint8_t)(~TIM4_IT_UPDATE);
}


INTERRUPT_HANDLER(TIM2_UPD_IRQHandler, TIM2_UPD_ISR)
{
    //LED_TOGGLE();
    TIM2_SR1 = (uint8_t)(~TIM2_IT_UPDATE);
}

INTERRUPT_HANDLER(TIM3_CC_IRQHandler, TIM3_CC_ISR)
{
    uint8_t tmpccrl, tmpccrh;
    uint16_t IC1Value = 0, IC2Value = 0;
    //volatile uint32_t SignalDutyCycle = 0;
    //volatile uint32_t SignalFrequency = 0;

    // CLear  TIM3_IT_CC1
    TIM3_SR1 = (uint8_t)(~TIM3_IT_CC1);
    tmpccrh = TIM3_CCR1H;
    tmpccrl = TIM3_CCR1L;
    IC1Value = (uint16_t)(tmpccrl); // Cycle - T
    IC1Value |= (uint16_t)((uint16_t)tmpccrh << 8);

    if (IC1Value != 0)
    {
        /*tmpccrh = TIM3_CCR2H;
        tmpccrl = TIM3_CCR2L;
        IC2Value = (uint16_t)(tmpccrl);
        IC2Value |= (uint16_t)((uint16_t)tmpccrh << 8);*/

        /* Duty cycle computation */
        //SignalDutyCycle = ((uint32_t) IC2Value * 100) / IC1Value;
        /* Frequency computation */
        //SignalFrequency = (uint32_t) (2000000 / IC1Value);
    }
    //else
    //{
    //    SignalDutyCycle = 0;
    //    SignalFrequency = 0;
    //}
}

INTERRUPT_HANDLER(TIM3_UPD_IRQHandler, TIM3_UPD_ISR)
{
    // count++;
    // if (count == 50)
    // {
    //     count = 0;
      //   PC_ODR ^= (1 << LED_PIN);
    // }

    //TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
    TIM3_SR1 = (uint8_t)(~TIM3_IT_UPDATE);
}


void tim4_init()
{
    //===============================
     /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM4);

    /* Prescaler = 128 */
    TIM4_PSCR = TIM2_PRESCALER_128;

    /* Frequency = F_CLK / (2 * prescaler * (1 + ARR))
     *           = 2 MHz / (2 * 128 * (1 + 77)) = 100 Hz */
    TIM4_ARR = 77;

    //TIM4_IER |= (1 << TIM4_IER_UIE); // Enable Update Interrupt
    TIM4_IER |= (uint8_t)TIM4_IT_UPDATE;

    TIM4_CR1 |= (1 << TIM4_CR1_CEN); // Enable TIM4
}


void TIM3_PWMIConfig()
{
    //GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Clear Data */
    PB_ODR &= (uint8_t)(~(1 << 1));
    /* Set Output mode */
    PB_DDR |= (uint8_t)(1 << 1);
    /* Push-Pull/Open-Drain (Output) modes selection */
    PB_CR1 |= (uint8_t)(1 << 1);
    /* Slow slope */
    PB_CR2 |= (uint8_t)(1 << 1);

    //   TIM3_ICPolarity_Rising  = ((uint8_t)0x00), /*!< Input Capture on Rising Edge*/
    //   TIM3_ICPolarity_Falling  = ((uint8_t)0x01)  /*!< Input Capture on Falling Edge*/
    uint8_t icpolarity = 0x01;
    
    //TIM3_ICSelection_DirectTI    = ((uint8_t)0x01), /*!< Input Capture mapped on the direct input*/
    //TIM3_ICSelection_IndirectTI  = ((uint8_t)0x02), /*!< Input Capture mapped on the indirect input*/
    //TIM3_ICSelection_TRGI        = ((uint8_t)0x03)  /*!< Input Capture mapped on the Trigger Input*/
    uint8_t icselection = 0x01;
    uint8_t tmpccmr1 = TIM3_CCMR1;

    /* TI1 Configuration */
    //TI1_Config(TIM3_ICPolarity, TIM3_ICSelection, TIM3_ICFilter);

    /* Disable the Channel 1: Reset the CCE Bit TIM_CCER1_CC1E = 0x01 */
    TIM3_CCER1 &=  (uint8_t)(~0x01);

    /* Select the Input and set the filter - 0x01 TIM3_ICSelection_DirectTI */
    tmpccmr1 &= (uint8_t)(~0x03) & (uint8_t)(~0xF0);
    tmpccmr1 |= (uint8_t)(((uint8_t)(0x01)) | ((uint8_t)(0 << 4))); // TIM3_ICFilter = 0
    TIM3_CCMR1 = tmpccmr1;

    /* Select the Polarity */
    // TIM_CCER1_CC1E = 0x01, TIM_CCER1_CC1P = 0x02
    //TIM3_ICPolarity_Falling
    //TIM3_CCER1 = (uint8_t)((TIM3_CCER1 & (uint8_t)(~0x01)) | (uint8_t)0x02);
    //TIM3_ICPolarity_Rising
    TIM3_CCER1 = (uint8_t)((TIM3_CCER1 & (uint8_t)(~0x01)) & (uint8_t)~0x02);

    /* Set the CCE Bit TIM_CCER1_CC1E = 0x01 */
    TIM3_CCER1 |=  0x01;

    /* Set the Input Capture Prescaler value */
    //TIM3_SetIC1Prescaler(TIM3_ICPSC_DIV1);
    //TIM3_ICPSC_DIV1  = ((uint8_t)0x00),  /*!< Input Capture Prescaler = 1 (one capture every 1 event) */
    //TIM3_ICPSC_DIV2  = ((uint8_t)0x04),  /*!< Input Capture Prescaler = 2 (one capture every 2 events) */
    //TIM3_ICPSC_DIV4  = ((uint8_t)0x08),  /*!< Input Capture Prescaler = 4 (one capture every 4 events) */
    //TIM3_ICPSC_DIV8  = ((uint8_t)0x0C)   /*!< Input Capture Prescaler = 8 (one capture every 8 events) */
    TIM3_CCMR1 = (uint8_t)((TIM3_CCMR1 & (uint8_t)(~TIM3_CCMR_ICxPSC)) | (uint8_t)0x00);

    /* TI2 Configuration */
    //TI2_Config((TIM3_ICPolarity_TypeDef)icpolarity, (TIM3_ICSelection_TypeDef)icselection, TIM3_ICFilter);
    tmpccmr1 = TIM3_CCMR2;
    /* Disable the Channel 2: Reset the CCE Bit - TIM_CCER1_CC2E */
    TIM3_CCER1 &=  (uint8_t)(~0x10);

      /* Select the Input and set the filter - TIM3_ICSelection_IndirectTI */
    tmpccmr1 &= (uint8_t)(~0x03) & (uint8_t)(~0xF0);
    tmpccmr1 |= (uint8_t)(((uint8_t)(0x02)) | ((uint8_t)(0 << 4)));
    TIM3_CCMR2 = tmpccmr1;

    /* Select the Polarity */
    // TIM_CCER1_CC1E = 0x01, TIM_CCER1_CC1P = 0x02
    //TIM3_ICPolarity_Falling
    TIM3_CCER1 = (uint8_t)((TIM3_CCER1 & (uint8_t)(~0x20)) | (uint8_t)0x02);
    //TIM3_ICPolarity_Rising
    //TIM3_CCER1 = (uint8_t)((TIM3_CCER1 & (uint8_t)(~0x20)) & (uint8_t)~0x02);

    /* Set the CCE Bit TIM_CCER1_CC2E */
    TIM3_CCER1 |=  0x10;

    /* Set the Input Capture Prescaler value */
    //TIM3_SetIC2Prescaler(TIM3_ICPSC_DIV1);
    TIM3_CCMR2 = (uint8_t)((TIM3_CCMR2 & (uint8_t)(~TIM3_CCMR_ICxPSC)) | (uint8_t)0x00);
}

void tim3Input_init()
{
    TIM3_PWMIConfig();
    // TIM3_SelectInputTrigger(TIM3_TRGSelection_TI1FP1 = 0x50);
    TIM3_SMCR = (uint8_t)((TIM3_SMCR & (uint8_t)(~0x70)) | (uint8_t)0x50);
    // TIM3_SelectSlaveMode(TIM3_SlaveMode_Reset = 0x04);
    TIM3_SMCR = (uint8_t)((TIM3_SMCR & (uint8_t)(~0x70)) | (uint8_t)0x04);
    // TIM3_ITConfig(TIM3_IT_CC1 = 0x02, ENABLE);
    TIM3_IER |= (uint8_t)(0x02);
    // TIM3_Cmd(ENABLE);
    TIM3_CR1 |= 0x01;
}

#if 0
void tim3_init()
{
    /* REMAP_Pin_TIM3Channel1: TIM3 Channel 1 (PB1) remapping to PI0 
     * REMAP_Pin_TIM3Channel2: TIM3 Channel 2 (PD0) remapping to PI3 
     */
    // 0b1110 0000
	//GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_MODE_OUT_PP_LOW_FAST);
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
#endif

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

    /* Frequency = F_CLK / (2 * prescaler * (1 + ARR))
     *           = 2 MHz / (2 * 1 * (1 + 15))) = 125K Hz */
    //clock at 2MHz
    /* Set TIM2 Frequency to 2Mhz */ 
    //TIM2_TimeBaseInit(TIM2_PRESCALER_1, 40000);
    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)(TIM2_PRESCALER_1);
    /* Set the Autoreload value */
    TIM2_ARRH = (uint8_t)(15 >> 8);
    TIM2_ARRL = (uint8_t)(15);
  
	/* Channel 1 PWM configuration */ 
    //TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ppm, TIM2_OCPOLARITY_HIGH);
    /* Disable the Channel 1 - PB0 : Reset the CCE Bit, Set the Output State , the Output Polarity */
    TIM2_CCER1 &= (uint8_t)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P)); 
    /* Set the Output State &  Set the Output Polarity  */
    TIM2_CCER1 |= (uint8_t)((TIM2_OUTPUTSTATE_ENABLE  & TIM2_CCER1_CC1E   ) | (TIM2_OCPOLARITY_HIGH   & TIM2_CCER1_CC1P   ));

    /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
    TIM2_CCMR1 = (uint8_t)((TIM2_CCMR1 & (uint8_t)(~TIM2_CCMR_OCM)) | (uint8_t)TIM2_OCMODE_PWM1);

    /* Set the Pulse value */
    uint16_t ppm = 8;
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