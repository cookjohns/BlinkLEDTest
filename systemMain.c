/* Name: systemMain.c
 * Author: John Cook
 */

#include <avr/io.h>
#include "USI_TWI_master.h"
#include <util/delay.h>
#include <stdint.h>

#define I2C_MAX_MSG_SIZE 5

bool static PASS = true;
bool static FAIL = false;

void long_delay_ms(uint16_t ms) {
    for (ms /= 10;ms>0;ms--) {
        _delay_ms(10);
    }
}

// returns true if downward acceleration is too fast (need exact value)
bool checkAccel() {
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
    
    // check value
    if (/*i2cMessageBuf[x] > y*/) {
        return FAIL;
    }
    
    return PASS;
}

bool checkVoltage() {
    return PASS;
}

// initialize onboard ADC
void initADC() {
    //Configure ADMUX register
    ADMUX =
    (1 << ADLAR)| //shift in a 1 and follow 8bit procedure
    (1 << MUX1)| //Use ADC2 or PB4 pin for Vin
    (0 << REFS0)| //set refs0 and 1 to 0 to use Vcc as Vref
    (0 << REFS1);
    //Configure ADCSRA register
    ADCSRA =
    (1 << ADEN)| //set ADEN bit to 1 to enable the ADC
    (0 << ADSC); //set ADSC to 0 to make sure no conversions are happening
}

// get voltage reading
double checkVoltage() {
    //make sure we define result as a double so we can mult/divide w/o error
    double result = 0;
    int temp = 0;
    //set ADSC pin to 1 in order to start reading the AIN value
    ADCSRA |= (1 << ADSC);
    //do nothing until the ADSC pin returns back to 0;
    while (((ADCSRA >> ADSC) & 1)){}
    //for 8 bit precision we can just read ADCH:
    result = ADCH;
    //to properly interpret the results as a voltage we need to
    //divide the result by the maximum range: 2^(#bits precision)-1
    //and multiply the result by the Vref value, Vcc or 5V
    result = result * 5/255;
    if (result == 0) return FAIL;
    return PASS;
}

int main(void) {
    uint8_t i2cMessageBuf[I2C_MAX_MSG_SIZE];
    USI_TWI_Master_Initialise();
    initADC();
    
    // runs continuously
    for(;;){
        if (checkAccel() == FAIL || checkVoltage() == FAIL) {
            
        }
        
    }
    return 0;   /* never reached */
}