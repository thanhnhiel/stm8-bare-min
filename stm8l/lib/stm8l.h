#ifndef STM8L_H
#define STM8L_H

#include <stdint.h>

#define INTERRUPT_HANDLER(a,b) void a() __interrupt(b)

 /* traps require >=v3.4.3 -> else warn and skip */
 #if SDCC_VERSION >= 30403
   #define INTERRUPT_HANDLER_TRAP(a) void a() __trap 
 #else
   #warning traps require SDCC>=v3.4.3. Update if required
   #define INTERRUPT_HANDLER_TRAP(a) void a()
 #endif 
 
 #define enableInterrupts()    __asm__("rim")    /* enable interrupts */
 #define disableInterrupts()   __asm__("sim")    /* disable interrupts */
 #define rim()                 __asm__("rim")    /* enable interrupts */
 #define sim()                 __asm__("sim")    /* disable interrupts */
 #define nop()                 __asm__("nop")    /* no operation */
 #define trap()                __asm__("trap")   /* trap (soft IT) */
 #define wfi()                 __asm__("wfi")    /* wait for interrupt */
 #define wfe()                 __asm__("wfe")    /* wait for event */
 #define halt()                __asm__("halt")   /* halt CPU */

//#define DISABLE (0)
//#define ENABLE (1)
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


/** @defgroup EXTI_Interrupts
  *
  * @brief EXTI IT pending bit possible values
  * Values are coded in 0xXY format where
  * X: the register index
  *    X = 00:  EXTI_SR1
  *    X = 01:  EXTI_SR2
  * Y: the IT pending bit mask
  * @{
  */
typedef enum
{
  EXTI_IT_Pin0    = (uint16_t)0x0001, /*!< GPIO Pin pos 0 */
  EXTI_IT_Pin1    = (uint16_t)0x0002, /*!< GPIO Pin pos 1 */
  EXTI_IT_Pin2    = (uint16_t)0x0004, /*!< GPIO Pin pos 2 */
  EXTI_IT_Pin3    = (uint16_t)0x0008, /*!< GPIO Pin pos 3 */
  EXTI_IT_Pin4    = (uint16_t)0x0010, /*!< GPIO Pin pos 4 */
  EXTI_IT_Pin5    = (uint16_t)0x0020, /*!< GPIO Pin pos 5 */
  EXTI_IT_Pin6    = (uint16_t)0x0040, /*!< GPIO Pin pos 6 */
  EXTI_IT_Pin7    = (uint16_t)0x0080, /*!< GPIO Pin pos 7 */
  EXTI_IT_PortB   = (uint16_t)0x0101, /*!< GPIO Port B    */
  EXTI_IT_PortD   = (uint16_t)0x0102, /*!< GPIO Port D    */
  EXTI_IT_PortE   = (uint16_t)0x0104, /*!< GPIO Port E    */
  EXTI_IT_PortF   = (uint16_t)0x0108, /*!< GPIO Port F    */
  EXTI_IT_PortG   = (uint16_t)0x0110, /*!< GPIO Port G    */
  EXTI_IT_PortH   = (uint16_t)0x0120  /*!< GPIO Port H    */
} EXTI_IT_TypeDef;


#define     __IO    volatile         /*!< defines 'read / write' permissions  */

typedef enum {
  CLK_Peripheral1_TIM2    = (uint8_t)0x00, /*!< Peripheral Clock Enable 1, TIM2 */
  CLK_Peripheral1_TIM3    = (uint8_t)0x01, /*!< Peripheral Clock Enable 1, TIM3 */
  CLK_Peripheral1_TIM4    = (uint8_t)0x02, /*!< Peripheral Clock Enable 1, TIM4 */
  CLK_Peripheral1_I2C1    = (uint8_t)0x03, /*!< Peripheral Clock Enable 1, I2C1 */
  CLK_Peripheral1_SPI1    = (uint8_t)0x04, /*!< Peripheral Clock Enable 1, SPI1 */
  CLK_Peripheral1_USART1  = (uint8_t)0x05, /*!< Peripheral Clock Enable 1, USART1 */
  CLK_Peripheral1_BEEP    = (uint8_t)0x06, /*!< Peripheral Clock Enable 1, BEEP */
  CLK_Peripheral1_DAC     = (uint8_t)0x07, /*!< Peripheral Clock Enable 1, DAC */
  CLK_Peripheral2_ADC1    = (uint8_t)0x10, /*!< Peripheral Clock Enable 2, ADC1 */
  CLK_Peripheral2_TIM1    = (uint8_t)0x11, /*!< Peripheral Clock Enable 2, TIM1 */
  CLK_Peripheral2_RTC     = (uint8_t)0x12, /*!< Peripheral Clock Enable 2, RTC */
  CLK_Peripheral2_LCD     = (uint8_t)0x13, /*!< Peripheral Clock Enable 2, LCD */
  CLK_Peripheral2_DMA1    = (uint8_t)0x14, /*!< Peripheral Clock Enable 2, DMA1 */
  CLK_Peripheral2_COMP    = (uint8_t)0x15, /*!< Peripheral Clock Enable 2, COMP1 and COMP2 */
  CLK_Peripheral2_BOOTROM = (uint8_t)0x17,/*!< Peripheral Clock Enable 2, Boot ROM */
  CLK_Peripheral3_AES     = (uint8_t)0x20, /*!< Peripheral Clock Enable 3, AES */
  CLK_Peripheral3_TIM5    = (uint8_t)0x21, /*!< Peripheral Clock Enable 3, TIM5 */
  CLK_Peripheral3_SPI2    = (uint8_t)0x22, /*!< Peripheral Clock Enable 3, SPI2 */
  CLK_Peripheral3_USART2  = (uint8_t)0x23, /*!< Peripheral Clock Enable 3, USART2 */
  CLK_Peripheral3_USART3  = (uint8_t)0x24,  /*!< Peripheral Clock Enable 3, USART3 */
  CLK_Peripheral3_CSSLSE  = (uint8_t)0x25   /*!< Peripheral Clock Enable 3, CSS on LSE */
} CLK_Peripheral_TypeDef;

#define _MEM_(mem_addr)                 (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr)                 (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define _SFR16_(mem_addr)               (*(volatile uint16_t *)(0x5000 + (mem_addr)))

typedef enum
{
  EXTI_Trigger_Falling_Low    = (uint8_t)0x00, /*!< Interrupt on Falling edge and Low level */
  EXTI_Trigger_Rising         = (uint8_t)0x01, /*!< Interrupt on Rising edge only */
  EXTI_Trigger_Falling        = (uint8_t)0x02, /*!< Interrupt on Falling edge only */
  EXTI_Trigger_Rising_Falling = (uint8_t)0x03  /*!< Interrupt on Rising and Falling edges */
} EXTI_Trigger_TypeDef;

typedef enum
{
  EXTI_Pin_0 = (uint8_t)0x00, /*!< GPIO Pin 0 */
  EXTI_Pin_1 = (uint8_t)0x02, /*!< GPIO Pin 1 */
  EXTI_Pin_2 = (uint8_t)0x04, /*!< GPIO Pin 2 */
  EXTI_Pin_3 = (uint8_t)0x06, /*!< GPIO Pin 3 */
  EXTI_Pin_4 = (uint8_t)0x10, /*!< GPIO Pin 4 */
  EXTI_Pin_5 = (uint8_t)0x12, /*!< GPIO Pin 5 */
  EXTI_Pin_6 = (uint8_t)0x14, /*!< GPIO Pin 6 */
  EXTI_Pin_7 = (uint8_t)0x16  /*!< GPIO Pin 7 */
} EXTI_Pin_TypeDef;

/** @addtogroup EXTI_Registers_Bits_Definition
  * @{
  */
/* CR1 */
#define EXTI_CR1_P3IS      ((uint8_t)0xC0) /*!< EXTI Pin 3 external interrupt sensitivity bit Mask */
#define EXTI_CR1_P2IS      ((uint8_t)0x30) /*!< EXTI Pin 2 external interrupt sensitivity bit Mask */
#define EXTI_CR1_P1IS      ((uint8_t)0x0C) /*!< EXTI Pin 1  external interrupt sensitivity bit Mask */
#define EXTI_CR1_P0IS      ((uint8_t)0x03) /*!< EXTI Pin 0 external interrupt sensitivity bit Mask */

/* CR2 */
#define EXTI_CR2_P7IS      ((uint8_t)0xC0) /*!< EXTI Pin 7 external interrupt sensitivity bit Mask */
#define EXTI_CR2_P6IS      ((uint8_t)0x30) /*!< EXTI Pin 6 external interrupt sensitivity bit Mask */
#define EXTI_CR2_P5IS      ((uint8_t)0x0C) /*!< EXTI Pin 5  external interrupt sensitivity bit Mask */
#define EXTI_CR2_P4IS      ((uint8_t)0x03) /*!< EXTI Pin 4 external interrupt sensitivity bit Mask */

