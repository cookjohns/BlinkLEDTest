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
    while (TRUE) {
        // construct a command to read channel 0 of a 9150 accelerometer
        i2cMessageBuf[0] = (uint8_t) 0xD0;
    
        // send the message
        USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x02);
        USI_TWI_Start_Read_Write(0x75, (uint16_t) 0x43); // gyro x - ????
    
        // wait
        long_delay_ms(50);
    
        // read the msg
        i2cMessageBuf[0]= 0xD1; // lsb == 1 => read
        USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x03);
    
        // wait
        long_delay_ms(500);
    }
    
    // never reached
    return 0;
}
