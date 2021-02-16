#include "i2c.h"
#include "stm8l.h"

void i2c_init() {
    I2C1_FREQR = (1 << I2C1_FREQR_FREQ1);
    I2C1_CCRL = 0x0A; // 100kHz
    I2C1_OARH = (1 << I2C1_OARH_ADDMODE); // 7-bit addressing
    I2C1_CR1 = (1 << I2C1_CR1_PE);
}

void i2c_start() {
    I2C1_CR2 |= (1 << I2C1_CR2_START);
    while (!(I2C1_SR1 & (1 << I2C1_SR1_SB)));
}

void i2c_stop() {
    I2C1_CR2 |= (1 << I2C1_CR2_STOP);
    while (I2C1_SR3 & (1 << I2C1_SR3_MSL));
}

void i2c_write(uint8_t data) {
    I2C1_DR = data;
    while (!(I2C1_SR1 & (1 << I2C1_SR1_TXE)));
}

void i2c_write_addr(uint8_t addr) {
    I2C1_DR = addr;
    while (!(I2C1_SR1 & (1 << I2C1_SR1_ADDR)));
    (void) I2C1_SR3; // check BUS_BUSY
    I2C1_CR2 |= (1 << I2C1_CR2_ACK);
}

uint8_t i2c_read() {
    I2C1_CR2 &= ~(1 << I2C1_CR2_ACK);
    i2c_stop();
    while (!(I2C1_SR1 & (1 << I2C1_SR1_RXNE)));
    return I2C1_DR;
}

void i2c_read_arr(uint8_t *buf, int len) {
    while (len-- > 1) {
        I2C1_CR2 |= (1 << I2C1_CR2_ACK);
        while (!(I2C1_SR1 & (1 << I2C1_SR1_RXNE)));
        *(buf++) = I2C1_DR;
    }
    *buf = i2c_read();
}