/* CR3 */
#define EXTI_CR3_PBIS      ((uint8_t)0x03) /*!< EXTI PORTB external interrupt sensitivity bits Mask */
#define EXTI_CR3_PDIS      ((uint8_t)0x0C) /*!< EXTI PORTD external interrupt sensitivity bits Mask */
#define EXTI_CR3_PEIS      ((uint8_t)0x30) /*!< EXTI PORTE external interrupt sensitivity bits Mask */
#define EXTI_CR3_PFIS      ((uint8_t)0xC0) /*!< EXTI PORTF external interrupt sensitivity bits Mask */


/* PORT A */
#define PA_ODR                          _SFR_(0x00)
#define PA_IDR                          _SFR_(0x01)
#define PA_DDR                          _SFR_(0x02)
#define PA_CR1                          _SFR_(0x03)
#define PA_CR2                          _SFR_(0x04)

/* PORT B */
#define PB_ODR                          _SFR_(0x05)
#define PB_IDR                          _SFR_(0x06)
#define PB_DDR                          _SFR_(0x07)
#define PB_CR1                          _SFR_(0x08)
#define PB_CR2                          _SFR_(0x09)

/* PORT C */
#define PC_ODR                          _SFR_(0x0A)
#define PC_IDR                          _SFR_(0x0B)
#define PC_DDR                          _SFR_(0x0C)
#define PC_CR1                          _SFR_(0x0D)
#define PC_CR2                          _SFR_(0x0E)

/* PORT D */
#define PD_ODR                          _SFR_(0x0F)
#define PD_IDR                          _SFR_(0x10)
#define PD_DDR                          _SFR_(0x11)
#define PD_CR1                          _SFR_(0x12)
#define PD_CR2                          _SFR_(0x13)

/* PORT E */
#define PE_ODR                          _SFR_(0x14)
#define PE_IDR                          _SFR_(0x15)
#define PE_DDR                          _SFR_(0x16)
#define PE_CR1                          _SFR_(0x17)
#define PE_CR2                          _SFR_(0x18)

/* PORT F */
#define PF_ODR                          _SFR_(0x19)
#define PF_IDR                          _SFR_(0x1A)
#define PF_DDR                          _SFR_(0x1B)
#define PF_CR1                          _SFR_(0x1C)
#define PF_CR2                          _SFR_(0x1D)

/* Flash */
#define FLASH_CR1                       _SFR_(0x50)
#define FLASH_CR1_EEPM                  3
#define FLASH_CR1_WAITM                 2
#define FLASH_CR1_IE                    1
#define FLASH_CR1_FIX                   0
#define FLASH_CR2                       _SFR_(0x51)
#define FLASH_CR2_OPT                   7
#define FLASH_CR2_WPRG                  6
#define FLASH_CR2_ERASE                 5
#define FLASH_CR2_FPRG                  4
#define FLASH_CR2_PRG                   0
#define FLASH_PUKR                      _SFR_(0x52)
#define FLASH_PUKR_KEY1                 0x56
#define FLASH_PUKR_KEY2                 0xAE
#define FLASH_DUKR                      _SFR_(0x53)
#define FLASH_DUKR_KEY1                 FLASH_PUKR_KEY2
#define FLASH_DUKR_KEY2                 FLASH_PUKR_KEY1
#define FLASH_IAPSR                     _SFR_(0x54)
#define FLASH_IAPSR_HVOFF               6
#define FLASH_IAPSR_DUL                 3
#define FLASH_IAPSR_EOP                 2
#define FLASH_IAPSR_PUL                 1
#define FLASH_IAPSR_WR_PG_DIS           0

/* DMA1 */
#define DMA1_GCSR                       _SFR_(0x70)
#define DMA1_GIR1                       _SFR_(0x71)
#define DMA1_C0CR                       _SFR_(0x75)
#define DMA1_C0SPR                      _SFR_(0x76)
#define DMA1_C0NDTR                     _SFR_(0x77)
#define DMA1_C0PARH                     _SFR_(0x78)
#define DMA1_C0PARL                     _SFR_(0x79)
#define DMA1_C0M0ARH                    _SFR_(0x7B)
#define DMA1_C0M0ARL                    _SFR_(0x7C)
#define DMA1_C1CR                       _SFR_(0x7F)
#define DMA1_C1SPR                      _SFR_(0x80)
#define DMA1_C1NDTR                     _SFR_(0x81)
#define DMA1_C1PARH                     _SFR_(0x82)
#define DMA1_C1PARL                     _SFR_(0x83)
#define DMA1_C1M0ARH                    _SFR_(0x85)
#define DMA1_C1M0ARL                    _SFR_(0x86)
#define DMA1_C2CR                       _SFR_(0x89)
#define DMA1_C2SPR                      _SFR_(0x8A)
#define DMA1_C2NDTR                     _SFR_(0x8B)
#define DMA1_C2PARH                     _SFR_(0x8C)
#define DMA1_C2PARL                     _SFR_(0x8D)
#define DMA1_C2M0ARH                    _SFR_(0x8F)
#define DMA1_C2M0ARL                    _SFR_(0x90)
#define DMA1_C3CR                       _SFR_(0x93)
#define DMA1_C3SPR                      _SFR_(0x94)
#define DMA1_C3NDTR                     _SFR_(0x95)
#define DMA1_C3PARH_C3M1ARH             _SFR_(0x96)
#define DMA1_C3PARL_C3M1ARL             _SFR_(0x97)
#define DMA1_C3M0EAR                    _SFR_(0x98)
#define DMA1_C3M0ARH                    _SFR_(0x99)
#define DMA1_C3M0ARL                    _SFR_(0x9A)

/* SYSCFG */
#define SYSCFG_RMPCR3                   _SFR_(0x9D)
#define SYSCFG_RMPCR1                   _SFR_(0x9E)
#define SYSCFG_RMPCR1_SPI1_REMAP        7
#define SYSCFG_RMPCR1_USART1CK_REMAP    6
#define SYSCFG_RMPCR1_USART1TR_REMAP1   5
#define SYSCFG_RMPCR1_USART1TR_REMAP0   4
#define SYSCFG_RMPCR1_TIM4DMA_REMAP1    3
#define SYSCFG_RMPCR1_TIM4DMA_REMAP0    2
#define SYSCFG_RMPCR1_ADC1DMA_REAMP1    1
#define SYSCFG_RMPCR1_ADC1DMA_REAMP0    0
#define SYSCFG_RMPCR2                   _SFR_(0x9F)

/* ITC */
#define EXTI_CR1                        _SFR_(0xA0)
#define EXTI_CR2                        _SFR_(0xA1)
#define EXTI_CR3                        _SFR_(0xA2)
#define EXTI_SR1                        _SFR_(0xA3)
#define EXTI_SR2                        _SFR_(0xA4)
#define EXTI_CONF1                      _SFR_(0xA5)
#define WFE_CR1                         _SFR_(0xA6)
#define WFE_CR1_EXTI_EV3                7
#define WFE_CR1_EXTI_EV2                6
#define WFE_CR1_EXTI_EV1                5
#define WFE_CR1_EXTI_EV0                4
#define WFE_CR1_TIM1_EV1                3
#define WFE_CR1_TIM1_EV0                2
#define WFE_CR1_TIM2_EV1                1
#define WFE_CR1_TIM2_EV0                0
#define WFE_CR2                         _SFR_(0xA7)
#define WFE_CR2_ADC1_COMP_EV            7
#define WFE_CR2_EXTI_EVE_F              6
#define WFE_CR2_EXTI_EVD                5
#define WFE_CR2_EXTI_EVB                4
#define WFE_CR2_EXTI_EV7                3
#define WFE_CR2_EXTI_EV6                2
#define WFE_CR2_EXTI_EV5                1
#define WFE_CR2_EXTI_EV4                0
#define WFE_CR3                         _SFR_(0xA8)
#define WFE_CR3_DMA1CH23_EV             7
#define WFE_CR3_DMA1CH01_EV             6
#define WFE_CR3_USART1_EV               5
#define WFE_CR3_I2C1_EV                 4
#define WFE_CR3_SPI1_EV                 3
#define WFE_CR3_TIM4_EV                 2
#define WFE_CR3_TIM3_EV1                1
#define WFE_CR3_TIM3_EV0                0
#define WFE_CR4                         _SFR_(0xA9)
#define WFE_CR4_AES_EV                  6
#define WFE_CR4_TIM5_EV1                5
#define WFE_CR4_TIM5_EV0                4
#define WFE_CR4_USART3_EV               3
#define WFE_CR4_USART2_EV               2
#define WFE_CR4_SPI2_EV                 1
#define WFE_CR4_RTC_CSSLSE_EV           0
#define EXTI_CR4                        _SFR_(0xAA)
#define EXTI_CONF2                      _SFR_(0xAB)

