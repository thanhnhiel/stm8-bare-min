/**
  ******************************************************************************
  * @file    stm8l15x_beep.h
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   This file contains all the functions prototypes for the BEEP firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BEEP_H
#define __BEEP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef USE_BEEP

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */
  
/** @addtogroup BEEP
  * @{
  */ 
  
/* Exported types ------------------------------------------------------------*/
/** @defgroup BEEP_Exported_Types
  * @{
  */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef struct BEEP_struct
{
  __IO uint8_t CSR1; /*!< BEEP Control status register1 */
  uint8_t RSERVED1;
  uint8_t RESERVED2;
  __IO uint8_t CSR2; /*!< BEEP Control status register2 */
}
BEEP_TypeDef;

/** @addtogroup BEEP_Registers_Reset_Value
  * @{
  */
#define BEEP_CSR1_RESET_VALUE ((uint8_t)0x00)
#define BEEP_CSR2_RESET_VALUE ((uint8_t)0x1F)

/**
  * @}
  */

/** @addtogroup BEEP_Registers_Bits_Definition
  * @{
  */

#define BEEP_CSR1_MSR     ((uint8_t)0x01) /*!< Measurement enable mask */

#define BEEP_CSR2_BEEPSEL ((uint8_t)0xC0) /*!< Beeper frequency selection mask */
#define BEEP_CSR2_BEEPEN  ((uint8_t)0x20) /*!< Beeper enable mask */
#define BEEP_CSR2_BEEPDIV ((uint8_t)0x1F) /*!< Beeper Divider prescalar mask */

#define BEEP                        ((BEEP_TypeDef *) BEEP_BASE)
#define BEEP_BASE                   (uint16_t)0x50F0

/** @defgroup BEEP_Frequency
  * @{
  */
typedef enum {
  BEEP_Frequency_1KHz = (uint8_t)0x00,  /*!< Beep signal output frequency 1 KHz */
  BEEP_Frequency_2KHz = (uint8_t)0x40,  /*!< Beep signal output frequency 2 KHz */
  BEEP_Frequency_4KHz = (uint8_t)0x80   /*!< Beep signal output frequency 4 KHz */
} BEEP_Frequency_TypeDef;

#define IS_BEEP_FREQUENCY(FREQ) (((FREQ) == BEEP_Frequency_1KHz) || \
                                 ((FREQ) == BEEP_Frequency_2KHz) || \
                                 ((FREQ) == BEEP_Frequency_4KHz))
 
/**
  * @}
  */

/**
  * @}
  */
      
/* Exported constants --------------------------------------------------------*/
/** @defgroup BEEP_Exported_Constants
  * @{
  */

#define BEEP_CALIBRATION_DEFAULT ((uint8_t)0x01)  /*!< Default value when calibration is not done */
#define LSI_FREQUENCY_MIN ((uint32_t)25000)       /*!< LSI minimum value in Hertz */
#define LSI_FREQUENCY_MAX ((uint32_t)75000)       /*!< LSI maximum value in Hertz */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup BEEP_Exported_Macros
  * @{
  */
#define IS_LSI_FREQUENCY(FREQ) (((FREQ) >= LSI_FREQUENCY_MIN) && ((FREQ) <= LSI_FREQUENCY_MAX))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/*  Function used to set the BEEP configuration to the default reset state *****/  
void BEEP_DeInit(void);

/* Initialization and Configuration functions *********************************/
void BEEP_Init(BEEP_Frequency_TypeDef BEEP_Frequency);
void BEEP_Cmd(FunctionalState NewState);

/* Low Speed Internal Clock(LSI) Calibration functions  ***********************/
void BEEP_LSClockToTIMConnectCmd(FunctionalState NewState);
void BEEP_LSICalibrationConfig(uint32_t LSIFreqHz);

#endif

#endif /* __STM8L15x_BEEP_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
