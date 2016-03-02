#include <avr/io.h>
#include "USI_TWI_master.h"
#include <util/delay.h>
#include <stdint.h>

#define I2C_MAX_MSG_SIZE 5
#define TRUE  1
#define FALSE 0

void long_delay_ms(uint16_t ms) {
    for (ms /= 10;ms>0;ms--) {
        _delay_ms(10);
    }
}

int main(void) {
    // initialize
    uint8_t i2cMessageBuf[I2C_MAX_MSG_SIZE];
    USI_TWI_Master_Initialise();
    // reset the power mgmt register to wake it up
    i2cMessageBuf[0] = (uint8_t) 0xD0; // address 0x68, write
    i2cMessageBuf[1] = 0x6b; // low x-axis
    i2cMessageBuf[2] = 0x00;
    USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x03);
    long_delay_ms(50);
    
    while (TRUE) {
        // construct a command to high-order x value  of a 9150 accelerometer
        // register 59 = 0x3b; low-order register is 0x3c, then put them
        // together
        i2cMessageBuf[0] = (uint8_t) 0xD0;
        i2cMessageBuf[1] = 0x3b; // high x-axis
        //i2cMessageBuf[2] = 0x3c;
        
        // send the message
        USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x02);
        
        // wait
        long_delay_ms(50);
        
        // read the msg
        i2cMessageBuf[0] = 0xD1; // lsb == 1 => read
        i2cMessageBuf[1] = 0x3b;
        //i2cMessageBuf[2] = 0x3c;
        USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x03);
        //USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x04);
        
        // turn on LED if not flat on table
        // flat on table: high is FA, low is FF
        if (i2cMessageBuf[1] != 0xFA) {
            DDRB |= 1 << PB3;
            PORTB &= ~(1<<PB3);
        }
        else PORTB |= 1 << PB3;
        
        // wait
        long_delay_ms(500);
    }
    
    // never reached
    return 0;
}