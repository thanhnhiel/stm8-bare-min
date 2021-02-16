#include "mpr121.h"
#include "i2c.h"
#include "stm8l.h"

#ifdef USE_MPR121

void mpr121_get_data(uint8_t *id) {
    // i2c_start();
    // i2c_write_addr(MPR121_ADDR + I2C_WRITE);
    // i2c_write(MPR121_ID_REG_A);
    // i2c_stop();

    i2c_start();
    i2c_write_addr(MPR121_ADDR + I2C_READ);
    i2c_read_arr(id, 2);
}

void mpr121_write_reg(uint8_t reg, uint8_t val)
{
    i2c_start();
    i2c_write_addr(MPR121_ADDR + I2C_WRITE);
    i2c_write(reg);
    i2c_write(val);
    i2c_stop();
}

void mpr121_setup(void)
{
  i2c_init();
  
  mpr121_write_reg(ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  mpr121_write_reg(MHD_R, 0x01);
  mpr121_write_reg(NHD_R, 0x01);
  mpr121_write_reg(NCL_R, 0x00);
  mpr121_write_reg(FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  mpr121_write_reg(MHD_F, 0x01);
  mpr121_write_reg(NHD_F, 0x01);
  mpr121_write_reg(NCL_F, 0xFF);
  mpr121_write_reg(FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  mpr121_write_reg(ELE0_T, TOU_THRESH);
  mpr121_write_reg(ELE0_R, REL_THRESH);
 
  mpr121_write_reg(ELE1_T, TOU_THRESH);
  mpr121_write_reg(ELE1_R, REL_THRESH);
  
  mpr121_write_reg(ELE2_T, TOU_THRESH);
  mpr121_write_reg(ELE2_R, REL_THRESH);
  
  mpr121_write_reg(ELE3_T, TOU_THRESH);
  mpr121_write_reg(ELE3_R, REL_THRESH);
  
  mpr121_write_reg(ELE4_T, TOU_THRESH);
  mpr121_write_reg(ELE4_R, REL_THRESH);
  
  mpr121_write_reg(ELE5_T, TOU_THRESH);
  mpr121_write_reg(ELE5_R, REL_THRESH);
  
  mpr121_write_reg(ELE6_T, TOU_THRESH);
  mpr121_write_reg(ELE6_R, REL_THRESH);
  
  mpr121_write_reg(ELE7_T, TOU_THRESH);
  mpr121_write_reg(ELE7_R, REL_THRESH);
  
  mpr121_write_reg(ELE8_T, TOU_THRESH);
  mpr121_write_reg(ELE8_R, REL_THRESH);
  
  mpr121_write_reg(ELE9_T, TOU_THRESH);
  mpr121_write_reg(ELE9_R, REL_THRESH);
  
  mpr121_write_reg(ELE10_T, TOU_THRESH);
  mpr121_write_reg(ELE10_R, REL_THRESH);
  
  mpr121_write_reg(ELE11_T, TOU_THRESH);
  mpr121_write_reg(ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  mpr121_write_reg(FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  mpr121_write_reg(ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  #if 0
  /*mpr121_write_reg(ATO_CFG0, 0x0B);
  mpr121_write_reg(ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   mpr121_write_reg(ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  mpr121_write_reg(ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  #endif
  
  mpr121_write_reg(ELE_CFG, 0x0C);
} 

#endif