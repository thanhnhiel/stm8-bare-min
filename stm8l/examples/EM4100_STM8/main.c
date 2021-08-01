#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8l.h>
#include "uart.h"
#include "delay.h"
//#include "beep.h"

void RTC_WakeUpClockConfig(uint8_t RTC_WakeUpClock);
void RTC_ITConfig(uint8_t RTC_IT, uint8_t NewState);
void RTC_SetWakeUpCounter(uint16_t RTC_WakeupCounter);
void RTC_WakeUpCmd(uint8_t NewState);
void delayMS(uint16_t time);

#define LED_PIN     1
#define LED2_PIN     7
#define LED3_PIN     2

#define PWR_PIN     6

#define PWR_INIT() { \
       PB_DDR |= (1 << PWR_PIN); \
       PB_CR1 |= (1 << PWR_PIN); \
       }

#define PWR_OFF()  PB_ODR &= ~(1 << PWR_PIN)
#define PWR_ON()  PB_ODR |= (1 << PWR_PIN)
#define PWR_TOGGLE()  PB_ODR ^= (1 << PWR_PIN)       


#define LED_INIT() { \
       PC_DDR |= (1 << LED_PIN); \
       PC_CR1 |= (1 << LED_PIN); \
       }

#define LED2_INIT() { \
       PB_DDR |= (1 << LED2_PIN); \
       PB_CR1 |= (1 << LED2_PIN); \
       }

#define LED3_INIT() { \
       PB_DDR |= (1 << LED3_PIN); \
       PB_CR1 |= (1 << LED3_PIN); \
       }              
      
#define LED_ON()  PC_ODR &= ~(1 << LED_PIN)
#define LED_OFF()  PC_ODR |= (1 << LED_PIN)
#define LED_TOGGLE()  PC_ODR ^= (1 << LED_PIN)

// #define LED2_OFF()  PB_ODR &= ~(1 << LED2_PIN)
// #define LED2_ON()  PB_ODR |= (1 << LED2_PIN)
// #define LED2_TOGGLE()  PB_ODR ^= (1 << LED2_PIN)

// #define LED3_OFF()  PB_ODR &= ~(1 << LED3_PIN)
// #define LED3_ON()  PB_ODR |= (1 << LED3_PIN)
// #define LED3_TOGGLE()  PB_ODR ^= (1 << LED3_PIN)

void TIM3_DeInit(void);
void tim2_init();

//void tim3_init();
void TIM3_PWMIConfig(uint8_t icpolarity,
                uint8_t icselection,
                uint8_t ICPrescaler);
void tim3Input_init();
//void tim4_init();
void UART_LowLevel_Init(void);
void process(uint8_t val);
__IO uint8_t tim3_ov = 0;


uint16_t count = 0;
uint8_t bufLen = 0;
uint8_t dataBit = 0;
volatile uint8_t flag = 0;
uint8_t nfcData[8];
uint8_t buf[8];
volatile uint32_t _nfcData = 0;
volatile uint32_t _nfcData1 = 0;
void tim4_init();
volatile uint16_t tm4_count = 0;

void rtc_init();
uint8_t NumOfBits = 0;

void main() 
{
    uint16_t j;
    /* HSI clock prescaler */
    CLK_CKDIVR = CLK_SYSCLKDiv_8;
    rtc_init();

    LED_INIT();
    LED_ON();
    LED2_INIT();
    LED3_INIT();
    PWR_INIT();
  //  LED2_ON();
  //  LED3_OFF();
    PWR_OFF();
    tim4_init();
    enableInterrupts();

    delayMS(2000);
    
    UART_LowLevel_Init();
    uart_init();
    printf("Test\r\n");
   // LED2_OFF();
    LED_OFF();
    PWR_ON();

   tim2_init();
   tim3Input_init();
 
    


    while (1) 
    {
        TIM2_BKR |= TIM_BKR_MOE;
        PWR_ON();
        for (j=0;j<40;j++)      // 0.59 = 200 
        {
            if (flag!=0)
            {
                for (uint8_t i=0;i<8;i++)
                {
                    uart_write(buf[i]);
                }
                flag = 0;
            }
            delayMS(1);
        }

        PWR_OFF();
        TIM2_BKR &= (uint8_t)(~TIM_BKR_MOE);
        /* RTC will wake-up from halt every 3*2 second*/
        RTC_SetWakeUpCounter(2000 * 10 / 4); // 1000 -> 435.0mS
        RTC_WakeUpCmd(ENABLE);
        /* Enter Halt mode*/
        halt();
        RTC_WakeUpCmd(DISABLE);
    }
}

