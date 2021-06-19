#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm8s.h>
#include <uart.h>
#include <delay.h>

#define AWU_TIMEBASE_NO_IT  ((uint8_t)0)    /*!< No AWU interrupt selected */
#define AWU_TIMEBASE_250US  ((uint8_t)1)    /*!< AWU Timebase equals 0.25 ms */
#define AWU_TIMEBASE_500US  ((uint8_t)2)    /*!< AWU Timebase equals 0.5 ms */
#define AWU_TIMEBASE_1MS    ((uint8_t)3)    /*!< AWU Timebase equals 1 ms */
#define AWU_TIMEBASE_2MS    ((uint8_t)4)    /*!< AWU Timebase equals 2 ms */
#define AWU_TIMEBASE_4MS    ((uint8_t)5)    /*!< AWU Timebase equals 4 ms */
#define AWU_TIMEBASE_8MS    ((uint8_t)6)    /*!< AWU Timebase equals 8 ms */
#define AWU_TIMEBASE_16MS   ((uint8_t)7)    /*!< AWU Timebase equals 16 ms */
#define AWU_TIMEBASE_32MS   ((uint8_t)8)    /*!< AWU Timebase equals 32 ms */
#define AWU_TIMEBASE_64MS   ((uint8_t)9)    /*!< AWU Timebase equals 64 ms */
#define AWU_TIMEBASE_128MS  ((uint8_t)10)   /*!< AWU Timebase equals 128 ms */
#define AWU_TIMEBASE_256MS  ((uint8_t)11)   /*!< AWU Timebase equals 256 ms */
#define AWU_TIMEBASE_512MS  ((uint8_t)12)   /*!< AWU Timebase equals 512 ms */
#define AWU_TIMEBASE_1S     ((uint8_t)13)   /*!< AWU Timebase equals 1 s */
#define AWU_TIMEBASE_2S     ((uint8_t)14)   /*!< AWU Timebase equals 2 s */
#define AWU_TIMEBASE_12S    ((uint8_t)15)   /*!< AWU Timebase equals 12 s */
#define AWU_TIMEBASE_30S    ((uint8_t)16)   /*!< AWU Timebase equals 30 s */

#define LED_PIN     5

#define LED_INIT() { \
       PB_DDR |= (1 << LED_PIN); \
       PB_CR1 |= (1 << LED_PIN); \
       }
      
#define LED_ON()  PB_ODR &= ~(1 << LED_PIN)
#define LED_OFF()  PB_ODR |= (1 << LED_PIN)

uint32_t LSIMeasurment(void);
void TIM1_ICInit();
static void TI1_Config();
uint16_t TIM1_GetCapture1(void);
void TIM1_ClearFlag(uint8_t flag);
void AWU_LSICalibrationConfig(uint32_t LSIFreqHz);
void AWU_Config(uint8_t awu_time);
uint8_t AWU_GetFlagStatus(void);

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

void timer_isr() __interrupt(AWU_ISR) 
{
    AWU_GetFlagStatus();
}

/* 12 bytes */
volatile uint8_t *getUniqueId()
{
    return (volatile uint8_t *)UNIQUE_ID_ADDRESS;
}