/* RST */
#define RST_CR                          _SFR_(0xB0)
#define RST_SR                          _SFR_(0xB1)
#define PWR_CSR1                        _SFR_(0xB2)
#define PWR_CSR1_PVDOF                  6
#define PWR_CSR1_PVDIF                  5
#define PWR_CSR1_PVDIEN                 4
#define PWR_CSR1_PLS2                   3
#define PWR_CSR1_PLS1                   2
#define PWR_CSR1_PLS0                   1
#define PWR_CSR1_PVDE                   0
#define PWR_CSR2                        _SFR_(0xB3)
#define PWR_CSR2_FWU                    2
#define PWR_CSR2_ULP                    1
#define PWR_CSR2_VREFINTF               0

/* Clock */
#define CLK_SYSCLKDiv_1   ((uint8_t)0x00) /*!< System Clock Divider: 1 */
#define CLK_SYSCLKDiv_2   ((uint8_t)0x01) /*!< System Clock Divider: 2 */
#define CLK_SYSCLKDiv_4   ((uint8_t)0x02) /*!< System Clock Divider: 4 */
#define CLK_SYSCLKDiv_8   ((uint8_t)0x03) /*!< System Clock Divider: 8 */
#define CLK_SYSCLKDiv_16  ((uint8_t)0x04) /*!< System Clock Divider: 16 */
#define CLK_SYSCLKDiv_32  ((uint8_t)0x05) /*!< System Clock Divider: 32 */
#define CLK_SYSCLKDiv_64  ((uint8_t)0x06) /*!< System Clock Divider: 64 */
#define CLK_SYSCLKDiv_128 ((uint8_t)0x07) /*!< System Clock Divider: 128 */

#define CLK_RTCCLKSource_Off ((uint8_t)0x00) /*!< Clock RTC Off */
#define CLK_RTCCLKSource_HSI ((uint8_t)0x02) /*!< Clock RTC : HSI */
#define CLK_RTCCLKSource_LSI ((uint8_t)0x04) /*!< Clock RTC : LSI */
#define CLK_RTCCLKSource_HSE ((uint8_t)0x08) /*!< Clock RTC : HSE */
#define CLK_RTCCLKSource_LSE ((uint8_t)0x10) /*!< Clock RTC : LSE */

#define CLK_RTCCLKDiv_1  ((uint8_t)0x00) /*!< Clock RTC Div 1 */
#define CLK_RTCCLKDiv_2  ((uint8_t)0x20) /*!< Clock RTC Div 2  */
#define CLK_RTCCLKDiv_4  ((uint8_t)0x40) /*!< Clock RTC Div 4 */
#define CLK_RTCCLKDiv_8  ((uint8_t)0x60) /*!< Clock RTC Div 8 */
#define CLK_RTCCLKDiv_16 ((uint8_t)0x80) /*!< Clock RTC Div 16 */
#define CLK_RTCCLKDiv_32 ((uint8_t)0xA0) /*!< Clock RTC  Div 32 */
#define CLK_RTCCLKDiv_64 ((uint8_t)0xC0) /*!< Clock RTC  Div 64 */

#define CLK_CKDIVR                      _SFR_(0xC0)
#define CLK_CRTCR                       _SFR_(0xC1)
#define CLK_CRTCR_RTCDIV2               7
#define CLK_CRTCR_RTCDIV1               6
#define CLK_CRTCR_RTCDIV0               5
#define CLK_CRTCR_RTCSEL3               4
#define CLK_CRTCR_RTCSEL2               3
#define CLK_CRTCR_RTCSEL1               2
#define CLK_CRTCR_RTCSEL0               1
#define CLK_CRTCR_RTCSWBSY              0
#define CLK_ICKCR                       _SFR_(0xC2)
#define CLK_ICKCR_BEEPAHALT             6
#define CLK_ICKCR_FHWU                  5
#define CLK_ICKCR_SAHALT                4
#define CLK_ICKCR_LSIRDY                3
#define CLK_ICKCR_LSION                 2
#define CLK_ICKCR_HSIRDY                1
#define CLK_ICKCR_HSION                 0
#define CLK_PCKENR1                     _SFR_(0xC3)
#define CLK_PCKENR2                     _SFR_(0xC4)
#define CLK_CCOR                        _SFR_(0xC5)
#define CLK_CCOR_CCODIV2                7
#define CLK_CCOR_CCODIV1                6
#define CLK_CCOR_CCODIV0                5
#define CLK_CCOR_CCOSEL3                4
#define CLK_CCOR_CCOSEL2                3
#define CLK_CCOR_CCOSEL1                2
#define CLK_CCOR_CCOSEL0                1
#define CLK_CCOR_CCOSWBSY               0
#define CLK_ECKCR                       _SFR_(0xC6)
#define CLK_ECKCR_LSEBYP                5
#define CLK_ECKCR_HSEBYP                4
#define CLK_ECKCR_LSERDY                3
#define CLK_ECKCR_LSEON                 2
#define CLK_ECKCR_HSERDY                1
#define CLK_ECKCR_HSEON                 0
#define CLK_SCSR                        _SFR_(0xC7)
#define CLK_SWR                         _SFR_(0xC8)
#define CLK_SWCR                        _SFR_(0xC9)
#define CLK_SWCR_SWIF                   3
#define CLK_SWCR_SWIEN                  2
#define CLK_SWCR_SWEN                   1
#define CLK_SWCR_SWBSY                  0
#define CLK_CSSR                        _SFR_(0xCA)
#define CLK_CSSR_CSSDGON                4
#define CLK_CSSR_CSSD                   3
#define CLK_CSSR_CSSDIE                 2
#define CLK_CSSR_AUX                    1
#define CLK_CSSR_CSSEN                  0
#define CLK_CBEEPR                      _SFR_(0xCB)
#define CLK_CBEEPR_CLKBEEPSEL1          2
#define CLK_CBEEPR_CLKBEEPSEL0          1
#define CLK_CBEEPR_BEEPSWBSY            0
#define CLK_HSICALR                     _SFR_(0xCC)
#define CLK_HSITRIMR                    _SFR_(0xCD)
#define CLK_HSIUNLCKR                   _SFR_(0xCE)
#define CLK_REGCSR                      _SFR_(0xCF)
#define CLK_REGCSR_EEREADY              7
#define CLK_REGCSR_EEBUSY               6
#define CLK_REGCSR_LSEPD                5
#define CLK_REGCSR_HSEPD                4
#define CLK_REGCSR_LSIPD                3
#define CLK_REGCSR_HSIPD                2
#define CLK_REGCSR_REGOFF               1
#define CLK_REGCSR_REGREADY             0
#define CLK_PCKENR3                     _SFR_(0xD0)

/* Watchdog */
#define WWDG_CR                         _SFR_(0xD3)
#define WWDG_WR                         _SFR_(0xD4)
#define IWDG_KR                         _SFR_(0xE0)
#define IWDG_KEY_ENABLE                 0xCC
#define IWDG_KEY_REFRESH                0xAA
#define IWDG_KEY_ACCESS                 0x55
#define IWDG_PR                         _SFR_(0xE1)
#define IWDG_RLR                        _SFR_(0xE2)

/* Beeper */
#define BEEP_CSR1                       _SFR_(0xF0)
#define BEEP_CSR2                       _SFR_(0xF3)

/* RTC */
#define RTC_WakeUpClock_RTCCLK_Div16    ((uint8_t)0x00) /*!< (RTC clock) div 16*/
#define RTC_WakeUpClock_RTCCLK_Div8     ((uint8_t)0x01) /*!< (RTC clock) div 8*/
#define RTC_WakeUpClock_RTCCLK_Div4     ((uint8_t)0x02) /*!< (RTC clock) div 4*/
#define RTC_WakeUpClock_RTCCLK_Div2     ((uint8_t)0x03) /*!< (RTC clock) div 2*/
#define RTC_WakeUpClock_CK_SPRE_16bits  ((uint8_t)0x04) /*!< CK SPRE with a counter from 0x0000 to 0xFFFF */
#define RTC_WakeUpClock_CK_SPRE_17bits  ((uint8_t)0x06) /*!< CK SPRE with a counter from 0x10000 to 0x1FFFF */

#define RTC_IT_WUT   ((uint8_t)0x40)  /*!< Wake up Timer Interrupt */
#define RTC_IT_ALRA  ((uint8_t)0x10)  /*!< Alarm A Interrupt */
#define RTC_IT_TAMP  ((uint8_t)0x01)  /*!< Tamper Interrupt */

#define WUTWF_TIMEOUT      ((uint16_t)0xFFFF)

