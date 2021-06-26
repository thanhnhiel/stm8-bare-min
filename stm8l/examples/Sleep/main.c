#include <stdint.h>
#include <stm8l.h>
#include <delay.h>

// PCB Green LED PB1
// PCB Red LED PC4
#define LED_PIN     4

#define LED_INIT() { \
       PC_DDR |= (1 << LED_PIN); \
       PC_CR1 |= (1 << LED_PIN); \
       }
      
#define LED_OFF()  PC_ODR &= ~(1 << LED_PIN)
#define LED_ON()  PC_ODR |= (1 << LED_PIN)

#define CLK_SYSCLKDiv 8

void RTC_WakeUpClockConfig(uint8_t RTC_WakeUpClock);
void RTC_ITConfig(uint8_t RTC_IT, uint8_t NewState);
void RTC_SetWakeUpCounter(uint16_t RTC_WakeupCounter);
void RTC_WakeUpCmd(uint8_t NewState);

INTERRUPT_HANDLER(RTC_IRQHandler, RTC_ISR)
{
    /* RTC_ClearITPendingBit */
    RTC_ISR2 = (uint8_t)~(uint8_t)((uint16_t)RTC_IT_WUT >> 4);
}

void main() 
{
    /* HSI clock prescaler */
    CLK_CKDIVR = CLK_SYSCLKDiv_8;
    /* CLK_RTCClockConfig use LSI */
    CLK_CRTCR = (uint8_t)((uint8_t)CLK_RTCCLKSource_LSI | (uint8_t)CLK_RTCCLKDiv_1);
    /* Enable RTC clock */
    CLK_PCKENR2 |= (uint8_t)((uint8_t)1 << 2);
    /* RTC_WakeUpClockConfig */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    /* Enable RTC WUT */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    LED_INIT();
    LED_ON();
    delay_ms(5000);
    LED_OFF();
    
    // Input PU
    //PB_DDR &= (uint8_t)(~(1<<0));
    //PB_CR1 |= (uint8_t)  (1<<0);
    PA_DDR &= (uint8_t)(~(1<<0 | 1<<1 | 1<<2 | 1<<3));
    PA_CR1 |= (uint8_t)  (1<<0 | 1<<1 | 1<<2 | 1<<3);
    PB_DDR &= (uint8_t)(~(1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6  | 1<<7));
    PB_CR1 |= (uint8_t)  (1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    PC_DDR &= (uint8_t)(~(1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<5 | 1<<6));
    PC_CR1 |= (uint8_t)  (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<5 | 1<<6);
    PD_DDR &= (uint8_t)(~(1<<0));
    PD_CR1 |= (uint8_t)  (1<<0);

    enableInterrupts();

    while (1) 
    {
        LED_ON();
        /* Waiting Delay */
        delay_ms(100);
        LED_OFF();

        /* RTC will wake-up from halt every 3*2 second*/
        RTC_SetWakeUpCounter(3);
        RTC_WakeUpCmd(ENABLE);
        /* Enter Halt mode*/
        halt();
        RTC_WakeUpCmd(DISABLE);
    }
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