INTERRUPT_HANDLER(RTC_IRQHandler, RTC_ISR)
{
    /* RTC_ClearITPendingBit */
    RTC_ISR2 = (uint8_t)~(uint8_t)((uint16_t)RTC_IT_WUT >> 4);
}

void rtc_init()
{
     /* CLK_RTCClockConfig use LSI */
    CLK_CRTCR = (uint8_t)((uint8_t)CLK_RTCCLKSource_LSI | (uint8_t)CLK_RTCCLKDiv_1);
    /* Enable RTC clock */
    CLK_PCKENR2 |= (uint8_t)((uint8_t)1 << 2);
    /* RTC_WakeUpClockConfig */
   // RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    /* Enable RTC WUT */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
}


INTERRUPT_HANDLER(TIM4_IRQHandler, TIM4_ISR) 
{
    /* Clear the IT pending Bit */
    TIM4_SR = (uint8_t)(~TIM4_IT_UPDATE);
    if (tm4_count) tm4_count--;
    //LED_TOGGLE();
}


void delayMS(uint16_t time)
{
	tm4_count = time;

	/* Reset Counter Register value */
	TIM4_CNTR = (uint8_t)(0);

	/* Enable Timer */
	TIM4_CR1 |= TIM4_CR1_CEN;

	while(tm4_count);
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
    TIM4_ARR = 15;

    //TIM4_IER |= (1 << TIM4_IER_UIE); // Enable Update Interrupt
    TIM4_IER |= (uint8_t)TIM4_IT_UPDATE;

    TIM4_CR1 |= (1 << TIM4_CR1_CEN); // Enable TIM4
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




// INTERRUPT_HANDLER(TIM2_UPD_IRQHandler, TIM2_UPD_ISR)
// {
//     //LED_TOGGLE();
//     TIM2_SR1 = (uint8_t)(~TIM2_IT_UPDATE);
// }

#define Noise      0
#define DoubleHigh 1
#define High       2
#define Low        3
#define StartBit   4


#define INIT  0
#define DATA  1
#define END   2

/*
L H . Detect Falling
Duty : Low 
T:  228
2T: 486
LowDuty : High
T:  283
2T: 540
*/

#define Duty_1T    228
#define Duty_2T    486
#define Delta      110

#define LowDuty_1T    283
#define LowDuty_2T    540


INTERRUPT_HANDLER(TIM3_CC_IRQHandler, TIM3_CC_ISR)
{
    uint8_t tmpccrl, tmpccrh;
    uint16_t Cycle = 0, Duty = 0;
    uint16_t LowDuty = 0;
    uint8_t isStartBit = 0;
    static uint8_t state = INIT;
    static uint8_t Start = 0;
    //volatile uint32_t SignalDutyCycle = 0;
    //volatile uint32_t SignalFrequency = 0;

    // CLear  TIM3_IT_CC1
    TIM3_SR1 = (uint8_t)(~TIM3_IT_CC1);
    tmpccrh = TIM3_CCR1H;
    tmpccrl = TIM3_CCR1L;
    Cycle = (uint16_t)(tmpccrl); // Cycle - T
    Cycle |= (uint16_t)((uint16_t)tmpccrh << 8);

    tmpccrh = TIM3_CCR2H;
    tmpccrl = TIM3_CCR2L;
      

    if (Cycle != 0)
    {
        Duty = (uint16_t)(tmpccrl);
        Duty |= (uint16_t)((uint16_t)tmpccrh << 8);
        LowDuty = Cycle - Duty;

        if (Duty < (Duty_1T - Delta)*2 || Duty > (Duty_2T + Delta)*2)
        {
            Duty = 0; // Noise
        }
        else if (Duty < (Duty_1T + Delta)*2)
        {
            Duty = 1; // 1T
        }
        else
        {
            Duty = 2;  // 2T   
        }

        if (LowDuty < (LowDuty_1T - Delta)*2 || LowDuty > (LowDuty_2T + Delta)*2)
        {
            LowDuty = 0; // Noise
        }
        else if (LowDuty < (LowDuty_1T + Delta)*2)
        {
            LowDuty = 1; // 1T
        }
        else
        {
            LowDuty = 2;  // 2T   
        }
   
        LED_OFF(); // Start Bit

        if (Duty == 2 && LowDuty == 1)
        { // 2T Low - T High  ()
            Start = 1;
            //putchar(Start);
        }
        else if (Duty == 1 && LowDuty == 1)
        { // Low
            Start++;
            //putchar(Start);
        }
        else if (Duty == 1 && LowDuty == 2 && 
                    Start >= 9)
        {   // Start Bit - 1T/2T
            LED_ON();
           // LED2_OFF();
            //putchar(Start);
            Start = 0;
            //putchar(2);
            process(StartBit);
            state = DATA;
        }        
        else
        {
            //putchar(0x0F);
            Start = 0;
        }

        if (state == DATA)
        {
            if (Duty == 0 || LowDuty == 0) 
            {
                state = INIT;
            }
            else 
            {
                //LED3_TOGGLE();
                process(Low);
                if (Duty == 2) process(Low);

                process(High);
                if (LowDuty == 2) process(High);
            }
        }

        // if (bufLen == 1)
        // {
        //     uart_write(NumOfBits);
        //     uart_write(buf[0]);
        //     bufLen = 0;
        // }
        // else if (bufLen == 2)
        // {
        //     uart_write(buf[0]);
        //     uart_write(buf[1]);
        //     uart_write(NumOfBits);
        //     bufLen = 0;
        // }
    }
}

uint8_t i = 0;
uint8_t lastBit = 0;

void process(uint8_t val)
{
    uint8_t tmp = 0;
    if (val == StartBit)
    {
        for (uint8_t i=0;i<8;i++)
        {
            nfcData[i] = 0;
        }
        NumOfBits = 0;

        lastBit = 0;
        i = 0;
        NumOfBits = 0;
    }
    else if (val != Noise)
    {
        i++;
        if (i == 2)
        {
            if (val == Low)
            {
                // if (lastBit != (uint8_t)(NumOfBits >> 3))
                // {
                //     uart_write(nfcData[lastBit]);
                // }
                lastBit = NumOfBits >> 3;
                tmp = nfcData[lastBit];
                tmp <<= 1;
                nfcData[lastBit] = tmp;
                 
                NumOfBits++;
                //buf[bufLen] = 0;
                //bufLen++;
                
            }
            else if (val == High)
            {
                // if (lastBit != (uint8_t)(NumOfBits >> 3))
                // {
                //     uart_write(nfcData[lastBit]);
                // }
                lastBit = NumOfBits >> 3;
                tmp = nfcData[lastBit];
                tmp <<= 1;
                tmp |= 1;
                nfcData[lastBit] = tmp;

                NumOfBits++;
                //buf[bufLen] = 1;
                //bufLen++;
            }
            
            i = 0;
        }

        if (NumOfBits == 64 && flag == 0)
        {
            for (uint8_t i=0;i<8;i++)
            {
                //uart_write(nfcData[i]);
                buf[i] = nfcData[i];
            }
            flag = 1;
        }
    }
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


// void tim4_init()
// {
//     //===============================
//      /* Enable the peripheral Clock */
//     CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << CLK_Peripheral1_TIM4);

//     /* Prescaler = 128 */
//     TIM4_PSCR = TIM2_PRESCALER_1;

//     /* Frequency = F_CLK / (2 * prescaler * (1 + ARR))
//      *           = 2 MHz / (2 * 128 * (1 + 77)) = 100 Hz */
//     TIM4_ARR = 99;

//     //TIM4_IER |= (1 << TIM4_IER_UIE); // Enable Update Interrupt
//     TIM4_IER |= (uint8_t)TIM4_IT_UPDATE;

//     TIM4_CR1 |= (1 << TIM4_CR1_CEN); // Enable TIM4
// }


void TIM3_PWMIConfig(uint8_t icpolarity,
                uint8_t icselection,
                uint8_t ICPrescaler)
{
    /* TI1 Configuration */
    //TI1_Config((TIM3_ICPolarity_TypeDef)TIM3_ICPolarity, (TIM3_ICSelection_TypeDef)TIM3_ICSelection, 0); // ICFilter = 0
    uint8_t tmpccmr1 = TIM3_CCMR1;
    TIM3_CCER1 &=  (uint8_t)(~0x01); // TIM_CCER1_CC1E
    /* Select the Input and set the filter */
    tmpccmr1 &= (uint8_t)(~0x03) & (uint8_t)(~0xF0);
    tmpccmr1 |= (uint8_t)((uint8_t)(icselection));
    TIM3_CCMR1 = tmpccmr1;
    /* Select the Polarity */
    if (icpolarity == (uint8_t)(ICPolarity_Falling))
    {
        TIM3_CCER1 |= 0x02; // TIM_CCER1_CC1P
    }
    else
    {
        TIM3_CCER1 &= (uint8_t)(~0x02); // TIM_CCER1_CC1P
    }
    /* Set the CCE Bit - TIM_CCER1_CC1E*/
    TIM3_CCER1 |=  0x01;
    
    /* Set the Input Capture Prescaler value */
    // TIM3_SetIC1Prescaler(TIM3_ICPrescaler);
    TIM3_CCMR1 = ((uint8_t)TIM3_SMCR & (uint8_t)(~0x0C)) | (uint8_t)ICPrescaler;
    
    /* TI2 Configuration */
    //TI2_Config((TIM3_ICPolarity_TypeDef)icpolarity, (TIM3_ICSelection_TypeDef)icselection, 0); // ICFilter = 0
    tmpccmr1 = TIM3_CCMR2;
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM3_CCER1 &=  (uint8_t)(~0x10); // TIM_CCER1_CC2E
    /* Select the Input and set the filter */
    tmpccmr1 &= (uint8_t)(~0x03) & (uint8_t)(~0xF0);
    if (icselection == ICSelection_DirectTI)
    {
        tmpccmr1 |= (uint8_t)((uint8_t)(ICSelection_IndirectTI)); // ICFilter = 0
    }
    else
    {
        tmpccmr1 |= (uint8_t)((uint8_t)(ICSelection_DirectTI)); // ICFilter = 0
    }
    
    TIM3_CCMR2 = tmpccmr1;
    /* Select the Polarity */
    if (icpolarity == ICPolarity_Rising)
    {
        TIM3_CCER1 |= 0x20; // TIM_CCER1_CC2P
    }
    else
    {
        TIM3_CCER1 &= (uint8_t)(~0x20); // TIM_CCER1_CC2P
    }
    /* Set the CCE Bit */
    TIM3_CCER1 |=  0x10; //TIM_CCER1_CC2E
    
    /* Set the Input Capture Prescaler value */
    //TIM3_SetIC2Prescaler(TIM3_ICPrescaler);
    TIM3_CCMR2 = ((uint8_t)TIM3_SMCR & (uint8_t)(~0x0C)) | (uint8_t)ICPrescaler;  
}

void tim3Input_init()
{
    //GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Clear Data */
    // PB_ODR &= (uint8_t)(~(1 << 1));
    // /* Set Output mode */
    // PB_DDR |= (uint8_t)(1 << 1);
    // /* Push-Pull/Open-Drain (Output) modes selection */
    // PB_CR1 |= (uint8_t)(1 << 1);
    // /* Slow slope */
    // PB_CR2 |= (uint8_t)(1 << 1);
    // CLK_Peripheral_TIM3 = 1 , Enable
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << (uint8_t)0x01);

    /* configure TIM1 channel 1 to capture a PWM signal */
    //TIM3_PWMIConfig(TIM3_Channel_1, TIM3_ICPolarity_Rising, TIM3_ICSelection_DirectTI,
    //             TIM3_ICPSC_DIV1, ICFilter);
    TIM3_PWMIConfig(ICPolarity_Falling, ICSelection_DirectTI, ICPSC_DIV1);
    
    /* Select the TIM1 Input Trigger: TI1FP1 */
    //TIM3_SelectInputTrigger(TIM3_TRGSelection_TI1FP1 - 0x50);
    TIM3_SMCR = ((uint8_t)TIM3_SMCR & (uint8_t)(~TIM_SMCR_TS)) | (uint8_t)0x50;
    //TIM3_SelectSlaveMode(TIM3_SlaveMode_Reset - 0x04);
    TIM3_SMCR = ((uint8_t)TIM3_SMCR & (uint8_t)(~TIM_SMCR_SMS)) | (uint8_t)0x04;

    /* Enable CC1 interrupt request */
    //TIM3_ITConfig(TIM3_IT_CC1, ENABLE); // TIM3_IT_CC1
    TIM3_IER |= (uint8_t)0x02;
    //enableInterrupts();

    /* Enable TIM1 */
    //TIM3_Cmd(ENABLE) - TIM_CR1_CEN;
    TIM3_CR1 |= 0x01;
}

//#define PWM_PIN 0
#define PWM_PIN 2

void tim2_init()
{
    /* REMAP_Pin_TIM2Channel1: TIM2 Channel 1 (PB0) remapping to PC5
     * REMAP_Pin_TIM2Channel2: TIM2 Channel 2 (PB2) remapping to PC6
     */
    // 0b1110 0000
	//GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Clear Data */
    PB_ODR &= (uint8_t)(~(1 << PWM_PIN));
    /* Set Output mode */
    PB_DDR |= (uint8_t)(1 << PWM_PIN);
    /* Push-Pull/Open-Drain (Output) modes selection */
    PB_CR1 |= (uint8_t)(1 << PWM_PIN);
    /* Slow slope */
    PB_CR2 |= (uint8_t)(1 << PWM_PIN);

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
  
  #if 0
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
 //   TIM2_IER |= (uint8_t)TIM2_IT_UPDATE;

    //TIM2_OC1PreloadConfig(ENABLE);
    TIM2_CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE;
#endif
	/* Channel 1 PWM configuration */ 
    //TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ppm, TIM2_OCPOLARITY_HIGH);
    /* Disable the Channel 1 - PB2 : Reset the CCE Bit, Set the Output State , the Output Polarity */
    TIM2_CCER1 &= (uint8_t)(~(TIM2_CCER1_CC2E)); 

    /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
    TIM2_CCMR2 = (uint8_t)((TIM2_CCMR1 & (uint8_t)(~(TIM2_CCMR_OCM))) | (uint8_t)TIM2_OCMODE_PWM1);

    /* Set the Output State */
    TIM2_CCER1 |= TIM2_CCER1_CC2E;

    /* Set the Output Polarity */
    TIM2_CCER1 |= TIM2_CCER1_CC2P;

    /* Set the Output Idle state */
    //TIM2_OISR |= TIM2_OISR_OIS2;

    /* Set the Pulse value */
    uint16_t ppm = 8;
    TIM2_CCR2H = (uint8_t)(ppm >> 8);
    TIM2_CCR2L = (uint8_t)(ppm);

    /* Enable TIM2 */
    //TIM2_ITConfig(TIM2_IT_UPDATE ,ENABLE);
 //   TIM2_IER |= (uint8_t)TIM2_IT_UPDATE;

    //TIM2_OC2PreloadConfig(ENABLE);
    TIM2_CCMR2 |= (uint8_t)TIM2_CCMR_OCxPE;
    //===============================================

	/* Enables TIM2 peripheral Preload register on ARR */
	//TIM2_ARRPreloadConfig(ENABLE);
	TIM2_CR1 |= (uint8_t)TIM2_CR1_ARPE;

    /* Enable TIM2 */
    TIM2_CR1 |= (uint8_t)TIM2_CR1_CEN;

    /* TIM2_Ctrl PWM Outputs */
    TIM2_BKR |= TIM_BKR_MOE;
}


void RTC_WakeUpClockConfig(uint8_t RTC_WakeUpClock)
{
  /* Disable the write protection for RTC registers */
  RTC_WPR = 0xCA;
  RTC_WPR = 0x53;

  /* Disable the Wakeup timer in RTC_CR2 register */
  RTC_CR2 &= (uint8_t)~0x04;

  /* Clear the Wakeup Timer clock source bits in CR1 register */
  RTC_CR1 &= (uint8_t)~0x07;

  /* Configure the clock source */
  RTC_CR1 |= (uint8_t)RTC_WakeUpClock;

  /* Enable the write protection for RTC registers */
  RTC_WPR = 0xFF; 
}

void RTC_ITConfig(uint8_t RTC_IT, uint8_t NewState)
{
  /* Disable the write protection for RTC registers */
  RTC_WPR = 0xCA;
  RTC_WPR = 0x53;

  if (NewState != 0)
  {
    /* Enable the Interrupts */
    RTC_CR2 |= (uint8_t)((uint8_t)RTC_IT & (uint8_t)0xF0);
    RTC_TCR1 |= (uint8_t)((uint8_t)RTC_IT & 0x01);
  }
  else
  {
    /* Disable the Interrupts */
    RTC_CR2  &= (uint8_t)~(uint8_t)((uint8_t)RTC_IT & (uint8_t)0x00F0);
    RTC_TCR1 &= (uint8_t)~(uint8_t)((uint8_t)RTC_IT & (uint8_t)0x01);
  }

  /* Enable the write protection for RTC registers */
  RTC_WPR = 0xFF; 
}

void RTC_SetWakeUpCounter(uint16_t RTC_WakeupCounter)
{
  /* Disable the write protection for RTC registers */
  RTC_WPR = 0xCA;
  RTC_WPR = 0x53;


  /* Configure the Wakeup Timer counter */
  RTC_WUTRH = (uint8_t)(RTC_WakeupCounter >> 8);
  RTC_WUTRL = (uint8_t)(RTC_WakeupCounter);

  /* Enable the write protection for RTC registers */
  RTC_WPR = 0xFF; 
}

void RTC_WakeUpCmd(uint8_t NewState)
{
  //ErrorStatus status = ERROR;
  uint16_t wutwfcount = 0;

  /* Disable the write protection for RTC registers */
  RTC_WPR = 0xCA;
  RTC_WPR = 0x53;

  if (NewState != 0)
  {
    /* Enable the Wakeup Timer */
    RTC_CR2 |= (uint8_t)(1<<RTC_CR2_WUTE);

    //status = SUCCESS;
  }
  else
  {
    /* Disable the Wakeup Timer */
    RTC_CR2 &= (uint8_t)~(1<<RTC_CR2_WUTE);

    /* Wait until WUTWF flag is set */
    while (((RTC_ISR1 & 1<<RTC_ISR1_WUTWF) == 0) && ( wutwfcount != WUTWF_TIMEOUT))
    {
      wutwfcount++;
    }

    /* Check WUTWF flag is set or not */
    if ((RTC_ISR1 & 1<<RTC_ISR1_WUTWF) == 0)
    {
      //status = ERROR;
    }
    else
    {
      //status = SUCCESS;
    }
  }

  /* Enable the write protection for RTC registers */
  RTC_WPR = 0xFF; 
}