#define RTC_TR1                         _SFR_(0x140)
#define RTC_TR2                         _SFR_(0x141)
#define RTC_TR3                         _SFR_(0x142)
#define RTC_DR1                         _SFR_(0x144)
#define RTC_DR2                         _SFR_(0x145)
#define RTC_DR3                         _SFR_(0x146)
#define RTC_CR1                         _SFR_(0x148)
#define RTC_CR1_FMT                     6
#define RTC_CR1_RATIO                   4
#define RTC_CR1_BYPSHAD                 4
#define RTC_CR1_WUCKSEL2                2
#define RTC_CR1_WUCKSEL1                1
#define RTC_CR1_WUCKSEL0                0
#define RTC_CR2                         _SFR_(0x149)
#define RTC_CR2_WUTIE                   6
#define RTC_CR2_ALRAIE                  4
#define RTC_CR2_WUTE                    2
#define RTC_CR2_ALRAE                   0
#define RTC_CR3                         _SFR_(0x14A)
#define RTC_CR3_COE                     7
#define RTC_CR3_OSEL1                   6
#define RTC_CR3_OSEL0                   5
#define RTC_CR3_POL                     4
#define RTC_CR3_COSEL                   3
#define RTC_CR3_BCK                     2
#define RTC_CR3_SUB1H                   1
#define RTC_CR3_ADD1H                   0
#define RTC_ISR1                        _SFR_(0x14C)
#define RTC_ISR1_INIT                   7
#define RTC_ISR1_INITF                  6
#define RTC_ISR1_RSF                    5
#define RTC_ISR1_INITS                  4
#define RTC_ISR1_SHPF                   3
#define RTC_ISR1_WUTWF                  2
#define RTC_ISR1_RECALPF                1
#define RTC_ISR1_ALRAWF                 0
#define RTC_ISR2                        _SFR_(0x14D)
#define RTC_ISR2_TAMP3F                 7
#define RTC_ISR2_TAMP2F                 6
#define RTC_ISR2_TAMP1F                 5
#define RTC_ISR2_WUTF                   2
#define RTC_ISR2_ALRAF                  0
#define RTC_SPRERH                      _SFR_(0x150)
#define RTC_SPRERL                      _SFR_(0x151)
#define RTC_APRER                       _SFR_(0x152)
#define RTC_WUTR                        _SFR16_(0x154)
#define RTC_WUTRH                       _SFR_(0x154)
#define RTC_WUTRL                       _SFR_(0x155)
#define RTC_SSRL                        _SFR_(0x157)
#define RTC_SSRH                        _SFR_(0x158)
#define RTC_WPR                         _SFR_(0x159)
#define RTC_SSRH                        _SFR_(0x158)
#define RTC_WPR                         _SFR_(0x159)
#define RTC_SHIFTRH                     _SFR_(0x15A)
#define RTC_SHIFTRL                     _SFR_(0x15B)
#define RTC_ALRMAR1                     _SFR_(0x15C)
#define RTC_ALRMAR2                     _SFR_(0x15D)
#define RTC_ALRMAR3                     _SFR_(0x15E)
#define RTC_ALRMAR4                     _SFR_(0x15F)
#define RTC_ALRMASSRH                   _SFR_(0x164)
#define RTC_ALRMASSRL                   _SFR_(0x165)
#define RTC_ALRMASSMSKR                 _SFR_(0x166)
#define RTC_CALRH                       _SFR_(0x16A)
#define RTC_CALRL                       _SFR_(0x16B)
#define RTC_TCR1                        _SFR_(0x16C)
#define RTC_TCR2                        _SFR_(0x16D)

#define CSSLSE_CSR                      _SFR_(0x190)

/* SPI1 */
#define SPI1_CR1                        _SFR_(0x200)
#define SPI1_CR1_LSBFIRST               7
#define SPI1_CR1_SPE                    6
#define SPI1_CR1_BR2                    5
#define SPI1_CR1_BR1                    4
#define SPI1_CR1_BR0                    3
#define SPI1_CR1_MSTR                   2
#define SPI1_CR1_CPOL                   1
#define SPI1_CR1_CPHA                   0
#define SPI1_CR2                        _SFR_(0x201)
#define SPI1_CR2_BDM                    7
#define SPI1_CR2_BDOE                   6
#define SPI1_CR2_CRCEN                  5
#define SPI1_CR2_CRCNEXT                4
#define SPI1_CR2_RXONLY                 2
#define SPI1_CR2_SSM                    1
#define SPI1_CR2_SSI                    0
#define SPI1_ICR                        _SFR_(0x202)
#define SPI1_SR                         _SFR_(0x203)
#define SPI1_SR_BSY                     7
#define SPI1_SR_OVR                     6
#define SPI1_SR_MODF                    5
#define SPI1_SR_CRCERR                  4
#define SPI1_SR_WKUP                    3
#define SPI1_SR_TXE                     1
#define SPI1_SR_RXNE                    0
#define SPI1_DR                         _SFR_(0x204)
#define SPI1_CRCPR                      _SFR_(0x205)
#define SPI1_RXCRCR                     _SFR_(0x206)
#define SPI1_TXCRCR                     _SFR_(0x207)

/* I2C1 */
#define I2C1_CR1                        _SFR_(0x210)
#define I2C1_CR1_NOSTRETCH              7
#define I2C1_CR1_ENGC                   6
#define I2C1_CR1_ENPEC                  5
#define I2C1_CR1_ENARP                  4
#define I2C1_CR1_SMBTYPE                3
#define I2C1_CR1_SMBUS                  1
#define I2C1_CR1_PE                     0
#define I2C1_CR2                        _SFR_(0x211)
#define I2C1_CR2_SWRST                  7
#define I2C1_CR2_ALERT                  5
#define I2C1_CR2_PEC                    4
#define I2C1_CR2_POS                    3
#define I2C1_CR2_ACK                    2
#define I2C1_CR2_STOP                   1
#define I2C1_CR2_START                  0
#define I2C1_FREQR                      _SFR_(0x212)
#define I2C1_FREQR_FREQ5                5
#define I2C1_FREQR_FREQ4                4
#define I2C1_FREQR_FREQ3                3
#define I2C1_FREQR_FREQ2                2
#define I2C1_FREQR_FREQ1                1
#define I2C1_FREQR_FREQ0                0
#define I2C1_OARL                       _SFR_(0x213)
#define I2C1_OARH                       _SFR_(0x214)
#define I2C1_OARH_ADDMODE               7
#define I2C1_OARH_ADDCONF               6
#define I2C1_OAR2                       _SFR_(0x215)
#define I2C1_DR                         _SFR_(0x216)
#define I2C1_SR1                        _SFR_(0x217)
#define I2C1_SR1_TXE                    7
#define I2C1_SR1_RXNE                   6
#define I2C1_SR1_BTF                    2
#define I2C1_SR1_ADDR                   1
#define I2C1_SR1_SB                     0
#define I2C1_SR2                        _SFR_(0x218)
#define I2C1_SR3                        _SFR_(0x219)
#define I2C1_SR3_BUSY                   1
#define I2C1_SR3_MSL                    0
#define I2C1_ITR                        _SFR_(0x21A)
#define I2C1_CCRL                       _SFR_(0x21B)
#define I2C1_CCRH                       _SFR_(0x21C)
#define I2C1_TRISER                     _SFR_(0x21D)
#define I2C1_PECR                       _SFR_(0x21E)

/* USART1 */
#define USART1_SR                       _SFR_(0x230)
#define USART1_SR_TXE                   7
#define USART1_SR_TC                    6
#define USART1_SR_RXNE                  5
#define USART1_DR                       _SFR_(0x231)
#define USART1_BRR1                     _SFR_(0x232)
#define USART1_BRR2                     _SFR_(0x233)
#define USART1_CR1                      _SFR_(0x234)
#define USART1_CR2                      _SFR_(0x235)
#define USART1_CR2_TEN                  3
#define USART1_CR2_REN                  2
#define USART1_CR3                      _SFR_(0x236)
#define USART1_CR4                      _SFR_(0x237)
#define USART1_CR5                      _SFR_(0x238)
#define USART1_GTR                      _SFR_(0x239)
#define USART1_PSCR                     _SFR_(0x23A)


/** TIM3 interrupt sources */
typedef enum
{
  TIM3_IT_UPDATE                     = ((uint8_t)0x01),
  TIM3_IT_CC1                        = ((uint8_t)0x02),
  TIM3_IT_CC2                        = ((uint8_t)0x04)
} TIM3_IT_TypeDef;

/*CCMR*/
#define TIM3_CCMR_ICxPSC ((uint8_t)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM3_CCMR_ICxF   ((uint8_t)0xF0) /*!< Input Capture x Filter mask. */
#define TIM3_CCMR_OCM    ((uint8_t)0x70) /*!< Output Compare x Mode mask. */
#define TIM3_CCMR_OCxPE  ((uint8_t)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM3_CCMR_CCxS   ((uint8_t)0x03) /*!< Capture/Compare x Selection mask. */

