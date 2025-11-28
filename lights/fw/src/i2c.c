#include "i2c.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void i2c_init(void) {
    TWSR = ( (0 << TWPS1) | (1 << TWPS0) );  // TWI Bit Rate Prescaler Value: 4
    TWBR = 198;  // = TWI Bit Rate Generator Division Factor 
    // => F_CPU_HZ / [ 16 + (2 * TWBR)() ] = SCL Frequency = 5 kHz (estimated bare minimum for proper I2C using internal resistors)
    // TODO: Test rise/fall times with scope! Might need to adjust TWBR to optimize it..

    TWCR = (1 << TWINT) |  // "Clear" TWI Interrupt Flag by writing a logic 1 to it (weird, ik)
           (1 << TWEN);  // TWI Enable, duh!
            
}

void i2c_start(void) {
    // Note: Don't want to OR the bits into the TWCR register
    TWCR = (1 << TWINT) |  
           (1 << TWSTA) |  // TWI START Condition
           (1 << TWEN);

    // Note: TWI hardware immediately writes 0 to TWINT (meaning it is busy tryna send START) 

    while (!(TWCR & (1 << TWINT))) {
        // Wait until START condition has been successfully submitted (aka TWINT becomes 1 again)
    }
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | 
           (1 << TWSTO) |  // TWI STOP Condition
           (1 << TWEN);
} 

void i2c_send_address(uint8_t address_byte) {
    TWDR = address_byte;
    TWCR = (1 << TWINT) |
           (1 << TWEN);

    // Note: TWI hardware immediately writes 0 to TWINT (meaning it is busy tryna send SLA + R/W) 

    while (!(TWCR & (1 << TWINT))) {
        // Wait until SLA + R/W has been successfully transmitted
    }
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) |
           (1 << TWEN);

    // Note: TWI hardware immediately writes 0 to TWINT (meaning it is busy tryna send data)

    while (!(TWCR & (1 << TWINT))) {
        // Wait until data has been successfully transmitted
    } 
}

uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWINT) |
           (1 << TWEA) |  // Send an ACK after reading byte (later)
           (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
        // Wait until byte successfully received from and ACK transmitted back to slave!
    } 

    return (TWDR);
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) |
           (0 << TWEA) |  // Send a NACK after reading byte (later)
           (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
        // Wait until byte successfully received from and NACK transmitted back to slave!
    } 

    return (TWDR);
}

void i2c_write_register(uint8_t dev_addr, uint8_t reg, uint8_t value) {
    
}