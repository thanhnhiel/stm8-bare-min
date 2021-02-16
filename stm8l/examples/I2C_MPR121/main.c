#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stm8l.h>
#include <delay.h>
#include <uart.h>
#include <i2c.h>

#define MPR121_ADDR        (uint8_t)(0x5A << 1)


int putchar(int c) {
    uart_write(c);
    return 0;
}

void UART_LowLevel_Init(void)
{
    /*!< USART1 Tx- Rx (PC3- PC2) remapping to PA2- PA3 */
    SYSCFG_RMPCR1 &= (uint8_t)((uint8_t)((uint8_t)0x011C << 4) | (uint8_t)0x0F);
    SYSCFG_RMPCR1 |= (uint8_t)((uint16_t)0x011C & (uint16_t)0x00F0);

    /* Enable USART clock */
    //CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
      /* Enable the peripheral Clock */
    CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << 0x05);
}


void I2C_LowLevel_Init(void)
{
  /* Enable the peripheral Clock */
  CLK_PCKENR1 |= (uint8_t)((uint8_t)1 << 0x03);
  //CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
  
  //GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_6, ENABLE);
  // GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_7, ENABLE);
  // GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_5, ENABLE);
  //GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
}

void mpr121_get_data(uint8_t *id) {
    // i2c_start();
    // i2c_write_addr(MPR121_ADDR + I2C_WRITE);
    // i2c_write(MPR121_ID_REG_A);
    // i2c_stop();

    i2c_start();
    i2c_write_addr(MPR121_ADDR + I2C_READ);
    i2c_read_arr(id, 2);
}

void i2c_write_reg(uint8_t reg, uint8_t val)
{
    i2c_start();
    i2c_write_addr(MPR121_ADDR + I2C_WRITE);
    i2c_write(reg);
    i2c_write(val);
    i2c_stop();
}

/* Exported macro ------------------------------------------------------------*/
// MPR121 Register Defines
#define TOU_THRESH	0x06
#define	REL_THRESH	0x0A

#define MHD_R	0x2B
#define NHD_R	0x2C
#define	NCL_R 	0x2D
#define	FDL_R	0x2E
#define	MHD_F	0x2F
#define	NHD_F	0x30
#define	NCL_F	0x31
#define	FDL_F	0x32
#define	ELE0_T	0x41
#define	ELE0_R	0x42
#define	ELE1_T	0x43
#define	ELE1_R	0x44
#define	ELE2_T	0x45
#define	ELE2_R	0x46
#define	ELE3_T	0x47
#define	ELE3_R	0x48
#define	ELE4_T	0x49
#define	ELE4_R	0x4A
#define	ELE5_T	0x4B
#define	ELE5_R	0x4C
#define	ELE6_T	0x4D
#define	ELE6_R	0x4E
#define	ELE7_T	0x4F
#define	ELE7_R	0x50
#define	ELE8_T	0x51
#define	ELE8_R	0x52
#define	ELE9_T	0x53
#define	ELE9_R	0x54
#define	ELE10_T	0x55
#define	ELE10_R	0x56
#define	ELE11_T	0x57
#define	ELE11_R	0x58
#define	FIL_CFG	0x5D
#define	ELE_CFG	0x5E
#define GPIO_CTRL0	0x73
#define	GPIO_CTRL1	0x74
#define GPIO_DATA	0x75
#define	GPIO_DIR	0x76
#define	GPIO_EN		0x77
#define	GPIO_SET	0x78
#define	GPIO_CLEAR	0x79
#define	GPIO_TOGGLE	0x7A
#define	ATO_CFG0	0x7B
#define	ATO_CFGU	0x7D
#define	ATO_CFGL	0x7E
#define	ATO_CFGT	0x7F

void mpr121_setup(void)
{
  i2c_write_reg( ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  i2c_write_reg( MHD_R, 0x01);
  i2c_write_reg( NHD_R, 0x01);
  i2c_write_reg( NCL_R, 0x00);
  i2c_write_reg( FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  i2c_write_reg( MHD_F, 0x01);
  i2c_write_reg( NHD_F, 0x01);
  i2c_write_reg( NCL_F, 0xFF);
  i2c_write_reg( FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  i2c_write_reg( ELE0_T, TOU_THRESH);
  i2c_write_reg( ELE0_R, REL_THRESH);
 
  i2c_write_reg( ELE1_T, TOU_THRESH);
  i2c_write_reg( ELE1_R, REL_THRESH);
  
  i2c_write_reg( ELE2_T, TOU_THRESH);
  i2c_write_reg( ELE2_R, REL_THRESH);
  
  i2c_write_reg( ELE3_T, TOU_THRESH);
  i2c_write_reg( ELE3_R, REL_THRESH);
  
  i2c_write_reg( ELE4_T, TOU_THRESH);
  i2c_write_reg( ELE4_R, REL_THRESH);
  
  i2c_write_reg( ELE5_T, TOU_THRESH);
  i2c_write_reg( ELE5_R, REL_THRESH);
  
  i2c_write_reg( ELE6_T, TOU_THRESH);
  i2c_write_reg( ELE6_R, REL_THRESH);
  
  i2c_write_reg( ELE7_T, TOU_THRESH);
  i2c_write_reg( ELE7_R, REL_THRESH);
  
  i2c_write_reg( ELE8_T, TOU_THRESH);
  i2c_write_reg( ELE8_R, REL_THRESH);
  
  i2c_write_reg( ELE9_T, TOU_THRESH);
  i2c_write_reg( ELE9_R, REL_THRESH);
  
  i2c_write_reg( ELE10_T, TOU_THRESH);
  i2c_write_reg( ELE10_R, REL_THRESH);
  
  i2c_write_reg( ELE11_T, TOU_THRESH);
  i2c_write_reg( ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  i2c_write_reg( FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  i2c_write_reg( ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  #if 0
  /*i2c_write_reg( ATO_CFG0, 0x0B);
  i2c_write_reg( ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   i2c_write_reg( ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  i2c_write_reg( ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  #endif
  
  i2c_write_reg( ELE_CFG, 0x0C);
} 


void main() 
{
    uint8_t counter = 0;
    uint8_t id[2];
    UART_LowLevel_Init();
    uart_init();
    printf("MCU Init\r\n");

    I2C_LowLevel_Init();
    i2c_init();

    printf("mpr121_setup...");

    mpr121_setup();
    printf("done\r\n");

    while (1) {
        //measure();
        printf("Test, %d\r\n", counter++);
        delay_ms(2000);

        if (!(PB_IDR & (1<<2)))
        {
            mpr121_get_data(id);
            printf("Touch data:: %d %d\r\n", id[0], id[1]);
        }
    }
}