/** TIM3 Prescaler */
typedef enum
{
  TIM3_PRESCALER_1  = ((uint8_t)0x00),
  TIM3_PRESCALER_2    = ((uint8_t)0x01),
  TIM3_PRESCALER_4    = ((uint8_t)0x02),
  TIM3_PRESCALER_8     = ((uint8_t)0x03),
  TIM3_PRESCALER_16   = ((uint8_t)0x04),
  TIM3_PRESCALER_32     = ((uint8_t)0x05),
  TIM3_PRESCALER_64    = ((uint8_t)0x06),
  TIM3_PRESCALER_128   = ((uint8_t)0x07),
  TIM3_PRESCALER_256   = ((uint8_t)0x08),
  TIM3_PRESCALER_512   = ((uint8_t)0x09),
  TIM3_PRESCALER_1024  = ((uint8_t)0x0A),
  TIM3_PRESCALER_2048 = ((uint8_t)0x0B),
  TIM3_PRESCALER_4096   = ((uint8_t)0x0C),
  TIM3_PRESCALER_8192 = ((uint8_t)0x0D),
  TIM3_PRESCALER_16384 = ((uint8_t)0x0E),
  TIM3_PRESCALER_32768 = ((uint8_t)0x0F)
} TIM3_Prescaler_TypeDef;

/* TIM2 */
/** TIM2 interrupt sources */
typedef enum
{
  TIM2_IT_UPDATE                     = ((uint8_t)0x01),
  TIM2_IT_CC1                        = ((uint8_t)0x02),
  TIM2_IT_CC2                        = ((uint8_t)0x04),
  TIM2_IT_CC3                        = ((uint8_t)0x08)
}TIM2_IT_TypeDef;

/** TIM2 Prescaler */
typedef enum
{
  TIM2_PRESCALER_1  = ((uint8_t)0x00),
  TIM2_PRESCALER_2    = ((uint8_t)0x01),
  TIM2_PRESCALER_4    = ((uint8_t)0x02),
  TIM2_PRESCALER_8     = ((uint8_t)0x03),
  TIM2_PRESCALER_16   = ((uint8_t)0x04),
  TIM2_PRESCALER_32     = ((uint8_t)0x05),
  TIM2_PRESCALER_64    = ((uint8_t)0x06),
  TIM2_PRESCALER_128   = ((uint8_t)0x07),
  TIM2_PRESCALER_256   = ((uint8_t)0x08),
  TIM2_PRESCALER_512   = ((uint8_t)0x09),
  TIM2_PRESCALER_1024  = ((uint8_t)0x0A),
  TIM2_PRESCALER_2048 = ((uint8_t)0x0B),
  TIM2_PRESCALER_4096   = ((uint8_t)0x0C),
  TIM2_PRESCALER_8192 = ((uint8_t)0x0D),
  TIM2_PRESCALER_16384 = ((uint8_t)0x0E),
  TIM2_PRESCALER_32768 = ((uint8_t)0x0F)
}TIM2_Prescaler_TypeDef;

/** TIM2 Output Compare and PWM modes */
typedef enum
{
  TIM2_OCMODE_TIMING     = ((uint8_t)0x00),
  TIM2_OCMODE_ACTIVE     = ((uint8_t)0x10),
  TIM2_OCMODE_INACTIVE   = ((uint8_t)0x20),
  TIM2_OCMODE_TOGGLE     = ((uint8_t)0x30),
  TIM2_OCMODE_PWM1       = ((uint8_t)0x60),
  TIM2_OCMODE_PWM2       = ((uint8_t)0x70)
}TIM2_OCMode_TypeDef;

/** @addtogroup TIM2_Registers_Bits_Definition
  * @{
  */
/*CR1*/
#define TIM2_CR1_ARPE ((uint8_t)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM2_CR1_OPM  ((uint8_t)0x08) /*!< One Pulse Mode mask. */
#define TIM2_CR1_URS  ((uint8_t)0x04) /*!< Update Request Source mask. */
#define TIM2_CR1_UDIS ((uint8_t)0x02) /*!< Update DIsable mask. */
#define TIM2_CR1_CEN  ((uint8_t)0x01) /*!< Counter Enable mask. */
/*IER*/
#define TIM2_IER_CC3IE ((uint8_t)0x08) /*!< Capture/Compare 3 Interrupt Enable mask. */
#define TIM2_IER_CC2IE ((uint8_t)0x04) /*!< Capture/Compare 2 Interrupt Enable mask. */
#define TIM2_IER_CC1IE ((uint8_t)0x02) /*!< Capture/Compare 1 Interrupt Enable mask. */
#define TIM2_IER_UIE   ((uint8_t)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM2_SR1_CC3IF ((uint8_t)0x08) /*!< Capture/Compare 3 Interrupt Flag mask. */
#define TIM2_SR1_CC2IF ((uint8_t)0x04) /*!< Capture/Compare 2 Interrupt Flag mask. */
#define TIM2_SR1_CC1IF ((uint8_t)0x02) /*!< Capture/Compare 1 Interrupt Flag mask. */
#define TIM2_SR1_UIF   ((uint8_t)0x01) /*!< Update Interrupt Flag mask. */
/*SR2*/
#define TIM2_SR2_CC3OF ((uint8_t)0x08) /*!< Capture/Compare 3 Overcapture Flag mask. */
#define TIM2_SR2_CC2OF ((uint8_t)0x04) /*!< Capture/Compare 2 Overcapture Flag mask. */
#define TIM2_SR2_CC1OF ((uint8_t)0x02) /*!< Capture/Compare 1 Overcapture Flag mask. */
/*EGR*/
#define TIM2_EGR_CC3G  ((uint8_t)0x08) /*!< Capture/Compare 3 Generation mask. */
#define TIM2_EGR_CC2G  ((uint8_t)0x04) /*!< Capture/Compare 2 Generation mask. */
#define TIM2_EGR_CC1G  ((uint8_t)0x02) /*!< Capture/Compare 1 Generation mask. */
#define TIM2_EGR_UG    ((uint8_t)0x01) /*!< Update Generation mask. */
/*CCMR*/
#define TIM2_CCMR_ICxPSC ((uint8_t)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM2_CCMR_ICxF   ((uint8_t)0xF0) /*!< Input Capture x Filter mask. */
#define TIM2_CCMR_OCM    ((uint8_t)0x70) /*!< Output Compare x Mode mask. */
#define TIM2_CCMR_OCxPE  ((uint8_t)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM2_CCMR_CCxS   ((uint8_t)0x03) /*!< Capture/Compare x Selection mask. */
/*CCER1*/
#define TIM2_CCER1_CC2P ((uint8_t)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM2_CCER1_CC2E ((uint8_t)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM2_CCER1_CC1P ((uint8_t)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM2_CCER1_CC1E ((uint8_t)0x01) /*!< Capture/Compare 1 output enable mask. */
/*CCER2*/
#define TIM2_CCER2_CC3P ((uint8_t)0x02) /*!< Capture/Compare 3 output Polarity mask. */
#define TIM2_CCER2_CC3E ((uint8_t)0x01) /*!< Capture/Compare 3 output enable mask. */
/*CNTR*/
#define TIM2_CNTRH_CNT ((uint8_t)0xFF) /*!< Counter Value (MSB) mask. */
#define TIM2_CNTRL_CNT ((uint8_t)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCR*/
#define TIM2_PSCR_PSC ((uint8_t)0xFF) /*!< Prescaler Value (MSB) mask. */
/*ARR*/
#define TIM2_ARRH_ARR ((uint8_t)0xFF) /*!< Autoreload Value (MSB) mask. */
#define TIM2_ARRL_ARR ((uint8_t)0xFF) /*!< Autoreload Value (LSB) mask. */
/*CCR1*/
#define TIM2_CCR1H_CCR1 ((uint8_t)0xFF) /*!< Capture/Compare 1 Value (MSB) mask. */
#define TIM2_CCR1L_CCR1 ((uint8_t)0xFF) /*!< Capture/Compare 1 Value (LSB) mask. */
/*CCR2*/
#define TIM2_CCR2H_CCR2 ((uint8_t)0xFF) /*!< Capture/Compare 2 Value (MSB) mask. */
#define TIM2_CCR2L_CCR2 ((uint8_t)0xFF) /*!< Capture/Compare 2 Value (LSB) mask. */
/*CCR3*/
#define TIM2_CCR3H_CCR3 ((uint8_t)0xFF) /*!< Capture/Compare 3 Value (MSB) mask. */
#define TIM2_CCR3L_CCR3 ((uint8_t)0xFF) /*!< Capture/Compare 3 Value (LSB) mask. */

typedef enum
{
  TIM2_OUTPUTSTATE_DISABLE           = ((uint8_t)0x00),
  TIM2_OUTPUTSTATE_ENABLE            = ((uint8_t)0x11)
}TIM2_OutputState_TypeDef;

typedef enum
{
  TIM2_OCPOLARITY_HIGH               = ((uint8_t)0x00),
  TIM2_OCPOLARITY_LOW                = ((uint8_t)0x22)
}TIM2_OCPolarity_TypeDef;