void main() 
{
    /* HSI Max : 16Mhz */
    CLK_CKDIVR &= (uint8_t)(~0x18); // 16 / 8 = 2MHz
    CLK_CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV8;



    /* Set Input mode */
    //PB_DDR &= (uint8_t)(~(GPIO_Pin));
    /* Pull-Up */
    //PB_CR1 |= (uint8_t)GPIO_Pin;
    /* No Interrupt */
    //PB_CR2 &= (uint8_t)~(GPIO_Pin);

    // Input PU
    PA_DDR &= (uint8_t)(~(1<<1 | 1<<2 | 1<<3));
    PA_CR1 |= (uint8_t)(1<<1 | 1<<2 | 1<<3);
    PB_DDR &= (uint8_t)(~(1<<4 | 1<<5));
    PB_CR1 |= (uint8_t)(1<<4 | 1<<5);
    PC_DDR &= (uint8_t)(~(1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7 ));
    PC_CR1 |= (uint8_t)(1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    PD_DDR &= (uint8_t)(~(1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6));
    PD_CR1 |= (uint8_t)(1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6);

    LED_INIT();
    LED_ON();

    uart_init();

    volatile uint8_t *id = getUniqueId();
    printf("Mcu Inited Unique Id: ");
    for (int i=0;i<12;i++)
    {
      printf("%x ", id[i]);  
    }
    printf("\r\n");
    
    delay_ms(4000);
    LED_OFF();

    AWU_Config(AWU_TIMEBASE_12S);
    enableInterrupts();

    while (1) 
    {
        halt(); /* Program halted */
        LED_ON();
        delay_ms(100);
        LED_OFF();
    }
}

uint16_t TIM1_GetCapture1(void)
{
    /* Get the Capture 1 Register value */
    uint16_t tmpccr1 = 0;
    uint8_t tmpccr1l=0, tmpccr1h=0;
    
    tmpccr1h = TIM1_CCR1H;
    tmpccr1l = TIM1_CCR1L;
    
    tmpccr1 = (uint16_t)(tmpccr1l);
    tmpccr1 |= (uint16_t)((uint16_t)tmpccr1h << 8);
    /* Get the Capture 1 Register value */
    return (uint16_t)tmpccr1;
}

static void TI1_Config()
{
    /* Disable the Channel 1: Reset the CCE Bit - TIM1_CCER1_CC1E 0x01*/
    TIM1_CCER1 &= (uint8_t)(~0x01);
    
    /* Select the Input and set the filter - TIM1_ICSELECTION_DIRECTTI 0x01*/
    TIM1_CCMR1 = (uint8_t)((uint8_t)(TIM1_CCMR1 & (uint8_t)(~(uint8_t)( 0x03 | 0xF0 ))) | 
                            (uint8_t)(( (0x01))));
    
    /* Select the Polarity - TIM1_ICPOLARITY_RISING */
    TIM1_CCER1 &= (uint8_t)(~0x02);
    
    
    /* Set the CCE Bit */
    TIM1_CCER1 |=  0x01;
}

void TIM1_ICInit()
{
    /* TI1 Configuration */
    /* Disable the Channel 1: Reset the CCE Bit - TIM1_CCER1_CC1E 0x01*/
    TIM1_CCER1 &= (uint8_t)(~0x01);
    
    /* Select the Input and set the filter - TIM1_ICSELECTION_DIRECTTI 0x01*/
    TIM1_CCMR1 = (uint8_t)((uint8_t)(TIM1_CCMR1 & (uint8_t)(~(uint8_t)( 0x03 | 0xF0 ))) | 
                            (uint8_t)(( (0x01))));
    /* Select the Polarity - TIM1_ICPOLARITY_RISING */
    TIM1_CCER1 &= (uint8_t)(~0x02);
    
    /* Set the CCE Bit */
    TIM1_CCER1 |=  0x01;
    
    /* Set the Input Capture Prescaler value */
    //TIM1_SetIC1Prescaler(TIM1_ICPSC_DIV8);
    TIM1_CCMR1 = (uint8_t)((uint8_t)(TIM1_CCMR1 & (uint8_t)(~0x0C)) 
                          | (uint8_t)0x0C);
}

void TIM1_ClearFlag(uint8_t flag)
{
  /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
  TIM1_SR1 = (uint8_t)(~(uint8_t)(flag));
  TIM1_SR2 = (uint8_t)((uint8_t)(~((uint8_t)((uint16_t)flag >> 8))) & (uint8_t)0x1E);
}


/**
  * @brief  Measure the LSI frequency using timer IC1 and update the calibration registers.
  * @note   It is recommended to use a timer clock frequency of at least 10MHz in order 
	*         to obtain a better in the LSI frequency measurement.
	* @param  None
  * @retval None
  */
uint32_t LSIMeasurment(void)
{
    uint32_t lsi_freq_hz = 0x0;
    uint16_t ICValue1 = 0x0;
    uint16_t ICValue2 = 0x0;
    
    /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 - AWU_CSR_MSR */
    AWU_CSR |= 0x01;
    
    /* Measure the LSI frequency with TIMER Input Capture 1 */
    
    /* Capture only every 8 events!!! */
    /* Enable capture of TI1 */
    // TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV8, 0
    TIM1_ICInit();
    
    /* Enable TIM1 */
    TIM1_CR1 |= 0x01;
    
    /* wait a capture on TIM1_FLAG_CC1 */
    while((TIM1_SR1 & 0x0002) != 0x0002);
    /* Get CCR1 value*/
    ICValue1 = TIM1_GetCapture1();
    TIM1_ClearFlag(0x0002);
    
    /* wait a capture on cc1 */
    while((TIM1_SR1 & 0x0002) != 0x0002);
    /* Get CCR1 value*/
    ICValue2 = TIM1_GetCapture1();
    TIM1_ClearFlag(0x0002);
    
    /* Disable IC1 input capture TIM1_CCER1_CC1E */
    TIM1_CCER1 &= (uint8_t)(~0x01);
    /* Disable timer2 TIM1_CR1_CEN */
    TIM1_CR1 &= (uint8_t)(~0x01);
    
    
    /* Compute LSI clock frequency */
    lsi_freq_hz = (8 * (uint32_t)F_CPU) / (ICValue2 - ICValue1);
    
    /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 - AWU_CSR_MSR */
    AWU_CSR &= (uint8_t)(~0x01);
    
    return (lsi_freq_hz);
}

void AWU_LSICalibrationConfig(uint32_t LSIFreqHz)
{
  uint16_t lsifreqkhz = 0x0;
  uint16_t A = 0x0;
  
  lsifreqkhz = (uint16_t)(LSIFreqHz / 1000); /* Converts value in kHz */
  
  /* Calculation of AWU calibration value */
  
  A = (uint16_t)(lsifreqkhz >> 2U); /* Division by 4, keep integer part only */
  
  if ((4U * A) >= ((lsifreqkhz - (4U * A)) * (1U + (2U * A))))
  {
    AWU_APR = (uint8_t)(A - 2U);
  }
  else
  {
    AWU_APR = (uint8_t)(A - 1U);
  }
}

/** Contains the different values to write in the APR register (used by AWU_Init function) */
const uint8_t APR_Array[17] =
{
    0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 61, 23, 23, 62
};
/** Contains the different values to write in the TBR register (used by AWU_Init function) */
const uint8_t TBR_Array[17] =
{
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 12, 14, 15, 15
};

void AWU_Config(uint8_t awu_time)
{
    uint32_t lsi_measurement = 0;

    /* Initialization of AWU */
    /* LSI calibration for accurate auto wake up time base*/
    lsi_measurement = LSIMeasurment();
    AWU_LSICalibrationConfig(lsi_measurement);
    //printf("Mcu Inited. LSI: %ld\r\n", lsi_measurement);
    
    /* The delay corresponds to the time we will stay in Halt mode */
    //AWU_Init(AWU_TIMEBASE_12S);
    /* Enable the AWU peripheral - AWU_CSR_AWUEN*/
    AWU_CSR |= 0x10;
    /* Set the TimeBase - AWU_TBR_AWUTB */
    AWU_TBR &= (uint8_t)(~0x0F);
    AWU_TBR |= TBR_Array[(uint8_t)awu_time];
    /* Set the APR divider - AWU_APR_APR */
    AWU_APR &= (uint8_t)(~0x3F);
    AWU_APR |= APR_Array[(uint8_t)awu_time];
}

uint8_t AWU_GetFlagStatus(void)
{
  return ((((uint8_t)(AWU_CSR & 0x20) == (uint8_t)0x00) ? 0 : 1));
}
