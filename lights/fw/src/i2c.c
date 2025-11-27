#include "i2c.h"

#include <avr/io.h>

void i2c_init(void) {
    TWSR |= (0 << TWPS1) | (1 << TWPS0);  // TWI Bit Rate Prescaler Value: 4
    TWBR = 198;  // = TWI Bit Rate Generator Division Factor 
    // => F_CPU_HZ / [ 16 + (2 * TWBR)() ] = SCL Frequency = 5 kHz (estimated bare minimum for proper I2C using internal resistors)
    // TODO: Test rise/fall times with scope! Might need to adjust TWBR to optimize it..

    TWCR |= (1 << TWEN);  // TWI Enable, duh!
}