typedef enum
{
  TIM2_FORCEDACTION_ACTIVE           = ((uint8_t)0x50),
  TIM2_FORCEDACTION_INACTIVE         = ((uint8_t)0x40)
}TIM2_ForcedAction_TypeDef;

#define TIM2_CR1                        _SFR_(0x250)
// #define TIM2_CR1_ARPE                   7
// #define TIM2_CR1_DIR                    6
// #define TIM2_CR1_OPM                    3
// #define TIM2_CR1_URS                    2
// #define TIM2_CR1_UDIS                   1
// #define TIM2_CR1_CEN                    0
#define TIM2_CR2                        _SFR_(0x251)
#define TIM2_SMCR                       _SFR_(0x252)
#define TIM2_ETR                        _SFR_(0x253)
#define TIM2_DER                        _SFR_(0x254)
#define TIM2_IER                        _SFR_(0x255)
#define TIM2_SR1                        _SFR_(0x256)
#define TIM2_SR2                        _SFR_(0x257)
#define TIM2_EGR                        _SFR_(0x258)
// #define TIM2_EGR_BG                     7
// #define TIM2_EGR_TG                     6
// #define TIM2_EGR_CC2G                   2
// #define TIM2_EGR_CC1G                   1
// #define TIM2_EGR_UG                     0
#define TIM2_CCMR1                      _SFR_(0x259)
#define TIM2_CCMR2                      _SFR_(0x25A)
#define TIM2_CCER1                      _SFR_(0x25B)
#define TIM2_CNTR                       _SFR16_(0x25C)
#define TIM2_CNTRH                      _SFR_(0x25C)
#define TIM2_CNTRL                      _SFR_(0x25D)
#define TIM2_PSCR                       _SFR_(0x25E)
#define TIM2_ARR                        _SFR16_(0x25F)
#define TIM2_ARRH                       _SFR_(0x25F)
#define TIM2_ARRL                       _SFR_(0x260)
#define TIM2_CCR1H                      _SFR_(0x261)
#define TIM2_CCR1L                      _SFR_(0x262)
#define TIM2_CCR2H                      _SFR_(0x263)
#define TIM2_CCR2L                      _SFR_(0x264)
#define TIM2_BKR                        _SFR_(0x265)
#define TIM_BKR_MOE      ((uint8_t)0x80) /*!< Main Output Enable Mask. */
#define TIM2_OISR                       _SFR_(0x266)

/* TIM3 */
/** TIM3 Output Compare Polarity */
typedef enum
{
  TIM3_OCPOLARITY_HIGH               = ((uint8_t)0x00),
  TIM3_OCPOLARITY_LOW                = ((uint8_t)0x22)
} TIM3_OCPolarity_TypeDef;

/** TIM3 Output Compare states */
typedef enum
{
  TIM3_OUTPUTSTATE_DISABLE           = ((uint8_t)0x00),
  TIM3_OUTPUTSTATE_ENABLE            = ((uint8_t)0x11)
} TIM3_OutputState_TypeDef;

/** TIM3 Output Compare and PWM modes */
typedef enum
{
  TIM3_OCMODE_TIMING     = ((uint8_t)0x00),
  TIM3_OCMODE_ACTIVE     = ((uint8_t)0x10),
  TIM3_OCMODE_INACTIVE   = ((uint8_t)0x20),
  TIM3_OCMODE_TOGGLE     = ((uint8_t)0x30),
  TIM3_OCMODE_PWM1       = ((uint8_t)0x60),
  TIM3_OCMODE_PWM2       = ((uint8_t)0x70)
} TIM3_OCMode_TypeDef;

#define  ICPolarity_Rising   ((uint8_t)0x00) /*!< Input Capture on Rising Edge*/
#define  ICPolarity_Falling  ((uint8_t)0x01)  /*!< Input Capture on Falling Edge*/


#define  ICSelection_DirectTI     ((uint8_t)0x01) /*!< Input Capture mapped on the direct input*/
#define  ICSelection_IndirectTI   ((uint8_t)0x02) /*!< Input Capture mapped on the indirect input*/
#define  ICSelection_TRGI         ((uint8_t)0x03)  /*!< Input Capture mapped on the Trigger Input*/

#define  ICPSC_DIV1   ((uint8_t)0x00)  /*!< Input Capture Prescaler = 1 (one capture every 1 event) */
#define  ICPSC_DIV2   ((uint8_t)0x04)  /*!< Input Capture Prescaler = 2 (one capture every 2 events) */
#define  ICPSC_DIV4   ((uint8_t)0x08)  /*!< Input Capture Prescaler = 4 (one capture every 4 events) */
#define  ICPSC_DIV8   ((uint8_t)0x0C)   /*!< Input Capture Prescaler = 8 (one capture every 8 events) */

#define TIM_SMCR_TS      ((uint8_t)0x70) /*!< Trigger Selection Mask. */
#define TIM_SMCR_SMS     ((uint8_t)0x07) /*!< Slave Mode Selection Mask. */

/*CCER1*/
#define TIM3_CCER1_CC2P ((uint8_t)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM3_CCER1_CC2E ((uint8_t)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM3_CCER1_CC1P ((uint8_t)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM3_CCER1_CC1E ((uint8_t)0x01) /*!< Capture/Compare 1 output enable mask. */

#define TIM3_CR1                        _SFR_(0x280)
#define TIM3_CR1_ARPE                   7
#define TIM3_CR1_CMS1                   6
#define TIM3_CR1_CMS0                   5
#define TIM3_CR1_DIR                    4
#define TIM3_CR1_OPM                    3
#define TIM3_CR1_URS                    2
#define TIM3_CR1_UDIS                   1
#define TIM3_CR1_CEN                    0
#define TIM3_CR2                        _SFR_(0x281)
#define TIM3_CR2_MMS2                   6
#define TIM3_CR2_MMS1                   5
#define TIM3_CR2_MMS0                   4
#define TIM3_CR2_CCDS                   3
#define TIM3_SMCR                       _SFR_(0x282)
#define TIM3_ETR                        _SFR_(0x283)
#define TIM3_DER                        _SFR_(0x284)
#define TIM3_IER                        _SFR_(0x285)
#define TIM3_IER_BIE                    7
#define TIM3_IER_TIE                    6
#define TIM3_IER_CC2IE                  2
#define TIM3_IER_CC1IE                  1
#define TIM3_IER_UIE                    0
#define TIM3_SR1                        _SFR_(0x286)
#define TIM3_SR1_BIF                    7
#define TIM3_SR1_TIF                    6
#define TIM3_SR1_CC2IF                  2
#define TIM3_SR1_CC1IF                  1
#define TIM3_SR1_UIF                    0
#define TIM3_SR2                        _SFR_(0x287)
#define TIM3_SR2_CC2OF                  2
#define TIM3_SR2_CC1OF                  1
#define TIM3_EGR                        _SFR_(0x288)
#define TIM3_EGR_BG                     7
#define TIM3_EGR_TG                     6
#define TIM3_EGR_CC2G                   2
#define TIM3_EGR_CC1G                   1
#define TIM3_EGR_UG                     0
#define TIM3_CCMR1                      _SFR_(0x289)
#define TIM3_CCMR2                      _SFR_(0x28A)
#define TIM3_CCER1                      _SFR_(0x28B)
#define TIM3_CNTR                       _SFR16_(0x28C)
#define TIM3_CNTRH                      _SFR_(0x28C)
#define TIM3_CNTRL                      _SFR_(0x28D)
#define TIM3_PSCR                       _SFR_(0x28E)
#define TIM3_ARR                        _SFR16_(0x28F)
#define TIM3_ARRH                       _SFR_(0x28F)
#define TIM3_ARRL                       _SFR_(0x290)
#define TIM3_CCR1H                      _SFR_(0x291)
#define TIM3_CCR1L                      _SFR_(0x292)
#define TIM3_CCR2H                      _SFR_(0x293)
#define TIM3_CCR2L                      _SFR_(0x294)
#define TIM3_BKR                        _SFR_(0x295)
#define TIM3_OISR                       _SFR_(0x296)

/* TIM1 */
#define TIM1_CR1                        _SFR_(0x2B0)
#define TIM1_CR2                        _SFR_(0x2B1)
#define TIM1_SMCR                       _SFR_(0x2B2)
#define TIM1_ETR                        _SFR_(0x2B3)
#define TIM1_DER                        _SFR_(0x2B4)
#define TIM1_IER                        _SFR_(0x2B5)
#define TIM1_SR1                        _SFR_(0x2B6)
#define TIM1_SR2                        _SFR_(0x2B7)
#define TIM1_EGR                        _SFR_(0x2B8)
#define TIM1_CCMR1                      _SFR_(0x2B9)
#define TIM1_CCMR2                      _SFR_(0x2BA)
#define TIM1_CCMR3                      _SFR_(0x2BB)
#define TIM1_CCMR4                      _SFR_(0x2BC)
#define TIM1_CCER1                      _SFR_(0x2BD)
#define TIM1_CCER2                      _SFR_(0x2BE)
#define TIM1_CNTRH                      _SFR_(0x2BF)
#define TIM1_CNTRL                      _SFR_(0x2C0)
#define TIM1_PSCRH                      _SFR_(0x2C1)
#define TIM1_PSCRL                      _SFR_(0x2C2)
#define TIM1_ARRH                       _SFR_(0x2C3)
#define TIM1_ARRL                       _SFR_(0x2C4)
#define TIM1_RCR                        _SFR_(0x2C5)
#define TIM1_CCR1H                      _SFR_(0x2C6)
#define TIM1_CCR1L                      _SFR_(0x2C7)
#define TIM1_CCR2H                      _SFR_(0x2C8)
#define TIM1_CCR2L                      _SFR_(0x2C9)
#define TIM1_CCR3H                      _SFR_(0x2CA)
#define TIM1_CCR3L                      _SFR_(0x2CB)
#define TIM1_CCR4H                      _SFR_(0x2CC)
#define TIM1_CCR4L                      _SFR_(0x2CD)
#define TIM1_BKR                        _SFR_(0x2CE)
#define TIM1_DTR                        _SFR_(0x2CF)
#define TIM1_OISR                       _SFR_(0x2D0)
#define TIM1_DCR1                       _SFR_(0x2D1)
#define TIM1_DCR2                       _SFR_(0x2D2)
#define TIM1_DMA1R                      _SFR_(0x2D3)

/* TIM4 */

/** @addtogroup TIM4_Registers_Reset_Value
  * @{
  */
#define TIM4_CR1_RESET_VALUE  ((uint8_t)0x00)
#define TIM4_IER_RESET_VALUE  ((uint8_t)0x00)
#define TIM4_SR1_RESET_VALUE  ((uint8_t)0x00)
#define TIM4_EGR_RESET_VALUE  ((uint8_t)0x00)
#define TIM4_CNTR_RESET_VALUE ((uint8_t)0x00)
#define TIM4_PSCR_RESET_VALUE ((uint8_t)0x00)
#define TIM4_ARR_RESET_VALUE  ((uint8_t)0xFF)

/** TIM4 interrupt sources */
typedef enum
{
  TIM4_IT_UPDATE                     = ((uint8_t)0x01)
}TIM4_IT_TypeDef;

#define TIM4_CR1                        _SFR_(0x2E0)
#define TIM4_CR1_ARPE                   7
#define TIM4_CR1_OPM                    3
#define TIM4_CR1_URS                    2
#define TIM4_CR1_UDIS                   1
#define TIM4_CR1_CEN                    0
#define TIM4_CR2                        _SFR_(0x2E1)
#define TIM4_SMCR                       _SFR_(0x2E2)
#define TIM4_DER                        _SFR_(0x2E3)
#define TIM4_IER                        _SFR_(0x2E4)
#define TIM4_IER_TIE                    6
#define TIM4_IER_UIE                    0
#define TIM4_SR                         _SFR_(0x2E5)
#define TIM4_SR_TIF                     6
#define TIM4_SR_UIF                     0
#define TIM4_EGR                        _SFR_(0x2E6)
#define TIM4_EGR_TG                     6
#define TIM4_EGR_UG                     0
#define TIM4_CNTR                       _SFR_(0x2E7)
#define TIM4_PSCR                       _SFR_(0x2E8)
#define TIM4_ARR                        _SFR_(0x2E9)

/* IR */
#define IR_CR                           _SFR_(0x2FF)

/* TIM5 */
#define TIM5_CR1                        _SFR_(0x300)
#define TIM5_CR2                        _SFR_(0x301)
#define TIM5_SMCR                       _SFR_(0x302)
#define TIM5_ETR                        _SFR_(0x303)
#define TIM5_DER                        _SFR_(0x304)
#define TIM5_IER                        _SFR_(0x305)
#define TIM5_SR1                        _SFR_(0x306)
#define TIM5_SR2                        _SFR_(0x307)
#define TIM5_EGR                        _SFR_(0x308)
#define TIM5_CCMR1                      _SFR_(0x309)
#define TIM5_CCMR2                      _SFR_(0x30A)
#define TIM5_CCER1                      _SFR_(0x30B)
#define TIM5_CNTRH                      _SFR_(0x30C)
#define TIM5_CNTRL                      _SFR_(0x30D)
#define TIM5_PSCR                       _SFR_(0x30E)
#define TIM5_ARRH                       _SFR_(0x30F)
#define TIM5_ARRL                       _SFR_(0x310)
#define TIM5_CCR1H                      _SFR_(0x311)
#define TIM5_CCR1L                      _SFR_(0x312)
#define TIM5_CCR2H                      _SFR_(0x313)
#define TIM5_CCR2L                      _SFR_(0x314)
#define TIM5_BKR                        _SFR_(0x315)
#define TIM5_OISR                       _SFR_(0x316)

/* ADC1 */
#define ADC1_CR1                        _SFR_(0x340)
#define ADC1_CR1_OVERIE                 7
#define ADC1_CR1_RES1                   6
#define ADC1_CR1_RES0                   5
#define ADC1_CR1_AWDIE                  4
#define ADC1_CR1_EOCIE                  3
#define ADC1_CR1_CONT                   2
#define ADC1_CR1_START                  1
#define ADC1_CR1_ADON                   0
#define ADC1_CR2                        _SFR_(0x341)
#define ADC1_CR2_PRESC                  7
#define ADC1_CR2_TRIG_EDGE1             6
#define ADC1_CR2_TRIG_EDGE0             5
#define ADC1_CR2_EXTSEL1                4
#define ADC1_CR2_EXTSEL0                3
#define ADC1_CR2_SMTP12                 2
#define ADC1_CR2_SMTP11                 1
#define ADC1_CR2_SMTP10                 0
#define ADC1_CR3                        _SFR_(0x342)
#define ADC1_CR3_SMTP22                 7
#define ADC1_CR3_SMTP21                 6
#define ADC1_CR3_SMTP20                 5
#define ADC1_CR3_CHSEL4                 4
#define ADC1_CR3_CHSEL3                 3
#define ADC1_CR3_CHSEL2                 2
#define ADC1_CR3_CHSEL1                 1
#define ADC1_CR3_CHSEL0                 0
#define ADC1_SR                         _SFR_(0x343)
#define ADC1_SR_OVER                    2
#define ADC1_SR_AWD                     1
#define ADC1_SR_EOC                     0
#define ADC1_DRH                        _SFR_(0x344)
#define ADC1_DRL                        _SFR_(0x345)
#define ADC1_HTRH                       _SFR_(0x346)
#define ADC1_HTRL                       _SFR_(0x347)
#define ADC1_LTRH                       _SFR_(0x348)
#define ADC1_LTRL                       _SFR_(0x349)
#define ADC1_SQR1                       _SFR_(0x34A)
#define ADC1_SQR1_DMAOFF                7
#define ADC1_SQR1_CHSEL_STS             5
#define ADC1_SQR1_CHSEL_SVREFINT        4
#define ADC1_SQR1_CHSEL_S27             3
#define ADC1_SQR1_CHSEL_S26             2
#define ADC1_SQR1_CHSEL_S25             1
#define ADC1_SQR1_CHSEL_S24             0
#define ADC1_SQR2                       _SFR_(0x34B)
#define ADC1_SQR3                       _SFR_(0x34C)
#define ADC1_SQR4                       _SFR_(0x34D)
#define ADC1_TRIGR1                     _SFR_(0x34E)
#define ADC1_TRIGR1_TSON                5
#define ADC1_TRIGR1_VREFINTON           4
#define ADC1_TRIGR1_TRIG27              3
#define ADC1_TRIGR1_TRIG26              2
#define ADC1_TRIGR1_TRIG25              1
#define ADC1_TRIGR1_TRIG24              0
#define ADC1_TRIGR2                     _SFR_(0x34F)
#define ADC1_TRIGR3                     _SFR_(0x350)
#define ADC1_TRIGR4                     _SFR_(0x351)

/* DAC */
#define DAC_CH1CR1                      _SFR_(0x380)
#define DAC_CH1CR2                      _SFR_(0x381)
#define DAC_CH2CR1                      _SFR_(0x382)
#define DAC_CH2CR2                      _SFR_(0x383)
#define DAC_SWTRIG                      _SFR_(0x384)
#define DAC_SR                          _SFR_(0x385)
#define DAC_CH1RDHRH                    _SFR_(0x388)
#define DAC_CH1RDHRL                    _SFR_(0x389)
#define DAC_CH1LDHRH                    _SFR_(0x38C)
#define DAC_CH1LDHRL                    _SFR_(0x38D)
#define DAC_CH1DHR8                     _SFR_(0x390)
#define DAC_CH2RDHRH                    _SFR_(0x394)
#define DAC_CH2RDHRL                    _SFR_(0x395)
#define DAC_CH2LDHRH                    _SFR_(0x398)
#define DAC_CH2LDHRL                    _SFR_(0x399)
#define DAC_CH2DHR8                     _SFR_(0x39C)
#define DAC_DCH1RDHRH                   _SFR_(0x3A0)
#define DAC_DCH1RDHRL                   _SFR_(0x3A1)
#define DAC_DORH                        _SFR_(0x3AC)
#define DAC_DORL                        _SFR_(0x3AD)
#define DAC_DCH2RDHRH                   _SFR_(0x3A2)
#define DAC_DCH2RDHRL                   _SFR_(0x3A3)
#define DAC_DCH1LDHRH                   _SFR_(0x3A4)
#define DAC_DCH1LDHRL                   _SFR_(0x3A5)
#define DAC_DCH2LDHRH                   _SFR_(0x3A6)
#define DAC_DCH2LDHRL                   _SFR_(0x3A7)
#define DAC_DCH1DHR8                    _SFR_(0x3A8)
#define DAC_DCH2DHR8                    _SFR_(0x3A9)
#define DAC_CH1DORH                     _SFR_(0x3AC)
#define DAC_CH1DORL                     _SFR_(0x3AD)
#define DAC_CH2DORH                     _SFR_(0x3B0)
#define DAC_CH2DORL                     _SFR_(0x3B1)

/* SPI2 */
#define SPI2_CR1                        _SFR_(0x3C0)
#define SPI2_CR2                        _SFR_(0x3C1)
#define SPI2_ICR                        _SFR_(0x3C2)
#define SPI2_SR                         _SFR_(0x3C3)
#define SPI2_DR                         _SFR_(0x3C4)
#define SPI2_CRCPR                      _SFR_(0x3C5)
#define SPI2_RXCRCR                     _SFR_(0x3C6)
#define SPI2_TXCRCR                     _SFR_(0x3C7)

/* AES */
#define AES_CR                          _SFR_(0x3D0)
#define AES_SR                          _SFR_(0x3D1)
#define AES_DINR                        _SFR_(0x3D2)
#define AES_DOUTR                       _SFR_(0x3D3)

/* USART2 */
#define USART2_SR                       _SFR_(0x3E0)
#define USART2_DR                       _SFR_(0x3E1)
#define USART2_BRR1                     _SFR_(0x3E2)
#define USART2_BRR2                     _SFR_(0x3E3)
#define USART2_CR1                      _SFR_(0x3E4)
#define USART2_CR2                      _SFR_(0x3E5)
#define USART2_CR3                      _SFR_(0x3E6)
#define USART2_CR4                      _SFR_(0x3E7)
#define USART2_CR5                      _SFR_(0x3E8)
#define USART2_GTR                      _SFR_(0x3E9)
#define USART2_PSCR                     _SFR_(0x3EA)

/* USART3 */
#define USART3_SR                       _SFR_(0x3F0)
#define USART3_DR                       _SFR_(0x3F1)
#define USART3_BRR1                     _SFR_(0x3F2)
#define USART3_BRR2                     _SFR_(0x3F3)
#define USART3_CR1                      _SFR_(0x3F4)
#define USART3_CR2                      _SFR_(0x3F5)
#define USART3_CR3                      _SFR_(0x3F6)
#define USART3_CR4                      _SFR_(0x3F7)
#define USART3_CR5                      _SFR_(0x3F8)
#define USART3_GTR                      _SFR_(0x3F9)
#define USART3_PSCR                     _SFR_(0x3FA)

/* LCD */
#define LCD_CR1                         _SFR_(0x400)
#define LCD_CR2                         _SFR_(0x401)
#define LCD_CR3                         _SFR_(0x402)
#define LCD_FRQ                         _SFR_(0x403)
#define LCD_PM0                         _SFR_(0x404)
#define LCD_PM1                         _SFR_(0x405)
#define LCD_PM2                         _SFR_(0x406)
#define LCD_PM3                         _SFR_(0x407)
#define LCD_PM4                         _SFR_(0x408)
#define LCD_PM5                         _SFR_(0x409)
#define LCD_RAM0                        _SFR_(0x40C)
#define LCD_RAM1                        _SFR_(0x40D)
#define LCD_RAM2                        _SFR_(0x40E)
#define LCD_RAM3                        _SFR_(0x40F)
#define LCD_RAM4                        _SFR_(0x410)
#define LCD_RAM5                        _SFR_(0x411)
#define LCD_RAM6                        _SFR_(0x412)
#define LCD_RAM7                        _SFR_(0x413)
#define LCD_RAM8                        _SFR_(0x414)
#define LCD_RAM9                        _SFR_(0x415)
#define LCD_RAM10                       _SFR_(0x416)
#define LCD_RAM11                       _SFR_(0x417)
#define LCD_RAM12                       _SFR_(0x418)
#define LCD_RAM13                       _SFR_(0x419)
#define LCD_RAM14                       _SFR_(0x41A)
#define LCD_RAM15                       _SFR_(0x41B)
#define LCD_RAM16                       _SFR_(0x41C)
#define LCD_RAM17                       _SFR_(0x41D)
#define LCD_RAM18                       _SFR_(0x41E)
#define LCD_RAM19                       _SFR_(0x41F)
#define LCD_RAM20                       _SFR_(0x420)
#define LCD_RAM21                       _SFR_(0x421)
#define LCD_CR4                         _SFR_(0x42F)

/* RI */
#define RI_ICR1                         _SFR_(0x431)
#define RI_ICR2                         _SFR_(0x432)
#define RI_IOIR1                        _SFR_(0x433)
#define RI_IOIR2                        _SFR_(0x434)
#define RI_IOIR3                        _SFR_(0x435)
#define RI_IOCMR1                       _SFR_(0x436)
#define RI_IOCMR2                       _SFR_(0x437)
#define RI_IOCMR3                       _SFR_(0x438)
#define RI_IOSR1                        _SFR_(0x439)
#define RI_IOSR2                        _SFR_(0x43A)
#define RI_IOSR3                        _SFR_(0x43B)
#define RI_IOGCR                        _SFR_(0x43C)
#define RI_ASCR1                        _SFR_(0x43D)
#define RI_ASCR2                        _SFR_(0x43E)
#define RI_RCR                          _SFR_(0x43F)
#define RI_CR                           _SFR_(0x450)
#define RI_MASKR1                       _SFR_(0x451)
#define RI_MASKR2                       _SFR_(0x452)
#define RI_MASKR3                       _SFR_(0x453)
#define RI_MASKR4                       _SFR_(0x454)
#define RI_IOIR4                        _SFR_(0x455)
#define RI_IOCMR4                       _SFR_(0x456)
#define RI_IOSR4                        _SFR_(0x457)

/* Comparator */
#define COMP_CSR1                       _SFR_(0x440)
#define COMP_CSR2                       _SFR_(0x441)
#define COMP_CSR3                       _SFR_(0x442)
#define COMP_CSR4                       _SFR_(0x443)
#define COMP_CSR5                       _SFR_(0x444)

/* Interrupts */
#define TLI_ISR                         0
#define FLASH_ISR                       1
#define DMA1_01_ISR                     2
#define DMA1_23_ISR                     3
#define RTC_ISR                         4
#define PVD_ISR                         5
#define EXTIB_ISR                       6
#define EXTID_ISR                       7
#define EXTI0_ISR                       8
#define EXTI1_ISR                       9
#define EXTI2_ISR                       10
#define EXTI3_ISR                       11
#define EXTI4_ISR                       12
#define EXTI5_ISR                       13
#define EXTI6_ISR                       14
#define EXTI7_ISR                       15
#define CLK_ISR                         17
#define ADC1_ISR                        18
#define TIM2_UPD_ISR                    19
#define TIM2_CC_ISR                     20
#define TIM3_UPD_ISR                    21
#define TIM3_CC_ISR                     22
#define RI_ISR                          23
#define TIM4_ISR                        25
#define SPI1_ISR                        26
#define USART1_TXE_ISR                  27
#define USART1_RXNE_ISR                 28
#define I2C1_ISR                        29

/* CPU */
#define CFG_GCR                         _MEM_(0x7F60)
#define ITC_SPR1                        _MEM_(0x7F70)
#define ITC_SPR2                        _MEM_(0x7F71)
#define ITC_SPR3                        _MEM_(0x7F72)
#define ITC_SPR4                        _MEM_(0x7F73)
#define ITC_SPR5                        _MEM_(0x7F74)
#define ITC_SPR6                        _MEM_(0x7F75)
#define ITC_SPR7                        _MEM_(0x7F76)
#define ITC_SPR8                        _MEM_(0x7F77)

#define enable_interrupts()             __asm__("rim");
#define disable_interrupts()            __asm__("sim");

#endif /* STM8L_H */
